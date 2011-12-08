//==========================================================================
/**
* @file	  : Material.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-5-4   19:52
* purpose : 
*/
//==========================================================================

#ifndef __Material_h__
#define __Material_h__

//
#include "stdafx.h"
#include "Common.h"

namespace Euclid
{
	class _EuclidExport_ Material
	{
	public:
		Material(float r, float g, float b, float a);
		Material();
		~Material();

		//
	public:
		//
		D3DMATERIAL9*	getMaterial();

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
		void setEmissive(D3DCOLORVALUE emissive);
		D3DCOLORVALUE getEmissive();

		//
		void setPower(float power);
		float getPower();

		//
	public:
		//
		static const Material Yellow;

		//
	private:
		D3DMATERIAL9*	_material;
	};
}

#endif // __Material_h__
