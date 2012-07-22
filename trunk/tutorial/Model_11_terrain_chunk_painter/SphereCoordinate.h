#pragma once
#include "renderImp.h"
//球面坐标系，起初是想用来做第三人称视角控制，球心为主角中心点，玩家通过键盘或鼠标控制摄像机在球面坐标系下运动
//这里用来实现模型查看，废了些周折，围绕XZ平面旋转的角alpha，范围[0, PI]，过了极点，会带来两个问题：第一，半径在Y轴投影有正有负，余弦，
//但是在XZ平面的投影是正弦，没有负值，其符号由角beta确定；第二，beta的运动不影响alpha，但反之不成立，alpha过了极点，beta要加PI；
//第三，默认的view space coordination的Y轴向上，一旦越过极点，Y轴应该往下
//此三点，思考了一晚上，好在任何问题总有解决的办法，其实，作为模型查看而已，不必用此球面坐标系，直接在模型本地坐标系下运动即可
struct sSphereCoordination
{
	sSphereCoordination();
	void updateDistance(float delta);
	void updateAngleUpFromXZ(float delta);
	void reset();
	Vec3 getPosition();
	Real mDistance;
	Radian mAngleAroundY;//angle around y-axis, as measured from positive x-axis
	Radian mAngleUpFromXZ;//angle up from x-z plane, clamped to [0:Pi]
	Real mTime;
	Vec3 mOrigion;
	static Real smMaxDistance;
	static Real smMinDistance;
};
class Light
{
public:
	Light();
	sSphereCoordination mPosition;
};
class SphereMesh
{
public:
	Zen::IMaterial*		_material;
	std::vector<Zen::sPositionTexture> _vertices;
	std::vector<u16>		_indices;
	Zen::eVertexDeclarationType _VDT;
	Zen::IBuffer* _vertexBuffer;
	Zen::IBuffer* _indexBuffer;
	void clear();
	void destroy();
	SphereMesh();
	~SphereMesh();
	void render();
	bool _initBuffer();
	void create();
};