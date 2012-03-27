#include "common.h"
#include ".\feibian.h"
#include <maya/MFnPluginData.h>

extern void printMatrix(const MMatrix&);
extern void extractTranMatrix(const MMatrix& tm,float translation[3],float rotation[4],float scale[3]);
extern unsigned int getFps();
extern bool equal(float x,float y);
extern void addKeyFramesFloat(std::vector<keyframeT<float> > *pVector,keyframeT<float> *pData);
extern void addKeyFramesBool(std::vector<keyframeT<bool> > *pVector,keyframeT<bool> *pData);

feibian::feibian(void)
{
	clear();
}

feibian::~feibian(void)
{
	clear();
}

void feibian::clear()
{
	m_joints.clear();
}

// Load animations
MStatus feibian::loadAnims()
{
	MStatus stat;

	MTime initialTime = MAnimControl::currentTime();

	double d1 = MAnimControl::animationStartTime().value();
	double d2 = MAnimControl::animationEndTime().value();
	double d3 = MAnimControl::minTime().value();
	double d4 = MAnimControl::maxTime().value();
	loadClip("Clip",MAnimControl::animationStartTime().value(),MAnimControl::animationEndTime().value(),1);

	MAnimControl::setCurrentTime(initialTime);

	return MS::kSuccess;
}

MStatus feibian::load(MFnSkinCluster* pSkinCluster)
{
	MStatus stat;
	//update skin cluster pointer
	if (!pSkinCluster)
	{
		std::cout << "Could not load skeleton data, no skin cluster specified\n";
		return MS::kFailure;
	}
	//retrieve and load joints from the skin cluster
	MDagPath jointDag,rootDag;
	MDagPathArray influenceDags;
	int numInfluenceObjs = pSkinCluster->influenceObjects(influenceDags,&stat);
	for (int i=0; i<numInfluenceObjs; i++)
	{
		jointDag = influenceDags[i];
		if (influenceDags[i].hasFn(MFn::kJoint))
		{
			//retrieve root joint
			rootDag = jointDag;
			while (jointDag.length()>0)
			{
				jointDag.pop();
				if (jointDag.hasFn(MFn::kJoint) && jointDag.length()>0)
					rootDag = jointDag;
			}
			//check if skeleton has already been loaded
			bool skip = false;
			for (int j=0; j<m_joints.size() && !skip; j++)
			{
				//skip skeleton if already loaded
				if (rootDag.partialPathName() == m_joints[j].name)
				{
					skip = true;
				}
			}
			//load joints data from root
			if (!skip)
			{
				//load joints data
				loadJoint(rootDag,NULL);
			}
		}
	}

	return MS::kSuccess;
}


// Load a joint
void feibian::loadJoint(MDagPath& jointDag,joint* parent)
{
	int i;
	joint newJoint;
	joint* parentJoint = parent;
	if (jointDag.hasFn(MFn::kJoint))
	{
		MFnIkJoint jointFn(jointDag);

		// Get parent index
		int idx=-1;
		if (parent)
		{
			idx = parent->id;
		}
		// Get joint matrix
		MMatrix worldMatrix = jointDag.inclusiveMatrix();;
		printMatrix(worldMatrix);

		// Calculate scaling factor inherited by parent
		// Calculate Local Matrix
		MMatrix localMatrix = worldMatrix;
		if (parent)
			localMatrix = worldMatrix * parent->worldMatrix.inverse();
		printMatrix(localMatrix);

		// Set joint info
		newJoint.name = jointFn.partialPathName();
		newJoint.id = m_joints.size();
		newJoint.parentIndex = idx;
		newJoint.jointDag = jointDag;
		newJoint.worldMatrix = worldMatrix;
		newJoint.localMatrix = localMatrix;

		for (int iRow = 0; iRow < 4; iRow++)
			for (int iCol = 0; iCol < 3; iCol++)
				newJoint.tran.m_mat[iRow][iCol] = (FLOAT)worldMatrix[iRow][iCol];
		/*MQuaternion q;
		q = worldMatrix;
		newJoint.tran.q[0] = (float)q.x;
		newJoint.tran.q[1] = (float)q.y;
		newJoint.tran.q[2] = (float)q.z;
		newJoint.tran.q[3] = (float)q.w;
		newJoint.tran.t[0] = (float)worldMatrix[3][0];
		newJoint.tran.t[1] = (float)worldMatrix[3][1];
		newJoint.tran.t[2] = (float)worldMatrix[3][2];*/

		m_joints.push_back(newJoint);
		// Get pointer to newly created joint
		parentJoint = &newJoint;
	}
	// Load children joints
	for (i=0; i<jointDag.childCount();i++)
	{
		MObject child;
		child = jointDag.child(i);
		MDagPath childDag = jointDag;
		childDag.push(child);
		loadJoint(childDag,parentJoint);
	}

}

extern int Round (float fValue) ;

