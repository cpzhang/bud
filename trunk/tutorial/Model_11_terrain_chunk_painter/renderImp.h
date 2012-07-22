#pragma once
#include "render/Euclid.h"

//
class RenderEngineImp: public Zen::SingletonEx<RenderEngineImp>
{
public:
	RenderEngineImp(HWND h);
	bool create();
	void destroy();
	Zen::IRenderEngine* getRenderEngine();
	bool isInitialized();
public:
	HWND _hwnd;
	Zen::IRenderEngine* _render;
	bool _initialized;
	float _width;
	float _height; 
};