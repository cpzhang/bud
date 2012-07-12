#include "RocketCommon.h"
#include "RocketTreeEnumProc.h"
#include "ExportOptions.h"
#include "IMaxMaterial.h"
#include "IMaxParticle.h"
#include "XRefFunctions.h"

bool IsRealEqual(const float& a, const float& b)
{
	if (a > b)
	{
		return (a - b) <= 0.0001f;
	}
	return (b - a) <= 0.0001f;
}
std::ostream& operator<< (std::ostream& out, const sKeyframeTranslation& k )
{
	out<<"tranlsate"<<std::endl<<"time:"<<k.time<<std::endl<<k.v[0]<<std::endl<<k.v[1]<<std::endl<<k.v[2]<<std::endl;
	return out;
}

std::ostream& operator<< (std::ostream& out, const sKeyframeRotation& k )
{
	out<<"rotate"<<std::endl<<"time:"<<k.time<<std::endl<<k.q[0]<<std::endl<<k.q[1]<<std::endl<<k.q[2]<<std::endl<<k.q[3]<<std::endl;
	return out;
}

std::ostream& operator<< (std::ostream& out, const sKeyframeScale& k )
{
	out<<"scale"<<std::endl<<"time:"<<std::endl<<k.time<<std::endl<<k.v[0]<<std::endl<<k.v[1]<<std::endl<<k.v[2]<<std::endl;
	return out;
}

std::ostream& operator<< (std::ostream& out, const Matrix3& m )
{
	///////////
	// Grab the transform
	Point3 p = m.GetTrans();
	out<<"tranlsate"<<std::endl<<p.x<<std::endl<<p.y<<std::endl<<p.z<<std::endl;

	Quat q(m);
	out<<"rotate"<<std::endl<<q.x<<std::endl<<q.y<<std::endl<<q.z<<std::endl<<q.w<<std::endl;

	Point3 s;
	s.x = m.GetRow(0).Length();
	s.y = m.GetRow(1).Length();
	s.z = m.GetRow(2).Length();
	out<<"scale"<<std::endl<<s.x<<std::endl<<s.y<<std::endl<<s.z<<std::endl;

	return out;
}

RocketTreeEnumProc::RocketTreeEnumProc()
{
	_clear();
}

RocketTreeEnumProc::~RocketTreeEnumProc()
{
	_clear();
}

int RocketTreeEnumProc::callback( INode *node )
{
	//	Vertices and other data are not statically stored, but dynamically calculated each
	//	time. To access data, we must first perform the geometry pipeline evaluation, specifying
	//	the time at which we want to get the object state
	// Get the object from the node
	// apply each modifier in the pipeline from beginning to end
	ObjectState o = node->EvalWorldState(_time);
	
	eType t = _extractType(node);
	switch(t)
	{
	case eType_Mesh:
		{
			_extractSkin(node);
			_extractMesh(node);
			unsigned short matID;
			if (_extractMaterial(node, matID))
			{
				_subMeshes[_subMeshes.size() - 1].matId = matID;
			}
			else
			{
				std::string errorMsg(node->GetName());
				errorMsg += " 没有可识别的材质";
				MessageBox(NULL, errorMsg.c_str(), "不好意思！出错了！", MB_OK);
			}
		}break;

	case eType_Bone:
		{
			_extractBone(node);
		}break;
	}

	return TREE_CONTINUE;
}

void RocketTreeEnumProc::setTime( TimeValue t )
{
	_time = t;
}

