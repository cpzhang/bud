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

private:
	//
	bool createModules();
	
	//
	bool setViewport();

	bool initGeometry();

	//
	bool createFonts();

	void renderGeometry();

	//
private:
	IModules*				_modules;
	Euclid::IRenderSystem*	_renderSystem;
	Euclid::ISceneManager*	_sceneManager;
	Euclid::IBufferFactory*	_bufferManager;
	Euclid::IFontFactory*	_fontManager;
	Euclid::IZIPManager*	_zipManager;
	Euclid::ITextureFactory* _texManager;
	Euclid::VertexBuffer*	_vb;
	std::string				_freeTypeFont;
	Euclid::EntityMD2*		_model;
	Euclid::SceneNode*		_node;
	Buddha::CSVReader<Euclid::ManualPara>* _blooding;
	std::string				_csvPath;
};

#endif // __WaitingForYou_h__
