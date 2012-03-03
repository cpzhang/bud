//==========================================================================
/**
* @file	  : GameImp.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-4-29   18:25
* purpose : 
*/
//==========================================================================

#ifndef __GameImp_h__
#define __GameImp_h__

//
#include "base/Buddha.h"

//
#include "IModules.h"
//
class GameImp : public Buddha::Game
{
public:
	GameImp();
	~GameImp();

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

	bool _initNet();
	void _destroyNet();
	void _initAddress();
	//
private:
	IModules*				_modules;
	Euclid::FreeType*		_font;
	std::ostringstream		_ss;
};

#endif // __GameImp_h__
