#include "common.h"
#include ".\FBTranslator.h"

FBTranslator::~FBTranslator(void)
{
	safeDelete(m_pMesh);
	safeDelete(m_pMaterialSet);
}

void* FBTranslator::creator()
{
	return new FBTranslator();
}


MStatus	FBTranslator::reader(const MFileObject& file, const MString& ExportOptionsString, MPxFileTranslator::FileAccessMode mode) 
{ 
	return MS::kSuccess; 
};

bool FBTranslator::haveReadMethod() const		
{ 
	return false; 
};

bool FBTranslator::haveWriteMethod() const		
{ 
	return true; 
};

MString FBTranslator::defaultExtension() const	
{ 
	return "Fb"; 
}

bool FBTranslator::canBeOpened() const 
{ 
	return false;
}
MString FBTranslator::filter() const				
{ 
	return "*.Fb"; 
}

MPxFileTranslator::MFileKind FBTranslator::identifyFile(  const MFileObject& fileName,
													  const char* buffer, short size) const
{ 
	LPSTR extension = PathFindExtensionA(fileName.name().asChar());
	if(0==stricmp(extension, ".Fb"))
	{
		return kIsMyFileType;
	}
	else
	{
		return kNotMyFileType; 
	}
}


MStatus	FBTranslator::writer(const MFileObject& mfoMMFile,			// save file object
						   const MString& msExportOptions,					// ExportOptions::instance() string
						   MPxFileTranslator::FileAccessMode mode)		// ExportOptions::instance() string
{	
	//��������
	MStatus stat = MS::kSuccess;
	stat = ExportScene(mfoMMFile, msExportOptions, mode);
	if(!stat)
	{
		MGlobal::displayInfo("Failed in Export Scene.");
	}
	return stat;
}

extern bool isVisible(MDagPath& dagPath);
extern unsigned int getFps();

void FBTranslator::loadMtlKeyframes()
{
	uint fps = getFps();
	float frameTime = 1000.0f / fps;

	MTime initialTime = MAnimControl::currentTime();

	int start = MAnimControl::animationStartTime().value();
	int end = MAnimControl::animationEndTime().value();
	for(int i = start;i <= end;i++)
	{
		MAnimControl::setCurrentTime(MTime(i,MTime::uiUnit()));
		size_t size = MaterialSet::getSingleton().m_materials.size();
		for(size_t j = 0;j < size;j++)
		{
			material *pmtl = MaterialSet::getSingleton().m_materials[j];
			pmtl->loadKeyframe(frameTime * (i - start));
		}
	}

	MAnimControl::setCurrentTime(initialTime);

}

