#include "common.h"
#include "writer.h"
#include "stdio.h"

void printMatrix(const MMatrix& mm)
{
#if 0
	for (int iRow = 0; iRow < 4; iRow++)
	{
		for (int iCol = 0; iCol < 4; iCol++)
		{
			char str[256];
			sprintf(str,"%f,",(FLOAT)mm[iRow][iCol]);
			OutputDebugString(str);
		}
		OutputDebugString("\n");
	}
	OutputDebugString("\n\n");
#endif
}

MWriter::MWriter(void)
{
	m_pMesh =NULL;
	m_pSkeleton = NULL;
	m_pMaterialSet =NULL;
}

MWriter::~MWriter(void)
{
	clear();
}

void MWriter::clear()
{
	if (NULL != m_pMesh)
		delete m_pMesh;
	if(NULL != m_pSkeleton)
		delete m_pSkeleton;
	if (NULL != m_pMaterialSet)
		delete m_pMaterialSet;
}
extern unsigned int getFps();
void MWriter::loadMtlKeyframes()
{
	uint fps = getFps();
	float frameTime = 1000.0f / fps;

	MTime initialTime = MAnimControl::currentTime();

	int start = MAnimControl::animationStartTime().value();
	int end = MAnimControl::animationEndTime().value();
	for(int i = start;i <= end;i++)
	{
		MAnimControl::setCurrentTime(MTime(i,MTime::uiUnit()));
		size_t size = this->m_pMaterialSet->m_materials.size();
		for(size_t j = 0;j < size;j++)
		{
			material *pmtl = this->m_pMaterialSet->m_materials[j];
			pmtl->loadKeyframe(frameTime * (i - start));
		}
	}

	MAnimControl::setCurrentTime(initialTime);

}

//#include "bindPoseTool.h"

MStatus MWriter::read()
{
	MStatus stat = MS::kSuccess;

	/*BindPoseTool bindPoseTool;
	bindPoseTool.GoIntoBindPose();
	for (MItDag dagIt(MItDag::kDepthFirst, MFn::kMesh, &stat);
		!dagIt.isDone();
		dagIt.next())
	{
		MDagPath currentMayaMeshPath;
		stat = dagIt.getPath(currentMayaMeshPath);

		unsigned childCount = currentMayaMeshPath.childCount();

		const char * fname = currentMayaMeshPath.fullPathName().asChar();
		const char * pname = currentMayaMeshPath.partialPathName().asChar();
		bool kMesh = currentMayaMeshPath.hasFn(MFn::kMesh);
		bool kTransform = currentMayaMeshPath.hasFn(MFn::kTransform);
		if(kMesh && !kTransform && !childCount)
		{
			MFnMesh mesh(currentMayaMeshPath);
			int vertices = mesh.numVertices();
			if(vertices == 243)
			{
				MPointArray a;
				mesh.getPoints(a);
				float aa[243][4];
				a.get(aa);

				vertices = vertices;
			}
			vertices = vertices;
		}
	}
	bindPoseTool.UndoGoIntoBindPose();
	return MS::kFailure;*/

	clear();
	m_pMesh = new Mesh();
	m_pSkeleton = new skeleton();
	m_pMaterialSet = new MaterialSet();

	if (ExportOptions::instance().exportAll)
	{/*
		MItDag it(MItDag::kDepthFirst,MFn::kInvalid);

		int objectCount = 0;
		for ( ; !it.isDone(); it.next() ) 
		{
			MDagPath path;
			it.getPath(path);
			translateNode(path);
		}*/

		MItDag dagIter;
		MFnDagNode worldDag (dagIter.root());
		MDagPath worldPath;
		worldDag.getPath(worldPath);
		const char *name = worldPath.fullPathName().asChar();
		stat = translateNode(worldPath,0);
	}
	else
	{	
		MSelectionList activeList;
		stat = MGlobal::getActiveSelectionList(activeList);
		if (MS::kSuccess != stat)
		{
			clear();
			return MS::kFailure;
		}

		MItSelectionList iter(activeList);
		for ( ; !iter.isDone(); iter.next())
		{								
			MDagPath dagPath;
			stat = iter.getDagPath(dagPath);	
			stat = translateNode(dagPath,0); 
		}							
	}

	// ���ض���
	if (NULL != m_pSkeleton && ExportOptions::instance().exportAnims)
	{
		m_pSkeleton->loadAnims();
	}

	//���ʶ���
	loadMtlKeyframes();

	return stat;
}

