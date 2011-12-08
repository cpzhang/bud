#include "stdafx.h"
#include "EntityYZ.h"
#include "MemoryFileReader.h"
#include "EntityYZMaterial.h"
#include "Color.h"
#include "RenderSystem.h"
#include "SceneNode.h"
#include "D3D9Mapping.h"
#include "BufferFactory.h"
#include "TextureFactory.h"
#include "Skeleton.h"
#include "BoneNode.h"
#include "IEffect.h"


namespace Euclid
{
	template<class T,class OT> void ReadKeyFrames(IFileRead* mem, sKeyFrames<T>& pKeyFrames, u32 last_time = 0)
	{
		u32 numKeyFrames;
		mem->read(&numKeyFrames,sizeof(u32));

		for (u32 i = 0;i < numKeyFrames;i++)
		{
			int time;
			OT data;
			mem->read(&time,sizeof(int));
			mem->read(&data,sizeof(OT));

			sKeyFrame<T> keyFrame;
			keyFrame.time = last_time + time;
			
			u32 tm1 = keyFrame.time % 100;
			if (tm1 == 2 || tm1 == 5 || tm1 == 8)
			{
				keyFrame.time++;
			}

			keyFrame.v = data;
			
			pKeyFrames.addKeyFrame(keyFrame);
		}
	}

	EntityYZ::EntityYZ( EntityYZPara* p )
		:	_para(NULL), _version(0),/* _ib(NULL), _vb(NULL),*/ _skeletons(NULL), _speed(0.5f)
	{
		_para = new EntityYZPara(p);
		_name = _para->_name;
	}


	EntityYZ::~EntityYZ()
	{
		destroy();
	}

	bool EntityYZ::create()
	{
		RenderableObject::create();

		_vdt = VD_POSITION_TEXTURE;
		// *.mz
		_decodeMZ();
		
		// *.mzi
		_decodeMZI();

		// *.fb
		_decodeFB();

		_iniIB();
		
		_initVB();

		_initSkeleton();

		//setAnimation("Stand");
		//setSpeed(1.0f);

		return true;
	}

	void EntityYZ::destroy()
	{
		safeDelete(_para);
		STLDeleteAssociate(_am);

		STLDestroyAssociate(_sm);

		STLDeleteSequence(_vertices);

		STLDeleteSequence(_faces);

		STLDeleteSequence(_materials);

		STLDeleteSequence(_bones);

		STLDeleteSequence(_subEntities);

		STLDestroySequence(_skeletons);

		safeDestroy(_ib);
		safeDestroy(_vb);
		
	}

	void EntityYZ::update( u32 current, u32 delta )
	{
	
		return;

		if (_clock.current == _clock.end)
		{
			_clock.current = _clock.start;
		}
		
		_clock.current += delta * _speed;
		
		if (_clock.current > _clock.end)
		{
			_clock.current = _clock.end;
		}

		//_clock.current = _clock.end;
		const AnimationTime *pAT  = &_clock;

		// update bones
		for (size_t i = 0; i < _skeletons.size(); ++i)
		{
			Skeleton* b = _skeletons[i];
			b->update(_clock);
		}

#if 0
		for (size_t i = 0; i < _materials.size(); ++i)
		{
			EntityYZMaterial* m = _materials[i];
			EntityYZMaterialLayer& l = m->getLayer(0);

			if (l._rows > 1 || l._cols >1)
			{
				breakable;
			}
			else
			{
				if (l._flowU)
				{
					l._flowSpeedU = m->_ani.flowSpeedU.getFrame(pAT);

					l._flowedU += l._flowSpeedU * delta * 0.0001f;
				}

				if (l._flowV)
				{
					l._flowSpeedV = m->_ani.flowSpeedV.getFrame(pAT);

					l._flowedV += l._flowSpeedV * delta * 0.0001f;
				}

				l._flowedTime += delta;

				if (l._flowTime > 0)
				{
					if (l._flowedTime > l._flowTime)
					{
						l._flowedTime = 0.0f;
						l._flowedU = 0.0f;
						l._flowedV = 0.0f;
					}
				}
			}
		}
#endif

#if 1
		// update vertex
		POSITION_TEXTURE* data = (POSITION_TEXTURE*)_vb->lock(0, 0, 0);

		if (data == NULL)
		{
			return;
		}

		for (size_t i = 0; i < _vertices.size(); ++i)
		{
			sVertexEx* e = _vertices[i];

			data[i]._pos = Vec3::ZERO;

			bool gotValue =false;
			for (size_t k = 0; k < 4; ++k)
			{
				if(e->weights[k] > 0)
				{
					if (e->bones[k] < _bones.size())
					{
						gotValue = true;
						Vec3 v;
						sBone* b = _bones[e->bones[k]];
						Mat4 m = b->getFinalMatrix();
						v = m * e->pos;
						v *= (float)e->weights[k];

						data[i]._pos += v;
					}
					else
					{
						Error("Illegal Bone!");
						break;
					}
				}
			}

			if (!gotValue)
			{
				data[i]._pos = /*Mat4::XYZ2XZY * */e->pos;
			}
		}

		_vb->unLock();
#endif
	}

	void EntityYZ::preRender()
	{
		//
		if (_currentNode)
		{
			_renderSystem->setVertexDeclaration(VD_POSITION_TEXTURE);

			_renderSystem->setSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
			_renderSystem->setSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);

			_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//
			_renderSystem->setRenderState(D3DRS_LIGHTING, false);

			_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, true);
			_renderSystem->setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

			//_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			//
			_currentNode->_updateFromParent();
			D3DXMATRIX m = D3D9Mapping::makeD3DXMatrix(_currentNode->_getFullTransform());
			_renderSystem->setWorldMatrix(&m);

