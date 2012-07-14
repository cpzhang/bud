#pragma once
//球面坐标系，起初是想用来做第三人称视角控制，球心为主角中心点，玩家通过键盘或鼠标控制摄像机在球面坐标系下运动
//这里用来实现模型查看，废了些周折，围绕XZ平面旋转的角alpha，范围[0, PI]，过了极点，会带来两个问题：第一，半径在Y轴投影有正有负，余弦，
//但是在XZ平面的投影是正弦，没有负值，其符号由角beta确定；第二，beta的运动不影响alpha，但反之不成立，alpha过了极点，beta要加PI；
//第三，默认的view space coordination的Y轴向上，一旦越过极点，Y轴应该往下
//此三点，思考了一晚上，好在任何问题总有解决的办法，其实，作为模型查看而已，不必用此球面坐标系，直接在模型本地坐标系下运动即可
struct sSphereCoordination
{
	sSphereCoordination()
	{
		reset();
	}
	void updateDistance(float delta)
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
	void updateAngleUpFromXZ(float delta)
	{
 		mAngleUpFromXZ._radian -= delta;
	}
	void reset()
	{
		mDistance = 500.0f;
		mAngleAroundY = Euler::HalfPI;
		mAngleUpFromXZ = Euler::HalfPI * 0.63f;
	}
	Vec3 getPosition()
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
		return p;
	}
	Real mDistance;
	Radian mAngleAroundY;//angle around y-axis, as measured from positive x-axis
	Radian mAngleUpFromXZ;//angle up from x-z plane, clamped to [0:Pi]
	Real mTime;
	static Real smMaxDistance;
	static Real smMinDistance;
};
Real sSphereCoordination::smMaxDistance(1000.0f);

Real sSphereCoordination::smMinDistance(50.0f);

class Light
{
public:
	Light()
	{
		mPosition.mDistance = 200;
	}
	sSphereCoordination mPosition;
};
class SphereMesh
{
public:
	Euclid::IMaterial*		_material;
	std::vector<Euclid::sPositionTexture> _vertices;
	std::vector<u16>		_indices;
	Euclid::eVertexDeclarationType _VDT;
	Euclid::IBuffer* _vertexBuffer;
	Euclid::IBuffer* _indexBuffer;
	void clear()
	{
		_material = NULL;
		_vertexBuffer = NULL;
		_indexBuffer = NULL;
		_vertices.clear();
		_indices.clear();
	}
	void destroy()
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
	SphereMesh()
	{
		clear();
	}
	~SphereMesh()
	{
	}
	void render()
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
	bool _initBuffer()
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
	void create()
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
		_initBuffer();
	}
};