bool isVisible(MDagPath& dagPath)
{
	MStatus status;
    MFnDagNode dagNode(dagPath.node());
	if (dagNode.isIntermediateObject()) return false;

	bool isVisible;
	MPlug visibilityPlug = dagNode.findPlug("visibility", &status);
	if (status)
	{
		visibilityPlug.getValue(isVisible);
		if (!isVisible) return false;
	}

	MPlug lodVisibilityPlug = dagNode.findPlug("lodVisibility", &status);
	if (status)
	{
		lodVisibilityPlug.getValue(isVisible);
		if (!isVisible) return false;
	}

	MPlug overrideEnablePlug = dagNode.findPlug("overrideEnabled", &status);
	if (status)
	{
		bool isOverridden;
		overrideEnablePlug.getValue(isOverridden);
		if (isOverridden)
		{
			MPlug overrideVisibilityPlug = dagNode.findPlug("overrideVisibility", &status);
			if (status)
			{
				overrideVisibilityPlug.getValue(isVisible);
				if (!isVisible) return false;
			}
		}
	}

	return true;
}

MStatus MWriter::translateNode(MDagPath& dagPath,MDagPath *pDagPath)
{
	MStatus stat = MS::kSuccess;

	//MGlobal::displayInfo(dagPath.fullPathName().asChar());
	/*MFn::Type type = dagPath.apiType();

	if(dagPath.hasFn(MFn::kDagNode))
	{
		MFnDagNode fxDag(dagPath);
		MObject m = fxDag.attribute("Hello");
		MFnAttribute a(m);
		MFn::Type tt = a.type();
		const char *an = a.shortName().asChar();
		unsigned num = fxDag.attributeCount();
		const char *name = fxDag.fullPathName().asChar();
		bool b1 = fxDag.hasAttribute("Hello");
		bool b2 = fxDag.hasAttribute("output");
		bool b3 = dagPath.hasFn(MFn::kMesh);
		bool b4 = dagPath.hasFn(MFn::kJoint);
		const char *name1 = fxDag.name().asChar();
		b1 = b1;
		MPlug plug = fxDag.findPlug("Hello");
		MFnTypedAttribute data(m);
		MFnData::Type t = data.attrType();
		double fV;
		if(t == MFnNumericData::kDouble)
		{
			plug.getValue(fV);
		}
		fV = fV;
	}*/

	if(!isVisible(dagPath))
		return stat;

	unsigned childCount = dagPath.childCount();

	const char * fname = dagPath.fullPathName().asChar();
	const char * pname = dagPath.partialPathName().asChar();
	bool kMesh = dagPath.hasFn(MFn::kMesh);
	bool kTransform = dagPath.hasFn(MFn::kTransform);
	if (kMesh && (ExportOptions::instance().exportMesh||ExportOptions::instance().exportMaterials||ExportOptions::instance().exportSkeleton) && (dagPath.childCount() == 0))
	{	
		MDagPath meshDag = dagPath;
		MFnMesh meshFn(meshDag);
		if (!meshFn.isIntermediateObject())
		{	
			stat = m_pMesh->load(meshDag,pDagPath);	
			if (MS::kSuccess != stat)
				std::cout << "Error, mesh skipped\n";
		}
	}

	if(dagPath.hasFn(MFn::kJoint) && ExportOptions::instance().exportSkeleton)
	{
		MDagPath rootDag = dagPath;
		//check if skeleton has already been loaded
		bool skip = false;
		for (int j=0; j<m_pSkeleton->m_joints.size() && !skip; j++)
		{
			//skip skeleton if already loaded
			if (rootDag.partialPathName() == m_pSkeleton->m_joints[j].name)
			{
				skip = true;
			}
		}
		//load joints data from root
		if (!skip)
		{
			//load joints data
			m_pSkeleton->loadJoint(rootDag,NULL);
		}
	}
	
	const char *name = dagPath.partialPathName().asChar();
	// look for meshes and cameras within the node's children
	for (unsigned int i=0; i<dagPath.childCount(); i++)
	{
		MObject child = dagPath.child(i);
		MDagPath childPath;
		stat = MDagPath::getAPathTo(child,childPath);
		if (MS::kSuccess != stat)
		{
			std::cout << "Error retrieving path to child " << i << " of: " << dagPath.fullPathName().asChar();
			return MS::kFailure;
		}
		//MessageBox(0,dagPath.partialPathName().asChar(),0,0);
		//OutputDebugString(dagPath.partialPathName().asChar());
		//OutputDebugString("\n");
		stat = translateNode(childPath,&dagPath);
		if (MS::kSuccess != stat)
			return MS::kFailure;
	}
	return MS::kSuccess;
}

extern unsigned int getFps();

