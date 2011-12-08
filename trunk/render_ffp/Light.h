//==========================================================================
/**
* @file	  : Light.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-2   14:13
* purpose : 
*/
//==========================================================================

#ifndef __Light_h__
#define __Light_h__

//
#include "Common.h"
#include "RenderableObject.h"
//
namespace Euclid
{
	class /*_EuclidExport_*/ Light : public RenderableObject
	{
	public:
		Light();
		~Light();

	public:
		//
		virtual bool create();

		//
		virtual void render();

		//
	public:
		D3DLIGHT9* getLight();

		//
		void setType(D3DLIGHTTYPE t);
		D3DLIGHTTYPE getType();

		//
		void setDiffuse(D3DCOLORVALUE diffuse);
		D3DCOLORVALUE getDiffuse();

		//
		void setSpecular(D3DCOLORVALUE specular);
		D3DCOLORVALUE getSpecular();

		//
		void setAmbient(D3DCOLORVALUE ambient);
		D3DCOLORVALUE getAmbient();

		//
		void setPosition(D3DVECTOR position);
		void setPosition(Real x, Real y, Real z);
		D3DVECTOR getPosition();

		//
		void setDirection(D3DVECTOR direction);
		D3DVECTOR getDirection();

		//
		void setRange(float range);
		float getRange();

		//
		void setFalloff(float falloff);
		float getFalloff();

		// Atten = 1/( att0i + att1i * d + att2i * d2)
		void setAttenuation(float constant, float linear, float quadratic);
		float getAttenuationConstant();
		float getAttenuationLinear();
		float getAttenuationQuadratic();

		//
		void setTheta(float theta); 
		float getTheta();

		//
		void setPhi(float phi);
		float getPhi();

	public:
		static const Light White;
		//
	private:
		
		//
	private:
		D3DLIGHT9*	_light;
	};
}

#endif // __Light_h__
