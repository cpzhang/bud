#include "common.h"
#include ".\skeleton.h"
#include <maya/MFnPluginData.h>
//#include "ReMath.h"

extern void printMatrix(const MMatrix&);

skeleton::skeleton(void)
{
	clear();
}

skeleton::~skeleton(void)
{
	clear();
}

void skeleton::clear()
{
	m_joints.clear();
	m_animations.clear();
}

MStatus skeleton::load(MFnSkinCluster* pSkinCluster)
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
extern void extractTranMatrix(const MMatrix& tm,float translation[3],float rotation[4],float scale[3]);

// Load a joint
void skeleton::loadJoint(MDagPath& jointDag,joint* parent)
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
		MMatrix worldMatrix = jointDag.inclusiveMatrix();
		/*float translation1[3];
		float rotation1[4];
		float scale1[3];
		extractTranMatrix(worldMatrix,translation1,rotation1,scale1);
		Quaternion q(rotation1[0],rotation1[1],rotation1[2],rotation1[3]);
		float angle;
		Vector3 axis;
		q.ToAngleAxis(angle,axis);
		Vector3 x,y,z;
		q.ToAxes(x,y,z);*/
		//printMatrix(worldMatrix);
	
		// Calculate scaling factor inherited by parent
		// Calculate Local Matrix
		MMatrix localMatrix = worldMatrix;
		if (parent)
			localMatrix = worldMatrix * parent->worldMatrix.inverse();
		float translation2[3];
		float rotation2[4];
		float scale2[3];
		extractTranMatrix(worldMatrix,translation2,rotation2,scale2);
		//printMatrix(localMatrix);
	
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

		//printMatrix(worldMatrix);

		/*MQuaternion q;
		q = worldMatrix;
		newJoint.tran.q[0] = (float)q.x;
		newJoint.tran.q[1] = (float)q.y;
		newJoint.tran.q[2] = (float)q.z;
		newJoint.tran.q[3] = (float)q.w;
		newJoint.tran.t[0] = (float)worldMatrix[3][0];
		newJoint.tran.t[1] = (float)worldMatrix[3][1];
		newJoint.tran.t[2] = (float)worldMatrix[3][2];*/

		MPlug plug = jointFn.findPlug("unRibbonEnabled");
		if(!plug.isNull())
		{
			bool enabled;
			plug.getValue(enabled);
			if(enabled)
			{
				plug = jointFn.findPlug("unRibbonVisible");
				bool visible;
				plug.getValue(visible);
				plug = jointFn.findPlug("unRibbonAbove");
				float above;
				plug.getValue(above);
				plug = jointFn.findPlug("unRibbonBelow");
				float below;
				plug.getValue(below);
				plug = jointFn.findPlug("unRibbonEdgesPerSecond");
				short edgePerSecond;
				plug.getValue(edgePerSecond);
				plug = jointFn.findPlug("unRibbonEdgeLife");
				float edgeLife;
				plug.getValue(edgeLife);
				plug = jointFn.findPlug("unRibbonGravity");
				float gravity;
				plug.getValue(gravity);
				plug = jointFn.findPlug("unRibbonTextureRows");
				short rows;
				plug.getValue(rows);
				plug = jointFn.findPlug("unRibbonTextureCols");
				short cols;
				plug.getValue(cols);
				plug = jointFn.findPlug("unRibbonTextureSlot");
				short slot;
				plug.getValue(slot);
				plug = jointFn.findPlug("unRibbonVertexColor");
				MObject object;
				plug.getValue(object);
				MFnNumericData data(object);
				float r,g,b;
				data.getData(r,g,b);
				plug = jointFn.findPlug("unRibbonVertexAlpha");
				float alpha;
				plug.getValue(alpha);
				plug = jointFn.findPlug("unRibbonBlendMode");
				short blendMode;
				plug.getValue(blendMode);
				plug = jointFn.findPlug("unRibbonTextureFilename");

				MItDependencyGraph dgIt(plug, MFn::kFileTexture,
					MItDependencyGraph::kUpstream, 
					MItDependencyGraph::kBreadthFirst,
					MItDependencyGraph::kNodeLevel);

				dgIt.disablePruningOnFilter();

				MString textureName;
				if (!dgIt.isDone())
				{
					MObject textureNode = dgIt.thisNode();
					MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
					filenamePlug.getValue(textureName);
				}
				else
				{
					char str[256];
					sprintf(str,"%s ribbon system must has file-texture",newJoint.name.asChar());
					MessageBox(0,str,0,0);
				}

				newJoint.hasRibbonSystem = true;
				newJoint.ribbon.visible = visible;
				newJoint.ribbon.above = above;
				newJoint.ribbon.below = below;
				newJoint.ribbon.gravity = gravity;
				newJoint.ribbon.edgePerSecond = edgePerSecond;
				newJoint.ribbon.edgeLife = edgeLife;
				newJoint.ribbon.rows = rows;
				newJoint.ribbon.cols = cols;
				newJoint.ribbon.slot = slot;
				newJoint.ribbon.color[0] = r;
				newJoint.ribbon.color[1] = g;
				newJoint.ribbon.color[2] = b;
				newJoint.ribbon.alpha = alpha;
				newJoint.ribbon.blendMode = blendMode;
				newJoint.ribbon.textureFilename = textureName.asChar();
			}
		}
		plug = jointFn.findPlug("unParticleEnabled");
		if(!plug.isNull())
		{
			bool enabled;
			plug.getValue(enabled);
			if(enabled)
			{
				newJoint.hasParticleSystem = true;

				plug = jointFn.findPlug("unParticleVisible");
				bool visible;
				plug.getValue(visible);
				plug = jointFn.findPlug("unParticleSpeed");
				float speed;
				plug.getValue(speed);
				plug = jointFn.findPlug("unParticleVariationPercent");
				float variation;
				plug.getValue(variation);
				plug = jointFn.findPlug("unParticleConeAngle");
				float coneAngle;
				plug.getValue(coneAngle);
				plug = jointFn.findPlug("unParticleGravity");
				float gravity;
				plug.getValue(gravity);
				plug = jointFn.findPlug("unParticleExplosiveForce");
				float explosiveForce = 0.0f;
				if(!plug.isNull())
				{
					plug.getValue(explosiveForce);
				}
				plug = jointFn.findPlug("unParticleLife");
				float life;
				plug.getValue(life);
				plug = jointFn.findPlug("unParticleLifeVariation");
				float lifeVar;
				if(plug.isNull())
				{
					lifeVar = 0.0f;
				}
				else
				{
					plug.getValue(lifeVar);
				}
				plug = jointFn.findPlug("unParticleEmissionRate");
				float emissionRate;
				plug.getValue(emissionRate);
				plug = jointFn.findPlug("unParticleLimitNum");
				short limitNum;
				plug.getValue(limitNum);
				plug = jointFn.findPlug("unParticleInitialNum");
				short initialNum = 0;
				if(!plug.isNull())plug.getValue(initialNum);
				plug = jointFn.findPlug("unParticleAttachToEmitter");
				bool attachToEmitter;
				plug.getValue(attachToEmitter);
				plug = jointFn.findPlug("unParticleMoveWithEmitter");
				bool moveWithEmitter = false;
				if(!plug.isNull())plug.getValue(moveWithEmitter);
				//23
				plug = jointFn.findPlug("unParticleForTheSword");
				bool forTheSword = false;
				if(!plug.isNull())plug.getValue(forTheSword);
				//24
				plug = jointFn.findPlug("unParticleForTheSwordInitialAngle");
				float forTheSwordInitialAngle = 0;
				if(!plug.isNull())plug.getValue(forTheSwordInitialAngle);
				//25
				plug = jointFn.findPlug("unParticleWander");
				bool wander = false;
				if(!plug.isNull())plug.getValue(wander);
				//25
				plug = jointFn.findPlug("unParticleWanderRadius");
				float wanderRadius = 0.0f;
				if(!plug.isNull())plug.getValue(wanderRadius);
				//25
				plug = jointFn.findPlug("unParticleWanderSpeed");
				float wanderSpeed = 0.0f;
				if(!plug.isNull())plug.getValue(wanderSpeed);
				plug = jointFn.findPlug("unParticleAspectRatio");
				float aspectRatio;
				if(plug.isNull())
				{
					aspectRatio = 1.0f;
				}
				else
				{
					plug.getValue(aspectRatio);
				}
				plug = jointFn.findPlug("unParticleInitialAngleBegin");
				float angleBegin;
				if(plug.isNull())
				{
					angleBegin = 0.0f;
				}
				else
				{
					plug.getValue(angleBegin);
				}
				plug = jointFn.findPlug("unParticleInitialAngleEnd");
				float angleEnd;
				if(plug.isNull())
				{
					angleEnd = 0.0f;
				}
				else
				{
					plug.getValue(angleEnd);
				}
				plug = jointFn.findPlug("unParticleRotationSpeed");
				float rotationSpeed;
				if(plug.isNull())
				{
					rotationSpeed = 0;
				}
				else
				{
					plug.getValue(rotationSpeed);
				}
				plug = jointFn.findPlug("unParticleRotationSpeedVar");
				float rotationSpeedVar;
				if(plug.isNull())
				{
					rotationSpeedVar = 0;
				}
				else
				{
					plug.getValue(rotationSpeedVar);
				}
				plug = jointFn.findPlug("unParticleEmitterWidth");
				float width;
				plug.getValue(width);
				plug = jointFn.findPlug("unParticleEmitterLength");
				float length;
				plug.getValue(length);
				plug = jointFn.findPlug("unParticleEmitterHeight");
				float height = 0.0f;
				if(!plug.isNull())
				{
					plug.getValue(height);
				}
				plug = jointFn.findPlug("unParticleBlendMode");
				short blendMode;
				plug.getValue(blendMode);
				plug = jointFn.findPlug("unParticleTextureFilename");

				MItDependencyGraph dgIt(plug, MFn::kFileTexture,
					MItDependencyGraph::kUpstream, 
					MItDependencyGraph::kBreadthFirst,
					MItDependencyGraph::kNodeLevel);

				dgIt.disablePruningOnFilter();

				MString textureName;
				if (!dgIt.isDone())
				{
					MObject textureNode = dgIt.thisNode();
					MPlug filenamePlug = MFnDependencyNode(textureNode).findPlug("fileTextureName");
					filenamePlug.getValue(textureName);
				}
				else
				{
					char str[256];
					sprintf(str,"%s particle system must has file-texture",newJoint.name.asChar());
					MessageBox(0,str,0,0);
				}
				plug = jointFn.findPlug("unParticleTextureRows");
				short rows;
				plug.getValue(rows);
				plug = jointFn.findPlug("unParticleTextureCols");
				short cols;
				plug.getValue(cols);
				plug = jointFn.findPlug("unParticleTextureChangeStyle");
				short changeStyle;
				if(plug.isNull())
				{
					//0 - 顺序
					//1 - 随机
					changeStyle = 0;
				}
				else
				{
					plug.getValue(changeStyle);
				}
				plug = jointFn.findPlug("unParticleTextureChangeInterval");
				short changeInterval;
				if(plug.isNull())
				{
					//默认30ms换一个
					changeInterval = 30;
				}
				else
				{
					plug.getValue(changeInterval);
				}
				plug = jointFn.findPlug("unParticleTailLength");
				float tailLength;
				plug.getValue(tailLength);
				plug = jointFn.findPlug("unParticleTimeMiddle");
				float timeMiddle;
				plug.getValue(timeMiddle);
				plug = jointFn.findPlug("unParticleColorStart");
				MObject object;
				plug.getValue(object);
				MFnNumericData dataS(object);
				float colorStart[3];
				dataS.getData(colorStart[0],colorStart[1],colorStart[2]);
				plug = jointFn.findPlug("unParticleColorMiddle");
				plug.getValue(object);
				MFnNumericData dataM(object);
				float colorMiddle[3];
				dataM.getData(colorMiddle[0],colorMiddle[1],colorMiddle[2]);
				plug = jointFn.findPlug("unParticleColorEnd");
				plug.getValue(object);
				MFnNumericData dataE(object);
				float colorEnd[3];
				dataE.getData(colorEnd[0],colorEnd[1],colorEnd[2]);
				plug = jointFn.findPlug("unParticleAlpha");
				plug.getValue(object);
				MFnNumericData dataAlpha(object);
				float alpha[3];
				dataAlpha.getData(alpha[0],alpha[1],alpha[2]);
				//Scale
				plug = jointFn.findPlug("unParticleScale");
				plug.getValue(object);
				MFnNumericData dataScale(object);
				float scale[3];
				dataScale.getData(scale[0],scale[1],scale[2]);
				//ScaleVar
				plug = jointFn.findPlug("unParticleScaleVar");
				float scaleVar[3] = {0.0f,0.0f,0.0f};
				if(!plug.isNull())
				{
					plug.getValue(object);
					MFnNumericData dataScaleVar(object);
					dataScaleVar.getData(scaleVar[0],scaleVar[1],scaleVar[2]);
				}
				//FixedSize
				plug = jointFn.findPlug("unParticleFixedSize");
				bool fixedSize = false;
				if(!plug.isNull())
				{
					plug.getValue(fixedSize);
				}
				//HeadLifeSpan
				plug = jointFn.findPlug("unParticleHeadLifeSpan");
				plug.getValue(object);
				MFnNumericData dataHeadLifeSpan(object);
				short headLifeSpan[3];
				dataHeadLifeSpan.getData(headLifeSpan[0],headLifeSpan[1],headLifeSpan[2]);
				plug = jointFn.findPlug("unParticleHeadDecay");
				plug.getValue(object);
				MFnNumericData dataHeadDecay(object);
				short headDecay[3];
				dataHeadDecay.getData(headDecay[0],headDecay[1],headDecay[2]);
				plug = jointFn.findPlug("unParticleTailLifeSpan");
				plug.getValue(object);
				MFnNumericData dataTailLifeSpan(object);
				short tailLifeSpan[3];
				dataTailLifeSpan.getData(tailLifeSpan[0],tailLifeSpan[1],tailLifeSpan[2]);
				plug = jointFn.findPlug("unParticleTailDecay");
				plug.getValue(object);
				MFnNumericData dataTailDecay(object);
				short tailDecay[3];
				dataTailDecay.getData(tailDecay[0],tailDecay[1],tailDecay[2]);
				plug = jointFn.findPlug("unParticleHead");
				bool head;
				plug.getValue(head);
				plug = jointFn.findPlug("unParticleTail");
				bool tail;
				plug.getValue(tail);
				plug = jointFn.findPlug("unParticleUnShaded");
				bool unshaded;
				plug.getValue(unshaded);
				plug = jointFn.findPlug("unParticleUnFogged");
				bool unfogged;
				plug.getValue(unfogged);
				plug = jointFn.findPlug("unParticleBlockByY0");
				bool blockByY0 = false;
				if(!plug.isNull())
					plug.getValue(blockByY0);

				newJoint.particle.visible = visible;
				newJoint.particle.speed = speed;
				newJoint.particle.variation = variation / 100.0f;
				newJoint.particle.coneAngle = coneAngle;
				newJoint.particle.gravity = gravity;
				newJoint.particle.explosiveForce = explosiveForce;
				newJoint.particle.life = life;
				newJoint.particle.lifeVar = lifeVar;
				newJoint.particle.emissionRate = emissionRate;
				newJoint.particle.initialNum = initialNum;
				newJoint.particle.limitNum = limitNum;
				newJoint.particle.attachToEmitter = attachToEmitter;
				newJoint.particle.moveWithEmitter = moveWithEmitter;
				newJoint.particle.forTheSword = forTheSword;
				newJoint.particle.forTheSwordInitialAngle = forTheSwordInitialAngle;
				newJoint.particle.wander = wander;
				newJoint.particle.wanderRadius = wanderRadius;
				newJoint.particle.wanderSpeed = wanderSpeed;
				newJoint.particle.aspectRatio = aspectRatio;
				newJoint.particle.initialAngleBegin = angleBegin;
				newJoint.particle.initialAngleEnd = angleEnd;
				newJoint.particle.rotationSpeed = rotationSpeed;
				newJoint.particle.rotationSpeedVar = rotationSpeedVar;
				newJoint.particle.width = width;
				newJoint.particle.length = length;
				newJoint.particle.height = height;
				newJoint.particle.blendMode = blendMode;
				newJoint.particle.textureFilename = textureName.asChar();
				newJoint.particle.textureRows = rows;
				newJoint.particle.textureCols = cols;
				newJoint.particle.changeStyle = changeStyle;
				newJoint.particle.changeInterval = changeInterval;
				newJoint.particle.tailLength = tailLength;
				newJoint.particle.timeMiddle = timeMiddle;
				newJoint.particle.colorStart[0] = colorStart[0];
				newJoint.particle.colorStart[1] = colorStart[1];
				newJoint.particle.colorStart[2] = colorStart[2];
				newJoint.particle.colorMiddle[0] = colorMiddle[0];
				newJoint.particle.colorMiddle[1] = colorMiddle[1];
				newJoint.particle.colorMiddle[2] = colorMiddle[2];
				newJoint.particle.colorEnd[0] = colorEnd[0];
				newJoint.particle.colorEnd[1] = colorEnd[1];
				newJoint.particle.colorEnd[2] = colorEnd[2];
				newJoint.particle.alpha[0] = alpha[0];
				newJoint.particle.alpha[1] = alpha[1];
				newJoint.particle.alpha[2] = alpha[2];
				newJoint.particle.scale[0] = scale[0];
				newJoint.particle.scale[1] = scale[1];
				newJoint.particle.scale[2] = scale[2];
				newJoint.particle.scaleVar[0] = scaleVar[0];
				newJoint.particle.scaleVar[1] = scaleVar[1];
				newJoint.particle.scaleVar[2] = scaleVar[2];
				newJoint.particle.fixedSize = fixedSize;
				newJoint.particle.headLifeSpan[0] = headLifeSpan[0];
				newJoint.particle.headLifeSpan[1] = headLifeSpan[1];
				newJoint.particle.headLifeSpan[2] = headLifeSpan[2];
				newJoint.particle.headDecay[0] = headDecay[0];
				newJoint.particle.headDecay[1] = headDecay[1];
				newJoint.particle.headDecay[2] = headDecay[2];
				newJoint.particle.tailLifeSpan[0] = tailLifeSpan[0];
				newJoint.particle.tailLifeSpan[1] = tailLifeSpan[1];
				newJoint.particle.tailLifeSpan[2] = tailLifeSpan[2];
				newJoint.particle.tailDecay[0] = tailDecay[0];
				newJoint.particle.tailDecay[1] = tailDecay[1];
				newJoint.particle.tailDecay[2] = tailDecay[2];
				newJoint.particle.head = head;
				newJoint.particle.tail = tail;
				newJoint.particle.unshaded = unshaded;
				newJoint.particle.unfogged = unfogged;
				newJoint.particle.blockByY0 = blockByY0;
			}
		}

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


