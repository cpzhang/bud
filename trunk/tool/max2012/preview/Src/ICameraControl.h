#ifndef __I_CameraControl_H__
#define __I_CameraControl_H__

enum CameraControlMode
{
	CCM_THIRD_PERSON
};

#define CameraControlInterface(pure)\
	virtual void release() ## pure\
	virtual std::string getName() ## pure\
	virtual void onKeyDown(char c) ## pure\
	virtual void onRightButtonDown(ushort x,ushort y) ## pure\
	virtual void onMouseMove(ushort x,ushort y) ## pure\
	virtual void onRightButtonUp(ushort x,ushort y) ## pure\
	virtual void onLeftButtonDown(ushort x,ushort y) ## pure\
	virtual void onLeftButtonUp(ushort x,ushort y) ## pure\
	virtual void onMouseWheel(float delta) ## pure\
	virtual void onMButtonDown(ushort x,ushort y) ## pure\
	virtual void onMButtonUp(ushort x,ushort y) ## pure\
	virtual const Matrix4& getWorldMatrix() ## pure
struct ICameraControl
{
	CameraControlInterface(=0;)
};

#endif