void RocketTreeEnumProc::write()
{
	//============================================================================
	// 开始写入数据
	ChunkSet cs;
#ifdef Check_Biped_Bone
	std::string cbbName(GetExportOptions()->m_outFilePath);
	cbbName.replace(cbbName.size() - 2, 2, "bb");
	std::ofstream cbb(cbbName.c_str());
#endif
	
	//============================================================================
	// 版本号
	cs.beginChunk("MVER");	
	uint ver = 29;
	cs.write(&ver,sizeof(ver));
// 	uchar modelingSoftware = 1;
// 	cs.write(&modelingSoftware, sizeof(modelingSoftware));
	cs.endChunk();

	//============================================================================
	// 模型部件
	cs.beginChunk("MSUB");
	for(size_t i = 0;i < _subMeshes.size();i++)
	{
		SUBMESH* sm = &_subMeshes[i];
		SUBMESH_BASE sb;
		sb.animated = sm->animated;
		sb.icount = sm->icount;
		sb.istart = sm->istart;
		sb.matId = sm->matId;
		sb.vcount = sm->vcount;
		sb.vstart = sm->vstart;
		sb.nameLen = strlen(sm->name);
		cs.write(&sb,sizeof(sb));
		cs.write(sm->name,sb.nameLen);
	}
	cs.endChunk();

	//============================================================================
	// 顶点
	cs.beginChunk("MVTX");
	//版本号15加入是否导出顶点颜色，其实老版本的颜色都导出来了，但没有实际去用
	
	bool exportVertexColor = GetExportOptions()->exportVertexColour;
	cs.write(&exportVertexColor,sizeof(exportVertexColor));
	//版本号19如果不导出顶点颜色，就不输出到文件
	if(GetExportOptions()->exportVertexColour)
	{
		writeSequence(cs.getCurrentMemoryFileWriter(), _vertices);
	}
	else
	{
		for(size_t i = 0;i < _vertices.size();i++)
		{
			MODELVERTEX_19& mv = _vertices[i];
			cs.write(&mv,sizeof(MODELVERTEX_19BASE));
		}
	}
	cs.endChunk();

	//============================================================================
	// 索引
	cs.beginChunk("MFAC");
	writeSequence(cs.getCurrentMemoryFileWriter(), _faces);
	cs.endChunk();

	//============================================================================
	// 材质、纹理数据
	if (1)
	{
		cs.beginChunk("MMTX");
		uint nMaterials = _materials.size();
		cs.write(&nMaterials,sizeof(nMaterials));
		for(int i = 0;i< nMaterials; ++i)
		{
			material* mat = &_materials[i];

			// 材质名
			std::string matName = mat->m_name;
			uchar matNameLen = matName.size();
			cs.write(&matNameLen,sizeof(matNameLen));
			cs.write(matName.c_str(),matNameLen);

			// lighting
			uchar lighting = mat->m_lightingOff;
			cs.write(&lighting,sizeof(lighting));

			// ambient diffuse specular emissive
			cs.write(&mat->m_ambient,sizeof(mat->m_ambient));
			cs.write(&mat->m_diffuse,sizeof(mat->m_diffuse));
			cs.write(&mat->m_specular,sizeof(mat->m_specular));
			cs.write(&mat->m_emissive,sizeof(mat->m_emissive));
			uchar iTrans = true;
			cs.write(&iTrans,sizeof(iTrans));
			uchar twoSide = mat->m_twoSide;
			cs.write(&twoSide,sizeof(twoSide));
			uchar tcFlowU = mat->m_tcFlowU;
			cs.write(&tcFlowU,sizeof(tcFlowU));
			uchar tcFlowV = mat->m_tcFlowV;
			cs.write(&tcFlowV,sizeof(tcFlowV));
			//版本号29加入RotateSpeed关键桢
			uchar tcRotate = mat->m_tcRotate;
			cs.write(&tcRotate,sizeof(tcRotate));

			//版本号29加入uv位置角度复位
			uchar tcAngleReset = mat->m_tcAngleReset;
			cs.write(&tcAngleReset,sizeof(tcAngleReset));

			uchar tcPositionReset = mat->m_tcPositionReset;
			cs.write(&tcPositionReset,sizeof(tcPositionReset));

			// static flow speedU
			float tcFlowSpeedU = mat->m_tcFlowSpeedU;
			cs.write(&tcFlowSpeedU,sizeof(tcFlowSpeedU));
			//版本号26加入FlowSpeedU关键桢
			uint nKeyframes = mat->m_vTcFlowSpeedU.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(), mat->m_vTcFlowSpeedU);
			// static flow speedV
			float tcFlowSpeedV = mat->m_tcFlowSpeedV;
			cs.write(&tcFlowSpeedV,sizeof(tcFlowSpeedV));
			//版本号26加入FlowSpeedV关键桢
			nKeyframes = mat->m_vTcFlowSpeedV.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vTcFlowSpeedV);
			//版本号28加入flow time，纯粹为了与登陆动画兼容
			ushort ft = 0.0f;
			cs.write(&ft,sizeof(ft));

			//版本号29加入RotateSpeed关键桢
			nKeyframes = mat->m_vRotateSpeed.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vRotateSpeed);
			//9版本加入材质不投影
			cs.write(&mat->m_dontCastShadow,sizeof(mat->m_dontCastShadow));
			//12版本加入材质不打光
			cs.write(&mat->m_unlit,sizeof(mat->m_unlit));
			//14版本加入深度测试和写入的选项
			cs.write(&mat->m_noDepthCheck,sizeof(mat->m_noDepthCheck));
			cs.write(&mat->m_noDepthWrite,sizeof(mat->m_noDepthWrite));
			//27版本加入additiveLayer
			cs.write(&mat->m_additiveLayer,sizeof(mat->m_additiveLayer));

			//7版本加入透明度
			float transparency = mat->m_transparency;
			cs.write(&transparency,sizeof(transparency));
			//7版本加入透明度keyframe
			nKeyframes = mat->m_vTransparencies.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vTransparencies);
			//10版本加入材质颜色
			colorNalpha color = mat->m_color;
			cs.write(&color,sizeof(color));
			//10版本加入材质颜色关键桢
			nKeyframes = mat->m_vColors.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vColors);
			//版本7加入序列图支持，以下4个属性
			cs.write(&mat->m_rows,sizeof(mat->m_rows));
			cs.write(&mat->m_cols,sizeof(mat->m_cols));
			cs.write(&mat->m_changeStyle,sizeof(mat->m_changeStyle));
			//版本号26加入ChangeStyle关键桢
			nKeyframes = mat->m_vChangeStyle.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vChangeStyle);
			cs.write(&mat->m_changeInterval,sizeof(mat->m_changeInterval));

			// 纹理,缺省为一层纹理
			uint nTextures = mat->m_textures.size();
			cs.write(&nTextures,sizeof(nTextures));
			for(int j= 0;j < nTextures;j++)
			{	
				texture tex = mat->m_textures[j];
				std::string strFilename = tex.filename;
				uchar filenameLen = strFilename.size();
				cs.write(&filenameLen,sizeof(filenameLen));
				cs.write(strFilename.c_str(),filenameLen);
				uchar blendModes = tex.blendModes;
				cs.write(&blendModes,sizeof(blendModes));
			}


			if(GetExportOptions()->copyTextures)
			{
				mat->copyTextures();
			}
		}
		cs.endChunk();
	}

	//============================================================================
	// 动画
	if (1)
	{
		cs.beginChunk("MANM");
		//
		Interface* i = GetCOREInterface();
		if (i == NULL)
		{
			return;
		}

		if (GetExportOptions()->exportMode == eExportMode_MZ && !GetExportOptions()->exportAnims)
		{
			unsigned int numAnimation = 0;
			cs.write(&numAnimation, sizeof(unsigned int));
			size_t numBones = _boneSet.size();
			cs.write(&numBones, sizeof(size_t));
			for(size_t i = 0; i < numBones; ++i)
			{
				sBone& b = _boneSet[i];
				b.translationSet.clear();
				b.rotationSet.clear();
				b.scaleSet.clear();
			}
			//_boneSet.clear();
		}
		else
		{
			unsigned int numAnimation = 1;
			cs.write(&numAnimation, sizeof(unsigned int));
			std::string name(GetExportOptions()->getAnimationName());
			uchar len = name.size();
			cs.write(&len, sizeof(len));
			cs.write(name.c_str(), len);

			int ticksPerFrame = GetTicksPerFrame();
			Interval inv = i->GetAnimRange();
			unsigned int st = 1000 * inv.Start() / 4800;
			unsigned int et = 1000 * inv.End() / 4800;
			cs.write(&st, sizeof(unsigned int));
			cs.write(&et, sizeof(unsigned int));
		}

		cs.endChunk();

		/////////
		//// 骨骼动画
		cs.beginChunk("MBON");
		size_t numBones = _boneSet.size();
		cs.write(&numBones, sizeof(size_t));
		for(size_t i = 0; i < numBones; ++i)
		{
			sBone& b = _boneSet[i];
			
			cs.write(&b.id, sizeof(int));
			
			uchar len = b.name.size();
			cs.write(&len, sizeof(len));
			
			cs.write(b.name.c_str(), len);
			
			cs.write(&b.parentId, sizeof(int));

			//
			//for (int r = 0; r != 4; ++r)
			//{
				cs.write(b.mat.GetAddr(), 4 * 3 * sizeof(float));
			//}

			//
// 			if (ver >= 31)
// 			{
// 				cs.write(b.cmat.GetAddr(), 4 * 3 * sizeof(float));
// 			}

			size_t s = b.translationSet.size();
			cs.write(&s, sizeof(size_t));
			writeSequence(cs.getCurrentMemoryFileWriter(), b.translationSet);

			s = b.rotationSet.size();
			cs.write(&s, sizeof(size_t));
			writeSequence(cs.getCurrentMemoryFileWriter(), b.rotationSet);

			s = b.scaleSet.size();
			cs.write(&s, sizeof(size_t));
			writeSequence(cs.getCurrentMemoryFileWriter(), b.scaleSet);

#ifdef Check_Biped_Bone
			cbb<<"name"<<std::endl<<b.name<<std::endl;
			cbb<<"parent"<<std::endl<<b.parentId<<std::endl;
			cbb<<"initMat"<<std::endl<<b.mat;
			printSet(cbb, b.translationSet);
			printSet(cbb, b.rotationSet);
			printSet(cbb, b.scaleSet);
#endif

			{
				bool hasRibbon = false;
				cs.write(&hasRibbon, sizeof(hasRibbon));
			}
			bool hasParticle = false;
			for (size_t i = 0; i != _particleSet.size(); ++i)
			{
				sParticle& p = _particleSet[i];
				if (p.bone == b.name)
				{
					hasParticle = true;
					cs.write(&hasParticle, sizeof(hasParticle));
					cs.write(&p.visible, sizeof(p.visible));
					cs.write(&p.speed, sizeof(p.speed));
					cs.write(&p.variation, sizeof(p.variation));
					cs.write(&p.coneAngle, sizeof(p.coneAngle));
					cs.write(&p.gravity, sizeof(p.gravity));
					cs.write(&p.explosiveForce, sizeof(p.explosiveForce));
					cs.write(&p.life, sizeof(p.life));
					cs.write(&p.lifeVar, sizeof(p.lifeVar));
					cs.write(&p.emissionRate, sizeof(p.emissionRate));
					cs.write(&p.initialNum, sizeof(p.initialNum));
					cs.write(&p.limitNum, sizeof(p.limitNum));
					cs.write(&p.attachToEmitter, sizeof(p.attachToEmitter));
					cs.write(&p.moveWithEmitter, sizeof(p.moveWithEmitter));
					cs.write(&p.forTheSword, sizeof(p.forTheSword));
					cs.write(&p.forTheSwordInitialAngle, sizeof(p.forTheSwordInitialAngle));
					cs.write(&p.wander, sizeof(p.wander));
					cs.write(&p.wanderRadius, sizeof(p.wanderRadius));
					cs.write(&p.wanderSpeed, sizeof(p.wanderSpeed));
					cs.write(&p.aspectRatio, sizeof(p.aspectRatio));
					cs.write(&p.initialAngleBegin, sizeof(p.initialAngleBegin));
					cs.write(&p.initialAngleEnd, sizeof(p.initialAngleEnd));
					cs.write(&p.rotationSpeed, sizeof(p.rotationSpeed));
					cs.write(&p.rotationSpeedVar, sizeof(p.rotationSpeedVar));
					cs.write(&p.width, sizeof(p.width));
					cs.write(&p.length, sizeof(p.length));
					cs.write(&p.height, sizeof(p.height));
					cs.write(&p.blendMode, sizeof(p.blendMode));
					cs.write(&p.textureRows, sizeof(p.textureRows));
					cs.write(&p.textureCols, sizeof(p.textureCols));
					cs.write(&p.changeStyle, sizeof(p.changeStyle));
					cs.write(&p.changeInterval, sizeof(p.changeInterval));
					cs.write(&p.tailLength, sizeof(p.tailLength));
					cs.write(&p.timeMiddle, sizeof(p.timeMiddle));
					cs.write(&p.colorStart, sizeof(p.colorStart));
					cs.write(&p.colorMiddle, sizeof(p.colorMiddle));
					cs.write(&p.colorEnd, sizeof(p.colorEnd));
					cs.write(&p.alpha, sizeof(p.alpha));
					cs.write(&p.scale, sizeof(p.scale));
					cs.write(&p.scaleVar, sizeof(p.scaleVar));
					cs.write(&p.fixedSize, sizeof(p.fixedSize));
					cs.write(&p.headLifeSpan, sizeof(p.headLifeSpan));
					cs.write(&p.headDecay, sizeof(p.headDecay));
					cs.write(&p.tailLifeSpan, sizeof(p.tailLifeSpan));
					cs.write(&p.tailDecay, sizeof(p.tailDecay));
					cs.write(&p.head, sizeof(p.head));
					cs.write(&p.tail, sizeof(p.tail));
					cs.write(&p.unshaded, sizeof(p.unshaded));
					cs.write(&p.unfogged, sizeof(p.unfogged));
					cs.write(&p.blockByY0, sizeof(p.blockByY0));
					unsigned char nameLen = p.textureFilename.size();
					cs.write(&nameLen, sizeof(nameLen));
					cs.write(p.textureFilename.c_str(), nameLen);

					unsigned int nKeyframes = p.keyframesVisible.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesVisible);

					nKeyframes = p.keyframesSpeed.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesSpeed);

					nKeyframes = p.keyframesVariation.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesVariation);

					nKeyframes = p.keyframesConeAngle.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesConeAngle);

					nKeyframes = p.keyframesGravity.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesGravity);

					nKeyframes = p.keyframesExplosiveForce.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));					
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesExplosiveForce);

					nKeyframes = p.keyframesEmissionRate.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesEmissionRate);

					nKeyframes = p.keyframesWidth.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesWidth);

					nKeyframes = p.keyframesLength.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesLength);

					nKeyframes = p.keyframesHeight.size();
					cs.write(&nKeyframes,sizeof(nKeyframes));
					writeSequence(cs.getCurrentMemoryFileWriter(), p.keyframesHeight);
					break;
				}
			}
			if (!hasParticle)
			{
				cs.write(&hasParticle, sizeof(hasParticle));
			}
		}
		cs.endChunk();
	}
	
	//============================================================================
	// 保存文件，结束
	std::string fileName = GetExportOptions()->m_outFilePath;
	cs.save(fileName);