// Load animations
MStatus skeleton::loadAnims()
{
	MStatus stat;

	MTime initialTime = MAnimControl::currentTime();
	m_animations.clear();

	for (int i=0; i<ExportOptions::instance().clipList.size(); i++)
	{
		clipInfo clip = ExportOptions::instance().clipList[i];

		char str[256];
		wsprintf(str,"startFrame = %d,endFrame = %d,stepFrame = %d",clip.startFrame,clip.endFrame,clip.stepFrame);
		MGlobal::displayInfo(str);

		stat = loadClip(clip.name,clip.startFrame,clip.endFrame,clip.stepFrame);
		
		if (stat != MS::kSuccess)
			std::cout << "Fail in load clip!\n";
	}

	MAnimControl::setCurrentTime(initialTime);
	return MS::kSuccess;
}

unsigned int getFps()
{
	switch(MTime::uiUnit())
	{
	case MTime::kSeconds:       return 1;
	case MTime::kMilliseconds:  return 1000;
	case MTime::kGames:         return 15;
	case MTime::kFilm:          return 24;
	case MTime::kPALFrame:      return 25;
	case MTime::kNTSCFrame:     return 30;
	case MTime::kShowScan:      return 48;
	case MTime::kPALField:      return 50;
	case MTime::kNTSCField:     return 60;
	default:                    return 1;
		// We are ignoring the following time-units.
		//case MTime::kHours:       return 1;
		//case MTime::kMinutes:     return 1;
	}
}
#include <math.h>
bool equal(float x,float y)
{
	return fabs(x - y) < 0.000001f;
}