MStatus FBTranslator::translateNode(MDagPath& dagPath,feibian *pSkeleton)
{
	MStatus stat = MS::kSuccess;

	if(!isVisible(dagPath))
		return stat;
#if 1
	if (dagPath.hasFn(MFn::kMesh) && (dagPath.childCount() == 0))
	{	
		MDagPath meshDag = dagPath;
		MFnMesh meshFn(meshDag);
		if (!meshFn.isIntermediateObject())
		{	
			stat = m_pMesh->load(meshDag,0);	
			if (MS::kSuccess != stat)
				std::cout << "Error, mesh skipped\n";
		}
	}
#endif
	if(dagPath.hasFn(MFn::kJoint))
	{
		MDagPath rootDag = dagPath;
		//check if skeleton has already been loaded
		bool skip = false;
		for (int j=0; j<pSkeleton->m_joints.size() && !skip; j++)
		{
			//skip skeleton if already loaded
			if (rootDag.partialPathName() == pSkeleton->m_joints[j].name)
			{
				skip = true;
			}
		}
		//load joints data from root
		if (!skip)
		{
			//load joints data
			pSkeleton->loadJoint(rootDag,NULL);
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
		stat = translateNode(childPath,pSkeleton);
		if (MS::kSuccess != stat)
			return MS::kFailure;
	}
	return MS::kSuccess;
}

extern unsigned int getFps();

MStatus	FBTranslator::ExportScene(const MFileObject& mfoMMFile,		// save file object
								const MString& msExportOptions,					// ExportOptions::instance() string
								MPxFileTranslator::FileAccessMode mode)
{
	//��ʵ�������
	if(MaterialSet::getSingletonPtr())
		MaterialSet::getSingletonPtr()->clear();

	const char *opt = msExportOptions.asChar();
	MStatus stat = MS::kSuccess;
	MString	msFile= mfoMMFile.fullName();
	const char * file = msFile.asChar();
	CStringA filename(file);
	const char* pExtension= strrchr(file, '.');
	if(!pExtension)
	{
		filename.Append( ".Fb");

		if(PathFileExistsA( filename.GetString() ))
		{
			CStringA Msg("Would you like to write over the existing file?\n");
			Msg.Append( filename );

			if( IDYES != MessageBoxA(NULL, Msg.GetString(), "File Exists", MB_YESNOCANCEL ) )
			{
				return stat;
			}
		}
	} 
	else if(0 != _stricoll(".Fb", pExtension) )
	{
		MessageBoxA(NULL, "Cannot write over non- \".Fb\" file", "Incompatible File Type", 0 );
		return stat;
	}

	bool bExportSelected = (mode == MPxFileTranslator::kExportActiveAccessMode);

	feibian fb;
	//��ȡ������Ϣ
	MItDag dagIter;
	MFnDagNode worldDag (dagIter.root());
	MDagPath worldPath;
	worldDag.getPath(worldPath);
	const char *name = worldPath.fullPathName().asChar();
	stat = translateNode(worldPath,&fb);
#if 0
	//��ȡ����ģ��
	const char *p_ = strrchr(file,'_');
	if(!p_)
	{
		MessageBox(0,"�Ҳ�����̬ģ��",0,0);
		return MS::kFailure;
	}
	std::string str(file,0,p_ - file);
	str += ".mz";

	FILE *fp = fopen(str.c_str(),"rb");
	if(!fp)
	{
		MessageBox(0,"�Ҳ�����̬ģ��",0,0);
		return MS::kFailure;
	}
	fseek(fp,0,SEEK_END);
	uint len = ftell(fp);
	uchar *pd = new uchar[len];
	fseek(fp,0,SEEK_SET);
	fread(pd,len,1,fp);

	//��ȡ������Ϣ
	rkt::DataChunk chunk;
	rkt::DataChunk::stChunk *pChunk = chunk.beginChunk(pd,len);
	while(pChunk)
	{
		if(pChunk->m_ui32DataSize == 0)
		{
			pChunk = chunk.nextChunk();
			continue;
		}
		rkt::MemoryStream stream((uchar*)pChunk->m_pData,pChunk->m_ui32DataSize);
		switch(pChunk->m_ui32Type)
		{
		case 'MWRD':
			{
				uint nBones;
				stream.read(&nBones,sizeof(nBones));

				tranMatrix tran;
				for(uint i = 0;i < nBones;i++)
				{
					stream.read(&tran,sizeof(tran));
					joint& jt = fb.m_joints[i];


					for (int iRow = 0; iRow < 4; iRow++)
					for (int iCol = 0; iCol < 3; iCol++)
					{
						jt.worldMatrix[iRow][iCol] = tran.m_mat[iRow][iCol];
					}
					jt.worldMatrix[0][3] = 0;
					jt.worldMatrix[1][3] = 0;
					jt.worldMatrix[2][3] = 0;
					jt.worldMatrix[3][3] = 1;
				}
			}
			break;
		}
		pChunk = chunk.nextChunk();
	}
	delete[] pd;
	fclose(fp);
#endif

	//��ȡ������Ϣ
	fb.loadAnims();
#if 1
	//��ȡ���ʶ�����Ϣ
	loadMtlKeyframes();
#endif
	//============================================================================
	// ��ʼд������
	ChunkSet writeChunk;

	// �汾��
	writeChunk.beginChunk("MVER");	
	uint ver = 10;
	writeChunk.write(&ver,sizeof(ver));
	writeChunk.endChunk();

	uint fps = getFps();
	float frameTime = 1000.0f / fps;

	// ����
	writeChunk.beginChunk("MANM");
	animation& anim = fb.m_animation;
	uint startTime = anim.startTime * frameTime;
	uint endTime = anim.endTime * frameTime;
	writeChunk.write(&startTime,sizeof(startTime));
	writeChunk.write(&endTime,sizeof(endTime));
	writeChunk.endChunk();

	// ��������
	writeChunk.beginChunk("MBON");
	{
		uint numBones = fb.m_joints.size();
		writeChunk.write(&numBones,sizeof(numBones));

		for(int i = 0;i < numBones;i++)
		{
			joint jt = fb.m_joints[i];
			writeChunk.write(&jt.id,sizeof(jt.id));

			int parent = -1;
			if(jt.parentIndex >=0)
			{
				parent = fb.m_joints[jt.parentIndex].id;
			}
			writeChunk.write(&parent,sizeof(parent));

			uint nKeyframes = jt.keyframesTranslation.size();
			writeChunk.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(), jt.keyframesTranslation);

			nKeyframes = jt.keyframesRotation.size();
			writeChunk.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),jt.keyframesRotation);

			nKeyframes = jt.keyframesScale.size();
			writeChunk.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),jt.keyframesScale);

			writeChunk.write(&jt.hasRibbonSystem,sizeof(jt.hasRibbonSystem));
			writeChunk.write(&jt.hasParticleSystem,sizeof(jt.hasParticleSystem));
		}
	}
	writeChunk.endChunk();
