#include "MD2Loader.h" 
#include "Paras.h"
#include "IRenderSystem.h"
#include "TextureFactory.h"
#include "FontFactory.h"
//
namespace Euclid
{

/* Table of precalculated normals */
	const vec3 anorms_table[162] = {
#include "MD2anorms.h"
	};

	MD2Loader::MD2Loader()
		:	_md2File(0)
	{
		_renderSystem = Paras::getInstancePtr()->_renderSystem;
		_Possible_Extend.push_back("tga");
		_Possible_Extend.push_back("bmp");
		_Possible_Extend.push_back("pcx");
		_Possible_Extend.push_back("png");
	}

	MD2Loader::~MD2Loader()
	{

	}

	bool MD2Loader::load( const std::string& fileName )
	{
		size_t pos = fileName.find_last_of("/");
		_fileBase = fileName.substr(0, pos+1);
		_fileName = fileName.substr(pos + 1, fileName.size() - 1);

		_md2File = new MD2File;
		std::ifstream f(fileName.c_str(), std::ios_base::binary);

		//
		if (!f.good())
		{
			return false;
		}

		//
		f.seekg(std::ios_base::beg);
		f.read((char*)&_md2File->_header, sizeof(_md2File->_header));

		//
		if (_md2File->_header.ident != 844121161 || _md2File->_header.version != 8)
		{
			return false;
		}

		if (_md2File->_header.num_skins > 0)
		{
			/* Memory allocations */
			_md2File->_data.skins = new skin[_md2File->_header.num_skins];

			/* Read model data */
			f.seekg(_md2File->_header.offset_skins, std::ios_base::beg);
			f.read((char*)_md2File->_data.skins, sizeof(skin) * _md2File->_header.num_skins);
		}

		if (_md2File->_header.num_st > 0)
		{
			_md2File->_data.texcoords = new texCoord[_md2File->_header.num_st];

			f.seekg(_md2File->_header.offset_st, std::ios_base::beg);
			f.read((char*)_md2File->_data.texcoords, sizeof(texCoord) * _md2File->_header.num_st);
		}

		if (_md2File->_header.num_tris > 0)
		{
			_md2File->_data.triangles = new triangle[_md2File->_header.num_tris];

			f.seekg(_md2File->_header.offset_tris, std::ios_base::beg);
			f.read((char*)_md2File->_data.triangles, sizeof(triangle) * _md2File->_header.num_tris);
		}
		
		if (_md2File->_header.num_frames > 0)
		{
			_md2File->_data.frames = new frame[_md2File->_header.num_frames];

			f.seekg(_md2File->_header.offset_frames, std::ios_base::beg);

			//
			for (int i = 0; i < _md2File->_header.num_frames; ++i)
			{
				_md2File->_data.frames[i].vs = new vertex[_md2File->_header.num_vertices];

				//
				f.read((char*)&_md2File->_data.frames[i].scale, sizeof(vec3));
				f.read((char*)&_md2File->_data.frames[i].translate, sizeof(vec3));
				f.read(_md2File->_data.frames[i].name, 16);
				f.read((char*)_md2File->_data.frames[i].vs, sizeof(vertex) * _md2File->_header.num_vertices);
			}		
		}

		//
		if (_md2File->_header.num_glcmds > 0)
		{
			_md2File->_data.glcmds = new int[_md2File->_header.num_glcmds];

			f.seekg(_md2File->_header.offset_glcmds, std::ios_base::beg);
			f.read((char*)_md2File->_data.glcmds, sizeof(int) * _md2File->_header.num_glcmds);
		}

		//
		f.close();

		//
		std::string texFile(fileName.substr(0, fileName.size() - 3));
		if (_md2File->_data.skins)
		{
			texFile = _fileBase + _md2File->_data.skins->name;
			_tex = TextureFactory::getInstancePtr()->createTextureFromFile(texFile);
			if (_tex)
			{
				_texName = _md2File->_data.skins->name;
				return true;
			}
			//texFile = _fileBase + _md2File->_data.skins->name;
			texFile = texFile.substr(0, texFile.size() - 3);
		}
		
		//
		for (int i = 0; i < _Possible_Extend.size(); ++i)
		{
			if (Buddha::FileSystem::getInstancePtr()->isFileExist(texFile + _Possible_Extend[i]))
			{
				_tex = TextureFactory::getInstancePtr()->createTextureFromFile(texFile + _Possible_Extend[i]);
				if (_tex)
				{
					size_t pos = texFile.find_last_of('//');
					if (pos != std::string::npos)
					{
						_texName = texFile.substr(pos + 1, texFile.size() - 1) + _Possible_Extend[i];
					}
					return true;
				}
			}
		}

		//
		return false;
	}

