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
	void toggleFillMode();
	void toggleLOD();
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
	void createVB();
	void fillVB();
	void createIB();
	void fillIB();
	//
	struct Patch
	{
		float distance;
		int	  lod;
	};
	static const u32 sc_PatchSize;
	//
private:
	IModules*				_modules;
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::IBuffer*		_vbInstances;
	Euclid::IBuffer*		_ib;
	Euclid::Effect*			_fx;
	Euclid::Effect*			_fx_axis;
	Euclid::Camera*			_camera;
	Euclid::CameraControllerThirdPerson*	_cameraController;
	Mat4					_modelMatrix;
	std::vector<Vec4>		_v4s;
	Euclid::ITexture*		_texture;
	Euclid::FreeType*		_font;
	u32						_vertexNumber;
	u32						_vertexNumber2GPU;
	u32						_triangleNumber2GPU;
	u32						_mapWidth;
	Euclid::sPositionColor	_axis_vertices[6];
	Euclid::sPositionColorTexture	_water_vertices[4];
	friend class InputMessageHandler;
	bool					_wireframe;
	int						_lod;
	std::string				_heightMapFileName;
	Euclid::IMaterial*		_water_material;
	Euclid::Effect*			_water_fx;
	Mat3					_water_matrix;
};

#endif // __WaitingForYou_h__