			////
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP,	D3DTOP_MODULATE);
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
			_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

			//默认值
			_renderSystem->setTextureStageState(1, D3DTSS_TEXCOORDINDEX, 1);		//默认值为1
			_renderSystem->setTextureStageState(1, D3DTSS_ALPHAOP,	D3DTOP_DISABLE);
			_renderSystem->setTextureStageState(1, D3DTSS_COLOROP,	D3DTOP_DISABLE);
			_renderSystem->setTextureStageState(0, D3DTSS_COLOROP,	D3DTOP_MODULATE);
			_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1,D3DTA_TEXTURE);
			_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2,D3DTA_CURRENT);
		}
	}

	void EntityYZ::postRender()
	{
		_renderSystem->setVertexDeclaration(VD_NULL);

			_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void EntityYZ::renderNormal()
	{
		SubMeshMap::iterator it = _sm.begin();
		for (; it != _sm.end(); ++it)
		{
			ITexture* tex = NULL;
 			SUBMESH_BASE* b = it->second->_base;
			if (b && b->matId != _materials.size())
			{
				EntityYZMaterial* m = _materials[b->matId];
				if (m && !m->_layers.empty())
				{
					tex = m->getLayer(0)._texture;
					if (tex == NULL)
					{
						tex = TextureFactory::getInstancePtr()->createTextureFromFile(m->getLayer(0)._tex);
						if (tex == NULL)
						{
							tex = TextureFactory::getInstancePtr()->createTextureFromFile("image\\monster_tujiu_mp.dds");
						}
						
						m->getLayer(0)._texture = tex;
					}
				}
			}
			_effect->setTexture("g_MeshTexture", tex);
			if (tex)
			{
				_effect->setInt("g_isTextured", true);
				//_renderSystem->setTexture(0, tex);
				_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			}
			else
			{
				_effect->setInt("g_isTextured", false);
				_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			}
			
			_renderSystem->setStreamSource(0, _vb, 0, sizeof(POSITION_TEXTURE));
			_renderSystem->setIndices(_ib);
			_renderSystem->drawIndexedPrimitive(D3DPT_TRIANGLELIST, b->vstart, 0, b->vcount, b->istart, b->icount / 3);
			break;
		}
	}

	void EntityYZ::calcAABB()
	{
		if (_vertices.size() <= 0)
		{
			return;
		}

		sVertexEx* v = _vertices[0];

		_aabb._min = Vec3(v->pos);
		_aabb._max = Vec3(v->pos);
		
		for (size_t i = 1; i < _vertices.size(); ++i)
		{
			v = _vertices[i];

			_aabb.updateMin(v->pos);
			_aabb.updateMax(v->pos);
		}
	}

	void EntityYZ::calcSphere()
	{
		_sphere = Sphere(_aabb);
	}

	bool EntityYZ::_decodeMZI()
	{
		std::string full_path;
		if (_para->_relativePath)
		{
			Buddha::FileSystem::getInstancePtr()->getDataDirectory(full_path);
			full_path += _para->_path;
		}
		else
		{
			full_path = _para->_path;
		}
		full_path += 'i';

		if (!Buddha::FileSystem::getInstancePtr()->isFileExist(full_path))
		{
			return true;
		}

		std::ifstream f(full_path.c_str(), std::ios::binary);
		if (!f.good())
		{
			return false;
		}

		u8 count = 0;
		f.read((char*)&count, sizeof(u8));

		for (size_t i = 0; i < count; ++i)
		{
			u8 name_length = 0;
			f.read((char*)&name_length, sizeof(u8));

			char* n = new char[name_length + 1];
			f.read((char*)n, name_length);
			n[name_length] = 0;
			
			_fbs.push_back(std::string(n));
		}

		f.close();

		return true;
	}

	bool EntityYZ::_decodeMZ()
	{
		std::string full_path;
		if (_para->_relativePath)
		{
			Buddha::FileSystem::getInstancePtr()->getDataDirectory(full_path);
			full_path += _para->_path;
		}
		else
		{
			full_path = _para->_path;
		}
		
		std::ifstream f(full_path.c_str(), std::ios::binary);
		if (!f.good())
		{
			return false;
		}

		//	[Tag Size Data]
		Tag t;
		Size s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(Tag));
			f.read((char*)&s, sizeof(Size));

			if (s <= 0)
			{
				continue;
			}

			char* buffer = new char[s];
			f.read(buffer, s);

			MemoryFileReader* mem = new MemoryFileReader(buffer, s);

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;
			
			switch (t)
			{
			case 'MVER':	// version
				{
					mem->read(&_version, sizeof(u32));
					if (_version < 1)
					{
						return false;
					}
				}
				break;

			case 'MSUB':	// [SUBMESH_BASE name]
				{
					if (_version >= 20)
					{
						while(!mem->isEnd())
						{
							SUBMESH_BASE* b = new SUBMESH_BASE;
							mem->read(b, sizeof(SUBMESH_BASE));

							char* n = new char[b->nameLen + 1];
							mem->read(n, b->nameLen);
							n[b->nameLen] = 0;
							std::string s(n);

							_sm[s] = new sSubMesh(b, s);
						}
					}
					else
					{
						while(!mem->isEnd())
						{
							sSubMeshOld* b = new sSubMeshOld;
							if (mem->read(b, sizeof(sSubMeshOld)) > 0)
							{
								//mem->seek(1);
								_sm[b->name] = new sSubMesh(b);
								safeDelete(b);
							}
							else
							{
								safeDelete(b);
								break;
							}
						}
					}

				}
				break;

			case 'MVTX':	// [use_color vertex]
				{
					_hasColor = true;

					// 版本号15加入了是否使用顶点颜色的选项
					if (_version >= 15)
					{
						mem->read(&_hasColor, sizeof(bool));
					
						if (_hasColor)
						{
							while(!mem->isEnd())
							{
								sVertexEx* v = new sVertexEx;
								mem->read(v, sizeof(sVertexEx));
								_vertices.push_back(v);
							}	
						}
						else
						{
							while(!mem->isEnd())
							{
								sVertex* v = new sVertex;
								mem->read(v, sizeof(sVertex));
								_vertices.push_back(new sVertexEx(v));
							}
						}
					}
					else
					{
						while(!mem->isEnd())
						{
							sVertexOld* v = new sVertexOld;
							mem->read(v, sizeof(sVertexOld));
							_vertices.push_back(new sVertexEx(v));
						}	
					}
					
					calcAABB();
					calcSphere();
				}
				break;

			case 'MFAC': // [Index0 + Index1 + Index2]
				{
					while(!mem->isEnd())
					{
						sFace* f = new sFace;
						mem->read(f, sizeof(sFace));
						_faces.push_back(f);
					}
				}
				break;

			case 'MMTX': // material_count + [material]
				{
					/*static std::vector<sKeyFrames<float> > tKeyframesTransparency;
					static std::vector<sKeyFrames<Color3> > tKeyframesColor;
					static std::vector<sKeyFrames<float> > tKeyframesFlowSpeedU;
					static std::vector<sKeyFrames<float> > tKeyframesFlowSpeedV;
					static std::vector<sKeyFrames<short> > tKeyframesChangeStyle;
					tKeyframesTransparency.clear();
					tKeyframesColor.clear();
					tKeyframesFlowSpeedU.clear();
					tKeyframesFlowSpeedV.clear();
					tKeyframesChangeStyle.clear();*/

					u32 c;
					mem->read(&c, sizeof(u32));
					for (size_t i = 0; i < c; ++i)
					{
						EntityYZMaterial* m = new EntityYZMaterial;

						u8 name_length = 0;
						mem->read(&name_length, sizeof(u8));

						char* name = new char[name_length + 1];
						name[name_length] = 0;
						mem->read(name, name_length);

						m->setName(name);

						bool lighting = false;
						mem->read(&lighting, sizeof(bool));

						Color ambient;
						mem->read(&ambient, sizeof(Color));

						Color diffuse;
						mem->read(&diffuse, sizeof(Color));

						Color specular;
						mem->read(&specular, sizeof(Color));

						Color emissive;
						mem->read(&emissive, sizeof(Color));

						bool trans;
						mem->read(&trans, sizeof(bool));

						bool twoSides;
						mem->read(&twoSides, sizeof(bool));

						float tcFlowSpeedU = 1.0f;
						float tcFlowSpeedV = 1.0f;

						//版本号4加入贴图坐标流动
						bool tcFlowU = false;
						bool tcFlowV = false;
						bool tcRotate = false;
						bool tcAngleReset = false;
						bool tcPositionReset = false;

						u16 flowTime = 0;

						static sKeyFrames<float> flowSpeedU,flowSpeedV, rotateSpeed;
						flowSpeedU.clear();
						flowSpeedU.setStaticData(1.0f);
						flowSpeedV.clear();
						flowSpeedV.setStaticData(1.0f);

						//版本号4加入贴图坐标流动
						if (_version >= 4)
						{
							mem->read(&tcFlowU, sizeof(bool));
							mem->read(&tcFlowV, sizeof(bool));
							if(_version >= 29)
							{
								mem->read(&tcRotate,sizeof(tcRotate));
								mem->read(&tcAngleReset,sizeof(tcAngleReset));
								mem->read(&tcPositionReset,sizeof(tcPositionReset));
							}
							mem->read(&tcFlowSpeedU, sizeof(Real));

							if (_version >= 26)
							{
								ReadKeyFrames<float,float>(mem, flowSpeedU);
							}

							//tKeyframesFlowSpeedU.push_back(flowSpeedU);

							mem->read(&tcFlowSpeedV,sizeof(float));
							flowSpeedV.setStaticData(tcFlowSpeedV);

							if(_version >= 26)
							{
								ReadKeyFrames<float,float>(mem, flowSpeedV);
							}
							
							//tKeyframesFlowSpeedV.push_back(flowSpeedV);
							if(_version >= 28)
							{
								mem->read(&flowTime, sizeof(u16));
							}
							//
							if(_version >= 29)
							{
								ReadKeyFrames<float,float>(mem, rotateSpeed);
							}
						}

						//版本号9加入材质不投影
						bool dontCastShadow = false;
						if (_version >= 9)
						{
							mem->read(&dontCastShadow,sizeof(bool));
						}

						//版本号12加入不打光材质
						bool unlit = false;
						if (_version >= 12)
						{
							mem->read(&unlit,sizeof(bool));
						}

						//版本号14加入深度测试和写入
						bool noDepthCheck = false;
						bool noDepthWrite = false;
						if (_version >= 14)
						{
							mem->read(&noDepthCheck,sizeof(bool));
							mem->read(&noDepthWrite,sizeof(bool));
						}

						//版本号27加入additiveLayer
						bool additiveLayer = false;
						if(_version >= 27)
						{
							mem->read(&additiveLayer,sizeof(bool));
						}

						static sKeyFrames<float> tTransparency;
						tTransparency.clear();
						tTransparency.setStaticData(1.0f);

						//版本号7加入透明度
						if (_version >= 7)
						{
							Real staticTransparency;
							mem->read(&staticTransparency,sizeof(Real));

							tTransparency.setStaticData(staticTransparency);
							
							ReadKeyFrames<float,float>(mem, tTransparency);

							//tKeyframesTransparency.push_back(tTransparency);
						}

						static sKeyFrames<Color3> tColor;
						tColor.clear();
						tColor.setStaticData(Color3(1.0f,1.0f,1.0f));

						//版本号10加入颜色关键桢
						if (_version >= 10)
						{
							Color3 staticColor;
							mem->read(&staticColor,sizeof(Color3));

							tColor.setStaticData(staticColor);

							ReadKeyFrames<Color3, Color3>(mem, tColor);
							
							//tKeyframesColor.push_back(tColor);
						}

						//版本号7加入序列图
						s16 rows = 1;
						s16 cols = 1;
						s16 changeInterval = 30;
						s16 changeStyle = 0;

						static sKeyFrames<short> tKFChangeStyle;
						tKFChangeStyle.clear();
						tKFChangeStyle.setStaticData(0);

						if (_version >= 7)
						{
							mem->read(&rows,sizeof(s16));
							mem->read(&cols,sizeof(s16));
							mem->read(&changeStyle,sizeof(s16));

							tKFChangeStyle.setStaticData(changeStyle);

							if(_version >= 26)
							{
								ReadKeyFrames<short,short>(mem, tKFChangeStyle);
							}

							//tKeyframesChangeStyle.push_back(tKFChangeStyle);

							mem->read(&changeInterval,sizeof(s16));
						}

						u32 nTexture = 0;
						mem->read(&nTexture,sizeof(u32));
						for (size_t i = 0; i < nTexture; i++)
						{
							EntityYZMaterialLayer layer;

							layer._twoSides = twoSides;
							layer._flowU = tcFlowU;
							layer._flowV = tcFlowV;
							layer._flowSpeedU = tcFlowSpeedU;
							layer._flowSpeedV = tcFlowSpeedV;
							layer._flowTime = flowTime;
							layer._cols = cols;
							layer._rows = rows;
							layer._changeStyle = changeStyle;
							layer._changeInterval = changeInterval;
							layer._notCastShadow = dontCastShadow;
							layer._unShaded = unlit;
							layer._noDepthSet = noDepthWrite;
							layer._noDepthTest = noDepthCheck;
							layer._additiveLayer = additiveLayer;

							u8 name_length = 0;
							mem->read(&name_length, sizeof(u8));

							char* name = new char[name_length + 1];
							name[name_length] = 0;
							mem->read(name, name_length);
							
							layer._tex = name;
// 							if (layer._tex.find(':') == std::string::npos)
// 							{
// 								std::string baseDir(full_path);
// 								Buddha::FileSystem::getInstancePtr()->getParent(baseDir);
// 								baseDir += '\\';
// 								layer._tex = baseDir + name;
// 							}
							std::string parentPath;
							{
								size_t pos = full_path.find_last_of('\\');
								if (pos != std::string::npos)
								{
									parentPath = full_path.substr(0, pos+1);
								}
							}
							
							{
								size_t pos = layer._tex.find_last_of('\\');
								if (pos != std::string::npos)
								{
									layer._tex = layer._tex.substr(pos + 1, layer._tex.size() - pos - 1);
								}
							}
							{
								size_t pos = layer._tex.find_last_of('//');
								if (pos != std::string::npos)
								{
									layer._tex = layer._tex.substr(pos + 1, layer._tex.size() - pos - 1);
								}
							}
							layer._tex = parentPath + layer._tex;

							u8 opType;
							mem->read(&opType,sizeof(u8));
							int op = opType;
							layer._bm = (eBlendMode)op;

							m->addLayer(layer);
						}

						m->_ani.alpha = tTransparency;
						m->_ani.color = tColor;
						m->_ani.flowSpeedU = flowSpeedU;
						m->_ani.flowSpeedV = flowSpeedV;
						m->_ani.changeStyle = tKFChangeStyle;

						_materials.push_back(m);
					}
				}
				break;

			case 'MANM': //count + [namelen + namearray + starttime + endtime]...
				{
					u32 count = 0;
					mem->read(&count, sizeof(u32));
				
					for (size_t i = 0; i < count; ++i)
					{
						u8 name_length = 0;
						mem->read(&name_length, sizeof(u8));

						char* name = new char[name_length + 1];
						mem->read(name, name_length);
						name[name_length] = 0;

						u32 time_start = 0;
						u32 time_end = 0;
						mem->read(&time_start, sizeof(u32));
						mem->read(&time_end, sizeof(u32));

						std::string s(name);
						_am[name] = new sAnimation(name, time_start, time_end);
					}
				}
				break;

			case 'MBON':
				{
					u32 nBones;
					mem->read(&nBones,sizeof(u32));

					for (size_t i = 0; i < nBones; ++i)
					{
						sBone* b = new sBone;

						int id;
						mem->read(&id,sizeof(int));

						u8 JointnameLen; 
						mem->read(&JointnameLen,sizeof(u8));
						char* name = new char[JointnameLen + 1];
						mem->read(name,JointnameLen);
						name[JointnameLen] = 0;
						b->setName(name);

						int parent;
						mem->read(&parent,sizeof(int));

						Mat4 initialMatrix = Mat4::IDENTITY;
						Vec3 pivot;
						if (_version < 5)
						{
							mem->read(&pivot, sizeof(Vec3));
							initialMatrix.setTrans(pivot);
						}
						else
						{
							//版本号为5以后，导出了初始矩阵
							sTranMatrix tm;
							mem->read(&tm,sizeof(tm));
							
							for (int i = 0;i < 4;i++)
								for (int j = 0;j < 3;j++)
								{
									initialMatrix[j][i] = tm._mat[i][j];
								}
							
							pivot.x = tm._mat[3][0];
							pivot.y = tm._mat[3][1];
							pivot.z = tm._mat[3][2];
						}

// 						Mat4 cMat = Mat4::IDENTITY;
// 						{
// 							sTranMatrix tm;
// 							mem->read(&tm,sizeof(tm));
// 
// 							for (int i = 0;i < 4;i++)
// 								for (int j = 0;j < 3;j++)
// 								{
// 									cMat[j][i] = tm._mat[i][j];
// 								}
// 						}

// 						b->_hasBillboarded = false;
// 						b->_billboardedLockX = false;
// 						b->_billboardedLockY =false;
// 						b->_billboardedLockZ = false;
// 						b->_cameraAnchored = false;
// 						b->_notInheritRotation = false;
// 						b->_notInheritScaling = false;
// 						b->_notInheritTranslation = false;
//						b->_name = name;
						strcpy(b->_name, name);
						b->_objectId = id;
						b->_parentId = parent;
						b->_pivotPoint = pivot;
						b->_initialMatrix = initialMatrix;
//						b->_cMtx = cMat;
//						b->_pivotRotation = false;

						b->_translation.setInterpolationType(eInterpolationType_Bezier_Linear);
						b->_rotation.setInterpolationType(eInterpolationType_Bezier_Cubic);
						b->_scale.setInterpolationType(eInterpolationType_Bezier_Linear);

						u32 nKeyframes;
						mem->read(&nKeyframes,sizeof(u32));
						for (size_t i = 0; i < nKeyframes; i++)
						{
							sModelKeyframeTranslation kf;
							mem->read(&kf,sizeof(sModelKeyframeTranslation));

							sKeyFrame<Vec3> kfTranslation(kf._time,Vec3(kf._v[0],kf._v[1],kf._v[2]));

							b->_translation.addKeyFrame(kfTranslation);
						}

						mem->read(&nKeyframes,sizeof(u32));
						for (size_t i = 0; i < nKeyframes; i++)
						{
							sModelKeyframeRotation kf;
							mem->read(&kf,sizeof(sModelKeyframeRotation));

							sKeyFrame<Quaternion> kfRotation(kf._time,Quaternion(kf._q[0], kf._q[1], kf._q[2], kf._q[3]));

							b->_rotation.addKeyFrame(kfRotation);
						}

						mem->read(&nKeyframes,sizeof(u32));
						for (size_t i = 0; i < nKeyframes; i++)
						{
							sModelKeyframeScale kf;
							mem->read(&kf,sizeof(sModelKeyframeScale));

							sKeyFrame<Vec3> kfScale(kf._time, Vec3(kf._v[0],kf._v[1],kf._v[2]));

							b->_scale.addKeyFrame(kfScale);
						}

						_bones.push_back(b);

						bool hasRibbonSystem;
						bool hasParticleSystem;
						mem->read(&hasRibbonSystem,sizeof(bool));
						mem->read(&hasParticleSystem,sizeof(bool));

						if (hasRibbonSystem)
						{
							breakable;
						}

						if (hasParticleSystem)
						{
							bool visible;
							mem->read(&visible,sizeof(bool));

							Real speed;
							mem->read(&speed,sizeof(Real));

							Real variation;
							mem->read(&variation,sizeof(Real));

							Real coneAngle;
							mem->read(&coneAngle,sizeof(Real));

							Real gravity;
							mem->read(&gravity,sizeof(Real));

							Real explosiveForce = 0;
							if (_version >= 17)
							{
								mem->read(&explosiveForce,sizeof(Real));
							}

							Real life;
							mem->read(&life,sizeof(Real));

							Real lifeVar = 0.0f;
							//版本号5之后，生命值加入随机值支持
							if (_version >= 5)
							{
								mem->read(&lifeVar,sizeof(Real));
							}

							Real emissionRate;
							mem->read(&emissionRate,sizeof(Real));

							s16 initialNum = 0;
							if (_version >= 17)
							{
								mem->read(&initialNum,sizeof(s16));
							}

							s16 limitNum = 32;
							if (_version >= 11)
							{
								mem->read(&limitNum,sizeof(s16));
							}

							bool attachToEmitter = false;
							if (_version >= 13)
							{
								mem->read(&attachToEmitter,sizeof(bool));
							}

							bool moveWithEmitter = false;
							if (_version >= 18)
							{
								mem->read(&moveWithEmitter,sizeof(bool));
							}

							bool forTheSword = false;
							if(_version >= 23)
							{
								mem->read(&forTheSword,sizeof(bool));
							}

							Real forTheSwordInitialAngle = 0.0f;
							if(_version >= 24)
							{
								mem->read(&forTheSwordInitialAngle,sizeof(Real));
							}

							bool wander = false;
							Real wanderRadius = 0.0f;
							Real wanderSpeed = 0.0f;
							if(_version >= 25)
							{
								mem->read(&wander,sizeof(bool));
								mem->read(&wanderRadius,sizeof(Real));
								mem->read(&wanderSpeed,sizeof(Real));
							}

							Real aspectRatio = 1.0f;
							//版本号5之后加入宽高比支持
							if (_version >= 5)
							{
								mem->read(&aspectRatio,sizeof(Real));
							}

							Real angleBegin = 0.0f;
							if (_version>= 6)
							{
								mem->read(&angleBegin,sizeof(Real));
							}

							Real angleEnd = 0.0f;
							if (_version >= 6)
							{
								mem->read(&angleEnd,sizeof(Real));
							}

							Real rotationSpeed = 0.0f;
							if (_version >= 6)
							{
								mem->read(&rotationSpeed,sizeof(Real));
							}

							Real rotationSpeedVar = 0.0f;
							if (_version >= 15)
							{
								mem->read(&rotationSpeedVar,sizeof(Real));
							}

							Real width;
							mem->read(&width,sizeof(Real));

							Real length;
							mem->read(&length,sizeof(Real));

							Real height = 0.0f;
							if (_version >= 16)
							{
								mem->read(&height,sizeof(Real));
							}

							s16 blendMode;
							mem->read(&blendMode,sizeof(s16));

							s16 rows;
							mem->read(&rows,sizeof(s16));

							s16 cols;
							mem->read(&cols,sizeof(s16));

							s16 changeStyle = 0;
							if (_version >= 6)
							{
								mem->read(&changeStyle,sizeof(s16));
							}

							s16 changeInterval = 1000;
							if (_version >= 6)
							{
								mem->read(&changeInterval,sizeof(s16));
							}

							Real tailLength;
							mem->read(&tailLength,sizeof(Real));

							Real timeMiddle;
							mem->read(&timeMiddle,sizeof(Real));

							Color3 colorStart;
							mem->read(&colorStart,sizeof(Color3));

							Color3 colorMiddle;
							mem->read(&colorMiddle,sizeof(Color3));

							Color3 colorEnd;
							mem->read(&colorEnd,sizeof(Color3));

							Vec3 alpha;
							mem->read(&alpha,sizeof(Vec3));

							Vec3 scale;
							mem->read(&scale,sizeof(Vec3));

							Vec3 scaleVar = Vec3(0.0f,0.0f,0.0f);
							//版本号5之后，加入缩放随机值
							if (_version >= 5)
							{
								mem->read(&scaleVar,sizeof(Vec3));
							}

							bool fixedSize = false;
							if(_version >= 21)
							{
								mem->read(&fixedSize,sizeof(bool));
							}

							typedef short short3[3];

							short3 headLifeSpan;
							mem->read(&headLifeSpan,sizeof(short3));

							short3 headDecay;
							mem->read(&headDecay,sizeof(short3));

							short3 tailLifeSpan;
							mem->read(&tailLifeSpan,sizeof(short3));

							short3 tailDecay;
							mem->read(&tailDecay,sizeof(short3));

							bool head;
							mem->read(&head,sizeof(bool));

							bool tail;
							mem->read(&tail,sizeof(bool));

							bool unshaded;
							mem->read(&unshaded,sizeof(bool));

							bool unfogged;
							mem->read(&unfogged,sizeof(bool));

							bool blockByY0 = false;
							if(_version >= 22)
							{
								mem->read(&blockByY0,sizeof(bool));
							}

							u8 textureFilenameLen;
							mem->read(&textureFilenameLen,sizeof(textureFilenameLen));
							char* filename = new char[textureFilenameLen + 1];
							mem->read(filename,textureFilenameLen);
							filename[textureFilenameLen] = 0;

							// 
							sParticleEmitter* pData = new sParticleEmitter;

							pData->filterMode = (eBlendMode)blendMode;
							pData->speed.setStaticData(speed);
							pData->variation.setStaticData(variation);
							pData->latitude.setStaticData(coneAngle);
							pData->gravity.setStaticData(gravity);
							pData->explosiveForce.setStaticData(explosiveForce);
							pData->visibility.setStaticData(visible);
							pData->emissionRate.setStaticData(emissionRate);
							pData->width.setStaticData(width);
							pData->length.setStaticData(length);
							pData->height.setStaticData(height);

							pData->initialNum = initialNum;
							pData->limitNum = limitNum;
							pData->attachToEmitter = attachToEmitter;
							pData->moveWithEmitter = moveWithEmitter;
							pData->forTheSword = forTheSword;
							pData->forTheSwordInitialAngle = forTheSwordInitialAngle;
							pData->wander = wander;
							pData->wanderRadius = wanderRadius;
							pData->wanderSpeed = wanderSpeed;

							std::string full_path;
							Buddha::FileSystem::getInstancePtr()->getDataDirectory(full_path);
							full_path += _para->_path;
							Buddha::FileSystem::getInstancePtr()->getParent(full_path);
							full_path += '\\';
							full_path += filename;
							pData->textureFilename = full_path;

							pData->segmentColor1 = colorStart;
							pData->segmentColor2 = colorMiddle;
							pData->segmentColor3 = colorEnd;

							pData->alpha = alpha;
							pData->particleScaling = scale;
							pData->particleScalingVar = scaleVar;
							pData->fixedSize = fixedSize;
							pData->headLifeSpan = Vec3(headLifeSpan[0],headLifeSpan[1],headLifeSpan[2]);
							pData->headDecay = Vec3(headDecay[0],headDecay[1],headDecay[2]);
							pData->tailLifeSpan = Vec3(tailLifeSpan[0],tailLifeSpan[1],tailLifeSpan[2]);
							pData->tailDecay = Vec3(tailDecay[0],tailDecay[1],tailDecay[2]);

							pData->rows = rows;
							pData->columns = cols;
							pData->changeStyle = changeStyle;
							pData->changeInterval = changeInterval;

							pData->time = timeMiddle;
							pData->lifeSpan = life;
							pData->lifeVar = lifeVar;
							pData->tailLength = tailLength;
							pData->aspectRatio = aspectRatio;
							pData->initialAngleBegin = angleBegin;
							pData->initialAngleEnd = angleEnd;
							pData->rotationSpeed = rotationSpeed;
							pData->rotationSpeedVar = rotationSpeedVar;

							pData->head = head;
							pData->tail = tail;
							pData->unshaded = unshaded;
							pData->unfogged = unfogged;
							pData->blockByY0 = blockByY0;

							ReadKeyFrames<float,bool>(mem, pData->visibility);
							ReadKeyFrames<float,float>(mem,pData->speed);
							ReadKeyFrames<float,float>(mem,pData->variation);
							ReadKeyFrames<float,float>(mem,pData->latitude);
							ReadKeyFrames<float,float>(mem,pData->gravity);
							if (_version >= 17)
							{
								ReadKeyFrames<float,float>(mem,pData->explosiveForce);
							}
							ReadKeyFrames<float,float>(mem,pData->emissionRate);
							ReadKeyFrames<float,float>(mem,pData->width);
							ReadKeyFrames<float,float>(mem,pData->length);
							if (_version >= 16)
							{
								ReadKeyFrames<float,float>(mem,pData->height);
							}

							pData->visibility.setInterpolationType(eInterpolationType_None);
							pData->speed.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->variation.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->latitude.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->gravity.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->emissionRate.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->width.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->length.setInterpolationType(eInterpolationType_Bezier_Linear);
							pData->height.setInterpolationType(eInterpolationType_Bezier_Linear);

							pData->boneObjectId = id;

							_pe.push_back(pData);
						}
					}
				}
				break;

			case 'MATR':
				{
					size_t num = 0;
					mem->read(&num, sizeof(size_t));
					for (int n = 0 ; n != num; ++n)
					{
						Mat4 initialMatrix = Mat4::IDENTITY;

						sTranMatrix tm;
						mem->read(&tm,sizeof(tm));
						//此tm是D3D矩阵模式，不需要转置
						for (int i = 0;i < 4;i++)
						for (int j = 0;j < 3;j++)
						{
							initialMatrix[i][j] = tm._mat[i][j];
						}
						_matrices.push_back(initialMatrix);
					}
				}break;

			default:
				break;
			}

			safeDelete(mem);
		}

		f.close();

		return true;
	}


	bool EntityYZ::_decodeFB()
	{
		std::string full_path;
		Buddha::FileSystem::getInstancePtr()->getDataDirectory(full_path);
		full_path += _para->_path;
		Buddha::FileSystem::getInstancePtr()->getParent(full_path);
		full_path += '\\';

		for (size_t i = 0; i < _fbs.size(); ++i)
		{
			std::string path(full_path + _fbs[i]);

			std::string s;
			size_t pos = path.find_last_of('_');
			size_t dot = path.find_last_of('.');
			if (++pos != std::string::npos && dot != std::string::npos)
			{
				s = path.substr(pos, dot - pos);
				_np[s] = path;
			}
			else
			{
				return false;
			}
			if (!_decodeFBImpl(path))
			{
				return false;
			}
		}
		
		return true;
	}

	bool EntityYZ::_decodeFBImpl(const std::string& path)
	{
		std::ifstream f(path.c_str(), std::ios::binary);
		if (!f.good())
		{
			return false;
		}

		u32 last_time = 0;

		//	format
		//	[Tag Size Data]
		Tag t;
		Size s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(Tag));
			f.read((char*)&s, sizeof(Size));

			if (s <= 0)
			{
				continue;
			}

			char* buffer = new char[s];
			f.read(buffer, s);

			MemoryFileReader* mem = new MemoryFileReader(buffer, s);

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;
			Error("Seg: ");
			Record(c);
			Record("Size: ");
			Record(s);

			switch (t)
			{
			case 'MVER':	// version
				{
					mem->read(&_version, sizeof(u32));
				}
				break;

			case 'MANM': // [starttime + endtime]...
				{
					u32 time_start = 0;
					u32 time_end = 0;
					mem->read(&time_start, sizeof(u32));
					mem->read(&time_end, sizeof(u32));

					std::string s;
					size_t pos = path.find_last_of('_');
					size_t dot = path.find_last_of('.');
					if (++pos != std::string::npos && dot != std::string::npos)
					{
						s = path.substr(pos, dot - pos);
					}
					if (!_animationSet.empty())
					{
						last_time = _animationSet[_animationSet.size() - 1]._time_end;
					}
						
					time_start += last_time;
					time_end += last_time;
			
					_animationSet.push_back(sAnimation(s, time_start, time_end));
				}
				break;


			case 'MBON':
				{
					u32 nBones;
					mem->read(&nBones,sizeof(u32));
					for (size_t i = 0; i < nBones; ++i)
					{
						sBone* b = _bones[i];

						int id;
						mem->read(&id,sizeof(int));

						int parent;
						mem->read(&parent,sizeof(int));

						static sKeyFrames<Vec3> tKeyFramesTranslation; // 设置成静态的，循环利用，防止频繁new/delete内存
						tKeyFramesTranslation.clear();
						ReadKeyFrames<Vec3, Vec3>(mem, tKeyFramesTranslation, last_time);

						static sKeyFrames<Quaternion> tKeyFramesRotation;
						tKeyFramesRotation.clear();
						ReadKeyFrames<Quaternion,Quaternion>(mem, tKeyFramesRotation, last_time);

						static sKeyFrames<Vec3> tKeyFramesScale;
						tKeyFramesScale.clear();
						ReadKeyFrames<Vec3,Vec3>(mem, tKeyFramesScale, last_time);

						b->_translation.insertKeyFrames(tKeyFramesTranslation);

						b->_rotation.insertKeyFrames(tKeyFramesRotation);

						b->_scale.insertKeyFrames(tKeyFramesScale);

						bool no_use;
						mem->read(&no_use, sizeof(bool));
						mem->read(&no_use, sizeof(bool));
					}
				}
				break;

			case 'MTRS': // material_count + [material]
				{
					u8 name_length = 0;
					mem->read(&name_length, sizeof(u8));

					char* name = new char[name_length + 1];
					name[name_length] = 0;
					mem->read(name, name_length);

					std::string s(name);

					for (size_t i = 0; i < _materials.size(); ++i)
					{
						EntityYZMaterial* m = _materials[i];

						if (m->getName() == s)
						{
							static sKeyFrames<float> keyFramesTransparency;
							keyFramesTransparency.clear();
							ReadKeyFrames<float,float>(mem, keyFramesTransparency, last_time);

							static sKeyFrames<Color3> keyFramesColor;
							keyFramesColor.clear();

							if (_version >= 10)
							{
								ReadKeyFrames<Color3,Color3>(mem, keyFramesColor, last_time);
							}

							static sKeyFrames<float> keyFramesFlowSpeedU,keyFramesFlowSpeedV,keyFramesRotateSpeed;
							static sKeyFrames<short> keyFramesChangeStyle;
							
							if(_version >= 26)
							{
								ReadKeyFrames<float,float>(mem, keyFramesFlowSpeedU, last_time);
								ReadKeyFrames<float,float>(mem, keyFramesFlowSpeedV, last_time);
								ReadKeyFrames<float,float>(mem, keyFramesRotateSpeed, last_time);
								ReadKeyFrames<short,short>(mem, keyFramesChangeStyle,last_time);
							}

							m->_ani.alpha.addKeyFrames(keyFramesTransparency);

							if (_version >= 10)
							{
								m->_ani.color.addKeyFrames(keyFramesColor);
							}
				
							if (_version >= 26)
							{
								m->_ani.flowSpeedU.addKeyFrames(keyFramesFlowSpeedU);
								m->_ani.flowSpeedV.addKeyFrames(keyFramesFlowSpeedV);
								m->_ani.rotateSpeed.addKeyFrames(keyFramesRotateSpeed);
								m->_ani.changeStyle.addKeyFrames(keyFramesChangeStyle);
							}

							break;
						}
					}
				}
				break;

			default:
				break;
			}

			safeDelete(mem);
		}

		f.close();

		return true;
	}

	bool EntityYZ::_iniIB()
	{
		_ib = BufferFactory::getInstancePtr()->createIndexBuffer(3 * _faces.size() * sizeof(s16), 0, D3DFMT_INDEX16, D3DPOOL_DEFAULT);

		if (_ib	== NULL)
		{
			return false;
		}

		s16* data = (s16*)_ib->lock(0, 0, 0);

		if (data == NULL)
		{
			return false;
		}

		for (size_t i = 0; i < _faces.size(); ++i)
		{
			data[3*i + 0] = _faces[i]->index[0];
			data[3*i + 1] = _faces[i]->index[1];
			data[3*i + 2] = _faces[i]->index[2];
		}

		_ib->unLock();

		return true;
	}

	bool EntityYZ::_initVB()
	{
		_vb = BufferFactory::getInstancePtr()->createVertexBuffer(sizeof(POSITION_TEXTURE) * _vertices.size(), D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);
		
		if (_vb == NULL)
		{
			return false;
		}
		
		POSITION_TEXTURE* data = (POSITION_TEXTURE*)_vb->lock(0, 0, 0);

		if (data == NULL)
		{
			return false;
		}

		for (size_t i = 0; i < _vertices.size(); ++i)
		{
			data[i]._pos.x = _vertices[i]->pos.x;
			data[i]._pos.y = _vertices[i]->pos.y;
			data[i]._pos.z = _vertices[i]->pos.z;
			data[i]._tex.x = _vertices[i]->texcoords.x;
			data[i]._tex.y = _vertices[i]->texcoords.y;
		}
		_vb->unLock();

		return true;
	}

	bool EntityYZ::_initSkeleton()
	{
		sBone* parent = NULL;
		for (size_t i = 0; i < _bones.size(); ++i)
		{
			if (_bones[i]->_parentId == -1)
			{
				parent = _bones[i];		
				if (parent == NULL)
				{
					continue;
				}

				Skeleton* s = new Skeleton;

				std::string virtualParentName = std::string(parent->_name) + "_Virtual";
				BoneNode* n = s->createBoneNode(virtualParentName);

				s->setRootBoneNode(virtualParentName);
				//n->setPosition(parent->_pivotPoint);

				n->attachObject(parent);
				
				// real parent
				{
// 					BoneNode* g = static_cast<BoneNode*>(n->createChild(parent->_name));
// 					g->attachObject(parent);

					_addBoneChild(n, parent);
				}
				_skeletons.push_back(s);
			}
		}

		return true;
	}

	void EntityYZ::_addBoneChild(BoneNode* n, sBone* b)
	{
		if (n == NULL || b == NULL)
		{
			return;
		}

		for (size_t i = 0; i < _bones.size(); ++i)
		{
			sBone* e = _bones[i];

			if (e->_parentId == b->_objectId)
			{
				BoneNode* g = static_cast<BoneNode*>(n->createChild(e->_name));
				g->attachObject(e);
				
				_addBoneChild(g, e);
			}
		}
	}

	bool EntityYZ::setAnimation( const std::string& name )
	{
		NamePathVector::iterator n = _np.begin();
		/*++n;
		++n;*/
		/*std::string name2 = n->second;
		if (!_decodeFBImpl(name2))
		{
			return false;
		}*/

		/*if (!_decodeFBImpl(_np[name]))
		{
			return false;
		}*/

		AnimationMap::iterator it = _am.begin();
		//AnimationMap::iterator it = _am.find(n->first);

		if (it == _am.end())
		{
			return false;
		}

		_clock.start = it->second->_time_start;
		_clock.end = it->second->_time_end;
		_clock.current = it->second->_time_start;

		return true;
	}

	void EntityYZ::setSpeed( Real speed )
	{
		_speed = speed;
	}

	void EntityYZ::renderWithEffect()
	{
		_renderSystem->setVertexDeclaration(_vdt);

		HRESULT hr;

		u32 effectNum = 0;
		// Apply the technique contained in the effect 
		_effect->begin(&effectNum);

		for (u32 i = 0; i < effectNum; ++i)
		{
			_effect->beginPass(i);

			renderNormal();

			_effect->endPass();
		}

		_effect->end();
	}

	size_t EntityYZ::getBoneNum()
	{
		return _bones.size();
	}

	sBone* EntityYZ::getBone( int i )
	{
		if (i == -1)
		{
			return NULL;
		}

		return _bones[i];
	}

	Vec3 EntityYZ::getBonePivot( int i )
	{
		return _bones[i]->_pivotPoint;
	}

	size_t EntityYZ::getSubEntityNum()
	{
		return _sm.size();
	}

	VertexBuffer* EntityYZ::cloneVB()
	{
		VertexBuffer* vb = BufferFactory::getInstancePtr()->createVertexBuffer(sizeof(POSITION_TEXTURE) * _vertices.size(), D3DUSAGE_DYNAMIC, D3DPOOL_DEFAULT);

		if (vb == NULL)
		{
			return NULL;
		}

		POSITION_TEXTURE* data = (POSITION_TEXTURE*)vb->lock(0, 0, 0);

		if (data == NULL)
		{
			return false;
		}

		for (size_t i = 0; i < _vertices.size(); ++i)
		{
			data[i]._pos.x = _vertices[i]->pos.x;
			data[i]._pos.y = _vertices[i]->pos.y;
			data[i]._pos.z = _vertices[i]->pos.z;
			data[i]._tex.x = _vertices[i]->texcoords.x;
			data[i]._tex.y = _vertices[i]->texcoords.y;
		}
		vb->unLock();

		return vb;
	}

}