#ifdef Check_Biped_Bone
	cbb.close();
#endif

	//
	_clear();
}

void RocketTreeEnumProc::_clear()
{
	//
	_materials.clear();
	_vertices.clear();
	_faces.clear();
	_subMeshes.clear();
	_boneSet.clear();
	_verticesBones.clear();
	_matrices.clear();
	_nodeSet.clear();
	_particleSet.clear();
	//
	tVStart = 0;
	tIStart = 0;	
	_gameScene = NULL;
}

void RocketTreeEnumProc::addNode( INode* n)
{
	if (n->IsRootNode())
	{
	}
	else
	{
		_nodeSet.push_back(n);
	}

	int numChild = n->NumberOfChildren();
	for (int i = 0; i != numChild; ++i)
	{
		addNode(n->GetChildNode(i));
	}
}

bool RocketTreeEnumProc::IsUnReferrenced(const sBone& b)
{
	for (size_t i = 0; i != b.children.size(); ++i)
	{
		if (!IsUnReferrenced(_boneSet[b.children[i]]))
		{
			return false;
		}
	}

	if (b.referrenced)
	{
		return false;
	}

	return true;
}
void CheckUnReferrenced(sBone& b)
{
	b.unReferrenced = RocketTreeEnumProc::IsUnReferrenced(b);
}

bool IsUnReferrencedAndChildren(const sBone& b)
{
	return b.unReferrenced;
}

template<int id>
bool IsReferrencedByID(const sBone& b)
{
	return b.oldId == id;
}
void RocketTreeEnumProc::createTreeItem(HWND hDlg, sBone& b, HTREEITEM h)
{
	TV_INSERTSTRUCT tvinsert; // struct to config the tree control
	tvinsert.hParent = h; // top most level Item
	// root level item attribute.
	if (h == NULL)
	{
		tvinsert.hInsertAfter = TVI_ROOT;                             
	}
	else
	{
		tvinsert.hInsertAfter = TVI_LAST;                             
	}
	// attributs
	tvinsert.item.mask = TVIF_TEXT; 
	// item text 
	tvinsert.item.pszText = (LPSTR)b.name.c_str();
	HTREEITEM p = (HTREEITEM)SendDlgItemMessage(hDlg, IDC_TREE1, TVM_INSERTITEM, 0, (LPARAM)&tvinsert);
	for (size_t k = 0; k != b.children.size(); ++k)
	{
		createTreeItem(hDlg, _boneSet[b.children[k]], p);
	}
}
//
INT_PTR CALLBACK DeleteRedundantBonesDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static RocketTreeEnumProc* rocket;
	switch(message) 
	{
		case WM_INITDIALOG:
			{
				rocket = (RocketTreeEnumProc*)lParam;
				SetWindowContextHelpId(hDlg, idh_3dsexp_export);
				CenterWindow(hDlg,GetParent(hDlg));
				SetFocus(hDlg);
				{
					//
					for (std::vector<sBone>::iterator i = rocket->_boneSet.begin(); i != rocket->_boneSet.end(); ++i)
					{
						sBone& b = *i;
						if (IsUnReferrencedAndChildren(b) && !IsUnReferrencedAndChildren(rocket->_boneSet[b.parentId]))
						{
							rocket->createTreeItem(hDlg, b, NULL);
						}
					}
				}
			}
			return FALSE;

		case WM_DESTROY:
			return FALSE;

		case WM_NOTIFY:
			{
			case IDC_TREE1:
				{		
					if(((LPNMHDR)lParam)->code == NM_CLICK)
					{
						HWND hTree = GetDlgItem(hDlg, IDC_TREE1);
						POINT cursorPos;
						GetCursorPos(&cursorPos);
						ScreenToClient(hTree, &cursorPos);

						TVHITTESTINFO thi;
						thi.pt = cursorPos;
						TreeView_HitTest(hTree, &thi);

						if (thi.flags == TVHT_ONITEMSTATEICON)
						{
							//state changed: check the state of the item under the cursor
							BOOL isChecked = TreeView_GetCheckState(hTree, thi.hItem);
							TV_ITEM tvi;
							char Text[255]="";
							memset(&tvi,0,sizeof(tvi));
							tvi.mask=TVIF_TEXT;
							tvi.pszText=Text;
							tvi.cchTextMax=256;
							tvi.hItem=thi.hItem;
							if(SendDlgItemMessage(hDlg, IDC_TREE1, TVM_GETITEM, TVGN_CARET,(LPARAM)&tvi))
							{
								std::string boneName = tvi.pszText;
								for (size_t k = 0; k != rocket->_boneSet.size(); ++k)
								{
									if (rocket->_boneSet[k].name == boneName)
									{
										if (isChecked)
										{
											rocket->_boneSet[k].referrenced = false;
										}
										else
										{
											rocket->_boneSet[k].referrenced = true;
										}
										break;
									}
								}
							}
						}
					}
				}break;
			}break;
			return false;
		case WM_COMMAND:
			{
				switch(LOWORD(wParam)) 
				{
				case IDOK:
					{
						EndDialog(hDlg, 1);
						return TRUE;
					}
				case IDCANCEL:
					EndDialog(hDlg, 0);
					return FALSE;
				}
			}break;

		case WM_SYSCOMMAND:
			if ((wParam & 0xfff0) == SC_CONTEXTHELP) 
			{
				DoHelp(HELP_CONTEXT, idh_3dsexp_export);
				return FALSE;
			}
	}
	return FALSE;
}
//
bool RocketTreeEnumProc::read()
{
//	if (GetExportOptions()->exportAll)
//	{
		readBone();
// 	}
// 	else
// 	{
// 		if (_isBoneSelected())
// 		{
// 			readBone();
// 		}
// 		else
// 		{
// 		}
// 	}

	readMesh();

	readParticle();
	//
	for(size_t i = 0;i < _particleSet.size();i++)
	{
		sParticle& mv = _particleSet[i];
		{
			for (size_t k = 0; k != _boneSet.size(); ++k)
			{
				if (_boneSet[k].name == mv.bone)
				{
					_boneSet[k].referrenced = true;
					break;
				}
			}
		}
	}

	if (GetExportOptions()->deleteUnReferrencedBones)
	{
		std::for_each(_boneSet.begin(), _boneSet.end(), CheckUnReferrenced);
		//
		if (!DialogBoxParam(gInstance, MAKEINTRESOURCE(IDD_DIALOG2), GetCOREInterface()->GetMAXHWnd(), DeleteRedundantBonesDlgProc, (LPARAM)this)) 
		{
			return false;
		}
		std::for_each(_boneSet.begin(), _boneSet.end(), CheckUnReferrenced);
		//
		for (std::vector<sBone>::iterator i = _boneSet.begin(); i != _boneSet.end();)
		{
			if (IsUnReferrencedAndChildren(*i))
			{
				i = _boneSet.erase(i);
			}
			else
			{
				++i;
			}
		}
		//std::remove_if(_boneSet.begin(), _boneSet.end(), IsUnReferrencedAndChildren);
		_setBoneParent();
		//
		for(size_t i = 0;i < _vertices.size();i++)
		{
			MODELVERTEX_19& mv = _vertices[i];
			for (int j = 0; j != 4; ++j)
			{
				for (size_t k = 0; k != _boneSet.size(); ++k)
				{
					if (_boneSet[k].oldId == mv.bones[j])
					{
						mv.bones[j] = _boneSet[k].id;
						break;
					}
				}
			}
		}
	}
	return true;
}

