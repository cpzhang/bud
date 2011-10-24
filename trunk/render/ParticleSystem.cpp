#include "stdafx.h"
#include "ParticleSystem.h" 
#include "RenderSystem.h"
#include "TextureFactory.h"
#include "Texture.h"
namespace Euclid
{
	Particle::Particle(ParticlePara* pp)
	{
		_pp = new ParticlePara(pp);
		_pp_backup = new ParticlePara(_pp);
		_bb = new Billboard(_pp->_bbp);
	}

	//Particle::Particle( BillboardPara* para )
	//{
	//	_bb = new Billboard(para);
	//}
	Particle::~Particle()
	{
		destroy();
	}

	bool Particle::isAlive()
	{
		return _pp->_isAlive;
	}

	void Particle::recuperate()
	{
		safeDelete(_pp);
		float angle = Euler::Basic::randomReal(3.14f*1.1f, 3.14f*1.5f);
		float norm = Euler::Basic::randomReal(0.01f, 0.05f);
		_pp_backup->_velocity = Vec3(Euler::Basic::Cos(angle), Euler::Basic::Sin(angle), 0.0f) * norm;
		_pp = new ParticlePara(_pp_backup);
		_bb->setCenter(_pp_backup->_bbp->_center);
	}

	void Particle::update(u32 current, u32 delta)
	{
		static u32 last_delta = 0;
		delta = (last_delta + delta) * 0.5f;
		last_delta = delta;
		_pp->_age += (Real)delta;
		if (_pp->_age >= _pp->_lifespan)
		{
			_pp->_isAlive = false;
		}

		_pp->_velocity += _pp->_acceleration*(Real)delta;

		if (_bb)
		{
			Vec3 c;
			c = _bb->getCenter() + _pp->_velocity * (Real)delta;
			_bb->setCenter(c);
			static Color col;
			col.random();
			_bb->setColor(col.getRGBA());
			_bb->update(current, delta);
		}
	}

	void Particle::render()
	{
		if (_bb != NULL)
		{
			_bb->render();
		}
	}

	bool Particle::create()
	{
		if (_bb != NULL)
		{
			return _bb->create();
		}

		return false;
	}

	void Particle::preRender()
	{
		if (_bb)
		{
			_bb->preRender();
		}
	}

	void Particle::postRender()
	{
		if (_bb)
		{
			_bb->postRender();
		}
	}

	void Particle::attachNode( Node* node )
	{
		if (_bb)
		{
			_bb->attachNode(node);
		}
	}

	void Particle::destroy()
	{
		safeDelete(_pp);
		safeDelete(_pp_backup);
		safeDestroy(_bb);
	}
	ParticleSystem::ParticleSystem()
	{

	}

	ParticleSystem::ParticleSystem( ParticleSystemPara* psp )
	{
		_psp = new ParticleSystemPara(psp);
	}
	ParticleSystem::~ParticleSystem()
	{
		destroy();
	}

	bool ParticleSystem::create()
	{
		_tex = TextureFactory::getInstancePtr()->createTextureFromFile(_psp->_pp->_bbp->_texName);
		if (_tex == NULL)
		{
			Error("Failed to create Texture : ");
			return false;
		}

		for (size_t i = 0; i < _psp->_maxCount; ++i)
		{
			addParticle();
		}
		return true;
	}

	void ParticleSystem::destroy()
	{
		safeDestroy(_tex);
		for (size_t i = 0; i < _particles.size(); ++i)
		{
			safeDestroy(_particles[i]);
		}
	}

	void ParticleSystem::update( u32 current, u32 delta )
	{
		for (size_t i = 0; i < _particles.size(); ++i)
		{
			if (_particles[i]->isAlive())
			{
				_particles[i]->update(current, delta);
			}
			else
			{
				_particles[i]->recuperate();
			}
		}
	}

	void ParticleSystem::render()
	{
		for (size_t i = 0; i < _particles.size(); ++i)
		{
			if (_particles[i]->isAlive())
			{
				_particles[i]->render();
			}
		}
	}

	void ParticleSystem::preRender()
	{
		/*for (size_t i = 0; i < _particles.size(); ++i)
		{
			if (_particles[i]->isAlive())
			{
				_particles[i]->attachNode(_currentNode);
				_particles[i]->preRender();
			}
		}*/

		//
		_renderSystem->setTexture(0, _tex);

		// VD
		_renderSystem->setVertexDeclaration(VD_POSITION_COLOR_TEXTURE);

		// turn off lighting
		_renderSystem->setRenderState(D3DRS_LIGHTING, false);

		// alpha blend
		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, true);
		_renderSystem->setRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		_renderSystem->setRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// select alpha from texture
	/*	_renderSystem->setTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		_renderSystem->setTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);*/

		// how to mix the diffuse color and the texture color ?
		_renderSystem->setTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		_renderSystem->setTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		_renderSystem->setTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	}

	void ParticleSystem::postRender()
	{
		/*for (size_t i = 0; i < _particles.size(); ++i)
		{
			if (_particles[i]->isAlive())
			{
				_particles[i]->postRender();
			}
		}*/
		_renderSystem->setTexture(0, NULL);
		_renderSystem->setVertexDeclaration(VD_NULL);
		_renderSystem->setRenderState(D3DRS_ALPHABLENDENABLE, false);
	}

	void ParticleSystem::reset()
	{
		for (size_t i = 0; i < _particles.size(); ++i)
		{
			_particles[i]->recuperate();
		}
	}

	bool ParticleSystem::isEmpty()
	{
		return _particles.size() == 0 ? true:false;
	}

	void ParticleSystem::addParticle()
	{
		ParticlePara* pp(_psp->_pp);
		float angle = Euler::Basic::randomReal(3.14f*1.1f, 3.14f*1.5f);
		float norm = Euler::Basic::randomReal(0.01f, 0.05f);
		pp->_velocity = Vec3(Euler::Basic::Cos(angle), Euler::Basic::Sin(angle), 0.0f) * norm;
		int r = Euler::Basic::randomReal(0.0f, 255.0f);
		int g = Euler::Basic::randomReal(0.0f, 255.0f);
		int b = Euler::Basic::randomReal(0.0f, 255.0f);
		int a = Euler::Basic::randomReal(0.0f, 255.0f);
		pp->_bbp->_color = D3DCOLOR_RGBA(r, g, b, a);

		Particle* p = new Particle(pp);
		if (p->create())
		{
			_particles.push_back(p);
		}
		else
		{
			Error("Failed to create Particle!");
		}
	}

	ParticleSystemPara::ParticleSystemPara( ParticleSystemPara* psp )
	{
		if (psp != NULL)
		{
			_maxCount = psp->_maxCount;
			_name = psp->_name;
			_pp = new ParticlePara(psp->_pp);
		}
	}

	ParticleSystemPara::ParticleSystemPara( ParticlePara* pp )
	{
		if (pp != NULL)
		{
			_pp = new ParticlePara(pp);
		}
	}

	ParticlePara::ParticlePara( ParticlePara* pp )
	{
		if (pp != NULL)
		{
			//_speed = pp->_speed;
			_age = pp->_age;
			_lifespan = pp->_lifespan;
			_isAlive = pp->_isAlive;
			_velocity = pp->_velocity;
			_acceleration = pp->_acceleration;
			_bbp = new BillboardPara(pp->_bbp);
		}
	}

	ParticlePara::ParticlePara( BillboardPara* bbp )
	{
		if (bbp != NULL)
		{
			_bbp = new BillboardPara(bbp);
		}
	}
}