#include "BSPLoader.h"
#include "ZIPManager.h"
#include "IFileRead.h"
#include "Paras.h"
#include "IRenderSystem.h"
#include "TextureFactory.h"
#include "Material.h"
#include "Light.h"
namespace Euclid
{
	BSPLoader::BSPLoader()
	{
		_file = NULL;
		_header = new sHeader;
		_renderSystem = Paras::getInstancePtr()->_renderSystem;
	}

	BSPLoader::~BSPLoader()
	{

	}

	bool BSPLoader::load( const std::string& name )
	{
		_file = ZIPManager::getInstancePtr()->getFileReader(name);
		if (_file == NULL)
		{
			return false;
		}

		_file->read(_header, sizeof(sHeader));

		_file->read(_lumps, kMaxLumps * sizeof(sLump));

		_loadEntity();
		_loadVisData();
		_loadOtherStuff();
		
		return true;
	}

	void BSPLoader::_loadEntity()
	{
		char* buffer = new char[_lumps[kEntities].length + 1];

		_file->seek(_lumps[kEntities].offset, false);
		
		_file->read(buffer, _lumps[kEntities].length);
		
		buffer[_lumps[kEntities].length] = 0;
		
		delete[] buffer;
	}


	void BSPLoader::_loadOtherStuff()
	{
#define Load_Stuff(elump, sType, v)	\
		{\
			int num = _lumps[elump].length / sizeof(sType);\
			_file->seek(_lumps[elump].offset, false);\
			for (int i = 0; i < num; ++i)\
			{\
				sType item;\
				_file->read(&item, sizeof(sType));\
				v.push_back(item);\
			}\
		}

		Load_Stuff(kShaders,	sBSPTexture,	_textures);

		Load_Stuff(kPlanes,		sBSPPlane,		_planes);

		Load_Stuff(kNodes,		sBSPNode,		_nodes);

		Load_Stuff(kLeafs,		sBSPLeaf,		_leafs);

		Load_Stuff(kLeafFaces,	sBSPLeafFace,	_leafFaces);

		Load_Stuff(kLeafBrushes,sBSPLeafBrush,	_leafBrushes);

		Load_Stuff(kModels,		sBSPModel,		_models);

		Load_Stuff(kBrushes,	sBSPBrush,		_brushes);

		Load_Stuff(kBrushSides,	sBSPBrushSides,	_brushSides);

		Load_Stuff(kVertices,	sBSPVertex,		_vertices);

		Load_Stuff(kMeshVerts,	sBSPMeshVertices,_meshVertices);

		Load_Stuff(kFogs,		sBSPFog,		 _fogs);

		Load_Stuff(kFaces,		sBSPFace,		 _faces);

		Load_Stuff(kLightmaps,	sBSPLightmap,	_lightmaps);

		Load_Stuff(kLightGrid,	sBSPLightGrid,	_lightGrids);
	}


	void BSPLoader::_loadVisData()
	{
		_file->seek(_lumps[kVisData].offset, false);
	
		sBSPVisData visData;

		_file->read(&visData, sizeof(int) + sizeof(int));

		visData.vecs = new unsigned char[visData.n_vecs * visData.sz_vecs]; 

		_file->read(visData.vecs, visData.n_vecs * visData.sz_vecs);
	}

	void BSPLoader::preRender()
	{
		//_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		//_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);

		//_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		//
		_renderSystem->setRenderState(D3DRS_ZENABLE, true);
		//_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		_renderSystem->setRenderState(D3DRS_LIGHTING, false);

		//	Set the ambient light.
		//_renderSystem->setRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(255, 255, 255));
		//_renderSystem->setRenderState(D3DRS_COLORVERTEX, true);
		//_renderSystem->setRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		_renderSystem->setVertexDeclaration(VD_POSITION_COLOR_TEXTURE);
		/*Light l(Light::White);
		_renderSystem->setLight(0, &l);
		Material m(Material::Yellow);
		_renderSystem->setMaterial(&m);*/
	}

