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
	void invalidateDevice();
	void restoreDevice();
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
	friend class InputMessageHandler;
	IModules*				_modules;
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::Effect*			_fx;
};

#endif // __WaitingForYou_h__
