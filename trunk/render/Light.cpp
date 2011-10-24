#include "stdafx.h"
#include "Light.h" 
#include "Paras.h"
#include "IRenderSystem.h"
//
namespace Euclid
{
	Light::Light()
		:	_light(0)
	{
		_type = "Light";

		_light = new D3DLIGHT9;
		memset(_light, 0, sizeof(D3DLIGHT9));
		//
		_light->Type = D3DLIGHT_DIRECTIONAL;
		_light->Diffuse.r = 1.0f;
		_light->Diffuse.g = 1.0f;
		_light->Diffuse.b = 1.0f;

		D3DXVECTOR3 vecDir = D3DXVECTOR3(-1.0f, -1.0f, 1.0f);
		D3DXVec3Normalize( (D3DXVECTOR3*)&_light->Direction, &vecDir );

		_light->Range = 1000.0f;
	}

	Light::~Light()
	{

	}

	D3DLIGHT9* Light::getLight()
	{
		return _light;
	}

	void Light::setType( D3DLIGHTTYPE t )
	{
		_light->Type = t;
	}

	D3DLIGHTTYPE Light::getType()
	{
		return _light->Type;
	}

	void Light::setDiffuse( D3DCOLORVALUE diffuse )
	{
		_light->Diffuse = diffuse;
	}

	D3DCOLORVALUE Light::getDiffuse()
	{
		return _light->Diffuse;
	}

	void Light::setSpecular( D3DCOLORVALUE specular )
	{
		_light->Specular = specular;
	}

	D3DCOLORVALUE Light::getSpecular()
	{
		return _light->Specular;
	}

	void Light::setAmbient( D3DCOLORVALUE ambient )
	{
		_light->Ambient = ambient;
	}

	D3DCOLORVALUE Light::getAmbient()
	{
		return _light->Ambient;
	}

	void Light::setPosition( D3DVECTOR position )
	{	
		_light->Position = position;
	}

	void Light::setPosition( Real x, Real y, Real z )
	{
		_light->Position = D3DXVECTOR3(x, y, z);
	}
	D3DVECTOR Light::getPosition()
	{
		return _light->Position;
	}

	void Light::setDirection( D3DVECTOR direction )
	{
		_light->Direction = direction;
	}

	D3DVECTOR Light::getDirection()
	{
		return _light->Direction;
	}

	void Light::setRange( float range )
	{
		_light->Range = range;
	}

	float Light::getRange()
	{
		return _light->Range;
	}

	void Light::setFalloff( float falloff )
	{
		_light->Falloff = falloff;
	}

	float Light::getFalloff()
	{
		return _light->Falloff;
	}

	void Light::setAttenuation( float constant, float linear, float quadratic )
	{
		_light->Attenuation0 = constant;
		_light->Attenuation1 = linear;
		_light->Attenuation2 = quadratic;
	}

	float Light::getAttenuationConstant()
	{
		return _light->Attenuation0;
	}

	float Light::getAttenuationLinear()
	{
		return _light->Attenuation1;
	}

	float Light::getAttenuationQuadratic()
	{
		return _light->Attenuation2;
	}

	void Light::setTheta( float theta )
	{
		_light->Theta = theta;
	}

	float Light::getTheta()
	{
		return _light->Theta;
	}

	void Light::setPhi( float phi )
	{
		_light->Phi = phi;
	}

	float Light::getPhi()
	{
		return _light->Phi;
	}

	bool Light::create()
	{
		return true;
	}

	void Light::render()
	{
		Paras::getInstancePtr()->_renderSystem->setLight(0, this);
	}
	const Light Light::White;
}