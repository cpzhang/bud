#pragma once
#include "render/Euclid.h"
class IPrimitive;
class IObserver;
class Canvas: public Buddha::SingletonEx<Canvas>
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
	LRESULT OnKeyUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
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
	void stopAnimation();
	void playAnimation();
private:
	void _renderGeometry();
	void _clear();
	void _clearModel();
	bool _create();
	bool _createModel();
	bool _isInitialized();
	void _calcFPS();
	void _destroymodel();
	void _createTerrain();
	void _setActorAngle(const Quaternion& q);
	void _updateActorAngle(float delta);
	Quaternion _getActorAngle();
	Vec3 _screen2World(const Vec2& p);
	Vec3 _getActorPosition();
	void _setActorPosition(const Vec3& p);
	void _updateActorPosition(float delta);
	void _stopActorPosition();
	Real _getTerrainHeight(const Vec3& p);
	Ray _getCameraCastRay(float x, float y);
	//
public:
	struct sSphereCoordination
	{
		sSphereCoordination()
		{
			reset();
		}
		void updateDistance(float delta)
		{
			mDistance -= delta;
			if (mDistance <= smMinDistance)
			{
				mDistance = smMinDistance;
			}
			if (mDistance >= smMaxDistance)
			{
				mDistance = smMaxDistance;
			}
			updateAngleUpFromXZ();
		}
		void updateAngleUpFromXZ()
		{
			// normalize
			mTime = (mDistance - smMinDistance) / (smMaxDistance - smMinDistance);
			// bezier interpolate
			mAngleUpFromXZ = Euclid::InterpolateBezier<Radian>(mTime, smMaxAngleUpFromXZ, 1.2f, smMinAngleUpFromXZ);
		}
		void reset()
		{
			mDistance = 500.0f;
			mAngleAroundY = Euler::HalfPI;
			mAngleUpFromXZ = 1.2f;
			updateAngleUpFromXZ();
		}
		Vec3 getPosition()
		{
			Vec3 p;
			p.y = mDistance * Euler::Basic::Cos(mAngleUpFromXZ);
			p.x = mDistance * Euler::Basic::Sin(mAngleUpFromXZ) * Euler::Basic::Cos(mAngleAroundY);
			p.z = mDistance * Euler::Basic::Sin(mAngleUpFromXZ) * Euler::Basic::Sin(mAngleAroundY);
			return p;
		}
		Real mDistance;
		Radian mAngleAroundY;//angle around y-axis, as measured from positive x-axis
		Radian mAngleUpFromXZ;//angle up from x-z plane, clamped to [0:Pi/2]
		Real mTime;
		static Real smMaxDistance;
		static Real smMinDistance;
		static Radian smMaxAngleUpFromXZ;
		static Radian smMinAngleUpFromXZ;
	};
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
	Vec3 _translation;//positon of the actor
	Quaternion _angle;// angle of the actor
	Quaternion _angleFinal;
	sSphereCoordination _relativeCameraCoordination;// relative position of the camera in Shpere Coordination
	bool _isRightButtonDown;
	bool _isKeyDown;
	Vec2 _mousePosition;
	Real _angleTime;
	Real _angleSpeed;
	Real _positionTime;
	Vec3 _positionFinal;
	Real _positionSpeed;
	Vec3 _intersection;
};
extern Canvas* gCanvas;
