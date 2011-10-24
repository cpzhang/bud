//==========================================================================
/**
* @file	  : ParticleSystem.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-18   12:13
* purpose : 
*/
//==========================================================================

#ifndef __ParticleSystem_h__
#define __ParticleSystem_h__

#include "Common.h"
#include "Billboard.h"

namespace Euclid
{
	class _EuclidExport_ ParticlePara : public RenderablePara
	{
	public:
		ParticlePara(ParticlePara* pp);
		ParticlePara(BillboardPara* bbp);
	public:
		//Real		_speed;
		Real		_age;
		Real		_lifespan;
		bool		_isAlive;
		Vec3		_velocity;
		Vec3		_acceleration;
		BillboardPara* _bbp;
	};

	class Particle
	{
	public:
		Particle(ParticlePara* pp);
		//Particle(BillboardPara* para);
		~Particle();
	public:
		/*Real		_speed;
		Real		_age;
		Real		_lifespan;
		bool		_isAlive;
		Vec3		_velocity;
		Vec3		_acceleration;*/
		Billboard*	_bb;
		ParticlePara*	_pp;
		ParticlePara*	_pp_backup;
	public:
		bool isAlive();
		void recuperate();
		void update(u32 current, u32 delta);
		void render();
		bool create();
		void destroy();
		void preRender();
		void postRender();
		void attachNode(Node* node);
	};

	class _EuclidExport_ ParticleSystemPara : public RenderablePara
	{
	public:
		ParticleSystemPara(ParticleSystemPara* psp);
		ParticleSystemPara(ParticlePara* pp);
	public:
		std::string		_name;
		int				_maxCount;
		
		ParticlePara*	_pp;
	};
	class ParticleSystem : public RenderableObject
	{
	public:
		ParticleSystem();
		ParticleSystem(ParticleSystemPara* psp);
		~ParticleSystem();
	public:
		//
		virtual bool create();
		//
		virtual void destroy();

		//
		virtual void update(u32 current, u32 delta);

		//
		virtual void render();

		//
		virtual void preRender();

		//
		virtual void postRender();
	public:
		bool isEmpty();
		void addParticle();
		void reset();
	public:
		/*int			_maxCount;*/
		std::vector<Particle*> _particles;
		ParticleSystemPara*		_psp;
		ITexture*				_tex;
	};
}

#endif // __ParticleSystem_h__
 
