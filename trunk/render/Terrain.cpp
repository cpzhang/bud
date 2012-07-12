#include "Terrain.h"
#include "RenderSystem.h"
#include "Buffer.h"
#include "BufferManager.h"
//#include "EffectManager.h"
//#include "Effect.h"
#include "MZ.h"
//
namespace Euclid
{
	Terrain::Terrain()
	{
		clear();
	}

	Terrain::~Terrain()
	{

	}

	bool Terrain::create( const std::string& fileName )
	{
		std::ifstream f(fileName.c_str(), std::ios::binary);
		if (!f.good())
		{
			return false;
		}

		//	[Tag Size Data]
		int t;
		size_t s;
		while(f.good())
		{
			t = 0;
			s = 0;
			f.read((char*)&t, sizeof(int));
			f.read((char*)&s, sizeof(size_t));

			if (s <= 0)
			{
				continue;
			}

			char c[5];
			c[0] = *((char*)&t + 3);
			c[1] = *((char*)&t + 2);
			c[2] = *((char*)&t + 1);
			c[3] = *((char*)&t + 0);
			c[4] = 0;

			switch (t)
			{
			case 'MVER':
				{
					f.read(&_version, s);
				}
				break;

			case 'MSUB':
				{
					_submeshes.resize(s / sizeof(sSubMesh));
					f.read((char*)&_submeshes[0], s);
				}
				break;
			case 'MVTX':
				{
					_vertices.resize(s / sizeof(sPositionTexture));
					f.read((char*)&_vertices[0], s);
				}
				break;
			case 'MFAC':
				{
					_faces.resize(s / sizeof(Mesh::sFace));
					f.read((char*)&_faces[0], s);
				}
				break;
			}
		}

		//_initBuffer();
		return true;
	}

	bool Terrain::create( )
	{
		_initBuffer();
		return true;
	}

	void Terrain::destroy()
	{
		if (_vertexBuffer)
		{
			_vertexBuffer->destroy();
			_vertexBuffer = NULL;
		}
		if (_indexBuffer)
		{
			_indexBuffer->destroy();
			_indexBuffer = NULL;
		}
		
		clear();
	}

	void Terrain::clear()
	{
		_version = 0;
		_vertexBuffer = 0;
		_indexBuffer = 0;
		_vertices.clear();
		_faces.clear();
		_submeshes.clear();
	}

	bool Terrain::_initBuffer()
	{
		{
			_vertexBuffer = BufferManager::getInstancePtr()->createVertexBuffer(_vertices.size() * sizeof(sPositionTexture), eUsage_Null, ePool_Manager);
			void* data = _vertexBuffer->lock(0, 0, Euclid::eLock_Null);
			memcpy(data, &_vertices[0], _vertices.size() * sizeof(sPositionTexture));
			_vertexBuffer->unLock();
		}
		//
		{
			_indexBuffer = BufferManager::getInstancePtr()->createIndexBuffer(_faces.size() * sizeof(Mesh::sFace), eUsage_Null, eFormat_Index16, ePool_Manager);
			void* data = _indexBuffer->lock(0, 0, Euclid::eLock_Null);
			memcpy(data, &_faces[0], _faces.size() * sizeof(Mesh::sFace));
			_indexBuffer->unLock();
		}

		return true;
	}

	void Terrain::render()
	{
		RenderSystem::getInstancePtr()->setStreamSource(0, _vertexBuffer, 0, sizeof(sPositionTexture));
		RenderSystem::getInstancePtr()->setIndices(_indexBuffer);
		RenderSystem::getInstancePtr()->setVertexDeclaration(eVertexDeclarationType_PositionTexture);
		for (size_t i = 0; i != _submeshes.size(); ++i)
		{
			const sSubMesh& s = _submeshes[i];
			RenderSystem::getInstancePtr()->drawIndexedPrimitive(ePrimitive_TriangleList, 0, 0, s.vcount, s.istart, s.icount / 3);
		}
	}

	int Terrain::getFaceNumber()
	{
		return _faces.size();
	}

	int Terrain::getVertexNumber()
	{
		return _vertices.size();
	}
}