void RocketTreeEnumProc::_getKeyFrame( Control* tmc )
{
	Control* pc = tmc->GetPositionController();
	if (pc)
	{
		IKeyControl* kc = GetKeyControlInterface(pc);
		if (kc)
		{
			int numKeys = kc->GetNumKeys();
			for (int k = 0; k != numKeys; ++k)
			{
				switch(pc->ClassID().PartA())
				{
				case LININTERP_POSITION_CLASS_ID:
					{
						ILinPoint3Key posKey;
						kc->GetKey(k, &posKey);
					}break;
				case HYBRIDINTERP_POINT3_CLASS_ID:
					{
						IBezPoint3Key posKey;
						kc->GetKey(k, &posKey);
					}break;
				case TCBINTERP_POSITION_CLASS_ID:
					{
						ITCBPoint3Key posKey;
						kc->GetKey(k, &posKey);
					}break;
				}
			}
		}
	}

	///////////////////////////
	Control* sc = tmc->GetScaleController();
	if (sc)
	{
		IKeyControl* kc = GetKeyControlInterface(sc);
		if (kc)
		{
			int numKeys = kc->GetNumKeys();
			for (int k = 0; k != numKeys; ++k)
			{
				switch(sc->ClassID().PartA())
				{
				case LININTERP_SCALE_CLASS_ID:
					{
						ILinScaleKey key;
						kc->GetKey(k, &key);
					}break;
				case HYBRIDINTERP_SCALE_CLASS_ID:
					{
						IBezScaleKey key;
						kc->GetKey(k, &key);
					}break;
				case TCBINTERP_SCALE_CLASS_ID:
					{
						ITCBScaleKey key;
						kc->GetKey(k, &key);
					}break;
				}
			}
		}
	}

	///////////////////////////////////////////////////////
	Control* rc = tmc->GetRotationController();
	if (rc)
	{
		IKeyControl* kc = GetKeyControlInterface(rc);
		if (kc)
		{
			int numKeys = kc->GetNumKeys();
			for (int k = 0; k != numKeys; ++k)
			{
				switch(rc->ClassID().PartA())
				{
				case LININTERP_ROTATION_CLASS_ID:
					{
						ILinRotKey key;
						kc->GetKey(k, &key);
					}break;
				case HYBRIDINTERP_ROTATION_CLASS_ID:
					{
						IBezQuatKey key;
						kc->GetKey(k, &key);
					}break;
				case TCBINTERP_ROTATION_CLASS_ID:
					{
						ITCBRotKey key;
						kc->GetKey(k, &key);
					}break;
				}
			}
		}
	}
}

