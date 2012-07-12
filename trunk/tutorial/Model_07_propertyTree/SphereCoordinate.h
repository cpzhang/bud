#pragma once
//��������ϵ��������������������˳��ӽǿ��ƣ�����Ϊ�������ĵ㣬���ͨ�����̻����������������������ϵ���˶�
//��������ʵ��ģ�Ͳ鿴������Щ���ۣ�Χ��XZƽ����ת�Ľ�alpha����Χ[0, PI]�����˼��㣬������������⣺��һ���뾶��Y��ͶӰ�����и������ң�
//������XZƽ���ͶӰ�����ң�û�и�ֵ��������ɽ�betaȷ�����ڶ���beta���˶���Ӱ��alpha������֮��������alpha���˼��㣬betaҪ��PI��
//������Ĭ�ϵ�view space coordination��Y�����ϣ�һ��Խ�����㣬Y��Ӧ������
//�����㣬˼����һ���ϣ������κ��������н���İ취����ʵ����Ϊģ�Ͳ鿴���ѣ������ô���������ϵ��ֱ����ģ�ͱ�������ϵ���˶�����
struct sSphereCoordination
{
	sSphereCoordination()
	{
		reset();
	}
	void updateDistance(float delta)
	{
		mDistance -= delta * mDistance;
		if (mDistance <= smMinDistance)
		{
			mDistance = smMinDistance;
		}
		if (mDistance >= smMaxDistance)
		{
			mDistance = smMaxDistance;
		}
	}
	void updateAngleUpFromXZ(float delta)
	{
 		mAngleUpFromXZ._radian -= delta;
	}
	void reset()
	{
		mDistance = 500.0f;
		mAngleAroundY = Euler::HalfPI;
		mAngleUpFromXZ = Euler::HalfPI * 0.63f;
	}
	Vec3 getPosition()
	{
		Vec3 p;
		p.y = mDistance * Euler::Basic::Cos(mAngleUpFromXZ);
		float s = Euler::Basic::Sin(mAngleUpFromXZ);
		Radian angle = mAngleAroundY;
		//������ı���
		if (s < 0)
		{
			s = -s;
			angle += Euler::PI;
		}
		p.x = mDistance * s * Euler::Basic::Cos(angle);
		p.z = mDistance * s * Euler::Basic::Sin(angle);
		return p;
	}
	Real mDistance;
	Radian mAngleAroundY;//angle around y-axis, as measured from positive x-axis
	Radian mAngleUpFromXZ;//angle up from x-z plane, clamped to [0:Pi]
	Real mTime;
	static Real smMaxDistance;
	static Real smMinDistance;
};
Real sSphereCoordination::smMaxDistance(1000.0f);

Real sSphereCoordination::smMinDistance(50.0f);

class Light
{
public:
	Light()
	{
		mPosition.mDistance = 200;
	}
	sSphereCoordination mPosition;
};