void addKeyFramesFloat(std::vector<keyframeT<float> > *pVector,keyframeT<float> *pData)
{
	size_t size = pVector->size();
	if(size > 0)
	{
		keyframeT<float>& s = (*pVector)[size - 1];
		if(!equal(s.data,pData->data))
		{
			pVector->push_back(*pData);
		}
	}
	else
	{
		pVector->push_back(*pData);
	}
}

void addKeyFramesBool(std::vector<keyframeT<bool> > *pVector,keyframeT<bool> *pData)
{
	size_t size = pVector->size();
	if(size > 0)
	{
		keyframeT<bool>& s = (*pVector)[size - 1];
		if(s.data != pData->data)
		{
			pVector->push_back(*pData);
		}
	}
	else
	{
		pVector->push_back(*pData);
	}
}
#include "math.h"
int IFloor (float fValue) { return int(floor(fValue)); }
int Round (float fValue) { if(fValue > 0)return IFloor(fValue + 0.5f);if(fValue < 0)return -IFloor(-fValue + 0.5f);return 0;}

// Load an animation clip
MStatus skeleton::loadClip(MString clipName,int start,int stop,int rate)
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
	animation a;
	a.name = clipName.asChar();
	
	if(m_animations.size() == 0)
	{
		a.startTime = 0;
		a.endTime = times[times.size()-1] - times[0];
	}
	else
	{
		a.startTime = m_animations[m_animations.size()-1].endTime + 1;
		a.endTime = a.startTime + times[times.size()-1] - times[0];
	}

	m_animations.push_back(a);
	int animIdx = m_animations.size() - 1;

	for (i=0; i<times.size(); i++)
	{
		MAnimControl::setCurrentTime(MTime(times[i],MTime::uiUnit()));
		for (j=0; j<m_joints.size(); j++)
		{
			keyframeTranslation translation;
			keyframeRotation rotation;
			keyframeScale scale;

			joint &jt = m_joints[j];
			int time = times[i] - times[0] + a.startTime;
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

			if(jt.hasRibbonSystem)
			{
				keyframeT<bool> keyframeVisible;
				keyframeT<float> keyframeAbove;
				keyframeT<float> keyframeBelow;
				keyframeT<short> keyframeSlot;
				keyframeT<float3> keyframeColor;
				keyframeT<float> keyframeAlpha;

				MFnIkJoint jointFn(jt.jointDag);
				MPlug plug;

				plug = jointFn.findPlug("unRibbonVisible");
				bool visible;
				plug.getValue(visible);
				plug = jointFn.findPlug("unRibbonAbove");
				float above;
				plug.getValue(above);
				plug = jointFn.findPlug("unRibbonBelow");
				float below;
				plug.getValue(below);
				plug = jointFn.findPlug("unRibbonTextureSlot");
				short slot;
				plug.getValue(slot);
				plug = jointFn.findPlug("unRibbonVertexColor");
				MObject object;
				plug.getValue(object);
				MFnNumericData data(object);
				float r,g,b;
				data.getData(r,g,b);
				plug = jointFn.findPlug("unRibbonVertexAlpha");
				float alpha;
				plug.getValue(alpha);

				keyframeVisible.time = time * frameTime;
				keyframeAbove.time = time * frameTime;
				keyframeBelow.time = time * frameTime;
				keyframeSlot.time = time * frameTime;
				keyframeColor.time = time * frameTime;
				keyframeAlpha.time = time * frameTime;

				keyframeVisible.data = visible;
				keyframeAbove.data = above;
				keyframeBelow.data = below;
				keyframeSlot.data = slot;
				keyframeColor.data[0] = r;
				keyframeColor.data[1] = g;
				keyframeColor.data[2] = b;
				keyframeAlpha.data = alpha;

				addKeyFramesBool(&jt.ribbon.keyframesVisible,&keyframeVisible);
				addKeyFramesFloat(&jt.ribbon.keyframeAbove,&keyframeAbove);
				addKeyFramesFloat(&jt.ribbon.keyframeBelow,&keyframeBelow);
				size = jt.ribbon.keyframeSlot.size();
				if(size > 0)
				{
					keyframeT<short>& s = jt.ribbon.keyframeSlot[size - 1];
					if(s.data == slot)
					{
						jt.ribbon.keyframeSlot.push_back(keyframeSlot);
					}
				}
				else
				{
					jt.ribbon.keyframeSlot.push_back(keyframeSlot);
				}
				size = jt.ribbon.keyframeColor.size();
				if(size > 0)
				{
					keyframeT<float3>& s = jt.ribbon.keyframeColor[size - 1];
					if(!equal(s.data[0],r) || !equal(s.data[1],g) || !equal(s.data[2],b))
					{
						jt.ribbon.keyframeColor.push_back(keyframeColor);
					}
				}
				else
				{
					jt.ribbon.keyframeColor.push_back(keyframeColor);
				}
				addKeyFramesFloat(&jt.ribbon.keyframeAlpha,&keyframeAlpha);
			}
			if(jt.hasParticleSystem)
			{
				keyframeT<bool> keyframeVisible;
				keyframeT<float> keyframeSpeed;
				keyframeT<float> keyframeVariation;
				keyframeT<float> keyframeConeAngle;
				keyframeT<float> keyframeGravity;
				keyframeT<float> keyframeExplosiveForce;
				keyframeT<float> keyframeEmissionRate;
				keyframeT<float> keyframeWidth;
				keyframeT<float> keyframeLength;
				keyframeT<float> keyframeHeight;

				MFnIkJoint jointFn(jt.jointDag);
				MPlug plug;

				plug = jointFn.findPlug("unParticleVisible");
				bool visible;
				plug.getValue(visible);
				plug = jointFn.findPlug("unParticleSpeed");
				float speed;
				plug.getValue(speed);
				plug = jointFn.findPlug("unParticleVariationPercent");
				float variation;
				plug.getValue(variation);
				plug = jointFn.findPlug("unParticleConeAngle");
				float coneAngle;
				plug.getValue(coneAngle);
				plug = jointFn.findPlug("unParticleGravity");
				float gravity;
				plug.getValue(gravity);
				plug = jointFn.findPlug("unParticleExplosiveForce");
				float explosiveForce = 0.0f;
				if(!plug.isNull())
				{
					plug.getValue(explosiveForce);
				}
				plug = jointFn.findPlug("unParticleEmissionRate");
				float emissionRate;
				plug.getValue(emissionRate);
				plug = jointFn.findPlug("unParticleEmitterWidth");
				float width;
				plug.getValue(width);
				plug = jointFn.findPlug("unParticleEmitterLength");
				float length;
				plug.getValue(length);
				plug = jointFn.findPlug("unParticleEmitterHeight");
				float height = 0.0f;
				if(!plug.isNull())
				{
					plug.getValue(height);
				}

				keyframeVisible.time = time * frameTime;
				keyframeSpeed.time = time * frameTime;
				keyframeVariation.time = time * frameTime;
				keyframeConeAngle.time = time * frameTime;
				keyframeGravity.time = time * frameTime;
				keyframeExplosiveForce.time = time * frameTime;
				keyframeEmissionRate.time = time * frameTime;
				keyframeWidth.time = time * frameTime;
				keyframeLength.time = time * frameTime;
				keyframeHeight.time = time * frameTime;

				keyframeVisible.data = visible;
				keyframeSpeed.data = speed;
				keyframeVariation.data = variation / 100.0f;
				keyframeConeAngle.data = coneAngle;
				keyframeGravity.data = gravity;
				keyframeExplosiveForce.data = explosiveForce;
				keyframeEmissionRate.data = emissionRate;
				keyframeWidth.data = width;
				keyframeLength.data = length;
				keyframeHeight.data = height;

				addKeyFramesBool(&jt.particle.keyframesVisible,&keyframeVisible);
				addKeyFramesFloat(&jt.particle.keyframesSpeed,&keyframeSpeed);
				addKeyFramesFloat(&jt.particle.keyframesVariation,&keyframeVariation);
				addKeyFramesFloat(&jt.particle.keyframesConeAngle,&keyframeConeAngle);
				addKeyFramesFloat(&jt.particle.keyframesGravity,&keyframeGravity);
				addKeyFramesFloat(&jt.particle.keyframesExplosiveForce,&keyframeExplosiveForce);
				addKeyFramesFloat(&jt.particle.keyframesEmissionRate,&keyframeEmissionRate);
				addKeyFramesFloat(&jt.particle.keyframesWidth,&keyframeWidth);
				addKeyFramesFloat(&jt.particle.keyframesLength,&keyframeLength);
				addKeyFramesFloat(&jt.particle.keyframesHeight,&keyframeHeight);
			}
		}
	}

	return MS::kSuccess;
}

