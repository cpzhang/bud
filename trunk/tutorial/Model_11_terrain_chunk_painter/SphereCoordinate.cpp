#include "SphereCoordinate.h"
#include "renderImp.h"
sSphereCoordination::sSphereCoordination()
	{
		reset();
	}
	void sSphereCoordination::updateDistance(float delta)
	{
		mDistance -= delta * mDistance;
		if (mDistance <= smMinDistance)
		{
			mDistance = smMinDistance;
		}
		if (mDistance >= smMaxDistance)
		{
			mDistance = smMaxDistance;
		}
	}
	void sSphereCoordination::updateAngleUpFromXZ(float delta)
	{
 		mAngleUpFromXZ._radian -= delta;
	}
	void sSphereCoordination::reset()
	{
		mOrigion = Vec3(0.0f, 2.0f, 0.0f);
		mDistance = 5.0f;
		mAngleAroundY = Euler::HalfPI;
		mAngleUpFromXZ = Euler::HalfPI * 0.63f;
	}
	Vec3 sSphereCoordination::getPosition()
	{
		Vec3 p;
		p.y = mDistance * Euler::Basic::Cos(mAngleUpFromXZ);
		float s = Euler::Basic::Sin(mAngleUpFromXZ);
		Radian angle = mAngleAroundY;
		//进入球的背面
		if (s < 0)
		{
			s = -s;
			angle += Euler::PI;
		}
		p.x = mDistance * s * Euler::Basic::Cos(angle);
		p.z = mDistance * s * Euler::Basic::Sin(angle);
		return p + mOrigion;
	}
	
Real sSphereCoordination::smMaxDistance(1000.0f);

Real sSphereCoordination::smMinDistance(2.0f);


Light::Light()
	{
		mPosition.mDistance = 200;
	}
	
void SphereMesh::clear()
	{
		_material = NULL;
		_vertexBuffer = NULL;
		_indexBuffer = NULL;
		_vertices.clear();
		_indices.clear();
	}
	void SphereMesh::destroy()
	{
		if (_vertexBuffer)
		{
			_vertexBuffer->destroy();
		}
		if (_indexBuffer)
		{
			_indexBuffer->destroy();
		}
		if (_material)
		{
			_material->destroy();
			delete _material;
		}
		
		clear();
	}
	SphereMesh::SphereMesh()
	{
		clear();
	}
	SphereMesh::~SphereMesh()
	{
	}
	void SphereMesh::render()
	{
		Euclid::Effect* fx = _material->getEffect();
		if (NULL == fx)
		{
			return;
		}
		//
		_material->apply();

		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);
			if (i == 0)
			{
				Euclid::sVDTNull* vdt = Euclid::VDTManager::getInstancePtr()->mLayouts[_VDT];
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setStreamSource(0, _vertexBuffer, 0, vdt->size());
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setIndices(_indexBuffer);
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setVertexDeclaration(_VDT);
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawIndexedPrimitive(Euclid::ePrimitive_TriangleList, 0, 0, _vertices.size(), 0, _indices.size() / 3);
			}
			fx->endPass();
		}
		fx->end();
	}
	bool SphereMesh::_initBuffer()
	{
		Euclid::sVDTNull* vdt = Euclid::VDTManager::getInstancePtr()->mLayouts[_VDT];
		{
			_vertexBuffer = Euclid::BufferManager::getInstancePtr()->createVertexBuffer(_vertices.size() * vdt->size(), Euclid::eUsage_Null, Euclid::ePool_Manager);
			void* data = _vertexBuffer->lock(0, 0, Euclid::eLock_Null);
			memcpy(data, &_vertices[0], _vertices.size() * vdt->size());
			_vertexBuffer->unLock();
		} 
		
		//
		{
			_indexBuffer = Euclid::BufferManager::getInstancePtr()->createIndexBuffer(_indices.size() * sizeof(u16), Euclid::eUsage_Null, Euclid::eFormat_Index16, Euclid::ePool_Manager);
			void* data = _indexBuffer->lock(0, 0, Euclid::eLock_Null);
			memcpy(data, &_indices[0], _indices.size() * sizeof(u16));
			_indexBuffer->unLock();
		}

		return true;
	}
	void SphereMesh::create()
	{
		_VDT = Euclid::eVertexDeclarationType_PositionTexture;
		_material = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_Vertex);
		_material->setEffect("shader\\Position.fx");
		_material->mFillMode = Euclid::eFillMode_WireFrame;

		u32 segment_skydome = 16;
		float radius = 500.0f;
		for (s32 beta = segment_skydome; beta >= 0; --beta)
		{
			float step_beta = beta * Euler::HalfPI / segment_skydome;

			Euclid::sPositionTexture vertex;
			vertex.position.y = radius * Euler::Basic::Sin(step_beta);
			vertex.texcoord.y = (float)beta / (float)segment_skydome;
			for (s32 alpha = segment_skydome; alpha >= 0; --alpha)
			{
				float step_alpha = alpha * Euler::TwoPI / segment_skydome;	
				vertex.position.x = radius * Euler::Basic::Cos(step_beta) * Euler::Basic::Cos(step_alpha);
				vertex.position.z = radius * Euler::Basic::Cos(step_beta) * Euler::Basic::Sin(step_alpha);
				vertex.texcoord.x = (float)alpha / (float)segment_skydome;
				_vertices.push_back(vertex);
			}
		}

		int offset = 1;
		for (int i = 0; i <= segment_skydome - 1;i += offset)
		{
			for (int j = 0; j <= segment_skydome - 1; j += offset)
			{
				u16 baseIndex = j + i * segment_skydome;
				//
				_indices.push_back(baseIndex);
				_indices.push_back(baseIndex + segment_skydome * offset);
				_indices.push_back(baseIndex + offset);
				//
				_indices.push_back(baseIndex + segment_skydome * offset);
				_indices.push_back(baseIndex + offset + segment_skydome * offset);
				_indices.push_back(baseIndex + offset);
			}
		}

		//
		SphereMesh::_initBuffer();
	}