bool RocketTreeEnumProc::_extractMaterial( INode* node, unsigned short& matID )
{
	/////////////////////////////////////////////////////////////////
	// In 3ds Max there is only one material per node.
	// In the INode class there are methods INode::GetMtl() and INode::SetMtl() that provide access to and allow changing of the node's material. 
	// INode::GetMtl() returns a pointer to an instance of class Mtl. If the value returned is NULL this means the user has not assigned a material to the node.
	// In such a case the renderer uses the wire frame color of the node (as well as many defaults) for the rendering properties
	/////////////////////////////////////////////////////////////////
	Mtl* m = node->GetMtl();

	if (_currentNodeIsRef)
	{
		//node = GetCOREInterface()->CreateObjectNode(oRef);
		m = XRefFunctions::getXRefMaterialSource(m);
	}
	if (m == NULL)
	{
		std::string errorMsg(m->GetName());
		errorMsg += " 是不可识别的材质";
		MessageBox(NULL, errorMsg.c_str(), "不好意思！出错了！", MB_OK);
		return false;
	}

	if (_isMaterialExtracted(m, matID))
	{
		return true; 
	}

// 
#ifdef DIY_MATERIAL
	// See if it's our DIY material
	if (m->ClassID() != MaxMaterial_CLASS_ID)
	{
		return false;
	}

	material mt;
	mt.m_name = m->GetName();

	//
	IMaxMaterial* sm = (IMaxMaterial*)m;
	mt.m_twoSide = sm->isTwoSided();
	mt.m_transparency = sm->getTransparency(_time);
	mt.m_lightingOff = sm->isUnlit();
	mt.m_tcFlowU = sm->isTcFlowU();
	mt.m_tcFlowV = sm->isTcFlowV();
	mt.m_tcRotate = sm->isTcRotate();
	mt.m_tcAngleReset = sm->isTcAngleReset();
	mt.m_tcPositionReset = sm->isTcPositionReset();
	mt.m_tcFlowSpeedU = sm->getTcFlowSpeedU(0);
	mt.m_tcFlowSpeedV = sm->getTcFlowSpeedV(0);
	mt.m_tcRotateSpeed = sm->getTcRotateSpeed(0);
	mt.m_transparency = sm->getTransparency(0);
	Color c = sm->getColor(0);
	mt.m_color.r = c.r;
	mt.m_color.g = c.g;
	mt.m_color.b = c.b;
	Color ambient = sm->GetAmbient(_time);
	mt.m_ambient.r = ambient.r;
	mt.m_ambient.g = ambient.g;
	mt.m_ambient.b = ambient.b;
	Color diffuse = sm->GetDiffuse(_time);
	mt.m_diffuse.r = diffuse.r;
	mt.m_diffuse.g = diffuse.g;
	mt.m_diffuse.b = diffuse.b;
	Color specular = sm->GetSpecular(_time);
	mt.m_specular.r = specular.r;
	mt.m_specular.g = specular.g;
	mt.m_specular.b = specular.b;
	float shininess = sm->GetShininess(_time);
	float shinestrength = sm->GetShinStr(_time);
	if (sm->GetSelfIllumColorOn())
	{
		Color emissive = sm->GetSelfIllumColor();
		mt.m_emissive.r = emissive.r;
		mt.m_emissive.g = emissive.g;
		mt.m_emissive.b = emissive.b;
	}
	// Access the Diffuse map and see if it's a Bitmap texture
	Texmap* tm = sm->GetSubTexmap(0);
	if (tm && tm->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		// It is -- Access the UV tiling settings at time 0.
		BitmapTex* bm = (BitmapTex*)tm;
		if (bm)
		{
			texture t;
			t.blendModes = (BlendModes)sm->getBlendMode();
			t.uvsetName = bm->GetName();
			t.absFilename = bm->GetMapName();
			MaxSDK::Util::Path filePath(t.absFilename.c_str());
			t.filename = filePath.StripToLeaf().GetCStr();
// 			t.filename = bm->GetMapName();
// 			{
// 				size_t pos = t.filename.find_last_of('\\');
// 				if (pos != std::string::npos)
// 				{
// 					t.filename = t.filename.substr(pos + 1, t.filename.size() - pos - 1);
// 				}
// 			}
// 			{
// 				size_t pos = t.filename.find_last_of('//');
// 				if (pos != std::string::npos)
// 				{
// 					t.filename = t.filename.substr(pos + 1, t.filename.size() - pos - 1);
// 				}
// 			}
			StdUVGen* uvg = bm->GetUVGen();

			//
			//
			Interface* i = GetCOREInterface();
			if (i == NULL)
			{
				return false;
			}
			int ticksPerFrame = GetTicksPerFrame();
			Interval inv = i->GetAnimRange();
			TimeValue st = inv.Start();
			TimeValue et = inv.End();
			int numKey = (et - st) / ticksPerFrame;

			for (TimeValue t = st; t <= et; t += ticksPerFrame)
			{
				int tChanged = 1000 * t / 4800;
				{
					keyframeT<float> k;
					k.data = sm->getTcFlowSpeedU(t);
					k.time = tChanged;
					mt.m_vTcFlowSpeedU.push_back(k);
				}
				/////////////////////
				{
					keyframeT<float> k;
					k.data = sm->getTcFlowSpeedV(t);
					k.time = tChanged;
					mt.m_vTcFlowSpeedV.push_back(k);
				}
				/////////////////////
				{
					keyframeT<float> k;
					k.data = sm->getTcRotateSpeed(t);
					k.time = tChanged;
					mt.m_vRotateSpeed.push_back(k);
				}
				/////////////////////
				{
					keyframeT<float> k;
					k.data = sm->getTransparency(t);
					k.time = tChanged;
					mt.m_vTransparencies.push_back(k);
				}
				/////////////////////
				{
					keyframeT<colorNalpha> k;
					Point3 ac= sm->getColor(t);
					k.data.r = ac[0];
					k.data.g = ac[1];
					k.data.b = ac[2];
					k.time = tChanged;
					mt.m_vColors.push_back(k);
				}
				/////////////////////
				{
					keyframeT<short> k;
					k.data = sm->getChangeStyle(t);
					k.time = tChanged;
					mt.m_vChangeStyle.push_back(k);
				}
			}

			mt.m_textures.push_back(t);
			mt.m_rows = sm->getRows();
			mt.m_cols = sm->getCols();
			mt.m_changeStyle = sm->getChangeStyle(0);
			mt.m_dontCastShadow = sm->dontCastShadow();
			mt.m_unlit = sm->isUnlit();
			mt.m_noDepthWrite = sm->isNoDepthWrite();
			mt.m_noDepthCheck = sm->isNoDepthCheck();
			mt.m_additiveLayer = sm->isAdditivelayer();
			mt.sampleLinear();
			mt.processStillSequence();
			matID = _materials.size();
			_materials.push_back(mt);
			return true;
		}
	}
#else
	// See if it's a Standard material
	if (m->ClassID() != Class_ID(DMTL_CLASS_ID, 0))
	{
		return false;
	}

	material mt;
	mt.m_name = m->GetName();

	StdMat* sm = (StdMat*)m;
	mt.m_twoSide = sm->GetTwoSided();
	Color ambient = sm->GetAmbient(_time);
	mt.m_ambient.r = ambient.r;
	mt.m_ambient.g = ambient.g;
	mt.m_ambient.b = ambient.b;
	Color diffuse = sm->GetDiffuse(_time);
	mt.m_diffuse.r = diffuse.r;
	mt.m_diffuse.g = diffuse.g;
	mt.m_diffuse.b = diffuse.b;
	Color specular = sm->GetSpecular(_time);
	mt.m_specular.r = specular.r;
	mt.m_specular.g = specular.g;
	mt.m_specular.b = specular.b;
	float shininess = sm->GetShininess(_time);
	float shinestrength = sm->GetShinStr(_time);
	if (sm->GetSelfIllumColorOn())
	{
		Color emissive = sm->GetSelfIllumColor();
		mt.m_emissive.r = emissive.r;
		mt.m_emissive.g = emissive.g;
		mt.m_emissive.b = emissive.b;
	}
	mt.m_twoSide = sm->GetTwoSided();
	mt.m_transparency = sm->GetOpacity(_time);

	//else
	{
		// Access the Diffuse map and see if it's a Bitmap texture
		Texmap* tm = sm->GetSubTexmap(ID_DI);
		if (tm && tm->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
		{
			// It is -- Access the UV tiling settings at time 0.
			BitmapTex* bm = (BitmapTex*)tm;
			if (bm)
			{
				texture t;
				t.uvsetName = bm->GetName();
				t.filename = bm->GetMapName();
				{
					size_t pos = t.filename.find_last_of('\\');
					if (pos != std::string::npos)
					{
						t.filename = t.filename.substr(pos + 1, t.filename.size() - pos - 1);
					}
				}
				{
					size_t pos = t.filename.find_last_of('//');
					if (pos != std::string::npos)
					{
						t.filename = t.filename.substr(pos + 1, t.filename.size() - pos - 1);
					}
				}
				StdUVGen* uvg = bm->GetUVGen();

				//
				//
				Interface* i = GetCOREInterface();
				if (i == NULL)
				{
					return false;
				}
				int ticksPerFrame = GetTicksPerFrame();
				Interval inv = i->GetAnimRange();
				TimeValue st = inv.Start();
				TimeValue et = inv.End();
				int numKey = (et - st) / ticksPerFrame;

				for (TimeValue t = st; t <= et; t += ticksPerFrame)
				{
					int tChanged = 1000 * t / 4800;
					keyframeT<float> u;
					u.data = uvg->GetUOffs(t);
					u.time = tChanged;
					mt.m_vTcFlowSpeedU.push_back(u);
					/////////////////////
					keyframeT<float> v;
					v.data = uvg->GetVOffs(t);
					v.time = tChanged;
					mt.m_vTcFlowSpeedV.push_back(v);
					/////////////////////
					keyframeT<float> r;
					r.data = uvg->GetAng(t);
					r.time = tChanged;
					mt.m_vRotateSpeed.push_back(r);
				}

				mt.m_textures.push_back(t);
				_materials.push_back(mt);
				return true;
			}
		}
	}
#endif

	return true;
}

void RocketTreeEnumProc::_extractMesh( INode* node )
{
	ObjectState os = node->EvalWorldState(0);
	if (os.obj == NULL)
	{
		return;
	}

	GeomObject* g = (GeomObject*)os.obj;

	if (g->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject* t = (TriObject*)g->ConvertToType(_time, Class_ID(TRIOBJ_CLASS_ID, 0));

		//
		Mesh* s = &t->GetMesh();
		/*
		DllExport MtlID Mesh::getFaceMtlIndex  (  int  i   )   


		Remarks:
		Retrieves the zero based material index of the 'i-th' face. 
		Parameters:
		int i

		Specifies the face index. This is the zero based index into the faces array.  
		*/
		s->buildNormals();

		int numVertices = s->getNumVerts();
		int numUVs = s->getNumTVerts();
		int numFaces = s->getNumFaces();
		
		// 
		int numVerticesLast = _vertices.size();
		for (int i = 0; i != numFaces; ++i)
		{
			TVFace tf = s->tvFace[i];
			MODELFACE f;
			for(int k = 0; k != 3; ++k)
			{
				DWORD index = s->faces[i].v[k];
				MODELVERTEX_19 v;
				Point3 p = s->getVert(index);
				Matrix3 mt = node->GetObjectTM(_time);
				p = p * mt;
				v.pos[0] = p.x;
#if SwitchYZ
				v.pos[1] = p.z;
				v.pos[2] = -p.y;
#else
				v.pos[1] = p.y;
				v.pos[2] = p.z;
#endif

				if (s->faces)
				{
					Point3 nom = s->getFaceNormal(i);
					v.normal[0] = nom.x;
					v.normal[1] = nom.y;
					v.normal[2] = nom.z;
				}

				if (s->vertCol)
				{
					VertColor c = s->vertCol[index];
					v.color[0] = c.x;
					v.color[1] = c.y;
					v.color[2] = c.z;
					v.color[3] = s->mapVerts(MAP_ALPHA)[index].x;
				}

				if (s->tVerts)
				{
					DWORD uvIndex = tf.getTVert(k);
					UVVert uv = s->tVerts[uvIndex];
					v.texcoords[0] = uv.x;
					v.texcoords[1] = 1 - uv.y;
				}
				memset(v.weights, 0, 4*sizeof(float));
				memset(v.bones, 0, 4*sizeof(uchar));
				if (index < _verticesBones.size())
				{
					VertexBW& bw = _verticesBones[index];
					static const unsigned int tBoneMax = 4;
					if (bw.size() > tBoneMax)
					{
						std::sort(bw.begin(), bw.end(), cmpBoneWeight);
					}
					for (size_t i = 0; i != tBoneMax && i < bw.size(); ++i)
					{
						v.bones[i] = 0;
						for (size_t k = 0; k != _boneSet.size(); ++k)
						{
							std::string boneName = bw[i].first;
							if (_boneSet[k].name == boneName)
							{
								_boneSet[k].referrenced = true;
								v.bones[i] = k;
								break;
							}
						}

						v.weights[i] = bw[i].second;
					}
				}
				bool isAlreadyExtracted = false;
				for (size_t i = numVerticesLast; i != _vertices.size(); ++i)
				{
					if (
						_vertices[i].pos[0] == v.pos[0] &&
						_vertices[i].pos[1] == v.pos[1] &&
						_vertices[i].pos[2] == v.pos[2] &&
						_vertices[i].texcoords[0] == v.texcoords[0] &&
						_vertices[i].texcoords[1] == v.texcoords[1] &&
						_vertices[i].color[0] == v.color[0] &&
						_vertices[i].color[1] == v.color[1] &&
						_vertices[i].color[2] == v.color[2] &&
						_vertices[i].bones[0] == v.bones[0] &&
						_vertices[i].bones[1] == v.bones[1] &&
						_vertices[i].bones[2] == v.bones[2] &&
						_vertices[i].bones[3] == v.bones[3] &&
						_vertices[i].weights[0] == v.weights[0] &&
						_vertices[i].weights[1] == v.weights[1] &&
						_vertices[i].weights[2] == v.weights[2] &&
						_vertices[i].weights[3] == v.weights[3])
					{
						isAlreadyExtracted = true;
						f.index[k] = i;
						break;
					}
				}
				if (!isAlreadyExtracted)
				{
					f.index[k] = _vertices.size();
					_vertices.push_back(v);
				}
			}

			_faces.push_back(f);
		}
		SUBMESH sm;
		if (_verticesBones.empty())
		{
			sm.animated = false;
		}
		else
		{
			sm.animated = true;
		}
		sm.icount = numFaces * 3;
		sm.istart = tIStart;
		tIStart += sm.icount;
		sm.matId = 0;
		std::string nodeName = node->GetName();
		strcpy(sm.name, nodeName.c_str());
		sm.vcount = _vertices.size() - numVerticesLast;
		sm.vstart = tVStart;
		tVStart += sm.vcount;

		_subMeshes.push_back(sm);
	}
}

bool RocketTreeEnumProc::_isMaterialExtracted( Mtl* m, unsigned short& matID)
{
	std::string mtlName = m->GetName();
	for (unsigned short i = 0; i != _materials.size(); ++i)
	{
		if (_materials[i].m_name == mtlName)
		{
			matID = i;
			return true;
		}
	}

	return false;
}

void RocketTreeEnumProc::_extractSkin( INode* node )
{
	///////////////////////////////////////////////
	// skin
	//////////////////////////////////////////////
	Object *oRef = node->GetObjectRef();
	if (_currentNodeIsRef)
	{
		oRef = XRefFunctions::getXRefItemSource(oRef);
	}
	if (oRef == NULL)
	{
		return;
	}
	// Is derived object ?
	SClass_ID superClassId = oRef->SuperClassID();
	if(	superClassId == GEN_DERIVOB_CLASS_ID ||
		superClassId == DERIVOB_CLASS_ID ||
		superClassId == WSM_DERIVOB_CLASS_ID)
	{
		IDerivedObject* dobj = (IDerivedObject*) oRef;

		ISkin *skin = NULL;
		for(int i = 0; i != dobj->NumModifiers(); ++i)
		{
			Modifier *m = dobj->GetModifier(i);
			if (m && m->ClassID() == SKIN_CLASSID)
			{
				skin = (ISkin *) m->GetInterface(I_SKIN);
				break;
			}
		}

		if (skin)
		{
			// wrangle a pointer to the skinning data
			ISkinContextData *skinData = skin->GetContextInterface(node);

			// loop through all the vertices that skin affects.  
			int skinnedVertexCount = skinData->GetNumPoints();
			for (int i = 0; i != skinnedVertexCount; ++i)
			{
				// the number of bones that control this vertex
				int vertexBoneInfluences = skinData->GetNumAssignedBones(i);
				VertexBW vbw;
				for (int k = 0; k != vertexBoneInfluences; ++k)
				{
					// get weight per bone influence -- ignore bones above
					// 4 and sum the weights to make it work, so leverage that feature
					int boneIdx = skinData->GetAssignedBone(i, k);
					INode* n = skin->GetBone(boneIdx);
					std::string boneName = n->GetName();
					float vertexWeight = skinData->GetBoneWeight(i, k);
					vbw.push_back(std::make_pair(boneName, vertexWeight));
				}
				_verticesBones.push_back(vbw);
			}

			// bone data
// 			int numBones =skin->GetNumBones();
// 			for (int i = 0; i != numBones; ++i)
// 			{
// 				INode* n = skin->GetBone(i);
// 				if (n)
// 				{
// 					std::string boneName = n->GetName();
// 					//
// 					Control* tmc = n->GetTMController();
// 					if (tmc)
// 					{
// 						_getKeyFrame(tmc);
// 					}
// 				}
// 			}

		}
	}
}

Modifier* RocketTreeEnumProc::_getPhysiqueModifier(INode *pNode)
{
	Object *pObj = pNode->GetObjectRef();
	if(pObj) return NULL;
	// Is it a derived object?
	while(pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		// Yes -> Cast
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>(pObj);
		// Iterate over all entries of the modifier stack
		int ModStacklndex = 0;
		while(ModStacklndex < pDerivedObj->NumModifiers())
		{
			// Get current modifier
			Modifier* pMod = pDerivedObj->GetModifier(ModStacklndex);
			//Is this Physique?
			if(pMod->ClassID() == Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B))
			{
				return pMod;
			}

			// Next modifier stack entry
			ModStacklndex++;
		}

		pObj = pDerivedObj->GetObjRef();
	} 
	
	// Not found
	return NULL;
}

