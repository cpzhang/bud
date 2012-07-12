#include "canvas.h"
#include <sstream>
#include "Entity.h"
#include "Geometry.h"
#include "Group.h"
#include "MzModel.h"
#include "global.h"
#include <cctype>
#include "SubEntity.h"
#include "IObserver.h"
void Canvas::_renderGeometry()
{
	if(_isModelOK)
	{
		if (_model)
		{
			_model->render();
			//
			RenderEngineImp::getInstancePtr()->renderPoint(RenderEngineImp::getInstancePtr()->mIntersection.position, Euclid::Color::Red);
			RenderEngineImp::getInstancePtr()->renderPoint(RenderEngineImp::getInstancePtr()->mIntersectionClicked.position, Euclid::Color::Green);
		}
	}

	// 文字最后画
	if (_font[Euclid::eFontProperty_Normal])
	{
		std::ostringstream ss;
		ss<<"FPS:       "<<_fps<<std::endl<<"___"<<std::endl;
		_font[Euclid::eFontProperty_Normal]->render(Vec3(10, 10, 0), Vec3(1, 0, 0), Euclid::Color::Red, ss.str());
	}
}

void Canvas::_clear()
{
	_isOK = false;
	for (size_t i = 0; i != Euclid::eFontProperty_Size; ++i)
	{
		_font[i] = NULL;
	}
	_fps = 0.0f;
	_isRightButtonDown = false;
	_isKeyDown = false;
	_clearModel();
}