	void BSPLoader::postRender()
	{
		//_renderSystem->setRenderState(D3DRS_LIGHTING, false);
		_renderSystem->setVertexDeclaration(VD_NULL);
		_renderSystem->setTexture(0, 0);
		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	ITexture* BSPLoader::_getTexture( const std::string& name )
	{
		int dot_pos = name.find_last_of('/');
		std::string path_base(name.substr(dot_pos + 1, name.size() - 1));
		
		if (_texMap.find(name) == _texMap.end())
		{
			IFileRead* f = NULL;
			f = ZIPManager::getInstancePtr()->getFileReader(path_base + ".tga");
			if (f == NULL)
			{
				f = ZIPManager::getInstancePtr()->getFileReader(path_base + ".jpg");
				if (f == NULL)
				{
					return NULL;
				}
			}
	
			unsigned char* buffer = new unsigned char[f->getSize()];
			f->read(buffer, f->getSize());

			_texMap[name] = TextureFactory::getInstancePtr()->createTexFromMemory(buffer, f->getSize());
		}

		return _texMap[name];
	}


	ITexture* BSPLoader::_getLightMapTexture( const int index )
	{
		if (_lightMap.find(index) == _lightMap.end())
		{
			if (index < 0 || index >= _lightMap.size())
			{
				return NULL;
			}

			ITexture* tex = TextureFactory::getInstancePtr()->createTextureFromRawData((unsigned char*)_lightmaps[index].imageBits, 
				128, 128, 128*3, D3DFMT_R8G8B8);

			if (tex == NULL)
			{
				return NULL;
			}

			_lightMap[index] = tex;
		}

		return _lightMap[index];
	}

	void BSPLoader::render()
	{
		renderNode(0);
	}

	void BSPLoader::renderNode( int index )
	{
		if (index < 0 || index >= _nodes.size())
		{
			return;
		}

		sBSPNode node = _nodes[index];

		if (node.children[0] < 0)
		{
			renderFace(-1 - node.children[0]);
		}
		else
		{
			renderNode(node.children[0]);
		}

		if (node.children[1] < 0)
		{
			renderFace(-1 - node.children[1]);
		}	
		else
		{
			renderNode(node.children[1]);
		}
	}

	void BSPLoader::renderLeaf( int index )
	{
		sBSPLeaf leaf = _leafs[index];

		if (leaf.cluster < 0)
		{
			return;
		}

		for (int i = 0; i < leaf.n_leaffaces; ++i)
		{
			renderFace(i + leaf.leafface);
		}
	}

	void BSPLoader::renderFace( int index )
	{
		sBSPFace face = _faces[index];

		ITexture* tex = NULL; 
		sBSPTexture st = _textures[face.textureID];
		tex = _getTexture(st.strName);
		if (tex != NULL)
		{
			_renderSystem->setTexture(0, tex);
		}

		switch(face.type)
		{
		case sBSPFace::eType_Polygon:
		case sBSPFace::eType_Mesh:
			{
				POSITION_COLOR_TEXTURE* v = new POSITION_COLOR_TEXTURE[face.numMeshVerts];

				for (int i = 0; i < face.numMeshVerts; ++i)
				{
					int indexI = face.meshVertIndex + i;

					int indexV = _meshVertices[indexI];

					indexV += face.vertexIndex;

					v[i]._pos.x = _vertices[indexV].vPosition[0] * 0.00001;
					v[i]._pos.z = _vertices[indexV].vPosition[1] * 0.00001;
					v[i]._pos.y = _vertices[indexV].vPosition[2] * 0.00001;

					v[i]._color = *(DWORD*)&_vertices[indexV].color;

					v[i]._tex.x = _vertices[indexV].vTextureCoord[0];
					v[i]._tex.y = _vertices[indexV].vTextureCoord[1];
				}

				_renderSystem->drawPrimitiveUP(D3DPT_TRIANGLELIST, 
					face.numMeshVerts / 3, v, sizeof(POSITION_COLOR_TEXTURE));

				delete[] v;
			}
			break;
		case sBSPFace::eType_Billboard:
			break;
		case sBSPFace::eType_Patch:
			break;
		default:
			break;
		}

		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, true);
		_renderSystem->setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		_renderLight(index);
		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void BSPLoader::_renderLight( int index )
	{
		sBSPFace face = _faces[index];

		ITexture* tex = NULL; 
		tex = _getLightMapTexture(face.lightmapID);
		if (tex != NULL)
		{
			_renderSystem->setTexture(0, tex);
		}

		switch(face.type)
		{
		case sBSPFace::eType_Polygon:
		case sBSPFace::eType_Mesh:
			{
				POSITION_COLOR_TEXTURE* v = new POSITION_COLOR_TEXTURE[face.numMeshVerts];

				for (int i = 0; i < face.numMeshVerts; ++i)
				{
					int indexI = face.meshVertIndex + i;

					int indexV = _meshVertices[indexI];

					indexV += face.vertexIndex;

					v[i]._pos.x = _vertices[indexV].vPosition[0] * 0.00001;
					v[i]._pos.z = _vertices[indexV].vPosition[1] * 0.00001;
					v[i]._pos.y = _vertices[indexV].vPosition[2] * 0.00001;

					v[i]._color = *(DWORD*)&_vertices[indexV].color;

					/*v[i]._tex.x = _vertices[indexV].vTextureCoord[0];
					v[i]._tex.y = _vertices[indexV].vTextureCoord[1];*/

					v[i]._tex.x = _vertices[indexV].vLightmapCoord[0];
					v[i]._tex.y = _vertices[indexV].vLightmapCoord[1];
				}

				_renderSystem->drawPrimitiveUP(D3DPT_TRIANGLELIST, 
					face.numMeshVerts / 3, v, sizeof(POSITION_COLOR_TEXTURE));

				delete[] v;
			}
			break;
		case sBSPFace::eType_Billboard:
			break;
		case sBSPFace::eType_Patch:
			break;
		default:
			break;
		}
	}
}
