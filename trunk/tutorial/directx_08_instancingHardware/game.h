//==========================================================================
/**
* @file	  : WaitingForYou.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   18:25
* purpose : 
*/
//==========================================================================

#ifndef __WaitingForYou_h__
#define __WaitingForYou_h__

//
#include "base/Buddha.h"

//
#include "IModules.h"
//
class WaitingForYou : public Buddha::Game
{
public:
	WaitingForYou();
	~WaitingForYou();

public:
	//
	virtual bool update(u32 current, u32 delta);

	//
	virtual bool forePlay();

	//
	virtual bool foreRender();

	//
	virtual bool rendering();

	//
	virtual bool postRender();

	//
	virtual void onDragAndDrop();
	virtual void onF1();

	virtual bool destroy();

public:
	Euclid::CameraControllerThirdPerson* getCameraController();
private:
	//
	bool createModules();
	
	//
	bool setViewport();

	bool initGeometry();

	//
	bool createFonts();

	void renderGeometry();

	bool _initAxis();

	//
private:
	IModules*				_modules;
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::IBuffer*		_vbInstances;
	Euclid::IBuffer*		_ib;
	Euclid::Effect*			_fx;
	Euclid::Camera*			_camera;
	Euclid::CameraControllerThirdPerson*	_cameraController;
	Mat4					_modelMatrix;
	Euclid::ITexture*		_texture;
	friend class InputMessageHandler;
};

#endif // __WaitingForYou_h__
