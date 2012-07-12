#pragma once
#include "render/Euclid.h"
class Canvas;
//
class RenderEngineImp: public Buddha::SingletonEx<RenderEngineImp>
{
public:
	RenderEngineImp(HWND h);
	bool create();
	void destroy();
	Euclid::IRenderEngine* getRenderEngine();
	bool isInitialized();
	void setClearColor(Euclid::Color c);
	Euclid::Color getClearColor();
	void setCanvas(Canvas* c);
	Canvas* getCanvas();
private:
	HWND _hwnd;
	Euclid::IRenderEngine* _render;
	bool _initialized;
	Euclid::Color _clearColor;
	Canvas* _canvas;
};