#pragma once
#include "render/Euclid.h"
class IPrimitive;
class IObserver;
class Canvas
{
public:
	Canvas(HWND h);

public:
	bool create();
	void destroy();
	void render();
	void update(u32 delta);
	void onSize(int w, int h);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnLeftButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnRightButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRightButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnMiddleButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMiddleButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	void changeModelFile(const std::string& fileName);
	void saveFile(const std::string& fileName);
	void addObserver(IObserver* o);
	void setAnimation(const std::string name);
private:
	void _renderGeometry();
	void _clear();
	void _clearModel();
	bool _create();
	bool _createModel();
	bool _isInitialized();
	void _calcFPS();
	void _destroymodel();
	//
private:
	HWND					_hwnd;
	Euclid::FreeType*		_font[Euclid::eFontProperty_Size];
	float					_fps;
	std::string				_modelFileName;
	std::string				_modelMeshName;
	std::string				_modelMaterialName;
	std::string				_shaderTexture;
	Euclid::Camera			_camera;
	Euclid::CameraControllerThirdPerson _cameraController;
	bool					_isModelOK;
	bool					_isOK;
	typedef std::pair<std::string, std::string> TextureFilePair;
	typedef std::vector<TextureFilePair> TextureFilePairVec;
	TextureFilePairVec		_textureFiles;
	IPrimitive* _model;
	typedef std::vector<IObserver*> IObserverVec;
	IObserverVec _observers;
};