bool Canvas::_create()
{
	new Buddha::Logger;
	new RenderEngineImp(_hwnd);
	if (!RenderEngineImp::isNull() && !RenderEngineImp::getInstancePtr()->isInitialized())
	{
		RenderEngineImp::getInstancePtr()->create();
	}

	//
	std::string ttfName("freetype/simkai.ttf");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(ttfName, 28, Euclid::eFontProperty_Normal, "freeNormal");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(ttfName, 28, Euclid::eFontProperty_Offset_1, "freeOffset1");
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->createFont(ttfName, 28, Euclid::eFontProperty_BG_1, "freeBG1");
	_font[Euclid::eFontProperty_Normal] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeNormal"));
	_font[Euclid::eFontProperty_Offset_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeOffset1"));
	_font[Euclid::eFontProperty_BG_1] = RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont(std::string("freeBG1"));

	//
	_camera.setPosition(Vec3(0.0f, 50.0f, 150.0f));
	_cameraController.attachCamera(&_camera);

	//
	RenderEngineImp::getInstancePtr()->setCanvas(this);
	_isOK = true;
//	_createTerrain;
	//
	return true;
}

bool Canvas::_isInitialized()
{
	return _isOK;
}

void Canvas::render()
{
	if (_hwnd == NULL)
	{
		return;
	}
	//
	RECT rect;
	GetClientRect(_hwnd, &rect);
	if (rect.left == rect.right)
	{
		return;
	}
	if (!_isInitialized())
	{
		_create();
	}
	if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
	{
		return;
	}
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->clear(0, NULL, Euclid::eClearFlags_Target | Euclid::eClearFlags_ZBuffer, RenderEngineImp::getInstancePtr()->getClearColor(), 1.0f, 0L);

	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->beginScene();
	//
	_renderGeometry();

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->endScene();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->present(NULL, NULL, NULL);

	//
	_calcFPS();
}

void Canvas::destroy()
{
	if (!_isInitialized())
	{
		return;
	}
	_destroymodel();
	RenderEngineImp::getInstancePtr()->destroy();
	delete RenderEngineImp::getInstancePtr();

	delete Buddha::Logger::getInstancePtr();
	//
	_clear();
}

void Canvas::onSize( int w, int h )
{
	if (RenderEngineImp::isNull() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine() || NULL == RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem())
	{
		return;
	}
	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onInvalidateDevice();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->onInvalidateDevice();
	//
	Euclid::sViewPort vp;
	vp.X = 0;
	vp.Y = 0;
	vp.Width = w;
	vp.Height = h;
	vp.MinZ = 0;
	vp.MaxZ = 1;
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->setViewport(&vp);

	//
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getEffectManager()->onRestoreDevice();
	RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->onRestoreDevice();
}

bool Canvas::create()
{
	return true;
}

Canvas::Canvas( HWND h )
{
	_hwnd = h;
	_clear();
}

void Canvas::update(u32 delta)
{
	if (_isInitialized())
	{
		_camera.update();
	}
	
	if (_model)
	{
		_updateActorAngle(delta);
		_updateActorPosition(delta);
		Mat4 worldM = Mat4::IDENTITY;
		Vec3 finalPos = _getActorPosition();
		finalPos.y = _getTerrainHeight(finalPos);
		worldM.makeTransform(finalPos , Vec3::UNIT_SCALE, _getActorAngle());
		//
		_camera.lookAt(finalPos + Vec3(0.0f, 70.0f, 0.0f));
		_camera.setPosition(finalPos + Vec3(0.0f, 70.0f, 0.0f) + _relativeCameraCoordination.getPosition());
		//
		Mat4 m = _camera.getProjectionMatrix() * _camera.getViewMatrix() * worldM/*_cameraController.getMatrix()*/;
		_model->setMatrix("gWorldViewProj", m);
		_model->setMatrix("gView", _camera.getViewMatrix() /** _cameraController.getMatrix()*/);
		_model->setMatrix("gProjection", _camera.getProjectionMatrix());
		//
		if(RenderEngineImp::getInstancePtr()->mSkeletonMaterial)
		{
			RenderEngineImp::getInstancePtr()->mSkeletonMaterial->getEffect()->setMatrix("gView", _camera.getViewMatrix());
			RenderEngineImp::getInstancePtr()->mSkeletonMaterial->getEffect()->setMatrix("gProjection", _camera.getProjectionMatrix());
		}

		//
		_model->update(delta);
	}
}

void Canvas::_calcFPS()
{
	static float last_time = (float)timeGetTime();
	float current_time = (float)timeGetTime();
	static int count_frames = 0;
	++count_frames;
	float delta = current_time - last_time;
	// calculate FPS per Second
	if(delta >= 1000.0f)
	{
		_fps = (float)count_frames * 1000.0f  / delta;
		last_time = current_time;
		count_frames = 0;
	}
}

LRESULT Canvas::OnKeyUp( UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled )
{
	bHandled = false;
	_isKeyDown = false;
	_stopActorPosition();
	return 0;
}

LRESULT Canvas::OnKeyDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//_cameraController.onKeyDown(wParam);
	bHandled = false;
	static float sf_step = 20.0f;
	Quaternion angle = Quaternion::IDENTITY;
	Vec3 ap = _getActorPosition();
	Vec3 tp;
	Vec3 tpCross;
	Radian wAngle;
	{
		//拾取射线
		Ray r(_camera.getDirection(), _camera.getPosition());
		//与xz平面的交点
		Real t = -r._origin.y / r._direction.y;
		Vec3 p = r.getPoint(t);
		Vec3 ap = _getActorPosition();
		p -= ap;
		//调整主角方向
		Quaternion q;
		wAngle = Euler::Basic::ATan2(p.x, p.z);
		tp = p;
		tp.y = 0;
		tp.normalise();
		//
		tpCross = tp.crossProduct(Vec3::UNIT_Y);
	}
	
	switch(wParam)
	{
	case 'W':
		{
			angle.FromAngleAxis(wAngle, Vec3::UNIT_Y);
			ap += tp * sf_step;
		}break;
	case 'S':
		{
			angle.FromAngleAxis(Euler::PI + wAngle._radian, Vec3::UNIT_Y);
			ap -= tp * sf_step;
		}break;
	case 'A':
		{
			angle.FromAngleAxis(Euler::HalfPI + wAngle._radian, Vec3::UNIT_Y);
			ap -= tpCross * sf_step;
		}break;
	case 'D':
		{
			angle.FromAngleAxis(-Euler::HalfPI + wAngle._radian, Vec3::UNIT_Y);
			ap += tpCross * sf_step;
		}break;
	case 'L':
		{
			static u32 lod = 0;
			_model->geomipmapping(lod++);
			if (lod >= 8)
			{
				lod = 0;
			}
		}
	}
	_model->setAnimationName("walk");
	if (!_isKeyDown)
	{
		_angleSpeed = 0.05f;
		_setActorAngle(angle);
	}
	
	_positionSpeed = 0.001f;
	_setActorPosition(ap);
	_isKeyDown = true;
	return 0;
}
LRESULT Canvas::OnLeftButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	//
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	//_cameraController.onLeftButtonDown(x,y);

	bHandled = false;
	
	//拾取射线
	Ray r = _getCameraCastRay(x, y);
	
	//与xz平面的交点
	Real t = 0.0f;
	//t = -r._origin.y / r._direction.y;
	//
	{
		bool b = _model->intersect(r, t);
		if (!b)
		{
			return 0;
		}
	}
	Vec3 p = r.getPoint(t);
	RenderEngineImp::getInstancePtr()->mIntersectionClicked.position = p;
	p.y = 0.0f;
	Vec3 ap = _getActorPosition();
	p -= ap;
	Real len = p.length();
	_positionSpeed = 0.010f / len;
	//目标点
	_model->setAnimationName("walk");
	_setActorPosition(p);
	
	//调整主角方向
	Quaternion q;
	Radian angle = Euler::Basic::ATan2(p.x, p.z);
	q.FromAngleAxis(angle, Vec3::UNIT_Y);
	_angleSpeed = 0.01f;
	_setActorAngle(q);
	return 0;
}
LRESULT Canvas::OnLeftButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onLeftButtonUp(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnRightButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onRightButtonDown(x,y);
	bHandled = false;
	_isRightButtonDown = true;
	_mousePosition.x = x;
	_mousePosition.y = y;
	return 0;
}

