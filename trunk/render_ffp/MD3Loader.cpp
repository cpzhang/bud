#include "MD3Loader.h" 
#include "Paras.h"
#include "IRenderSystem.h"
#include "TextureFactory.h"
namespace Euclid
{
	MD3Loader::MD3Loader()
		:	_header(0)
	{
		_renderSystem = Paras::getInstancePtr()->_renderSystem;
	}

	MD3Loader::~MD3Loader()
	{

	}

	bool MD3Loader::load( const std::string& fileName )
	{
		_header = new sHeader;

		std::ifstream f(fileName.c_str(), std::ios_base::binary);

		//
		if (!f.good())
		{
			return false;
		}

		//
		f.seekg(std::ios_base::beg);
		f.read((char*)_header, sizeof(sHeader));

		if (_header->indent != sHeader::INDENT || _header->version != sHeader::VERSION)
		{
			return false;
		}

		f.seekg(_header->offset_frames, std::ios_base::beg);
		for (int i = 0; i < _header->num_frames; ++i)
		{
			sFrame* frame = new sFrame;
			f.read((char*)frame, sizeof(sFrame));
			_frames.push_back(frame);
		}

		f.seekg(_header->offset_tags, std::ios_base::beg);
		for (int i = 0; i < _header->num_tags; ++i)
		{
			sTag* tag= new sTag;
			f.read((char*)tag, sizeof(sTag));
			_tags.push_back(tag);
		}

		f.seekg(_header->offset_surfaces, std::ios_base::beg);
		for (int i = 0; i < _header->num_surfaces; ++i)
		{
			int surface_start = f.tellg();

			sSurface* surface = new sSurface;
			surface->header = new sSurface::sHeader;
			f.read((char*)surface->header, sizeof(sSurface::sHeader));

			f.seekg(surface_start + surface->header->offset_shaders, std::ios_base::beg);
			for (int j = 0; j < surface->header->num_shaders; ++j)
			{
				sSurface::sShader* shader = new sSurface::sShader;
				f.read((char*)shader, sizeof(sSurface::sShader));
				surface->shaders.push_back(shader);
			}

			f.seekg(surface_start + surface->header->offset_triangles, std::ios_base::beg);
			for (int j = 0; j < surface->header->num_triangles; ++j)
			{
				sSurface::sTriangle* triangle = new sSurface::sTriangle;
				f.read((char*)triangle, sizeof(sSurface::sTriangle));
				surface->triangles.push_back(triangle);
			}

			f.seekg(surface_start + surface->header->offset_st, std::ios_base::beg);
			for (int j = 0; j < surface->header->num_vertices; ++j)
			{
				sSurface::sTexCoord* st = new sSurface::sTexCoord;
				f.read((char*)st, sizeof(sSurface::sTexCoord));
				surface->st.push_back(st);
			}

			f.seekg(surface_start + surface->header->offset_vertices, std::ios_base::beg);
			for (int j = 0; j < surface->header->num_vertices; ++j)
			{
				sSurface::sVertex* v = new sSurface::sVertex;
				f.read((char*)v, sizeof(sSurface::sVertex));
				surface->vertices.push_back(v);
			}

			_surfaces.push_back(surface);
		}

		//
		f.close();

		//
		std::string texFile("e:/20100423Refracture/bin/data/MD3/rocket/rockfls2.tga");
		_tex = TextureFactory::getInstancePtr()->createTextureFromFile(texFile);
		//
		return true;
	}

	void MD3Loader::renderFrame( int& index )
	{
		if (index < 0 || index > _header->num_surfaces - 1)
		{
			index = 0;
		}

		//
		_renderSystem->setTexture(0, _tex);

		sSurface* s = _surfaces[index];
		/* Draw each triangle */
		for (int i = 0; i < s->triangles.size(); ++i)
		{
			sSurface::sTriangle* tri = s->triangles[i];
			//
			POSITION_NORMAL_TEXTURE v[3];

			/* Fill each vertex */
			for (int j = 0; j < 3; ++j)
			{
				int vertex = tri->indexes[j];
				//
				v[j]._pos.x = s->vertices[vertex]->coord[0];
				v[j]._pos.y = s->vertices[vertex]->coord[1];
				v[j]._pos.z = s->vertices[vertex]->coord[2];

				float lng = 2 * 3.14 * s->vertices[vertex]->normal[0] / 255;
				float lat = 2 * 3.14 * s->vertices[vertex]->normal[1] / 255;
				v[j]._normal.x = Euler::Basic::Cos(lat) * Euler::Basic::Sin(lng);
				v[j]._normal.y = Euler::Basic::Sin(lat) * Euler::Basic::Sin(lng);
				v[j]._normal.z = Euler::Basic::Cos(lng);

				v[j]._tex.x = s->st[vertex]->st[0];
				v[j]._tex.y = s->st[vertex]->st[1];
			}

			//
			_renderSystem->drawPrimitiveUP(D3DPT_TRIANGLELIST, 1, v, sizeof(POSITION_NORMAL_TEXTURE));
		}
	}

	void MD3Loader::preRender()
	{
		//
		_renderSystem->setRenderState(D3DRS_ZENABLE, true);
		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		//_renderSystem->setRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		_renderSystem->setRenderState(D3DRS_LIGHTING, false);
		_renderSystem->setVertexDeclaration(Euclid::VD_POSITION_NORMAL_TEXTURE);
	}

	void MD3Loader::postRender()
	{
		_renderSystem->setVertexDeclaration(Euclid::VD_NULL);
		_renderSystem->setTexture(0, 0);
		_renderSystem->setRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	}

	const int MD3Loader::sHeader::INDENT(860898377);


	const int MD3Loader::sHeader::VERSION(15);

}