// Load an animation clip
MStatus feibian::loadClip(MString clipName,int start,int stop,int rate)
{
	uint fps = getFps();
	float frameTime = 1000.0f / fps;

	MStatus stat;
	int i,j;
	std::vector<int> times;
	if (m_joints.size() < 0)
		return MS::kFailure;

	times.clear();
	for (int t=start; t<stop; t+=rate)
		times.push_back(t);
	times.push_back(stop);

	// create the animation
	m_animation.name = clipName.asChar();

	m_animation.startTime = 0;
	m_animation.endTime = times[times.size()-1] - times[0];

	for (i=0; i<times.size(); i++)
	{
		MAnimControl::setCurrentTime(MTime(times[i],MTime::uiUnit()));
		for (j=0; j<m_joints.size(); j++)
		{
			keyframeTranslation translation;
			keyframeRotation rotation;
			keyframeScale scale;

			joint &jt = m_joints[j];
			int time = times[i] - times[0] + m_animation.startTime;
			loadKeyframe(m_joints[j],time,translation,rotation,scale);
			translation.time *= frameTime;
			rotation.time *= frameTime;
			scale.time *= frameTime;

			size_t size = jt.keyframesTranslation.size();
			if(size > 0)
			{
				keyframeTranslation& t = jt.keyframesTranslation[size - 1];
				if(!equal(translation.v[0],t.v[0]) || !equal(translation.v[1],t.v[1]) || !equal(translation.v[2],t.v[2]))
				{
					//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
					int lastTime = Round(t.time / frameTime);
					if(time - 1 > lastTime)
					{
						keyframeTranslation temp = t;
						temp.time = (time - 1) * frameTime;
						jt.keyframesTranslation.push_back(temp);
					}
					jt.keyframesTranslation.push_back(translation);
				}
			}
			else
			{
				jt.keyframesTranslation.push_back(translation);
			}

			MFnIkJoint jn(jt.jointDag);
			if(jn.name() == "Hips")
			{
//				breakable;
			}
			size = jt.keyframesRotation.size();
			if(size > 0)
			{
				keyframeRotation& r = jt.keyframesRotation[size - 1];
				if(!equal(rotation.q[0],r.q[0]) || !equal(rotation.q[1],r.q[1]) || !equal(rotation.q[2],r.q[2]) || !equal(rotation.q[3],r.q[3]))
				{
					//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
					int lastTime = Round(r.time / frameTime);
					if(time - 1 > lastTime)
					{
						keyframeRotation temp = r;
						temp.time = (time - 1) * frameTime;
						jt.keyframesRotation.push_back(temp);
					}
					jt.keyframesRotation.push_back(rotation);
				}
			}
			else
			{
				jt.keyframesRotation.push_back(rotation);
			}
			size = jt.keyframesScale.size();
			if(size > 0)
			{
				keyframeScale& s = jt.keyframesScale[size - 1];
				if(!equal(scale.v[0],s.v[0]) || !equal(scale.v[1],s.v[1]) || !equal(scale.v[2],s.v[2]))
				{
					//如果跟上一次不一样，并且跨越了桢，那么需要补一桢
					int lastTime = Round(s.time / frameTime);
					if(time - 1 > lastTime)
					{
						keyframeScale temp = s;
						temp.time = (time - 1) * frameTime;
						jt.keyframesScale.push_back(temp);
					}
					jt.keyframesScale.push_back(scale);
				}
			}
			else
			{
				jt.keyframesScale.push_back(scale);
			}
		}
	}

	return MS::kSuccess;
}
void feibian::loadKeyframe(joint& j,int time,keyframeTranslation& translation,keyframeRotation& rotation,keyframeScale& scale)
{
	MVector position;

	int parentIdx = j.parentIndex;

	MMatrix matrix;
	{
		matrix	= j.jointDag.inclusiveMatrix();
		printMatrix(matrix);
		MMatrix worldMatrix = j.worldMatrix;
		printMatrix(worldMatrix);

		MMatrix invWorldMatrix = worldMatrix.inverse();
		//matrix = invWorldMatrix * matrix;
		printMatrix(matrix);

		if(j.parentIndex >= 0)
		{
			MMatrix pMatrix = m_joints[parentIdx].jointDag.inclusiveMatrix();
			printMatrix(pMatrix);
			MMatrix pWorldMatrix = m_joints[parentIdx].worldMatrix;
			printMatrix(pWorldMatrix);
			MMatrix invpWorldMatrix = pWorldMatrix.inverse();
			//pMatrix = invpWorldMatrix * pMatrix;
			printMatrix(pMatrix);

			MMatrix pInvMatrix = pMatrix.inverse();
			matrix = matrix * pInvMatrix;
			printMatrix(matrix);
		}
	}
	{
		MMatrix matrix	= j.jointDag.inclusiveMatrix() * j.jointDag.exclusiveMatrixInverse();
		printMatrix(matrix);
		MMatrix worldMatrix = j.localMatrix;
		printMatrix(worldMatrix);

		MMatrix invWorldMatrix = worldMatrix.inverse();
		matrix = matrix * invWorldMatrix;
		if(j.parentIndex >= 0)
		{
			matrix = matrix * m_joints[j.parentIndex].localMatrix;
		}
		printMatrix(matrix);
	}

	extractTranMatrix(matrix,translation.v,rotation.q,scale.v);
	translation.time = time;
	rotation.time = time;
	scale.time = time;
}