LRESULT Canvas::OnRightButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onRightButtonUp(x,y);
	bHandled = false;
	_isRightButtonDown = false;
	return 0;
}

LRESULT Canvas::OnMiddleButtonDown( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMButtonDown(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMiddleButtonUp( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	_cameraController.onMButtonUp(x,y);
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMouseMove( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	if (_isRightButtonDown)
	{
		Vec2 p(x, y);
		float delta = _mousePosition.x - x;
		delta *= 0.01;
		_relativeCameraCoordination.mAngleAroundY += delta;
		_mousePosition.x = x;
		_mousePosition.y = y;
	}
	
	bHandled = false;
	return 0;
}

LRESULT Canvas::OnMouseWheel( UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled )
{
	short d = HIWORD(wParam);
	float delta = (float)d * 0.05f;
	_relativeCameraCoordination.updateDistance(delta);
	
	bHandled = false;
	return 0;
}

void Canvas::changeModelFile( const std::string& fileName )
{
	size_t dotPos = fileName.find('.');
	if (dotPos == std::string::npos)
	{
		return;
	}
	std::string suffix = fileName.substr(dotPos, fileName.size() - dotPos);
	std::transform(suffix.begin(), suffix.end(), suffix.begin(), std::tolower);
	if (suffix == ".group")
	{
		_destroymodel();
		_clearModel();
		_model = new Group;
	}
	else if (suffix == ".entity")
	{
		_destroymodel();
		_clearModel();
		_model = new Entity;
	} 
	else if(suffix == ".mesh")
	{
		_destroymodel();
		_clearModel();
		_model = new Geometry;
	}
	else if(suffix == ".mz")
	{
		_destroymodel();
		_clearModel();
		_model = new MzModel;
	}
	else if(suffix == ".subentity")
	{
		_destroymodel();
		_clearModel();
		_model = new SubEntity;
	}
	else
	{
		return;
	}

	_model->decode(fileName);
	_createModel();
	for (size_t i = 0; i != _observers.size(); ++i)
	{
		_observers[i]->update(_model);
	}
}

void Canvas::_clearModel()
{
	_modelFileName.clear();
	_modelMaterialName.clear();
	_modelMeshName.clear();
	_isModelOK = false;
	_textureFiles.clear();
	_model = NULL;
	_positionFinal = Vec3::ZERO;
	_translation = Vec3::ZERO;
	_angle = Quaternion::IDENTITY;
	_angleTime = 1.0f;
	_positionTime = 1.0f;
	_positionSpeed = 0.0f;
	_intersection = Vec3::ZERO;
	AABBEntity::reset();
}

bool Canvas::_createModel()
{
	_isModelOK = true;
	return true;
}

void Canvas::_destroymodel()
{
	if (_model)
	{
		_model->destroy();
		delete _model;
		_model = NULL;
	}
}

void Canvas::saveFile( const std::string& fileName )
{
	_model->saveFile(fileName);
}

void Canvas::addObserver( IObserver* o )
{
	_observers.push_back(o);
}

void Canvas::setAnimation( const std::string name )
{
	_model->setAnimationName(name);
}

void Canvas::stopAnimation()
{

}

void Canvas::playAnimation()
{

}

void Canvas::_createTerrain()
{
//	_terrain = new Euclid::Terrain;
//	_terrain->
}

void Canvas::_updateActorAngle(float delta)
{
	_angleTime += delta * _angleSpeed;
}

Quaternion Canvas::_getActorAngle()
{
	if (_angleTime >= 1)
	{
		_angle = _angleFinal;
		return _angle;
	}
	return Quaternion::Slerp(_angleTime, _angle, _angleFinal);
}

void Canvas::_setActorAngle( const Quaternion& q )
{
	if (_angleTime < 1.0f)
	{
		_angle = Quaternion::Slerp(_angleTime, _angle, _angleFinal);
	}
	_angleTime = 0.0f;
	_angleFinal = q;	
}

Vec3 Canvas::_screen2World( const Vec2& p )
{
	return Vec3::ZERO;
}

Vec3 Canvas::_getActorPosition()
{
	if (_positionTime >= 1.0f)
	{
		_translation = _positionFinal;
		_model->setAnimationName("stand");
		return _translation;
	}
	return Euclid::InterpolateBezier(_positionTime, _translation, _positionFinal);
}

void Canvas::_setActorPosition( const Vec3& p )
{
	if (_positionTime < 1.0f)
	{
		_translation = Euclid::InterpolateBezier(_positionTime, _translation, _positionFinal);
	}
	_positionTime = 0.0f;
	_positionFinal = p;
}

void Canvas::_updateActorPosition( float delta )
{
	_positionTime += delta * _positionSpeed;	
}

void Canvas::_stopActorPosition()
{
	_translation = Euclid::InterpolateBezier(_positionTime, _translation, _positionFinal);
	_positionTime = 1.0f;
	_positionFinal = _translation;
}

Real Canvas::_getTerrainHeight( const Vec3& p )
{
	if (_model)
	{
		static bool firstComing = true;
		static Real lastY = 0.0f;
		if (_positionTime >= 1.0f && !firstComing)
		{
			return lastY;
		}
		else
		{
			Ray r(Vec3::NEGATIVE_UNIT_Y, Vec3(p.x, 100000.0f, p.z));
			Real t = 0.0f;
			bool b = _model->intersect(r, t);
			if (b)
			{
				firstComing = false;
				_intersection = r.getPoint(t);
				RenderEngineImp::getInstancePtr()->mIntersection.position = _intersection;
				lastY = _intersection.y;
				return lastY;
			}
		}
	}
	return 0.0f;
}

Ray Canvas::_getCameraCastRay(float x, float y)
{
	return _camera.getCameraToViewportRay(x, y);
}

Real Canvas::sSphereCoordination::smMaxDistance(1000.0f);

Real Canvas::sSphereCoordination::smMinDistance(50.0f);

Radian Canvas::sSphereCoordination::smMaxAngleUpFromXZ(1.5f);

Radian Canvas::sSphereCoordination::smMinAngleUpFromXZ(1.0f);