class Vector3
{
public:
	union {
		struct {
			float x, y, z;        
		};
		float val[3];
	};
	inline Vector3( float fX, float fY, float fZ ) 
		: x( fX ), y( fY ), z( fZ )
	{
	}
	inline Vector3(){}
	inline float length () const
	{
		return sqrtf( x * x + y * y + z * z );
	}
};
class Matrix3
{
public:
	float m[3][3];
	// member access, allows use of construct mat[r][c]
	inline float* operator[] (size_t iRow) const
	{
		return (float*)m[iRow];
	}
};
class Quaternion
{
public:
	float x, y, z, w;
	inline Quaternion(){}
	inline Quaternion(const Matrix3& rot)
	{
		this->FromRotationMatrix(rot);
	}
	void FromRotationMatrix (const Matrix3& kRot)
	{
		// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
		// article "Quaternion Calculus and Fast Animation".

		float fTrace = kRot[0][0]+kRot[1][1]+kRot[2][2];
		float fRoot;

		if ( fTrace > 0.0 )
		{
			// |w| > 1/2, may as well choose w > 1/2
			fRoot = sqrtf(fTrace + 1.0);  // 2w
			w = 0.5*fRoot;
			fRoot = 0.5/fRoot;  // 1/(4w)
			x = (kRot[2][1]-kRot[1][2])*fRoot;
			y = (kRot[0][2]-kRot[2][0])*fRoot;
			z = (kRot[1][0]-kRot[0][1])*fRoot;
		}
		else
		{
			// |w| <= 1/2
			static size_t s_iNext[3] = { 1, 2, 0 };
			size_t i = 0;
			if ( kRot[1][1] > kRot[0][0] )
				i = 1;
			if ( kRot[2][2] > kRot[i][i] )
				i = 2;
			size_t j = s_iNext[i];
			size_t k = s_iNext[j];

			fRoot = sqrtf(kRot[i][i]-kRot[j][j]-kRot[k][k] + 1.0);
			float* apkQuat[3] = { &x, &y, &z };
			*apkQuat[i] = 0.5*fRoot;
			fRoot = 0.5/fRoot;
			w = (kRot[k][j]-kRot[j][k])*fRoot;
			*apkQuat[j] = (kRot[j][i]+kRot[i][j])*fRoot;
			*apkQuat[k] = (kRot[k][i]+kRot[i][k])*fRoot;
		}
	}
};
class Matrix4
{
protected:
	/// The matrix entries, indexed by [row][col].
	union {
		float m[4][4];
		float _m[16];
	};
public:
	inline Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33 )
	{
		m[0][0] = m00;
		m[0][1] = m01;
		m[0][2] = m02;
		m[0][3] = m03;
		m[1][0] = m10;
		m[1][1] = m11;
		m[1][2] = m12;
		m[1][3] = m13;
		m[2][0] = m20;
		m[2][1] = m21;
		m[2][2] = m22;
		m[2][3] = m23;
		m[3][0] = m30;
		m[3][1] = m31;
		m[3][2] = m32;
		m[3][3] = m33;
	}
	void resolve(Vector3& translation,Quaternion& rotation,Vector3& scale)
	{
		Matrix3 mtx;
		scale.x = Vector3(m[0][0],m[1][0],m[2][0]).length();
		scale.y = Vector3(m[0][1],m[1][1],m[2][1]).length();
		scale.z = Vector3(m[0][2],m[1][2],m[2][2]).length();

		if(equal(scale.x,0) || equal(scale.y,0) || equal(scale.z,0))
		{
			//出错了
			return;
		}

		translation.x = m[0][3];
		translation.y = m[1][3];
		translation.z = m[2][3];

		mtx[0][0] = m[0][0] / scale.x;
		mtx[1][0] = m[1][0] / scale.x;
		mtx[2][0] = m[2][0] / scale.x;
		mtx[0][1] = m[0][1] / scale.y;
		mtx[1][1] = m[1][1] / scale.y;
		mtx[2][1] = m[2][1] / scale.y;
		mtx[0][2] = m[0][2] / scale.z;
		mtx[1][2] = m[1][2] / scale.z;
		mtx[2][2] = m[2][2] / scale.z;

		rotation = mtx;
	}
};

