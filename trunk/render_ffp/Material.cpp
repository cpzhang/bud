#include "Material.h" 

//
namespace Euclid
{
	Material::Material()
		:	_material(0)
	{
		_material = new D3DMATERIAL9;
		memset(_material, 0, sizeof(D3DMATERIAL9));
	}

	Material::Material( float r, float g, float b, float a )
		:	_material(0)
	{
		_material = new D3DMATERIAL9;
		memset(_material, 0, sizeof(D3DMATERIAL9));

		//
		_material->Diffuse.r = _material->Ambient.r = r;
		_material->Diffuse.g = _material->Ambient.r = g;
		_material->Diffuse.b = _material->Ambient.r = b;
		_material->Diffuse.a = _material->Ambient.r = a;
	}
	Material::~Material()
	{

	}

	D3DMATERIAL9* Material::getMaterial()
	{
		return _material;
	}

	void Material::setDiffuse( D3DCOLORVALUE diffuse )
	{
		_material->Diffuse = diffuse;
	}

	D3DCOLORVALUE Material::getDiffuse()
	{
		return _material->Diffuse;
	}

	void Material::setSpecular( D3DCOLORVALUE specular )
	{
		_material->Specular = specular;
	}

	D3DCOLORVALUE Material::getSpecular()
	{
		return _material->Specular;
	}

	void Material::setAmbient( D3DCOLORVALUE ambient )
	{
		_material->Ambient = ambient;
	}

	D3DCOLORVALUE Material::getAmbient()
	{
		return _material->Ambient;
	}

	void Material::setEmissive( D3DCOLORVALUE emissive )
	{
		_material->Emissive = emissive;
	}

	D3DCOLORVALUE Material::getEmissive()
	{
		return _material->Emissive;
	}

	void Material::setPower( float power )
	{
		_material->Power = power;
	}

	float Material::getPower()
	{
		return _material->Power;
	}

	const Material Material::Yellow(1.0f, 1.0f, 0.0f, 1.0f);

}