eType RocketTreeEnumProc::_extractType( INode* node )
{
	_currentNodeIsRef = false;
	if (node == NULL)
	{
		return eType_Unknown;
	}

	Animatable* o = node->GetObjectRef();
	if (o == NULL)
	{
		return eType_Unknown;
	}

	// check for an XRef
	if (XRefFunctions::isXRefItem(o))
	{
		_currentNodeIsRef = true;
		// replace the current animatable by the x-ref object
		o = XRefFunctions::getXRefItemSource((Object*)o);
		if (o == NULL) 
			return eType_Unknown;
	}
	else if (XRefFunctions::isXRefMaterial(o))
	{
		//setIsXRefMaterial();
		_currentNodeIsRef = true;
		o = XRefFunctions::getXRefMaterialSource((Mtl*)o);
		if ( o == NULL) 
			return eType_Unknown;
	}

	// Modifiers are applied to the object, acquire the base object
	while ( o->SuperClassID() == GEN_DERIVOB_CLASS_ID )
	{
		IDerivedObject * derivedObject = ( IDerivedObject* ) o;
		o = derivedObject->GetObjRef();
	}

	SClass_ID superClassId = o->SuperClassID();

	Class_ID classId = o->ClassID();

	switch ( superClassId )
	{
	case GEOMOBJECT_CLASS_ID:
		{
			if (((Object*)o)->IsParticleSystem())
			{
				return eType_Particle;
			}
			if (classId == BONE_OBJ_CLASSID)
			{
				// bone
				return eType_Bone;
			}

			Control* ct = node->GetTMController();
			if (ct)
			{
				if (ct->ClassID() == BIPSLAVE_CONTROL_CLASS_ID	||
					ct->ClassID() == BIPBODY_CONTROL_CLASS_ID	|| 
					ct->ClassID() == FOOTPRINT_CLASS_ID			||
					ct->ClassID() == SKELOBJ_CLASS_ID	|| 
					ct->ClassID() == BIPED_CLASS_ID   			 
					)
				{
					// biped
					return eType_Biped;
				}
			}
			return eType_Mesh;
		}break;

	case HELPER_CLASS_ID:
		return (classId.PartA() == BONE_CLASS_ID) ? eType_Bone : eType_Helper;

	case MATERIAL_CLASS_ID: 
		return eType_Material;
	}
	

	return eType_Unknown;
}

void RocketTreeEnumProc::_extractBone( INode* node )
{
	Control* tmc = node->GetTMController();
	if (tmc)
	{
		_getKeyFrame(tmc);
	}

	//
	Interface* i = GetCOREInterface();
	if (i == NULL)
	{
		return;
	}

	//
	sBone b;
	b.id = _boneSet.size();
	b.name = node->GetName();
	if (node->GetParentNode())
	{
		b.parent = node->GetParentNode()->GetName();
	}
	//b.mat = node->GetObjectTM(0);
	b.mat = node->GetNodeTM(0);

	//Point3 pivot = b.mat.GetTrans();
	//Matrix3 firstOTM = b.mat;
	//
	if (GetExportOptions()->exportMode == eExportMode_MZ && !GetExportOptions()->exportAnims)
	{

	}
	else
	{
		int ticksPerFrame = GetTicksPerFrame();
		Interval inv = i->GetAnimRange();
		TimeValue st = inv.Start();
		TimeValue et = inv.End();
		int numKey = (et - st) / ticksPerFrame;
		Matrix3 initM;
		for (TimeValue t = st; t <= et; t += ticksPerFrame)
		{
			// This method returns the world space transformation matrix of the node at the specified time.
			// This matrix contains its parents transformation
			Matrix3 m = node->GetNodeTM(t);
			Matrix3 parentTM = node->GetParentTM(t);
			Matrix3 localTM = m * Inverse(parentTM);
			if(t == st)
			{
				b.cmat = convert2Standard(localTM);
			}
			Matrix3 iniTM = b.cmat;
			//Matrix3 dynamicTM = localTM * Inverse(iniTM);
			Matrix3 dynamicTM = localTM;

			///////////
			// Grab the transform
			Point3 p = dynamicTM.GetTrans();
			Point3 pp = convert2Standard(p);
			// Clear the transform on the matrix
			//m.NoTrans();
			int tChanged = 1000 * t / 4800;
			sKeyframeTranslation kt;
			kt.time = tChanged;
			kt.v[0] = pp.x;
			kt.v[1] = pp.y;
			kt.v[2] = pp.z;
			b.translationSet.push_back(kt);

			///////////////
			Quat q(dynamicTM);
			Quat qq = convert2Standard(q);
			qq.Normalize();
			sKeyframeRotation kr;
			kr.time = tChanged;
			kr.q[0] = qq.x;
			kr.q[1] = qq.y;
			kr.q[2] = qq.z; // ||(x y z)|| == sin(anlge/2)
			kr.q[3] = -qq.w; // cos(angle/2)
			b.rotationSet.push_back(kr);

			//////////////
			Point3 s;
			s.x = dynamicTM.GetRow(0).Length();
			s.y = dynamicTM.GetRow(1).Length();
			s.z = dynamicTM.GetRow(2).Length();
			Point3 ss = convert2Standard(s);
			sKeyframeScale ks;
			ks.time = tChanged;
			ks.v[0] = ss.x;
			ks.v[1] = ss.y;
			ks.v[2] = ss.z;
			b.scaleSet.push_back(ks);
		}
	}
	
	b.mat = convert2Standard(b.mat);
	_boneSet.push_back(b);
}

void RocketTreeEnumProc::readBone()
{
	for(size_t i = 0; i != _nodeSet.size(); ++i)
	{
		INode* node = _nodeSet[i];
		ObjectState o = node->EvalWorldState(_time);

		eType t = _extractType(node);
		switch(t)
		{
		case eType_Bone:
// 			{
// 				_extractBone(node);
// 			}break;
		case eType_Biped:
			{
				_extractBiped(node);
			}break;
		default:
			break;
		}
	}

	_setBoneParent();
}