void extractTranMatrix(const MMatrix& tm,float translation[3],float rotation[4],float scale[3])
{
	Matrix4 m(tm[0][0],tm[1][0],tm[2][0],tm[3][0],
		tm[0][1],tm[1][1],tm[2][1],tm[3][1],
		tm[0][2],tm[1][2],tm[2][2],tm[3][2],
		tm[0][3],tm[1][3],tm[2][3],tm[3][3]);

	Vector3 t,s;
	Quaternion q;
	m.resolve(t,q,s);

	translation[0] = t.x;
	translation[1] = t.y;
	translation[2] = t.z;

	rotation[0] = q.x;
	rotation[1] = q.y;
	rotation[2] = q.z;
	rotation[3] = q.w;

	scale[0] = s.x;
	scale[1] = s.y;
	scale[2] = s.z;
}
//#include "d3dx10.h"
//#pragma comment(lib, "d3dx10.lib")
void skeleton::loadKeyframe(joint& j,int time,keyframeTranslation& translation,keyframeRotation& rotation,keyframeScale& scale)
{
	MVector position;

	int parentIdx = j.parentIndex;
int DEBUG_TIME = 7;
	MMatrix matrix;
	{
		matrix = j.jointDag.inclusiveMatrix();
		MFnIkJoint jn(j.jointDag);
		if(jn.name() == "L_toe2")
		{
			char str[256];
			sprintf(str,"%5.5f,%5.5f,%5.5f\n\n",matrix[3][0],matrix[3][1],matrix[3][2]);
			OutputDebugString(str);
		}
		if(time == DEBUG_TIME)
		{
			MFnIkJoint jn(j.jointDag);
			if(jn.name() == "L_knee")
			{
				char str[256];
				sprintf(str,"%5.5f,%5.5f,%5.5f,%5.5f\n%5.5f,%5.5f,%5.5f,%5.5f\n%5.5f,%5.5f,%5.5f,%5.5f\n%5.5f,%5.5f,%5.5f,%5.5f\n\n",
					matrix[0][0],matrix[0][1],matrix[0][2],matrix[0][3],
					matrix[1][0],matrix[1][1],matrix[1][2],matrix[1][3],
					matrix[2][0],matrix[2][1],matrix[2][2],matrix[2][3],
					matrix[3][0],matrix[3][1],matrix[3][2],matrix[3][3]);
				//sprintf(str,"%5.5f,%5.5f,%5.5f\n\n",matrix[3][0],matrix[3][1],matrix[3][2]);
				OutputDebugString(str);
//				breakable;
			}
		}

		//printMatrix(matrix);
		MMatrix worldMatrix = j.worldMatrix;
		//printMatrix(worldMatrix);

		MMatrix invWorldMatrix = worldMatrix.inverse();
		//matrix = invWorldMatrix * matrix;

		//MTransformationMatrix mtm = matrix;
		//double q_x,q_y,q_z,q_w;
		//mtm.getRotationQuaternion(q_x,q_y,q_z,q_w);
		//double s_xyz[3];
		//mtm.getScale(s_xyz,MSpace::kWorld);
		//MVector t_xyz = mtm.getTranslation(MSpace::kWorld);

		//D3DXMATRIX d3dM(matrix[0][0],matrix[0][1],matrix[0][2],matrix[0][3],
		//	matrix[1][0],matrix[1][1],matrix[1][2],matrix[1][3],
		//	matrix[2][0],matrix[2][1],matrix[2][2],matrix[2][3],
		//	matrix[3][0],matrix[3][1],matrix[3][2],matrix[3][3]);

		//D3DXVECTOR3 d3dS,d3dT;
		//D3DXQUATERNION d3dQ;
		//HRESULT hr = D3DXMatrixDecompose(&d3dS,&d3dQ,&d3dT,&d3dM);

		//MMatrix matrixT = worldMatrix * matrix;


		//float t[3],q[4],s[3];
		//extractTranMatrix(matrix,t,q,s);

		//Vector3 t1(t[0],t[1],t[2]);
		//Quaternion q1(q_x,q_y,q_z,q_w);//q[0],q[1],q[2],q[3]);
		//Vector3 s1(s[0],s[1],s[2]);
		//Matrix4 m;
		//m.transform(t1,s1,q1);

		//printMatrix(matrix);

		if(j.parentIndex >= 0)
		{
			MMatrix pMatrix = m_joints[parentIdx].jointDag.inclusiveMatrix();
			//printMatrix(pMatrix);
			MMatrix pWorldMatrix = m_joints[parentIdx].worldMatrix;
			//printMatrix(pWorldMatrix);
			MMatrix invpWorldMatrix = pWorldMatrix.inverse();
			//pMatrix = invpWorldMatrix * pMatrix;
			//printMatrix(pMatrix);

			MMatrix pInvMatrix = pMatrix.inverse();
			matrix = matrix * pInvMatrix;
			//printMatrix(matrix);
		}
	}
	if(time == DEBUG_TIME)
	{
		MFnIkJoint jn(j.jointDag);
		if(jn.name() == "L_knee")
		{
			char str[256];
			sprintf(str,"%5.5f,%5.5f,%5.5f,%5.5f\n%5.5f,%5.5f,%5.5f,%5.5f\n%5.5f,%5.5f,%5.5f,%5.5f\n%5.5f,%5.5f,%5.5f,%5.5f\n\n",
				matrix[0][0],matrix[0][1],matrix[0][2],matrix[0][3],
				matrix[1][0],matrix[1][1],matrix[1][2],matrix[1][3],
				matrix[2][0],matrix[2][1],matrix[2][2],matrix[2][3],
				matrix[3][0],matrix[3][1],matrix[3][2],matrix[3][3]);
			OutputDebugString(str);
//			breakable;
		}
	}
	{
		MMatrix matrix	= j.jointDag.inclusiveMatrix() * j.jointDag.exclusiveMatrixInverse();
		//printMatrix(matrix);
		MMatrix worldMatrix = j.localMatrix;
		//printMatrix(worldMatrix);

		MMatrix invWorldMatrix = worldMatrix.inverse();
		matrix = matrix * invWorldMatrix;
		if(j.parentIndex >= 0)
		{
			matrix = matrix * m_joints[j.parentIndex].localMatrix;
		}
		//printMatrix(matrix);
	}

	extractTranMatrix(matrix,translation.v,rotation.q,scale.v);
	if(time >= DEBUG_TIME)
	{
		MFnIkJoint jn(j.jointDag);
		if(jn.name() == "L_leg")
		{
			char str[256];
			sprintf(str,"%5.5f,%5.5f,%5.5f\n\n",translation.v[0],translation.v[1],translation.v[2]);
			OutputDebugString(str);
			sprintf(str,"%5.5f,%5.5f,%5.5f,%5.5f\n\n",rotation.q[0],rotation.q[1],rotation.q[2],rotation.q[3]);
			OutputDebugString(str);
			sprintf(str,"%5.5f,%5.5f,%5.5f\n\n",scale.v[0],scale.v[1],scale.v[2]);
			OutputDebugString(str);

			/*D3DXMATRIX d3dM(matrix[0][0],matrix[0][1],matrix[0][2],matrix[0][3],
				matrix[1][0],matrix[1][1],matrix[1][2],matrix[1][3],
				matrix[2][0],matrix[2][1],matrix[2][2],matrix[2][3],
				matrix[3][0],matrix[3][1],matrix[3][2],matrix[3][3]);

			D3DXVECTOR3 d3dS,d3dT;
			D3DXQUATERNION d3dQ;
			HRESULT hr = D3DXMatrixDecompose(&d3dS,&d3dQ,&d3dT,&d3dM);
			hr = hr;*/

			//rotation.q[0] = -rotation.q[0];
			//rotation.q[1] = -rotation.q[1];
			//rotation.q[2] = -rotation.q[2];
			//rotation.q[3] = -rotation.q[3];
		}
	}

	/*D3DXQUATERNION q1 = D3DXQUATERNION(0.14122,0.09228,-0.82219,0.54365);
	D3DXQUATERNION q2 = D3DXQUATERNION(-0.13556,-0.09684,0.85495,-0.49124);
	D3DXQUATERNION q3;
	D3DXQuaternionSlerp(&q3,&q1,&q2,1-0.63636363);*/
	translation.time = time;
	rotation.time = time;
	scale.time = time;
}


std::vector<joint>& skeleton::getJoints()
{
	return m_joints;
}


// Get animations
std::vector<animation>& skeleton::getAnimations()
{
	return m_animations;
}

