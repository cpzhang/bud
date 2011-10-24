//==========================================================================
/**
* @file	  : Paras.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-30   14:56
* purpose : 
*/
//==========================================================================

#ifndef __Paras_h__
#define __Paras_h__

#include "stdafx.h"
#include "Common.h"

//
namespace Euclid
{
	class Paras : public Buddha::SingletonEx<Paras>
	{
	public:
		Paras()
			:	_device(0), _hwnd(0), _renderSystem(0)//, _camera(0)
		{

		}
		~Paras()
		{

		}
	public:
		IDirect3DDevice9*		_device;
		HWND					_hwnd;
		IRenderSystem*			_renderSystem;
		//Camera*					_camera;
		CameraEx*				_cameraEx;
	};
}

#endif // __Paras_h__
 