void RocketTreeEnumProc::writeBone()
{
	//============================================================================
	// 开始写入数据
	ChunkSet cs;

	//============================================================================
	// 版本号
	cs.beginChunk("MVER");	
	uint ver = 29;
	cs.write(&ver,sizeof(ver));
	cs.endChunk();

	//============================================================================
	// 动画起止时间
	cs.beginChunk("MANM");
	//
	Interface* i = GetCOREInterface();
	if (i == NULL)
	{
		return;
	}
	Interval inv = i->GetAnimRange();
	unsigned int st = 1000 * inv.Start() / 4800;
	unsigned int et = 1000 * inv.End() / 4800;
	cs.write(&st, sizeof(unsigned int));
	cs.write(&et, sizeof(unsigned int));
	cs.endChunk();

	//============================================================================
	// 骨骼
	cs.beginChunk("MBON");
	unsigned int numBones = _boneSet.size();
	cs.write(&numBones, sizeof(unsigned int));
	for (size_t i = 0; i != _boneSet.size(); ++i)
	{
		sBone& b = _boneSet[i];
		cs.write(&b.id, sizeof(int));
		cs.write(&b.parentId, sizeof(int));

		//
		size_t s = b.translationSet.size();
		cs.write(&s, sizeof(size_t));
		writeSequence(cs.getCurrentMemoryFileWriter(), b.translationSet);

		s = b.rotationSet.size();
		cs.write(&s, sizeof(size_t));
		writeSequence(cs.getCurrentMemoryFileWriter(), b.rotationSet);

		s = b.scaleSet.size();
		cs.write(&s, sizeof(size_t));
		writeSequence(cs.getCurrentMemoryFileWriter(), b.scaleSet);
	
		bool l = false;
		cs.write(&l, sizeof(l));
		cs.write(&l, sizeof(l));
	}
	cs.endChunk();

	//============================================================================
	// 材质、纹理数据
	if (0)
	{
		cs.beginChunk("MTRS");
		uint nMaterials = _materials.size();
		cs.write(&nMaterials,sizeof(nMaterials));
		for(int i = 0;i< nMaterials; ++i)
		{
			material* mat = &_materials[i];

			// 材质名
			std::string matName = mat->m_name;
			uchar matNameLen = matName.size();
			cs.write(&matNameLen,sizeof(matNameLen));
			cs.write(matName.c_str(),matNameLen);

			//7版本加入透明度keyframe
			uint nKeyframes = mat->m_vTransparencies.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vTransparencies);

			//10版本加入材质颜色关键桢
			nKeyframes = mat->m_vColors.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vColors);

			//版本号26加入FlowSpeedU关键桢
			nKeyframes = mat->m_vTcFlowSpeedU.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(), mat->m_vTcFlowSpeedU);

			//版本号26加入FlowSpeedV关键桢
			nKeyframes = mat->m_vTcFlowSpeedV.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vTcFlowSpeedV);

			//版本号29加入RotateSpeed关键桢
			nKeyframes = mat->m_vRotateSpeed.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(),mat->m_vRotateSpeed);

			//
			nKeyframes = mat->m_vChangeStyle.size();
			cs.write(&nKeyframes,sizeof(nKeyframes));
			writeSequence(cs.getCurrentMemoryFileWriter(), mat->m_vChangeStyle);
		}
		cs.endChunk();
	}

	//============================================================================
	// 保存文件，结束
	std::string fileName = GetExportOptions()->m_outFilePath;
	cs.save(fileName);

	//
	_clear();	
}

void RocketTreeEnumProc::_setBoneParent()
{
	for (size_t i = 0; i != _boneSet.size(); ++i)
	{
		for (size_t k = 0; k != _boneSet.size(); ++k)
		{
			if (_boneSet[i].parent == _boneSet[k].name)
			{
				_boneSet[i].oldId = _boneSet[i].id;
				_boneSet[i].id = i;
				_boneSet[i].parentId = k;//_boneSet[k].id;
				_boneSet[k].children.push_back(/*_boneSet[i].id*/i);
				break;
			}
		}
	}
}

bool RocketTreeEnumProc::readMesh()
{
	for(size_t i = 0; i != _nodeSet.size(); ++i)
	{
		INode* node = _nodeSet[i];
		ObjectState o = node->EvalWorldState(_time);

		eType t = _extractType(node);
		switch(t)
		{
		case eType_Mesh:
			{
				if (GetExportOptions()->exportAll)
				{
				}
				else
				{
					if (node->Selected())
					{
					}
					else
					{
						break;
					}
				}
				if (node->IsHidden())
				{
					break;
				}
				_verticesBones.clear();
				_extractSkin(node);
				_extractMesh(node);
				unsigned short matID;
				if (_extractMaterial(node, matID))
				{
					_subMeshes[_subMeshes.size() - 1].matId = matID;
				}
				else
				{
					std::string errorMsg(node->GetName());
					errorMsg += " 没有可识别的材质";
					MessageBox(NULL, errorMsg.c_str(), "出错了！", MB_OK);
					return false;
				}
			}break;
		default:
			break;
		}
	}

	return true;
}

void RocketTreeEnumProc::_extractParticle( INode* node )
{
	ObjectState os = node->EvalWorldState(0);
	if (os.obj && os.obj->IsParticleSystem())
	{
		if(os.obj->ClassID() != tRainParticleCID)
		{
			return;
		}
		ParticleObject* po = GetParticleInterface(os.obj);
		if (po == NULL)
		{
			return;
		}

		//
		IMaxParticle* mp = (IMaxParticle*)po;

		sParticle p;
		p.bone = _getParticleBone(node);
		p.visible = mp->isVisible();
		p.speed = mp->getSpeed();
		p.variation = mp->getVariation();
		p.coneAngle = mp->getConeAngle();
		p.gravity = mp->getGravity();
		p.explosiveForce = mp->getExplosiveForce();
		p.life = mp->getLife();
		p.lifeVar = mp->getLifeVariation();
		p.emissionRate = mp->getEmissionRate();
		p.initialNum = mp->getInitNum();
		p.limitNum = mp->getLimitNum();
		p.attachToEmitter = mp->isAttachToEmitter();
		p.moveWithEmitter = mp->isMoveWithEmitter();
		p.forTheSword = mp->isForTheSword();
		p.forTheSwordInitialAngle = mp->getInitialAngleForTheSword();
		p.wander = mp->isWander();
		p.wanderRadius = mp->getWanderRadius();
		p.wanderSpeed = mp->getWanderSpeed();
		p.aspectRatio = mp->getAspectRatio();
		p.initialAngleBegin = mp->getInitialAngleBegin();
		p.initialAngleEnd = mp->getInitialAngleEnd();
		p.rotationSpeed = mp->getRotationSpeed();
		p.rotationSpeedVar = mp->getRotationSpeedVariation();
		p.width = mp->getWidth();
		p.length = mp->getLength();
		p.height = mp->getHeight();
		p.blendMode = mp->getBlendMode();
		//MaxSDK::Util::Path filePath(mp->getTextureFileName());
		p.textureFilename = mp->getTextureFileName();
		// 	
		p.textureRows = mp->getTextureRows();
		p.textureCols = mp->getTextureCols();
		p.changeStyle = mp->getChangeStyle();
		p.changeInterval = mp->getChangeInterval();
		p.tailLength = mp->getTailLength();
		p.timeMiddle = mp->getTailMiddle();
		Color c;
		c = mp->getColorStart();
		p.colorStart[0] = c.r;
		p.colorStart[1] = c.g;
		p.colorStart[2] = c.b;

		c = mp->getColorMiddle();
		p.colorMiddle[0] = c.r;
		p.colorMiddle[1] = c.g;
		p.colorMiddle[2] = c.b;

		c = mp->getColorEnd();
		p.colorEnd[0] = c.r;
		p.colorEnd[1] = c.g;
		p.colorEnd[2] = c.b;

		Point3 p3;

		p3 = mp->getAlpha();
		p.alpha[0] = p3.x;
		p.alpha[1] = p3.y;
		p.alpha[2] = p3.z;

		p3 = mp->getScale();
		p.scale[0] = p3.x;
		p.scale[1] = p3.y;
		p.scale[2] = p3.z;

		p3 = mp->getScaleVariation();
		p.scaleVar[0] = p3.x;
		p.scaleVar[1] = p3.y;
		p.scaleVar[2] = p3.z;

		p.fixedSize = mp->isFixedSize();

		p3 = mp->getHeadLifeSpan();
		p.headLifeSpan[0] = p3.x;
		p.headLifeSpan[1] = p3.y;
		p.headLifeSpan[2] = p3.z;

		p3 = mp->getHeadDecay();
		p.headDecay[0] = p3.x;
		p.headDecay[1] = p3.y;
		p.headDecay[2] = p3.z;

		p3 = mp->getTailLifeSpan();
		p.tailLifeSpan[0] = p3.x;
		p.tailLifeSpan[1] = p3.y;
		p.tailLifeSpan[2] = p3.z;

		p3 = mp->getTailDecay();
		p.tailDecay[0] = p3.x;
		p.tailDecay[1] = p3.y;
		p.tailDecay[2] = p3.z;

		p.head = mp->isHead();
		p.tail = mp->isTail();
		p.unshaded = mp->isUnShaded();
		p.unfogged = mp->isUnFogged();
		p.blockByY0 = mp->isBlockByYO();

		//
		Interface* i = GetCOREInterface();
		if (i == NULL)
		{
			return;
		}
		int ticksPerFrame = GetTicksPerFrame();
		Interval inv = i->GetAnimRange();
		TimeValue st = inv.Start();
		TimeValue et = inv.End();
		int numKey = (et - st) / ticksPerFrame;

		for (TimeValue t = st; t <= et; t += ticksPerFrame)
		{
			int tChanged = 1000 * t / 4800;

			//
			keyframeT<bool> visible;
			visible.data = mp->isVisible(t);
			visible.time = tChanged;
			p.keyframesVisible.push_back(visible);

			//
			keyframeT<float> speed;
			speed.data = mp->getSpeed(t);
			speed.time = tChanged;
			p.keyframesSpeed.push_back(speed);

			//
			keyframeT<float> speedVariation;
			speedVariation.data = mp->getVariation(t);
			speedVariation.time = tChanged;
			p.keyframesVariation.push_back(speedVariation);

			//
			keyframeT<float> coneAngle;
			coneAngle.data = mp->getConeAngle(t);
			coneAngle.time = tChanged;
			p.keyframesConeAngle.push_back(coneAngle);

			//
			keyframeT<float> gravity;
			gravity.data = mp->getGravity(t);
			gravity.time = tChanged;
			p.keyframesGravity.push_back(gravity);

			//
			keyframeT<float> explosiveForce;
			explosiveForce.data = mp->getExplosiveForce(t);
			explosiveForce.time = tChanged;
			p.keyframesExplosiveForce.push_back(explosiveForce);

			//
			keyframeT<float> emissionRate;
			emissionRate.data = mp->getEmissionRate(t);
			emissionRate.time = tChanged;
			p.keyframesEmissionRate.push_back(emissionRate);

			//
			keyframeT<float> width;
			width.data = mp->getWidth(t);
			width.time = tChanged;
			p.keyframesWidth.push_back(width);

			//
			keyframeT<float> length;
			length.data = mp->getLength(t);
			length.time = tChanged;
			p.keyframesLength.push_back(length);

			//
			keyframeT<float> height;
			height.data = mp->getHeight(t);
			height.time = tChanged;
			p.keyframesHeight.push_back(height);
		}

		p.processStillSequence();
		_particleSet.push_back(p);
	}
}

