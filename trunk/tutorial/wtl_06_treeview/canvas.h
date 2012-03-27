#pragma once

#include "render/Euclid.h"

class Canvas
{
public:
	Canvas(HWND h);

public:
	bool create();
	void destroy();
	void render();
	void update();
	void onSize(int w, int h);
private:
	void _renderGeometry();
	void _clear();
	bool _create();
	bool _isInitialized();
	void _calcFPS();
	//
private:
	HWND					_hwnd;
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::Effect*			_fx;
	Euclid::FreeType*		_font;
	float					_fps;
};
