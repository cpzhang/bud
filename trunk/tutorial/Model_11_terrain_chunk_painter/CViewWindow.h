#pragma once
#include "atlHeaders.h"
#include "IdleHandler.h"
#include "EventArgs.h"
#include "SphereCoordinate.h"
#include "Sculptor.h"
//
class CViewWindow: 
	public CWindowImpl<CViewWindow>
	,public IdleHandler
{
public:
	BEGIN_MSG_MAP(CViewWindow)
		MESSAGE_HANDLER(WM_CREATE, onCreate)
		MESSAGE_HANDLER(WM_PAINT, onPaint)
		MESSAGE_HANDLER(WM_DESTROY, onDestroy)
		MESSAGE_HANDLER(WM_SIZE, onSize)
		MESSAGE_HANDLER(WM_MOUSEWHEEL, onMouseWheel)
		MESSAGE_HANDLER(WM_MOUSEMOVE, onMouseMove)
		MESSAGE_HANDLER(WM_KEYDOWN, onKeyDown)
	END_MSG_MAP()

	IdleHandler_Derived
public:
	CViewWindow();
public:
	LRESULT onKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled );
	LRESULT onCreate(UINT, WPARAM, LPARAM, BOOL&) ;
	LRESULT onPaint(UINT, WPARAM, LPARAM, BOOL& b) ;
	LRESULT onMouseWheel(UINT, WPARAM wParam, LPARAM, BOOL& b);
	LRESULT onMouseMove(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT onSize(UINT, WPARAM, LPARAM lParam, BOOL& b) ;
	LRESULT onDestroy(UINT, WPARAM, LPARAM, BOOL&) ;
	void _create();
	bool updateSculptorRadius(const EventArgs& e);
	bool updateSculptorStrength(const EventArgs& e);
	void _render();
	void _renderTerrain();
	void _renderSphere();
	void _renderLight();
	void _renderModel();
	void _renderFont();
	void _clear();
	void _calcFPS();
	void _clearModel();
	void _destroymodel();
	void changeModelFile( const std::string& fileName );
private:
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::FreeType*		_font[Euclid::eFontProperty_Size];
	float					_fps;
	IPrimitive*				_model;
	std::string				_modelFileName;
	Euclid::Camera			_camera;
	bool					_painted;
	sSphereCoordination		_cameraRelativeShpereCoordination;// relative position of the camera in Shpere Coordination
	Light					_light;
	SphereMesh				_sphere;
	Sculptor				mSculptor;
};