/*
	The biped hierarchy that is used to determine the relationships between the biped parent and child body parts is different than
	the hierarchy presented by the max nodes parent/child relationships.
	The main reason for this is to make the biped move more naturally when being animated. 
	For example, the upper arm links inherit their rotation from the center of mass, instead of from the clavicle, 
	which they are connected to. This allows the user to animate the spine without having to counter rotate the arms. 
*/
void RocketTreeEnumProc::_extractBiped( INode* node )
{
	//
	Interface* i = GetCOREInterface();
	if (i == NULL)
	{
		return;
	}

	//
	sBone b;
	b.id = _boneSet.size();
	b.name = node->GetName();
	if (node->GetParentNode())
	{
		b.parent = node->GetParentNode()->GetName();
	}
	b.mat = node->GetNodeTM(0);
#if SwitchYZ
	b.mat = convert2Standard(b.mat);
#endif
	//
	Quat offsetQ = node->GetObjOffsetRot();
	Point3 offsetP = node->GetObjOffsetPos();
	ScaleValue offsetS = node->GetObjOffsetScale();

	//Point3 pivot = b.mat.GetTrans();
	//Matrix3 firstOTM = b.mat;
	//
	int ticksPerFrame = GetTicksPerFrame();
	Interval inv = i->GetAnimRange();
	TimeValue st = inv.Start();
	TimeValue et = inv.End();
	int numKey = (et - st) / ticksPerFrame;
	Matrix3 initM;
	for (TimeValue t = st; t <= et; t += ticksPerFrame)
	{
		// This method returns the world space transformation matrix of the node at the specified time. 
		// This matrix contains its parents transformation
		Matrix3 m = node->GetNodeTM(t);
		Matrix3 parentTM = node->GetParentTM(t);
		Matrix3 localTM = m * Inverse(parentTM);
		if(t == st)
		{
			b.cmat = convert2Standard(localTM);
		}
		Matrix3 iniTM = b.cmat;
		//Matrix3 dynamicTM = localTM * Inverse(iniTM);
		Matrix3 dynamicTM = localTM;

#if SwitchYZ
		dynamicTM = convert2Standard(dynamicTM);
#endif
		///////////
		// Grab the transform
		Point3 p = dynamicTM.GetTrans();
		Point3 pp = convert2Standard(p);
		int tChanged = 1000 * t / 4800;
		sKeyframeTranslation kt;
		kt.time = tChanged;
		kt.v[0] = pp.x;
		kt.v[1] = pp.y;
		kt.v[2] = pp.z;
		b.translationSet.push_back(kt);

		///////////////
		Quat q(dynamicTM);
		Quat qq = convert2Standard(q);
		qq.Normalize();
		sKeyframeRotation kr;
		kr.time = tChanged;// boot
		kr.q[0] = qq.x; //*0.2258
		kr.q[1] = qq.y;
		kr.q[2] = qq.z; // ||(x y z)|| == sin(anlge/2)
		kr.q[3] = -qq.w; // cos(angle/2)// *1.146697808300388199035037909225
		b.rotationSet.push_back(kr);

		//////////////
		Point3 s;
		s.x = dynamicTM.GetRow(0).Length();
		s.y = dynamicTM.GetRow(1).Length();
		s.z = dynamicTM.GetRow(2).Length();
		Point3 ss = convert2Standard(s);
		if(0)
		{
			ss.x = 1.0f;
			ss.y = 1.0f;
			ss.z = 1.0f;
		}
		sKeyframeScale ks;
		ks.time = tChanged;
		ks.v[0] = ss.x;
		ks.v[1] = ss.y;
		ks.v[2] = ss.z;
		b.scaleSet.push_back(ks);
	}
	b.sampleLinear();
	//b.processStill();
	_boneSet.push_back(b);
}

void RocketTreeEnumProc::readParticle()
{
	for(size_t i = 0; i != _nodeSet.size(); ++i)
	{
		INode* node = _nodeSet[i];
		ObjectState o = node->EvalWorldState(_time);

		eType t = _extractType(node);
		switch(t)
		{
		case eType_Particle:
			{
				if (GetExportOptions()->exportAll)
				{
				}
				else
				{
					if (node->Selected())
					{
					}
					else
					{
						break;
					}
				}
				unsigned short matID;
				if (_extractMaterial(node, matID))
				{
					_extractParticle(node);		
					if(GetExportOptions()->copyTextures)
					{
						_materials[_materials.size() - 1].copyTextures();
						_materials.erase(_materials.end() - 1);
					}

				}
			}break;
		default:
			break;
		}
	}
}

std::string RocketTreeEnumProc::_getParticleBone( INode* node )
{
	///////////////////////////////////////////////
	// skin
	//////////////////////////////////////////////
	Object *oRef = node->GetObjectRef();
	// Is derived object ?
	if (oRef && oRef->SuperClassID() == GEN_DERIVOB_CLASS_ID)
	{
		IDerivedObject* dobj = (IDerivedObject*) oRef;

		ISkin *skin = NULL;
		for(int i = 0; i != dobj->NumModifiers(); ++i)
		{
			Modifier *m = dobj->GetModifier(i);
			if (m && m->ClassID() == SKIN_CLASSID)
			{
				skin = (ISkin *) m->GetInterface(I_SKIN);
				break;
			}
		}

		if (skin)
		{
			{
				int numBones = skin->GetNumBones();
				std::vector<std::string> boneNames;
				for (int i = 0; i != numBones; ++i)
				{
					boneNames.push_back(skin->GetBoneName(i));
				}
				if (boneNames.empty())
				{
					return "";
				}
				else
				{
					return boneNames[0];
				}
			}
			// wrangle a pointer to the skinning data
			ISkinContextData *skinData = skin->GetContextInterface(node);
			if (skinData == NULL)
			{
				return "";
			}
			// loop through all the vertices that skin affects.  
			int skinnedVertexCount = skinData->GetNumPoints();
			for (int i = 0; i != skinnedVertexCount; ++i)
			{
				// the number of bones that control this vertex
				int vertexBoneInfluences = skinData->GetNumAssignedBones(i);
				for (int k = 0; k != vertexBoneInfluences; ++k)
				{
					// get weight per bone influence -- ignore bones above
					// 4 and sum the weights to make it work, so leverage that feature
					int boneIdx = skinData->GetAssignedBone(i, k);
					INode* n = skin->GetBone(boneIdx);
					std::string boneName = n->GetName();
					float vertexWeight = skinData->GetBoneWeight(i, k);
					return boneName;
				}
			}
		}
	}

	return "";
}

bool RocketTreeEnumProc::_isBoneSelected()
{
	for(size_t i = 0; i != _nodeSet.size(); ++i)
	{
		INode* node = _nodeSet[i];
		ObjectState o = node->EvalWorldState(_time);

		eType t = _extractType(node);
		switch(t)
		{
		case eType_Bone:
		case eType_Biped:
			{
				if (node->Selected())
				{
					return true;
				}
			}break;
		default:
			break;
		}
	}

	return false;
}

std::vector<sBone> RocketTreeEnumProc::_boneSet;
