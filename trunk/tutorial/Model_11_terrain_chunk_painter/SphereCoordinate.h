#pragma once
#include "renderImp.h"
//��������ϵ��������������������˳��ӽǿ��ƣ�����Ϊ�������ĵ㣬���ͨ�����̻����������������������ϵ���˶�
//��������ʵ��ģ�Ͳ鿴������Щ���ۣ�Χ��XZƽ����ת�Ľ�alpha����Χ[0, PI]�����˼��㣬������������⣺��һ���뾶��Y��ͶӰ�����и������ң�
//������XZƽ���ͶӰ�����ң�û�и�ֵ��������ɽ�betaȷ�����ڶ���beta���˶���Ӱ��alpha������֮��������alpha���˼��㣬betaҪ��PI��
//������Ĭ�ϵ�view space coordination��Y�����ϣ�һ��Խ�����㣬Y��Ӧ������
//�����㣬˼����һ���ϣ������κ��������н���İ취����ʵ����Ϊģ�Ͳ鿴���ѣ������ô���������ϵ��ֱ����ģ�ͱ�������ϵ���˶�����
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
	Euclid::IMaterial*		_material;
	std::vector<Euclid::sPositionTexture> _vertices;
	std::vector<u16>		_indices;
	Euclid::eVertexDeclarationType _VDT;
	Euclid::IBuffer* _vertexBuffer;
	Euclid::IBuffer* _indexBuffer;
	void clear();
	void destroy();
	SphereMesh();
	~SphereMesh();
	void render();
	bool _initBuffer();
	void create();
};