#if 1
	//����͸���� - �汾��7����
	writeChunk.beginChunk("MTRS");
	{
		size_t size = MaterialSet::getSingleton().m_materials.size();
		for(size_t i = 0;i < size;i++)
		{
			material* pMtl = MaterialSet::getSingleton().m_materials[i];
			unsigned char len = strlen(pMtl->m_name.asChar());
			writeChunk.write(&len,sizeof(len));
			writeChunk.write(pMtl->m_name.asChar(),len);
			uint nKeyFrames = pMtl->m_vTransparencies.size();
			writeChunk.write(&nKeyFrames,sizeof(nKeyFrames));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),pMtl->m_vTransparencies);
			//�汾��10������ɫ�ؼ���
			nKeyFrames = pMtl->m_vColors.size();
			writeChunk.write(&nKeyFrames,sizeof(nKeyFrames));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),pMtl->m_vColors);
			//�汾��26����tcFlowU��tcFlowV��changeStyle�ؼ���
			nKeyFrames = pMtl->m_vTcFlowSpeedU.size();
			writeChunk.write(&nKeyFrames,sizeof(nKeyFrames));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),pMtl->m_vTcFlowSpeedU);
			nKeyFrames = pMtl->m_vTcFlowSpeedV.size();
			writeChunk.write(&nKeyFrames,sizeof(nKeyFrames));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),pMtl->m_vTcFlowSpeedV);
			//
			nKeyFrames = pMtl->m_vRotateSpeed.size();
			writeChunk.write(&nKeyFrames,sizeof(nKeyFrames));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),pMtl->m_vRotateSpeed);
			//
			nKeyFrames = pMtl->m_vChangeStyle.size();
			writeChunk.write(&nKeyFrames,sizeof(nKeyFrames));
			writeSequence(writeChunk.getCurrentMemoryFileWriter(),pMtl->m_vChangeStyle);
		}
	}
	writeChunk.endChunk();
#endif
	writeChunk.save(std::string(filename.GetBuffer()));

	return stat;
}