	void MD2Loader::renderFrame( int& index )
	{
		if (NULL == _md2File)
		{
			return;
		}

		//
		if (_md2File->_header.ident != 844121161 || _md2File->_header.version != 8)
		{
			return;
		}

		if (index < 0 || index > _md2File->_header.num_frames - 1)
		{
			//return;
			index = 0;
		}

		//
		_renderSystem->setTexture(_md2File->_data.tex_id, _tex);

		/* Draw the model */
		Data*  pData = &_md2File->_data;
		frame* pframe = &pData->frames[index];
		/* Draw each triangle */
		for (int i = 0; i < _md2File->_header.num_tris; ++i)
		{
			//
			triangle* pTriangle = &pData->triangles[i];
			//
			POSITION_NORMAL_TEXTURE s[3];

			/* Draw each vertex */
			for (int j = 0; j < 3; ++j)
			{
				vertex* pvert = &pframe->vs[pTriangle->v[j]];

				//
				s[j]._pos.x = (pframe->scale[0] * pvert->v[0]) + pframe->translate[0];
				s[j]._pos.z = (pframe->scale[1] * pvert->v[1]) + pframe->translate[1];
				s[j]._pos.y = (pframe->scale[2] * pvert->v[2]) + pframe->translate[2];

				//
				s[j]._normal.x = anorms_table[pvert->normalIndex][0];
				s[j]._normal.z = anorms_table[pvert->normalIndex][1];
				s[j]._normal.y = anorms_table[pvert->normalIndex][2];

				/* Compute texture coordinates */
				s[j]._tex.x = (float)(pData->texcoords[pTriangle->st[j]].s) / _md2File->_header.skinwidth;
				s[j]._tex.y = (float)(pData->texcoords[pTriangle->st[j]].t) / _md2File->_header.skinheight;
			}

			//
			_renderSystem->drawPrimitiveUP(D3DPT_TRIANGLELIST, 1, s, sizeof(POSITION_NORMAL_TEXTURE));
		}

		//
		_currentIndex = index;

		//
		renderInfo();
	}

	void MD2Loader::preRender()
	{
		//
		_renderSystem->setRenderState(D3DRS_ZENABLE, true);
		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		//_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		_renderSystem->setRenderState(D3DRS_LIGHTING, false);
		_renderSystem->setVertexDeclaration(Euclid::VD_POSITION_NORMAL_TEXTURE);
	}

	void MD2Loader::postRender()
	{
		//

		//_renderSystem->setRenderState(D3DRS_ZENABLE, false);
		//_renderSystem->setRenderState(D3DRS_LIGHTING, false);
		_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
		_renderSystem->setTexture(0, 0);
		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
			//_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}

	void MD2Loader::destroy()
	{
		if (_md2File)
		{
			delete _md2File;
			_md2File = 0;
		}

		if (_tex)
		{
			delete _tex;
			_tex = 0;
		}
	}

	void MD2Loader::renderInfo()
	{
		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 0, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), "File : " + _fileName);

		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 20, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), "Texture : " + _texName);

		std::ostringstream buf;
		buf<<"Skins : "<<_md2File->_header.num_skins;
		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 40, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), buf.str());

		buf.str("");
		buf<<"Vertices : "<<_md2File->_header.num_vertices;
		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 60, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), buf.str());

		buf.str("");
		buf<<"Triangles : "<<_md2File->_header.num_tris;
		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 80, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), buf.str());

		buf.str("");
		buf<<"Frames : "<<_md2File->_header.num_frames;
		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 100, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), buf.str());

		buf.str("");
		buf<<"Current Frame : "<<_currentIndex;
		FontFactory::getInstancePtr()->getFont()->render(FontDimention_2D, D3DXVECTOR3(0, 120, 0),
			D3DXVECTOR3(1, 0, 0), D3DCOLOR_ARGB(0, 255, 0, 0), buf.str());
	}
}