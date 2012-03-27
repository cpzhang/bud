#pragma once
#include "render/Euclid.h"

//
class RenderEngineImp: public Buddha::SingletonEx<RenderEngineImp>
{
public:
	RenderEngineImp(HWND h);
	bool create();
	void destroy();
	Euclid::IRenderEngine* getRenderEngine();
	bool isInitialized();
private:
	HWND _hwnd;
	Euclid::IRenderEngine* _render;
	bool _initialized;
};