template<class T> void writeKeyFrames(MemoryFileWriter *pStream,std::vector<keyframeT<T> > *pData)
{
	unsigned int size = pData->size();
	pStream->write(&size,sizeof(size));
	for(unsigned int i = 0;i < size;i++)
	{
		keyframeT<T>& kf = (*pData)[i];
		pStream->write(&kf.time,sizeof(kf.time));
		pStream->write(&kf.data,sizeof(kf.data));
	}
}

MStatus MWriter::write()
{
	MStatus stat = MS::kSuccess;

	// ��������
	int nIndices = 0;
	int vstart = 0;
	int istart = 0;
	std::vector<MODELVERTEX_19> vVertexes;
	std::vector<MODELFACE> vFaces;
	std::vector<SUBMESH> vSubmeshes;

	if(m_pMesh && m_pMesh->m_pSkeleton)
	{
		typedef stdext::hash_map<int,int> JointMap;
		JointMap jm;
		//�и����⣬MWriter�и�m_pSkeleton��m_pMesh����Ҳ��һ������2�߲�����ȫ��Ӧ�ģ�������ʱ�����ֹ�����ģ�Ͱ󶨴��ҵ����
		//����һ�Σ�����ȷ��һ��ģ�͵Ĺ���ID
		for(int i = 0;i < m_pMesh->m_submeshes.size();i++)
		{
			SubMesh *subMesh = m_pMesh->m_submeshes[i];
			for(int l = 0;l < subMesh->m_vertices.size();l++)
			{
				vertex& v = subMesh->m_vertices[l];
				for(int k = 0;k < v.vbas.size();k++)
				{
					vba& vb = v.vbas[k];
					//vb.jointIdx
					JointMap::iterator it = jm.find(vb.jointIdx);
					if(it != jm.end())
					{
						vb.jointIdx = it->second;
					}
					else
					{
						joint& j1 = m_pMesh->m_pSkeleton->m_joints[vb.jointIdx];
						for(int m = 0;m < m_pSkeleton->m_joints.size();m++)
						{
							joint& j2 = m_pSkeleton->m_joints[m];
							if(j1.name == j2.name)
							{
								jm[vb.jointIdx] = m;
								vb.jointIdx = m;
							}
						}
					}
				}
			}
		}
	}

	//��ʼ����������
	for (int i=0; i < m_pMesh->m_submeshes.size(); i++)
	{
		SubMesh * subMesh = m_pMesh->m_submeshes[i];

		int matId = -1;
		for(int l = 0;l< m_pMaterialSet->m_materials.size();l++)
		{	
			material* mat = m_pMaterialSet->m_materials[l];
			if (mat == subMesh->m_pMaterial)
			{
				matId = l;
				break;
			}
		}

		SUBMESH sm;
		sm.matId = matId;
		sm.vstart = vstart;
		sm.vcount = subMesh->m_vertices.size();
		sm.istart = istart;
		sm.icount = subMesh->m_faces.size()*3;	
		vstart += subMesh->m_vertices.size();
		istart += subMesh->m_faces.size()*3;
		sm.animated = subMesh->m_bAnimated;
		strcpy(sm.name,subMesh->m_name.asChar());
		vSubmeshes.push_back(sm);

		for(int l = 0;l<subMesh->m_faces.size();l++)
		{
			MODELFACE face;
			face.index[0] = nIndices+subMesh->m_faces[l].v[0];
			face.index[1] = nIndices+subMesh->m_faces[l].v[1];
			face.index[2] = nIndices+subMesh->m_faces[l].v[2];
			vFaces.push_back(face);
		}

		nIndices += (int) subMesh->m_vertices.size();

		for(int l= 0;l< subMesh->m_vertices.size();l++)
		{
			MODELVERTEX_19 mv;
			vertex vtx = subMesh->m_vertices[l];
			mv.pos[0] = vtx.x; mv.pos[1] = vtx.y; mv.pos[2] = vtx.z;
			mv.normal[0] = vtx.n.x; mv.normal[1] = vtx.n.y;mv.normal[2] = vtx.n.z;
			mv.color[0] = vtx.r; mv.color[1] = vtx.g; mv.color[2] = vtx.b; mv.color[3] = vtx.a;

			// ��������
			for (size_t k=0; k<m_pMesh->m_uvsets.size(); k++)
			{
				if (k < vtx.texcoords.size())
				{
					mv.texcoords[0] = vtx.texcoords[k].u;
					mv.texcoords[1] = vtx.texcoords[k].v;
				}
				else
				{
					if(vtx.texcoords.size() > 0)
					{
						mv.texcoords[0] = vtx.texcoords[0].u;
						mv.texcoords[1] = vtx.texcoords[0].v;
					}
					else
					{
						mv.texcoords[0] = 0.0f;
						mv.texcoords[1] = 0.0f;
					}
				}
			} 
			for(int j = 0;j < 4;j++)	// ֻ����Ӱ�����Ĺ���
			{
				mv.bones[j] = -1;
				mv.weights[j] = 0;

				if(vtx.vbas.size() < 4)
				{
					if(j < vtx.vbas.size())
					{	
						mv.bones[j] = vtx.vbas[j].jointIdx;
						mv.weights[j] = vtx.vbas[j].weight;
					}
				}
				else
				{
					std::vector<vba> vb = vtx.vbas;
					for(int k =0;k<vb.size()-1;k++)
					{
						for(int l = k+1;l<vb.size();l++)
						{
							if(vb[k].weight < vb[l].weight)
							{
								vba a = vb[k];
								vb[k] = vb[l];
								vb[l] = a;
							}
						}
					}

					float fTotal = vb[0].weight + vb[1].weight+ vb[2].weight+ vb[3].weight;
					mv.bones[j] = vb[j].jointIdx;
					mv.weights[j] = vb[j].weight/fTotal;
				}
			}

			vVertexes.push_back(mv);
		}
	}

	//============================================================================
	// ��ʼд������
	ChunkSet writeChunk;

	// �汾��
	writeChunk.beginChunk("MVER");	
	uint ver = 29;
	//�汾29������uv��ת���Źؼ�֡
	//(��½�����汾��Ȼ��28��)
	//�汾27������additiveLayer
	//�汾26��������ͼflowU��flowV��changeStyle�ؼ���
	//�汾25������Wander��������һ��������������ǻ�
	//�汾24�����Ӳ���forTheSwordInitialAngle��������ͼ�ķ���ָ���ҷ�
	//�汾23�����Ӳ���forTheSword����ѡ�д˲�����ͼ��Զ˳�����ӵ��ٶȷ�����ͼ����һ�ѽ�����ͷ��Ҫָ���ҷ�
	//�汾19��������int��Ϊshort����ʡ�˲��ٿռ䣬���ⶥ����ɫֻ������Ҫ��ʱ��ŵ���
	//7�汾����Բ���alpha keyframe��֧��
	writeChunk.write(&ver,sizeof(ver));
	uchar modelingSoftware = 0;
	writeChunk.write(&modelingSoftware, sizeof(modelingSoftware));
	writeChunk.endChunk();

	// ģ�Ͳ���
	writeChunk.beginChunk("MSUB");
	//writeSequence(writeChunk.getCurrentMemoryFileWriter(),vSubmeshes);
	for(size_t i = 0;i < vSubmeshes.size();i++)
	{
		SUBMESH &sm = vSubmeshes[i];
		SUBMESH_BASE sb;
		sb.animated = sm.animated;
		sb.icount = sm.icount;
		sb.istart = sm.istart;
		sb.matId = sm.matId;
		sb.vcount = sm.vcount;
		sb.vstart = sm.vstart;
		sb.nameLen = strlen(sm.name);
		writeChunk.write(&sb,sizeof(sb));
		writeChunk.write(sm.name,sb.nameLen);
	}
	writeChunk.endChunk();

	// ����
	writeChunk.beginChunk("MVTX");
	//�汾��15�����Ƿ񵼳�������ɫ����ʵ�ϰ汾����ɫ���������ˣ���û��ʵ��ȥ��
	bool exportVertexColor = ExportOptions::instance().exportVertexColour;
	writeChunk.write(&exportVertexColor,sizeof(exportVertexColor));
	//�汾��19���������������ɫ���Ͳ�������ļ�
	if(ExportOptions::instance().exportVertexColour)
	{
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),vVertexes);
	}
	else
	{
		for(size_t i = 0;i < vVertexes.size();i++)
		{
			MODELVERTEX_19& mv = vVertexes[i];
			writeChunk.write(&mv,sizeof(MODELVERTEX_19BASE));
		}
	}
	writeChunk.endChunk();

	// ����
	writeChunk.beginChunk("MFAC");
	writeSequence(writeChunk.getCurrentMemoryFileWriter(), vFaces);
	writeChunk.endChunk();

	// ���ʡ���������
	writeChunk.beginChunk("MMTX");
	uint nMaterials = m_pMaterialSet->m_materials.size();
	writeChunk.write(&nMaterials,sizeof(nMaterials));
	for(int i = 0;i<nMaterials;i++)
	{
		material* mat = m_pMaterialSet->m_materials[i];

		// ������
		std::string matName = mat->m_name.asChar();
		uchar matNameLen = matName.size();
		writeChunk.write(&matNameLen,sizeof(matNameLen));
		writeChunk.write(matName.c_str(),matNameLen);
		
		// lighting
		uchar lighting = mat->m_lightingOff;
		writeChunk.write(&lighting,sizeof(lighting));

		// ambient diffuse specular emissive
		writeChunk.write(&mat->m_ambient,sizeof(mat->m_ambient));
		writeChunk.write(&mat->m_diffuse,sizeof(mat->m_diffuse));
		writeChunk.write(&mat->m_specular,sizeof(mat->m_specular));
		writeChunk.write(&mat->m_emissive,sizeof(mat->m_emissive));
		uchar iTrans = true;
		writeChunk.write(&iTrans,sizeof(iTrans));
		uchar twoSide = mat->m_twoSide;
		writeChunk.write(&twoSide,sizeof(twoSide));
		uchar tcFlowU = mat->m_tcFlowU;
		writeChunk.write(&tcFlowU,sizeof(tcFlowU));
		uchar tcFlowV = mat->m_tcFlowV;
		writeChunk.write(&tcFlowV,sizeof(tcFlowV));
		//�汾��29����RotateSpeed�ؼ���
		uchar tcRotate = mat->m_tcRotate;
		writeChunk.write(&tcRotate,sizeof(tcRotate));

		//�汾��29����uvλ�ýǶȸ�λ
		uchar tcAngleReset = mat->m_tcAngleReset;
		writeChunk.write(&tcAngleReset,sizeof(tcAngleReset));

		uchar tcPositionReset = mat->m_tcPositionReset;
		writeChunk.write(&tcPositionReset,sizeof(tcPositionReset));

		// static flow speedU
		float tcFlowSpeedU = mat->m_tcFlowSpeedU;
		writeChunk.write(&tcFlowSpeedU,sizeof(tcFlowSpeedU));
		//�汾��26����FlowSpeedU�ؼ���
		uint nKeyframes = mat->m_vTcFlowSpeedU.size();
		writeChunk.write(&nKeyframes,sizeof(nKeyframes));
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),mat->m_vTcFlowSpeedU);
		// static flow speedV
		float tcFlowSpeedV = mat->m_tcFlowSpeedV;
		writeChunk.write(&tcFlowSpeedV,sizeof(tcFlowSpeedV));
		//�汾��26����FlowSpeedV�ؼ���
		nKeyframes = mat->m_vTcFlowSpeedV.size();
		writeChunk.write(&nKeyframes,sizeof(nKeyframes));
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),mat->m_vTcFlowSpeedV);
		//�汾��28����flow time������Ϊ�����½��������
		ushort ft = 0.0f;
		writeChunk.write(&ft,sizeof(ft));

		//�汾��29����RotateSpeed�ؼ���
		nKeyframes = mat->m_vRotateSpeed.size();
		writeChunk.write(&nKeyframes,sizeof(nKeyframes));
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),mat->m_vRotateSpeed);
		//9�汾������ʲ�ͶӰ
		writeChunk.write(&mat->m_dontCastShadow,sizeof(mat->m_dontCastShadow));
		//12�汾������ʲ����
		writeChunk.write(&mat->m_unlit,sizeof(mat->m_unlit));
		//14�汾������Ȳ��Ժ�д���ѡ��
		writeChunk.write(&mat->m_noDepthCheck,sizeof(mat->m_noDepthCheck));
		writeChunk.write(&mat->m_noDepthWrite,sizeof(mat->m_noDepthWrite));
		//27�汾����additiveLayer
		writeChunk.write(&mat->m_additiveLayer,sizeof(mat->m_additiveLayer));

		//7�汾����͸����
		float transparency = mat->m_transparency;
		writeChunk.write(&transparency,sizeof(transparency));
		//7�汾����͸����keyframe
		nKeyframes = mat->m_vTransparencies.size();
		writeChunk.write(&nKeyframes,sizeof(nKeyframes));
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),mat->m_vTransparencies);
		//10�汾���������ɫ
		colorNalpha color = mat->m_color;
		writeChunk.write(&color,sizeof(color));
		//10�汾���������ɫ�ؼ���
		nKeyframes = mat->m_vColors.size();
		writeChunk.write(&nKeyframes,sizeof(nKeyframes));
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),mat->m_vColors);
		//�汾7��������ͼ֧�֣�����4������
		writeChunk.write(&mat->m_rows,sizeof(mat->m_rows));
		writeChunk.write(&mat->m_cols,sizeof(mat->m_cols));
		writeChunk.write(&mat->m_changeStyle,sizeof(mat->m_changeStyle));
		//�汾��26����ChangeStyle�ؼ���
		nKeyframes = mat->m_vChangeStyle.size();
		writeChunk.write(&nKeyframes,sizeof(nKeyframes));
		writeSequence(writeChunk.getCurrentMemoryFileWriter(),mat->m_vChangeStyle);
		writeChunk.write(&mat->m_changeInterval,sizeof(mat->m_changeInterval));

		// ����,ȱʡΪһ������
		uint nTextures = mat->m_textures.size();
		writeChunk.write(&nTextures,sizeof(nTextures));
		for(int j= 0;j < nTextures;j++)
		{	
			texture tex = mat->m_textures[j];
			std::string strFilename = tex.filename.asChar();
			uchar filenameLen = strFilename.size();
			writeChunk.write(&filenameLen,sizeof(filenameLen));
			writeChunk.write(strFilename.c_str(),filenameLen);
			uchar blendModes = tex.blendModes;
			writeChunk.write(&blendModes,sizeof(blendModes));
		}

		if(ExportOptions::instance().copyTextures)
		{
			mat->copyTextures();
		}
	}
	writeChunk.endChunk();

	if(NULL != m_pSkeleton)
	{
		uint fps = getFps();
		float frameTime = 1000.0f / fps;
		// ����
		writeChunk.beginChunk("MANM");
		uint nAnim = m_pSkeleton->m_animations.size();
		writeChunk.write(&nAnim,sizeof(nAnim));
		for (int i=0; i<nAnim; i++)
		{	
			animation anim = m_pSkeleton->m_animations[i];
			std::string strAnimname = anim.name.asChar();
			uchar AnimnameLen = strAnimname.size();
			writeChunk.write(&AnimnameLen,sizeof(AnimnameLen));
			writeChunk.write(strAnimname.c_str(),AnimnameLen);

			uint startTime = anim.startTime * frameTime;
			uint endTime = anim.endTime * frameTime;
			writeChunk.write(&startTime,sizeof(startTime));
			writeChunk.write(&endTime,sizeof(endTime));
		}
		writeChunk.endChunk();

		//�������
		writeChunk.beginChunk("MWRD");
		{
			uint numBones = m_pSkeleton->m_joints.size();
			writeChunk.write(&numBones,sizeof(numBones));

			for(int i = 0;i < numBones;i++)
			{
				joint jt = m_pSkeleton->m_joints[i];
				writeChunk.write(&jt.tran,sizeof(jt.tran));
			}
		}
		writeChunk.endChunk();
		// ��������
		writeChunk.beginChunk("MBON");
		{
			uint numBones = m_pSkeleton->m_joints.size();
			writeChunk.write(&numBones,sizeof(numBones));

			for(int i = 0;i < numBones;i++)
			{
				joint jt = m_pSkeleton->m_joints[i];
				MODELBONE mb;
				mb.id = jt.id;
				writeChunk.write(&mb.id,sizeof(mb.id));

				MFnIkJoint jn(jt.jointDag);
				if(jn.name() == "Hips")
				{
					//breakable;
				}
				std::string strJointname = jn.name().asChar();
				uchar JointnameLen = strJointname.size();
				writeChunk.write(&JointnameLen,sizeof(JointnameLen));
				writeChunk.write(strJointname.c_str(),JointnameLen);

				mb.parent = -1;
				if(jt.parentIndex >=0)
				{
					mb.parent = m_pSkeleton->m_joints[jt.parentIndex].id;
				}
				writeChunk.write(&mb.parent,sizeof(mb.parent));
				mb.tran = jt.tran;

				//�����������ϵͳ����ô��Ҫ���������ĳ�ʼ����
				//�����ڳ�ʼ������£������ͻ�ӵ�з�����
				//��ͨ�Ľ�ɫģ�ͣ�������ʼ��û�з���ģ�ֻ��һ��pivot
				//�汾��Ϊ5��ʱ�򣬱�������tran matrix
					//writeChunk.write(&mb.tran.m_mat[3][0],sizeof(float));
					//writeChunk.write(&mb.tran.m_mat[3][1],sizeof(float));
					//writeChunk.write(&mb.tran.m_mat[3][2],sizeof(float));

				//5 - ����
				writeChunk.write(&mb.tran,sizeof(mb.tran));

				uint nKeyframes = jt.keyframesTranslation.size();
				writeChunk.write(&nKeyframes,sizeof(nKeyframes));
				writeSequence(writeChunk.getCurrentMemoryFileWriter(),jt.keyframesTranslation);

				nKeyframes = jt.keyframesRotation.size();
				writeChunk.write(&nKeyframes,sizeof(nKeyframes));
				writeSequence(writeChunk.getCurrentMemoryFileWriter(),jt.keyframesRotation);

				nKeyframes = jt.keyframesScale.size();
				writeChunk.write(&nKeyframes,sizeof(nKeyframes));
				writeSequence(writeChunk.getCurrentMemoryFileWriter(),jt.keyframesScale);

				writeChunk.write(&jt.hasRibbonSystem,sizeof(jt.hasRibbonSystem));
				writeChunk.write(&jt.hasParticleSystem,sizeof(jt.hasParticleSystem));
				if(jt.hasRibbonSystem)
				{
					writeChunk.write(&jt.ribbon.visible,sizeof(jt.ribbon.visible));
					writeChunk.write(&jt.ribbon.above,sizeof(jt.ribbon.above));
					writeChunk.write(&jt.ribbon.below,sizeof(jt.ribbon.below));
					writeChunk.write(&jt.ribbon.edgePerSecond,sizeof(jt.ribbon.edgePerSecond));
					writeChunk.write(&jt.ribbon.edgeLife,sizeof(jt.ribbon.edgeLife));
					writeChunk.write(&jt.ribbon.gravity,sizeof(jt.ribbon.gravity));
					writeChunk.write(&jt.ribbon.rows,sizeof(jt.ribbon.rows));
					writeChunk.write(&jt.ribbon.cols,sizeof(jt.ribbon.cols));
					writeChunk.write(&jt.ribbon.slot,sizeof(jt.ribbon.slot));
					writeChunk.write(&jt.ribbon.color,sizeof(jt.ribbon.color));
					writeChunk.write(&jt.ribbon.alpha,sizeof(jt.ribbon.alpha));
					writeChunk.write(&jt.ribbon.blendMode,sizeof(jt.ribbon.blendMode));

					std::string filename = jt.ribbon.textureFilename.substr(jt.ribbon.textureFilename.rfind('/') + 1,jt.ribbon.textureFilename.length() - 1);

					uchar textureNameLen = filename.size();
					writeChunk.write(&textureNameLen,sizeof(textureNameLen));
					writeChunk.write(filename.c_str(),textureNameLen);

					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.ribbon.keyframesVisible);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.ribbon.keyframeAbove);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.ribbon.keyframeBelow);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.ribbon.keyframeSlot);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.ribbon.keyframeColor);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.ribbon.keyframeAlpha);

					std::string path = ExportOptions::instance().m_outFilePath.asChar();
					std::string outFilename = path + "\\" + filename;
					BOOL overwrite = TRUE;
					/*if(rkt::isFile(outFilename.c_str()))
					{
						char str[256];
						sprintf(str,"%s�Ѿ����ڣ�Ҫ������",outFilename.c_str());
						overwrite = (MessageBox(0,str,"����",MB_YESNO) == IDYES);
					}*/
					CopyFile(jt.ribbon.textureFilename.c_str(),outFilename.c_str(),!overwrite);
				}
				if(jt.hasParticleSystem)
				{
					writeChunk.write(&jt.particle.visible,sizeof(jt.particle.visible));
					writeChunk.write(&jt.particle.speed,sizeof(jt.particle.speed));
					writeChunk.write(&jt.particle.variation,sizeof(jt.particle.variation));
					writeChunk.write(&jt.particle.coneAngle,sizeof(jt.particle.coneAngle));
					writeChunk.write(&jt.particle.gravity,sizeof(jt.particle.gravity));
					//17 - ����
					writeChunk.write(&jt.particle.explosiveForce,sizeof(jt.particle.explosiveForce));
					writeChunk.write(&jt.particle.life,sizeof(jt.particle.life));
					//5 - ����
					writeChunk.write(&jt.particle.lifeVar,sizeof(jt.particle.lifeVar));
					writeChunk.write(&jt.particle.emissionRate,sizeof(jt.particle.emissionRate));
					//17 - ����
					writeChunk.write(&jt.particle.initialNum,sizeof(jt.particle.initialNum));
					//11 - ����
					writeChunk.write(&jt.particle.limitNum,sizeof(jt.particle.limitNum));
					//13 - ����
					writeChunk.write(&jt.particle.attachToEmitter,sizeof(jt.particle.attachToEmitter));
					//18 - ����
					writeChunk.write(&jt.particle.moveWithEmitter,sizeof(jt.particle.moveWithEmitter));
					//23 - ����
					writeChunk.write(&jt.particle.forTheSword,sizeof(jt.particle.forTheSword));
					//24 - ����
					writeChunk.write(&jt.particle.forTheSwordInitialAngle,sizeof(jt.particle.forTheSwordInitialAngle));
					//25 - ����
					writeChunk.write(&jt.particle.wander,sizeof(jt.particle.wander));
					writeChunk.write(&jt.particle.wanderRadius,sizeof(jt.particle.wanderRadius));
					writeChunk.write(&jt.particle.wanderSpeed,sizeof(jt.particle.wanderSpeed));
					//5 - ����
					writeChunk.write(&jt.particle.aspectRatio,sizeof(jt.particle.aspectRatio));
					//6 - ����
					writeChunk.write(&jt.particle.initialAngleBegin,sizeof(jt.particle.initialAngleBegin));
					//6 - ����
					writeChunk.write(&jt.particle.initialAngleEnd,sizeof(jt.particle.initialAngleEnd));
					//6 - ����
					writeChunk.write(&jt.particle.rotationSpeed,sizeof(jt.particle.rotationSpeed));
					//15 - ����
					writeChunk.write(&jt.particle.rotationSpeedVar,sizeof(jt.particle.rotationSpeedVar));
					writeChunk.write(&jt.particle.width,sizeof(jt.particle.width));
					writeChunk.write(&jt.particle.length,sizeof(jt.particle.length));
					//16 - ����
					writeChunk.write(&jt.particle.height,sizeof(jt.particle.height));
					writeChunk.write(&jt.particle.blendMode,sizeof(jt.particle.blendMode));
					writeChunk.write(&jt.particle.textureRows,sizeof(jt.particle.textureRows));
					writeChunk.write(&jt.particle.textureCols,sizeof(jt.particle.textureCols));
					//6 - ����
					writeChunk.write(&jt.particle.changeStyle,sizeof(jt.particle.changeStyle));
					//6 - ����
					writeChunk.write(&jt.particle.changeInterval,sizeof(jt.particle.changeInterval));
					writeChunk.write(&jt.particle.tailLength,sizeof(jt.particle.tailLength));
					writeChunk.write(&jt.particle.timeMiddle,sizeof(jt.particle.timeMiddle));
					writeChunk.write(&jt.particle.colorStart,sizeof(jt.particle.colorStart));
					writeChunk.write(&jt.particle.colorMiddle,sizeof(jt.particle.colorMiddle));
					writeChunk.write(&jt.particle.colorEnd,sizeof(jt.particle.colorEnd));
					writeChunk.write(&jt.particle.alpha,sizeof(jt.particle.alpha));
					writeChunk.write(&jt.particle.scale,sizeof(jt.particle.scale));
					//5 - ����
					writeChunk.write(&jt.particle.scaleVar,sizeof(jt.particle.scaleVar));
					//21 - ����
					writeChunk.write(&jt.particle.fixedSize,sizeof(jt.particle.fixedSize));
					writeChunk.write(&jt.particle.headLifeSpan,sizeof(jt.particle.headLifeSpan));
					writeChunk.write(&jt.particle.headDecay,sizeof(jt.particle.headDecay));
					writeChunk.write(&jt.particle.tailLifeSpan,sizeof(jt.particle.tailLifeSpan));
					writeChunk.write(&jt.particle.tailDecay,sizeof(jt.particle.tailDecay));
					writeChunk.write(&jt.particle.head,sizeof(jt.particle.head));
					writeChunk.write(&jt.particle.tail,sizeof(jt.particle.tail));
					writeChunk.write(&jt.particle.unshaded,sizeof(jt.particle.unshaded));
					writeChunk.write(&jt.particle.unfogged,sizeof(jt.particle.unfogged));
					//22 - ����
					writeChunk.write(&jt.particle.blockByY0,sizeof(jt.particle.blockByY0));

					std::string filename = jt.particle.textureFilename.substr(jt.particle.textureFilename.rfind('/') + 1,jt.particle.textureFilename.length() - 1);

					uchar textureNameLen = filename.size();
					writeChunk.write(&textureNameLen,sizeof(textureNameLen));
					writeChunk.write(filename.c_str(),textureNameLen);

					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesVisible);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesSpeed);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesVariation);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesConeAngle);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesGravity);
					//17 - ����
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesExplosiveForce);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesEmissionRate);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesWidth);
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesLength);
					//16 - ����
					writeKeyFrames(writeChunk.getCurrentMemoryFileWriter(), &jt.particle.keyframesHeight);

					std::string path = ExportOptions::instance().m_outFilePath.asChar();
					std::string outFilename = path + "\\" + filename;
					BOOL overwrite = TRUE;
					/*if(rkt::isFile(outFilename.c_str()))
					{
						char str[256];
						sprintf(str,"%s�Ѿ����ڣ�Ҫ������",outFilename.c_str());
						overwrite = (MessageBox(0,str,"����",MB_YESNO) == IDYES);
					}*/
					CopyFile(jt.particle.textureFilename.c_str(),outFilename.c_str(),!overwrite);
				}
			}
		}
		writeChunk.endChunk();
	}

	// �����ļ�������
	std::string fileName = ExportOptions::instance().getOutFile();
	writeChunk.save(fileName);

	return stat;
}



