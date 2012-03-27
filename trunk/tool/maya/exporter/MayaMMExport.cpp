// MayaMmExport.cpp : 定义 DLL 应用程序的入口点。
//

/**
更新纪录:
版本号8:
导出的骨骼矩阵为不再依赖静态模型
*/

#include "common.h"
#include "translator.h"
#include "FBTranslator.h"
#include <maya/MFnPlugin.h>
#include <maya/MFnEnumAttribute.h>
#include "resource.h"

#define MZ_STRING	"ShenZhen Unnamed Network - Model"
#define FB_STRING	"ShenZhen Unnamed Network - Clip"
////////////////////////////////////////////////////////////////
class unCreateParticleSystem : public MPxCommand
{
public:
	unCreateParticleSystem() {};
	virtual			~unCreateParticleSystem(); 

	MStatus			doIt( const MArgList& args );
	static void*	creator();

public:
	static  MObject		unParticleEnabled;
	static  MObject		unParticleVisible;
	static  MObject		unParticleSpeed;
	static  MObject		unParticleVariation;
	static  MObject		unParticleConeAngle;
	static	MObject		unParticleGravity;
	static  MObject		unParticleExplosiveForce;
	static  MObject		unParticleLife;
	static  MObject		unParticleLifeVariation;
	static  MObject		unParticleEmissionRate;
	static  MObject		unParticleInitialNum;
	static  MObject		unParticleLimitNum;				//最大发射的粒子数量
	static  MObject		unParticleAttachToEmitter;		//挂接到发射点，版本号13
	static  MObject		unParticleMoveWithEmitter;		//挂接到发射点，版本号18
	static  MObject		unParticleForTheSword;			//粒子总是指向速度方向，版本号23
	static  MObject		unParticleForTheSwordInitialAngle;	//粒子总是指向速度方向，加一个初始的角度，不一定非要对着右方，版本号24
	static  MObject		unParticleWander;
	static  MObject		unParticleWanderRadius;
	static  MObject		unParticleWanderSpeed;
	static  MObject		unParticleAspectRatio;
	static  MObject		unParticleInitialAngleBegin;	//初始粒子旋转角度范围值 - 开始
	static  MObject		unParticleInitialAngleEnd;		//初始粒子旋转角度范围值 - 结束
	static  MObject		unParticleRotationSpeed;		//粒子旋转速度 - 0不旋转
	static  MObject		unParticleRotationSpeedVar;
	static  MObject		unParticleEmitterWidth;
	static  MObject		unParticleEmitterLength;
	static  MObject		unParticleEmitterHeight;
	static  MObject		unParticleBlendMode;
	//static  MObject		unParticleBlendModeSrc;
	//static  MObject		unParticleBlendModeDst;
	static  MObject		unParticleTextureFilename;
	static  MObject		unParticleTextureRows;
	static  MObject		unParticleTextureCols;
	static  MObject		unParticleTextureChangeStyle;
	static  MObject		unParticleTextureChangeInterval;
	static  MObject		unParticleTailLength;
	static  MObject		unParticleTimeMiddle;
	static  MObject		unParticleColorStart;
	static  MObject		unParticleColorMiddle;
	static  MObject		unParticleColorEnd;
	static  MObject		unParticleAlpha;
	static  MObject		unParticleScale;
	static  MObject		unParticleScaleVar;
	static  MObject		unParticleFixedSize;
	static  MObject		unParticleHeadLifeSpan;
	static  MObject		unParticleHeadDecay;
	static  MObject		unParticleTailLifeSpan;
	static  MObject		unParticleTailDecay;
	static  MObject		unParticleHead;
	static  MObject		unParticleTail;
	static  MObject		unParticleUnShaded;
	static  MObject		unParticleUnFogged;
	static  MObject		unParticleBlockByY0;
};

MObject		unCreateParticleSystem::unParticleBlockByY0;
MObject		unCreateParticleSystem::unParticleUnFogged;
MObject		unCreateParticleSystem::unParticleUnShaded;
MObject		unCreateParticleSystem::unParticleTail;
MObject		unCreateParticleSystem::unParticleHead;
MObject		unCreateParticleSystem::unParticleTailDecay;
MObject		unCreateParticleSystem::unParticleTailLifeSpan;
MObject		unCreateParticleSystem::unParticleHeadDecay;
MObject		unCreateParticleSystem::unParticleHeadLifeSpan;
MObject		unCreateParticleSystem::unParticleScale;
MObject		unCreateParticleSystem::unParticleScaleVar;
MObject		unCreateParticleSystem::unParticleFixedSize;
MObject		unCreateParticleSystem::unParticleAlpha;
MObject		unCreateParticleSystem::unParticleColorEnd;
MObject		unCreateParticleSystem::unParticleColorMiddle;
MObject		unCreateParticleSystem::unParticleColorStart;
MObject		unCreateParticleSystem::unParticleTimeMiddle;
MObject		unCreateParticleSystem::unParticleTailLength;
MObject		unCreateParticleSystem::unParticleTextureCols;
MObject		unCreateParticleSystem::unParticleTextureChangeStyle;
MObject		unCreateParticleSystem::unParticleTextureChangeInterval;
MObject		unCreateParticleSystem::unParticleTextureRows;
MObject		unCreateParticleSystem::unParticleTextureFilename;
MObject		unCreateParticleSystem::unParticleBlendMode;
//MObject		unCreateParticleSystem::unParticleBlendModeSrc;
//MObject		unCreateParticleSystem::unParticleBlendModeDst;
MObject		unCreateParticleSystem::unParticleEmitterLength;
MObject		unCreateParticleSystem::unParticleEmitterHeight;
MObject		unCreateParticleSystem::unParticleEmitterWidth;
MObject		unCreateParticleSystem::unParticleEmissionRate;
MObject		unCreateParticleSystem::unParticleInitialNum;
MObject		unCreateParticleSystem::unParticleLimitNum;
MObject		unCreateParticleSystem::unParticleAttachToEmitter;
MObject		unCreateParticleSystem::unParticleMoveWithEmitter;
MObject		unCreateParticleSystem::unParticleForTheSword;
MObject		unCreateParticleSystem::unParticleForTheSwordInitialAngle;
MObject		unCreateParticleSystem::unParticleWander;
MObject		unCreateParticleSystem::unParticleWanderRadius;
MObject		unCreateParticleSystem::unParticleWanderSpeed;
MObject		unCreateParticleSystem::unParticleAspectRatio;
MObject		unCreateParticleSystem::unParticleInitialAngleBegin;
MObject		unCreateParticleSystem::unParticleInitialAngleEnd;
MObject		unCreateParticleSystem::unParticleRotationSpeed;
MObject		unCreateParticleSystem::unParticleRotationSpeedVar;
MObject		unCreateParticleSystem::unParticleLife;
MObject		unCreateParticleSystem::unParticleLifeVariation;
MObject		unCreateParticleSystem::unParticleGravity;
MObject		unCreateParticleSystem::unParticleExplosiveForce;
MObject		unCreateParticleSystem::unParticleConeAngle;
MObject		unCreateParticleSystem::unParticleVariation;
MObject		unCreateParticleSystem::unParticleSpeed;
MObject		unCreateParticleSystem::unParticleVisible;
MObject		unCreateParticleSystem::unParticleEnabled;

////////////////////////////////////////////////////////////////
class unCreateRibbonSystem : public MPxCommand
{
public:
	unCreateRibbonSystem() {};
	virtual			~unCreateRibbonSystem(); 

	MStatus			doIt( const MArgList& args );
	static void*	creator();

public:
	static  MObject		unRibbonEnabled;
	static  MObject		unRibbonVisible;
	static  MObject		unRibbonAbove;
	static  MObject		unRibbonBelow;
	static	MObject		unRibbonEdgesPerSecond;
	static  MObject		unRibbonEdgeLife;
	static	MObject		unRibbonGravity;
	static	MObject		unRibbonTextureRows;
	static	MObject		unRibbonTextureCols;
	static	MObject		unRibbonTextureSlot;
	static	MObject		unRibbonVertexColor;
	static	MObject		unRibbonVertexAlpha;
	static	MObject		unRibbonBlendMode;
	//static  MObject		unRibbonBlendModeSrc;
	//static  MObject		unRibbonBlendModeDst;
	static	MObject		unRibbonTextureFilename;
};

MObject		unCreateRibbonSystem::unRibbonEnabled;
MObject		unCreateRibbonSystem::unRibbonVisible;
MObject		unCreateRibbonSystem::unRibbonAbove;
MObject		unCreateRibbonSystem::unRibbonBelow;
MObject		unCreateRibbonSystem::unRibbonEdgesPerSecond;
MObject		unCreateRibbonSystem::unRibbonEdgeLife;
MObject		unCreateRibbonSystem::unRibbonGravity;
MObject		unCreateRibbonSystem::unRibbonTextureRows;
MObject		unCreateRibbonSystem::unRibbonTextureCols;
MObject		unCreateRibbonSystem::unRibbonTextureSlot;
MObject		unCreateRibbonSystem::unRibbonVertexColor;
MObject		unCreateRibbonSystem::unRibbonVertexAlpha;
MObject		unCreateRibbonSystem::unRibbonBlendMode;
//MObject		unCreateRibbonSystem::unRibbonBlendModeSrc;
//MObject		unCreateRibbonSystem::unRibbonBlendModeDst;
MObject		unCreateRibbonSystem::unRibbonTextureFilename;

unCreateParticleSystem::~unCreateParticleSystem() {}

void* unCreateParticleSystem::creator()
{
	return new unCreateParticleSystem();
}

void removeParticleAttributes(MFnIkJoint& joint)
{
	joint.removeAttribute(joint.attribute("unParticleEnabled"));
	joint.removeAttribute(joint.attribute("unParticleVisible"));
	joint.removeAttribute(joint.attribute("unParticleSpeed"));
	joint.removeAttribute(joint.attribute("unParticleVariation"));
	joint.removeAttribute(joint.attribute("unParticleConeAngle"));
	joint.removeAttribute(joint.attribute("unParticleGravity"));
	joint.removeAttribute(joint.attribute("unParticleExplosiveForce"));
	joint.removeAttribute(joint.attribute("unParticleLife"));
	joint.removeAttribute(joint.attribute("unParticleLifeVariation"));
	joint.removeAttribute(joint.attribute("unParticleEmissionRate"));
	joint.removeAttribute(joint.attribute("unParticleInitialNum"));
	joint.removeAttribute(joint.attribute("unParticleLimitNum"));
	joint.removeAttribute(joint.attribute("unParticleAttachToEmitter"));
	joint.removeAttribute(joint.attribute("unParticleMoveWithEmitter"));
	joint.removeAttribute(joint.attribute("unParticleForTheSword"));
	joint.removeAttribute(joint.attribute("unParticleForTheSwordInitialAngle"));
	joint.removeAttribute(joint.attribute("unParticleWander"));
	joint.removeAttribute(joint.attribute("unParticleWanderRadius"));
	joint.removeAttribute(joint.attribute("unParticleWanderSpeed"));
	joint.removeAttribute(joint.attribute("unParticleAspectRatio"));
	joint.removeAttribute(joint.attribute("unParticleInitialAngleBegin"));
	joint.removeAttribute(joint.attribute("unParticleInitialAngleEnd"));
	joint.removeAttribute(joint.attribute("unParticleRotationSpeed"));
	joint.removeAttribute(joint.attribute("unParticleRotationSpeedVar"));
	joint.removeAttribute(joint.attribute("unParticleEmitterWidth"));
	joint.removeAttribute(joint.attribute("unParticleEmitterLength"));
	joint.removeAttribute(joint.attribute("unParticleEmitterHeight"));
	joint.removeAttribute(joint.attribute("unParticleBlendMode"));
	//joint.removeAttribute(joint.attribute("unParticleBlendModeSrc"));
	//joint.removeAttribute(joint.attribute("unParticleBlendModeDst"));
	joint.removeAttribute(joint.attribute("unParticleTextureFilename"));
	joint.removeAttribute(joint.attribute("unParticleTextureRows"));
	joint.removeAttribute(joint.attribute("unParticleTextureCols"));
	joint.removeAttribute(joint.attribute("unParticleTextureChangeStyle"));
	joint.removeAttribute(joint.attribute("unParticleTextureChangeInterval"));
	joint.removeAttribute(joint.attribute("unParticleTailLength"));
	joint.removeAttribute(joint.attribute("unParticleTimeMiddle"));
	joint.removeAttribute(joint.attribute("unParticleColorStart"));
	joint.removeAttribute(joint.attribute("unParticleColorMiddle"));
	joint.removeAttribute(joint.attribute("unParticleColorEnd"));
	joint.removeAttribute(joint.attribute("unParticleAlpha"));
	joint.removeAttribute(joint.attribute("unParticleScale"));
	joint.removeAttribute(joint.attribute("unParticleScaleVar"));
	joint.removeAttribute(joint.attribute("unParticleFixedSize"));
	joint.removeAttribute(joint.attribute("unParticleHeadLifeSpan"));
	joint.removeAttribute(joint.attribute("unParticleHeadDecay"));
	joint.removeAttribute(joint.attribute("unParticleTailLifeSpan"));
	joint.removeAttribute(joint.attribute("unParticleTailDecay"));
	joint.removeAttribute(joint.attribute("unParticleHead"));
	joint.removeAttribute(joint.attribute("unParticleTail"));
	joint.removeAttribute(joint.attribute("unParticleUnShaded"));
	joint.removeAttribute(joint.attribute("unParticleUnFogged"));
	joint.removeAttribute(joint.attribute("unParticleBlockByY0"));
}

void removeRibbonAttributes(MFnIkJoint& joint)
{
	joint.removeAttribute(joint.attribute("unRibbonEnabled"));
	joint.removeAttribute(joint.attribute("unRibbonVisible"));
	joint.removeAttribute(joint.attribute("unRibbonAbove"));
	joint.removeAttribute(joint.attribute("unRibbonBelow"));
	joint.removeAttribute(joint.attribute("unRibbonEdgesPerSecond"));
	joint.removeAttribute(joint.attribute("unRibbonEdgeLife"));
	joint.removeAttribute(joint.attribute("unRibbonGravity"));
	joint.removeAttribute(joint.attribute("unRibbonTextureRows"));
	joint.removeAttribute(joint.attribute("unRibbonTextureCols"));
	joint.removeAttribute(joint.attribute("unRibbonTextureSlot"));
	joint.removeAttribute(joint.attribute("unRibbonVertexColor"));
	joint.removeAttribute(joint.attribute("unRibbonVertexAlpha"));
	joint.removeAttribute(joint.attribute("unRibbonBlendMode"));
	//joint.removeAttribute(joint.attribute("unRibbonBlendModeSrc"));
	//joint.removeAttribute(joint.attribute("unRibbonBlendModeDst"));
	joint.removeAttribute(joint.attribute("unRibbonTextureFilename"));
}

void addParticleAttributes(MFnIkJoint& joint)
{
	joint.addAttribute(unCreateParticleSystem::unParticleEnabled);
	joint.addAttribute(unCreateParticleSystem::unParticleVisible);
	joint.addAttribute(unCreateParticleSystem::unParticleSpeed);
	joint.addAttribute(unCreateParticleSystem::unParticleVariation);
	joint.addAttribute(unCreateParticleSystem::unParticleConeAngle);
	joint.addAttribute(unCreateParticleSystem::unParticleGravity);
	joint.addAttribute(unCreateParticleSystem::unParticleExplosiveForce);
	joint.addAttribute(unCreateParticleSystem::unParticleLife);
	joint.addAttribute(unCreateParticleSystem::unParticleLifeVariation);
	joint.addAttribute(unCreateParticleSystem::unParticleEmissionRate);
	joint.addAttribute(unCreateParticleSystem::unParticleInitialNum);
	joint.addAttribute(unCreateParticleSystem::unParticleLimitNum);
	joint.addAttribute(unCreateParticleSystem::unParticleAttachToEmitter);
	joint.addAttribute(unCreateParticleSystem::unParticleMoveWithEmitter);
	joint.addAttribute(unCreateParticleSystem::unParticleForTheSword);
	joint.addAttribute(unCreateParticleSystem::unParticleForTheSwordInitialAngle);
	joint.addAttribute(unCreateParticleSystem::unParticleWander);
	joint.addAttribute(unCreateParticleSystem::unParticleWanderRadius);
	joint.addAttribute(unCreateParticleSystem::unParticleWanderSpeed);
	joint.addAttribute(unCreateParticleSystem::unParticleAspectRatio);
	joint.addAttribute(unCreateParticleSystem::unParticleInitialAngleBegin);
	joint.addAttribute(unCreateParticleSystem::unParticleInitialAngleEnd);
	joint.addAttribute(unCreateParticleSystem::unParticleRotationSpeed);
	joint.addAttribute(unCreateParticleSystem::unParticleRotationSpeedVar);
	joint.addAttribute(unCreateParticleSystem::unParticleEmitterWidth);
	joint.addAttribute(unCreateParticleSystem::unParticleEmitterLength);
	joint.addAttribute(unCreateParticleSystem::unParticleEmitterHeight);
	joint.addAttribute(unCreateParticleSystem::unParticleBlendMode);
	//joint.addAttribute(unCreateParticleSystem::unParticleBlendModeSrc);
	//joint.addAttribute(unCreateParticleSystem::unParticleBlendModeDst);
	joint.addAttribute(unCreateParticleSystem::unParticleTextureFilename);
	joint.addAttribute(unCreateParticleSystem::unParticleTextureRows);
	joint.addAttribute(unCreateParticleSystem::unParticleTextureCols);
	joint.addAttribute(unCreateParticleSystem::unParticleTextureChangeStyle);
	joint.addAttribute(unCreateParticleSystem::unParticleTextureChangeInterval);
	joint.addAttribute(unCreateParticleSystem::unParticleTailLength);
	joint.addAttribute(unCreateParticleSystem::unParticleTimeMiddle);
	joint.addAttribute(unCreateParticleSystem::unParticleColorStart);
	joint.addAttribute(unCreateParticleSystem::unParticleColorMiddle);
	joint.addAttribute(unCreateParticleSystem::unParticleColorEnd);
	joint.addAttribute(unCreateParticleSystem::unParticleAlpha);
	joint.addAttribute(unCreateParticleSystem::unParticleScale);
	joint.addAttribute(unCreateParticleSystem::unParticleScaleVar);
	joint.addAttribute(unCreateParticleSystem::unParticleFixedSize);
	joint.addAttribute(unCreateParticleSystem::unParticleHeadLifeSpan);
	joint.addAttribute(unCreateParticleSystem::unParticleHeadDecay);
	joint.addAttribute(unCreateParticleSystem::unParticleTailLifeSpan);
	joint.addAttribute(unCreateParticleSystem::unParticleTailDecay);
	joint.addAttribute(unCreateParticleSystem::unParticleHead);
	joint.addAttribute(unCreateParticleSystem::unParticleTail);
	joint.addAttribute(unCreateParticleSystem::unParticleUnShaded);
	joint.addAttribute(unCreateParticleSystem::unParticleUnFogged);
	joint.addAttribute(unCreateParticleSystem::unParticleBlockByY0);
}

void addRibbonAttributes(MFnIkJoint& joint)
{
	joint.addAttribute(unCreateRibbonSystem::unRibbonEnabled);
	joint.addAttribute(unCreateRibbonSystem::unRibbonVisible);
	joint.addAttribute(unCreateRibbonSystem::unRibbonAbove);
	joint.addAttribute(unCreateRibbonSystem::unRibbonBelow);
	joint.addAttribute(unCreateRibbonSystem::unRibbonEdgesPerSecond);
	joint.addAttribute(unCreateRibbonSystem::unRibbonEdgeLife);
	joint.addAttribute(unCreateRibbonSystem::unRibbonGravity);
	joint.addAttribute(unCreateRibbonSystem::unRibbonTextureRows);
	joint.addAttribute(unCreateRibbonSystem::unRibbonTextureCols);
	joint.addAttribute(unCreateRibbonSystem::unRibbonTextureSlot);
	joint.addAttribute(unCreateRibbonSystem::unRibbonVertexColor);
	joint.addAttribute(unCreateRibbonSystem::unRibbonVertexAlpha);
	joint.addAttribute(unCreateRibbonSystem::unRibbonBlendMode);
	//joint.addAttribute(unCreateRibbonSystem::unRibbonBlendModeSrc);
	//joint.addAttribute(unCreateRibbonSystem::unRibbonBlendModeDst);
	joint.addAttribute(unCreateRibbonSystem::unRibbonTextureFilename);
}

MStatus unCreateParticleSystem::doIt( const MArgList& args )
{
	bool create = true;

	MSelectionList activeList;
	MStatus stat = MGlobal::getActiveSelectionList(activeList);
	if (MS::kSuccess != stat)
	{
		MessageBox(0,"please select one joint first!",0,0);
		return MS::kFailure;
	}

	if(activeList.length() != 1)
	{
		MessageBox(0,"please select ONLY one joint!",0,0);
		return MS::kFailure;
	}
	MItSelectionList iter(activeList);
	MDagPath dagPath;
	for ( ; !iter.isDone(); iter.next())
	{
		stat = iter.getDagPath(dagPath);
		break;
	}
	if(!stat)
	{
		MessageBox(0,"can't getDagPath!",0,0);
		return MS::kFailure;
	}

	if(!dagPath.hasFn(MFn::kJoint))
	{
		MessageBox(0,"muse select joint node!",0,0);
		return MS::kFailure;
	}

	MFnIkJoint joint(dagPath);

	if(args.length())
		create = args.asBool(0);

	if(!create)
	{
		if(!joint.hasAttribute("unParticleEnabled"))
		{
			MessageBox(0,"no particle system exists!",0,0);
			return MS::kFailure;
		}
		removeParticleAttributes(joint);
		return MS::kSuccess;
	}

	if(joint.hasAttribute("unParticleEnabled"))
	{
		MessageBox(0,"particle system already exists!",0,0);
		return MS::kSuccess;
	}

	if(joint.hasAttribute("unRibbonEnabled"))
	{
		if(MessageBox(0,"will delete ribbon system and attach a particle system to the joint,continue?",0,MB_YESNO) == IDNO)return MS::kFailure;
		removeRibbonAttributes(joint);
	}

	MFnNumericAttribute attr;
	unParticleEnabled = attr.create("unParticleEnabled","Enabled",MFnNumericData::kBoolean,true);
	attr.setStorable(true);

	unParticleVisible = attr.create("unParticleVisible","Visible",MFnNumericData::kBoolean,true);
	attr.setStorable(true);
	attr.setKeyable(true);

	unParticleSpeed = attr.create("unParticleSpeed","Speed",MFnNumericData::kFloat,40.0f);
	attr.setStorable(true);
	attr.setMin(-1000.0f);
	attr.setMax(1000.0f);
	attr.setKeyable(true);

	unParticleVariation = attr.create("unParticleVariationPercent","VariationPercent",MFnNumericData::kFloat,2.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(100.0f);
	attr.setKeyable(true);

	unParticleConeAngle = attr.create("unParticleConeAngle","ConeAngle",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(180.0f);
	attr.setKeyable(true);

	unParticleGravity = attr.create("unParticleGravity","Gravity",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(-1000.0f);
	attr.setMax(1000.0f);
	attr.setKeyable(true);

	unParticleExplosiveForce = attr.create("unParticleExplosiveForce","ExplosiveForce",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(-100.0f);
	attr.setMax(100.0f);
	attr.setKeyable(true);

	unParticleLife = attr.create("unParticleLife","Life",MFnNumericData::kFloat,1.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1000.0f);

	unParticleLifeVariation = attr.create("unParticleLifeVariation","LifeVar",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(-16.0f);
	attr.setMax(16.0f);

	unParticleEmissionRate = attr.create("unParticleEmissionRate","EmissionRate",MFnNumericData::kFloat,50.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1000.0f);
	attr.setKeyable(true);

	//版本号17加入，初始粒子数
	unParticleInitialNum = attr.create("unParticleInitialNum","InitialNum",MFnNumericData	::kShort,0);
	attr.setStorable(true);
	attr.setMin(0);
	attr.setMax(512);

	//版本号11加入，最大粒子数
	unParticleLimitNum = attr.create("unParticleLimitNum","LimitNum",MFnNumericData	::kShort,32);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(512);

	//版本号13加入，粒子全部跟随发射点
	unParticleAttachToEmitter = attr.create("unParticleAttachToEmitter","AttachToEmitter",MFnNumericData::kBoolean,false);
	attr.setStorable(true);

	//版本号18加入，粒子跟随发射点移动
	unParticleMoveWithEmitter = attr.create("unParticleMoveWithEmitter","MoveWithEmitter",MFnNumericData::kBoolean,false);
	attr.setStorable(true);

	//版本号23加入，粒子指向速度方向
	unParticleForTheSword = attr.create("unParticleForTheSword","ForTheSword",MFnNumericData::kBoolean,false);
	attr.setStorable(true);

	//版本号24加入，粒子指向速度方向，提供一个角度供初始化
	unParticleForTheSwordInitialAngle = attr.create("unParticleForTheSwordInitialAngle","ForTheSwordInitialAngle",MFnNumericData::kFloat,0);
	attr.setStorable(true);
	attr.setMin(-180.0f);
	attr.setMax(180.0f);

	//版本号25，Wander
	unParticleWander = attr.create("unParticleWander","Wander",MFnNumericData::kBoolean,false);
	attr.setStorable(true);
	//版本号25，WanderRadius
	unParticleWanderRadius = attr.create("unParticleWanderRadius","WanderRadius",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1024.0f);
	//版本号25，WanderSpeed
	unParticleWanderSpeed = attr.create("unParticleWanderSpeed","WanderSpeed",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(16.0f);

	unParticleAspectRatio = attr.create("unParticleAspectRatio","AspectRatio",MFnNumericData::kFloat,1.0f);
	attr.setStorable(true);
	attr.setMin(0.0625f);
	attr.setMax(16.0f);

	unParticleInitialAngleBegin = attr.create("unParticleInitialAngleBegin","InitialAngleBegin",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(-180.0f);
	attr.setMax(180.0f);

	unParticleInitialAngleEnd = attr.create("unParticleInitialAngleEnd","InitialAngleEnd",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(-180.0f);
	attr.setMax(180.0f);

	unParticleRotationSpeed = attr.create("unParticleRotationSpeed","RotationSpeed",MFnNumericData::kFloat,0);
	attr.setStorable(true);
	attr.setMin(-64.0f);	//原来默认为16，鲍振伟要求改大
	attr.setMax(64.0f);

	unParticleRotationSpeedVar = attr.create("unParticleRotationSpeedVar","RotationSpeedVar",MFnNumericData::kFloat,0);
	attr.setStorable(true);
	attr.setMin(0);
	attr.setMax(64);

	unParticleEmitterWidth = attr.create("unParticleEmitterWidth","EmitterWidth",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1000.0f);
	attr.setKeyable(true);

	unParticleEmitterLength = attr.create("unParticleEmitterLength","EmitterLength",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1000.0f);
	attr.setKeyable(true);

	unParticleEmitterHeight = attr.create("unParticleEmitterHeight","EmitterHeight",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1000.0f);
	attr.setKeyable(true);

	MFnEnumAttribute eAttr;
	unParticleBlendMode = eAttr.create("unParticleBlendMode","BlendMode",3);
	eAttr.setStorable(true);
	eAttr.addField("Opaque",0);
	eAttr.addField("Transparent",1);
	eAttr.addField("AlphaBlend",2);
	eAttr.addField("Additive",3);
	eAttr.addField("AdditiveAlpha",4);
	eAttr.addField("Modulate",5);

	/*unParticleBlendModeSrc = eAttr.create("unParticleBlendModeSrc","BlendModeSrc",3);
	eAttr.setStorable(true);
	eAttr.addField("SBF_ONE",0);
	eAttr.addField("SBF_ZERO",1);
	eAttr.addField("SBF_DEST_COLOR",2);
	eAttr.addField("SBF_SOURCE_COLOR",3);
	eAttr.addField("SBF_ONE_MINUS_DEST_COLOR",4);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_COLOR",5);
	eAttr.addField("SBF_DEST_ALPHA",6);
	eAttr.addField("SBF_SOURCE_ALPHA",7);
	eAttr.addField("SBF_ONE_MINUS_DEST_ALPHA",8);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_ALPHA",9);

	unParticleBlendModeDst = eAttr.create("unParticleBlendModeDst","BlendModeDst",0);
	eAttr.setStorable(true);
	eAttr.addField("SBF_ONE",0);
	eAttr.addField("SBF_ZERO",1);
	eAttr.addField("SBF_DEST_COLOR",2);
	eAttr.addField("SBF_SOURCE_COLOR",3);
	eAttr.addField("SBF_ONE_MINUS_DEST_COLOR",4);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_COLOR",5);
	eAttr.addField("SBF_DEST_ALPHA",6);
	eAttr.addField("SBF_SOURCE_ALPHA",7);
	eAttr.addField("SBF_ONE_MINUS_DEST_ALPHA",8);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_ALPHA",9);*/

	unParticleTextureFilename = attr.createColor("unParticleTextureFilename","TextureFilename");
	attr.setStorable(true);

	unParticleTextureRows = attr.create("unParticleTextureRows","TextureRows",MFnNumericData::kShort,1);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(16);

	unParticleTextureCols = attr.create("unParticleTextureCols","TextureCols",MFnNumericData::kShort,1);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(16);

	unParticleTextureChangeStyle = eAttr.create("unParticleTextureChangeStyle","TextureChangeStyle",0);
	eAttr.setStorable(true);
	eAttr.addField("Sequence",0);
	eAttr.addField("Random",1);

	unParticleTextureChangeInterval = attr.create("unParticleTextureChangeInterval","TextureChangeInterval",MFnNumericData::kShort,30);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(5000);

	unParticleTailLength = attr.create("unParticleTailLength","TailLength",MFnNumericData::kFloat,1.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(10.0f);

	unParticleTimeMiddle = attr.create("unParticleTimeMiddle","TimeMiddle",MFnNumericData::kFloat,0.3f);
	attr.setStorable(true);
	attr.setMin(0.05f);
	attr.setMax(0.95f);

	unParticleColorStart = attr.createColor("unParticleColorStart","ColorStart");
	attr.setStorable(true);
	attr.setDefault(1.0f,1.0f,1.0f);

	unParticleColorMiddle = attr.createColor("unParticleColorMiddle","ColorMiddle");
	attr.setStorable(true);
	attr.setDefault(1.0f,1.0f,1.0f);

	unParticleColorEnd = attr.createColor("unParticleColorEnd","ColorEnd");
	attr.setStorable(true);
	attr.setDefault(1.0f,1.0f,1.0f);

	unParticleAlpha = attr.create("unParticleAlpha","Alpha",MFnNumericData::k3Float);
	attr.setStorable(true);
	attr.setDefault(1.0f,1.0f,0.0f);
	attr.setMin(0.0f,0.0f,0.0f);
	attr.setMax(1.0f,1.0f,1.0f);

	unParticleScale = attr.create("unParticleScale","Scale",MFnNumericData::k3Float);
	attr.setStorable(true);
	attr.setDefault(10.0f,10.0f,10.0f);
	attr.setMin(0.001f,0.001f,0.001f);
	attr.setMax(1500.0f,1500.0f,1500.0f);	//原来是500，鲍振伟要求改大

	unParticleScaleVar = attr.create("unParticleScaleVar","ScaleVar",MFnNumericData::k3Float);
	attr.setStorable(true);
	attr.setDefault(0.0f,0.0f,0.0f);
	attr.setMin(0.0f,0.0f,0.0f);
	attr.setMax(500.0f,500.0f,500.0f);//原来是100，鲍振伟要求改大

	unParticleFixedSize = attr.create("unParticleFixedSize","FixedSize",MFnNumericData::kBoolean,false);
	attr.setStorable(true);

	unParticleHeadLifeSpan = attr.create("unParticleHeadLifeSpan","HeadLifeSpan",MFnNumericData::k3Short);
	attr.setStorable(true);
	attr.setDefault(0,0,1);
	attr.setMin(0,0,1);
	attr.setMax(255,255,255);

	unParticleHeadDecay = attr.create("unParticleHeadDecay","HeadDecay",MFnNumericData::k3Short);
	attr.setStorable(true);
	attr.setDefault(0,0,1);
	attr.setMin(0,0,1);
	attr.setMax(255,255,255);

	unParticleTailLifeSpan = attr.create("unParticleTailLifeSpan","TailLifeSpan",MFnNumericData::k3Short);
	attr.setStorable(true);
	attr.setDefault(0,0,1);
	attr.setMin(0,0,1);
	attr.setMax(255,255,255);

	unParticleTailDecay = attr.create("unParticleTailDecay","TailDecay",MFnNumericData::k3Short);
	attr.setStorable(true);
	attr.setDefault(0,0,1);
	attr.setMin(0,0,1);
	attr.setMax(255,255,255);

	unParticleHead = attr.create("unParticleHead","Head",MFnNumericData::kBoolean,true);
	attr.setStorable(true);

	unParticleTail = attr.create("unParticleTail","Tail",MFnNumericData::kBoolean,false);
	attr.setStorable(true);

	unParticleUnShaded = attr.create("unParticleUnShaded","UnShaded",MFnNumericData::kBoolean,true);
	attr.setStorable(true);

	unParticleUnFogged = attr.create("unParticleUnFogged","UnFogged",MFnNumericData::kBoolean,true);
	attr.setStorable(true);

	unParticleBlockByY0 = attr.create("unParticleBlockByY0","blockByY0",MFnNumericData::kBoolean,false);
	attr.setStorable(true);

	addParticleAttributes(joint);

	return MS::kSuccess;
}

unCreateRibbonSystem::~unCreateRibbonSystem() {}

void* unCreateRibbonSystem::creator()
{
	return new unCreateRibbonSystem();
}

MStatus unCreateRibbonSystem::doIt( const MArgList& args )
{
	bool create = true;

	MSelectionList activeList;
	MStatus stat = MGlobal::getActiveSelectionList(activeList);
	if (MS::kSuccess != stat)
	{
		MessageBox(0,"please select one joint first!",0,0);
		return MS::kFailure;
	}

	if(activeList.length() != 1)
	{
		MessageBox(0,"please select ONLY one joint!",0,0);
		return MS::kFailure;
	}
	MItSelectionList iter(activeList);
	MDagPath dagPath;
	for ( ; !iter.isDone(); iter.next())
	{
		stat = iter.getDagPath(dagPath);
		break;
	}
	if(!stat)
	{
		MessageBox(0,"can't getDagPath!",0,0);
		return MS::kFailure;
	}

	if(!dagPath.hasFn(MFn::kJoint))
	{
		MessageBox(0,"muse select joint node!",0,0);
		return MS::kFailure;
	}

	MFnIkJoint joint(dagPath);

	if(args.length())
		create = args.asBool(0);

	if(!create)
	{
		if(!joint.hasAttribute("unRibbonEnabled"))
		{
			MessageBox(0,"no ribbon system exists!",0,0);
			return MS::kFailure;
		}
		removeRibbonAttributes(joint);
		return MS::kSuccess;
	}

	if(joint.hasAttribute("unRibbonEnabled"))
	{
		MessageBox(0,"ribbon system already exists!",0,0);
		return MS::kSuccess;
	}

	if(joint.hasAttribute("unParticleEnabled"))
	{
		if(MessageBox(0,"will delete particle system and attach a ribbon system to the joint,continue?",0,MB_YESNO) == IDNO)return MS::kFailure;
		removeParticleAttributes(joint);
	}

	MFnNumericAttribute attr;
	unRibbonEnabled = attr.create("unRibbonEnabled","enabled",MFnNumericData::kBoolean,true);
	attr.setStorable(true);
	unRibbonVisible = attr.create("unRibbonVisible","visible",MFnNumericData::kBoolean,true);
	attr.setStorable(true);
	attr.setKeyable(true);
	unRibbonAbove = attr.create("unRibbonAbove","above",MFnNumericData::kFloat,20.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(5000.0f);
	attr.setKeyable(true);
	unRibbonBelow = attr.create("unRibbonBelow","below",MFnNumericData::kFloat,20.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(5000.0f);
	attr.setKeyable(true);
	unRibbonEdgesPerSecond = attr.create("unRibbonEdgesPerSecond","EdgePerSec",MFnNumericData::kShort,10);
	attr.setStorable(true);
	attr.setMin(0);
	attr.setMax(100);
	unRibbonEdgeLife = attr.create("unRibbonEdgeLife","EdgeLife",MFnNumericData::kFloat,2.0f);
	attr.setStorable(true);
	attr.setMin(0.001f);
	attr.setMax(100.0f);
	unRibbonGravity = attr.create("unRibbonGravity","Gravity",MFnNumericData::kFloat,0.0f);
	attr.setStorable(true);
	attr.setMin(-1000.0f);
	attr.setMax(1000.0f);
	unRibbonTextureRows = attr.create("unRibbonTextureRows","TextureRows",MFnNumericData::kShort,1);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(16);
	unRibbonTextureCols = attr.create("unRibbonTextureCols","TextureCols",MFnNumericData::kShort,1);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(16);
	unRibbonTextureSlot = attr.create("unRibbonTextureSlot","TextureSlot",MFnNumericData::kShort,0);
	attr.setStorable(true);
	attr.setMin(0);
	attr.setMax(255);
	attr.setKeyable(true);
	unRibbonVertexColor = attr.create("unRibbonVertexColor","VertexColor",MFnNumericData::k3Float);
	attr.setStorable(true);
	attr.setDefault(1.0f,0.0f,0.0f);
	attr.setUsedAsColor(true);
	attr.setKeyable(true);
	unRibbonVertexAlpha = attr.create("unRibbonVertexAlpha","VertexAlpha",MFnNumericData::kFloat,1.0f);
	attr.setStorable(true);
	attr.setMin(0.0f);
	attr.setMax(1.0f);
	attr.setKeyable(true);
	MFnEnumAttribute eAttr;
	unRibbonBlendMode = eAttr.create("unRibbonBlendMode","BlendMode",3);
	eAttr.setStorable(true);
	eAttr.addField("Opaque",0);
	eAttr.addField("Transparent",1);
	eAttr.addField("AlphaBlend",2);
	eAttr.addField("Additive",3);
	eAttr.addField("AdditiveAlpha",4);
	eAttr.addField("Modulate",5);
	/*unRibbonBlendModeSrc = eAttr.create("unRibbonBlendModeSrc","BlendModeSrc",3);
	eAttr.setStorable(true);
	eAttr.addField("SBF_ONE",0);
	eAttr.addField("SBF_ZERO",1);
	eAttr.addField("SBF_DEST_COLOR",2);
	eAttr.addField("SBF_SOURCE_COLOR",3);
	eAttr.addField("SBF_ONE_MINUS_DEST_COLOR",4);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_COLOR",5);
	eAttr.addField("SBF_DEST_ALPHA",6);
	eAttr.addField("SBF_SOURCE_ALPHA",7);
	eAttr.addField("SBF_ONE_MINUS_DEST_ALPHA",8);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_ALPHA",9);
	unRibbonBlendModeDst = eAttr.create("unRibbonBlendModeDst","BlendModeDst",0);
	eAttr.setStorable(true);
	eAttr.addField("SBF_ONE",0);
	eAttr.addField("SBF_ZERO",1);
	eAttr.addField("SBF_DEST_COLOR",2);
	eAttr.addField("SBF_SOURCE_COLOR",3);
	eAttr.addField("SBF_ONE_MINUS_DEST_COLOR",4);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_COLOR",5);
	eAttr.addField("SBF_DEST_ALPHA",6);
	eAttr.addField("SBF_SOURCE_ALPHA",7);
	eAttr.addField("SBF_ONE_MINUS_DEST_ALPHA",8);
	eAttr.addField("SBF_ONE_MINUS_SOURCE_ALPHA",9);*/
	unRibbonTextureFilename = attr.createColor("unRibbonTextureFilename","TextureFilename");
	attr.setStorable(true);

	addRibbonAttributes(joint);

	return MS::kSuccess;
}

#include "ShellApi.h"
//#include "bindPoseTool.h"

MStatus preview(const MArgList& args,bool useVertexColor)
{
	//单实例先清除
	if(MaterialSet::getSingletonPtr())
		MaterialSet::getSingletonPtr()->clear();

	char tempPath[MAX_PATH];
	GetTempPath(MAX_PATH, tempPath);
	std::string tempFileName(tempPath);
	ExportOptions::instance().m_outFilePath = tempFileName.c_str();
	tempFileName += "\\";
	tempFileName += _T("maya.mz");

	ExportOptions::instance().m_outFile = tempFileName.c_str();

	ExportOptions::instance().clipList.clear();
	MTime kTimeMin   = MAnimControl::animationStartTime();		//整个场景的起始帧
	MTime kTimeMax   = MAnimControl::animationEndTime();		//整个场景的结束帧

	clipInfo clip;
	clip.name = "Animation";
	clip.startFrame = (int)kTimeMin.value();
	clip.endFrame = (int)kTimeMax.value();
	clip.stepFrame = 1;
	ExportOptions::instance().clipList.push_back(clip);
	ExportOptions::instance().exportAnims = true;
	ExportOptions::instance().exportVertexColour = useVertexColor;

	/*BindPoseTool bindPoseTool;
	bindPoseTool.GoIntoBindPose();*/

	MWriter writer;
	writer.read();
	MStatus status = writer.write();

#ifdef RELEASEDEBUG
#define DLL_NAME "MayaPreview_rd.exe"
#elif _DEBUG
#define DLL_NAME "MayaPreview_d.exe"
#else
#define DLL_NAME "MayaPreview.exe"
#endif

	if(status == MS::kSuccess)
	{
		HWND hWnd = FindWindowEx(0,0,0,"MayaPreview");
		//if(hWnd)
		//{
		//	SendMessage(hWnd,WM_CLOSE,0,0);
		//	hWnd = 0;
		//}
		if(!hWnd)
		{
			static const std::string tMaxProgramName("Maya.exe");
			char path[257];
			GetModuleFileName(GetModuleHandle(tMaxProgramName.c_str()),path,256);
			std::string parentPath(path);
			parentPath.erase(parentPath.size() - tMaxProgramName.size(), tMaxProgramName.size());
			std::string previewProgramPath(parentPath + "preview\\" + DLL_NAME); 
			
			if(!ShellExecute(0,"open",previewProgramPath.c_str(),"","",SW_SHOW))
			{

				MessageBox(0,previewProgramPath.c_str(),"Can't Find MayaPreview Program",0);
				return MS::kFailure;
			}			
			hWnd = FindWindowEx(0,0,0,"MayaPreview");
			DWORD tick = GetTickCount();
			while(!hWnd)
			{
				DWORD tickNow = GetTickCount();
				if(tickNow - tick > 3000)break;
				Sleep(1);
				hWnd = FindWindowEx(0,0,0,"MayaPreview");
			}
		}
		if(hWnd)
		{
			SendMessage(hWnd,WM_USER + 128,0,0);
			SetActiveWindow(hWnd);
			SetForegroundWindow(hWnd);
			BringWindowToTop(hWnd);
		}
	}
	/*bindPoseTool	.UndoGoIntoBindPose();*/

	return MS::kSuccess;
}
////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////
class unPreviewWithVertexColor : public MPxCommand
{
public:
	unPreviewWithVertexColor() {};
	virtual			~unPreviewWithVertexColor(); 

	MStatus			doIt( const MArgList& args );
	static void*	creator();
};

unPreviewWithVertexColor::~unPreviewWithVertexColor() 
{
}

void* unPreviewWithVertexColor::creator()
{
	return new unPreviewWithVertexColor();
}

MStatus unPreviewWithVertexColor::doIt( const MArgList& args )
{
	return preview(args,true);
}

////////////////////////////////////////////////////////////////
class unPreview : public MPxCommand
{
public:
	unPreview() {};
	virtual			~unPreview(); 

	MStatus			doIt( const MArgList& args );
	static void*	creator();
};

unPreview::~unPreview() 
{
}

void* unPreview::creator()
{
	return new unPreview();
}

MStatus unPreview::doIt( const MArgList& args )
{
	return preview(args,false);
}
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
class unShowAvailableSystems : public MPxCommand
{
public:
	unShowAvailableSystems() {};
	virtual			~unShowAvailableSystems(); 

	MStatus findAvailableSystems(std::string& str,MDagPath& dagPath);

	MStatus			doIt( const MArgList& args );
	static void*	creator();
};

unShowAvailableSystems::~unShowAvailableSystems() 
{
}

void* unShowAvailableSystems::creator()
{
	return new unShowAvailableSystems();
}


MStatus unShowAvailableSystems::findAvailableSystems(std::string& str,MDagPath& dagPath)
{
	MStatus stat = MS::kSuccess;

	if(dagPath.hasFn(MFn::kJoint))
	{
		MFnIkJoint jointFn(dagPath);
		std::string name = dagPath.partialPathName().asChar();
		MPlug plug = jointFn.findPlug("unRibbonEnabled");
		if(!plug.isNull())
		{
			bool enabled;
			plug.getValue(enabled);
			char s[256];
			sprintf(s,"%s RibbonSystem %s\n",name.c_str(),enabled ? "True" : "False");
			str += s;
		}
		plug = jointFn.findPlug("unParticleEnabled");
		if(!plug.isNull())
		{
			bool enabled;
			plug.getValue(enabled);
			char s[256];
			sprintf(s,"%s ParticleSystem %s\n",name.c_str(),enabled ? "True" : "False");
			str += s;
		}
	}

	for (unsigned int i = 0; i < dagPath.childCount(); i++)
	{
		MObject child = dagPath.child(i);
		MDagPath childPath;
		stat = MDagPath::getAPathTo(child,childPath);
		if (MS::kSuccess != stat)
		{
			return MS::kFailure;
		}
		stat = findAvailableSystems(str,childPath);
		if (MS::kSuccess != stat)
			return MS::kFailure;
	}
	return MS::kSuccess;
}

MStatus unShowAvailableSystems::doIt( const MArgList& args )
{
	MItDag dagIter;
	MFnDagNode worldDag (dagIter.root());
	MDagPath worldPath;
	worldDag.getPath(worldPath);

	std::string str;
	findAvailableSystems(str,worldPath);
	if(!str.empty())
	{
		MessageBox(0,str.c_str(),"Particle or Ribbon Systems",0);
	}
	else
	{
		MessageBox(0,"no particle or ribbon system yet","Particle or Ribbon Systems",0);
	}

	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////
class unRefreshParticleSystem : public MPxCommand
{
public:
	unRefreshParticleSystem() {};
	virtual			~unRefreshParticleSystem(); 

	MStatus			doIt( const MArgList& args );
	static void*	creator();
};

unRefreshParticleSystem::~unRefreshParticleSystem() 
{
}

void* unRefreshParticleSystem::creator()
{
	return new unRefreshParticleSystem();
}

MStatus unRefreshParticleSystem::doIt( const MArgList& args )
{
	MessageBox(0,"尚未实现此功能","提示",MB_OK);
	return MS::kSuccess;
}

////////////////////////////////////////////////////////////////
class unRefreshRibbonSystem : public MPxCommand
{
public:
	unRefreshRibbonSystem() {};
	virtual			~unRefreshRibbonSystem(); 

	MStatus			doIt( const MArgList& args );
	static void*	creator();
};

unRefreshRibbonSystem::~unRefreshRibbonSystem() 
{
}

void* unRefreshRibbonSystem::creator()
{
	return new unRefreshRibbonSystem();
}

MStatus unRefreshRibbonSystem::doIt( const MArgList& args )
{
	MessageBox(0,"尚未实现此功能","提示",MB_OK);
	return MS::kSuccess;
}

extern "C" __declspec(dllexport) MStatus	initializePlugin(MObject obj)
{
	const char* const vendor = "Unnamed Network（深圳市未名网络技术有限公司）";
	const char* const version = "7.0";
	const char* const requiredApiVersion = "Any";
	MFnPlugin plugin(obj, vendor, version, requiredApiVersion);

	// register the translator
	const char* const pixmapName = "none";
	const char* const ExportOptionsScript = "";/*MMFileExportOptions*/
	const char* const defaultExportOptions = "";
	MStatus stat = plugin.registerFileTranslator(MZ_STRING,	// name
		(char*)pixmapName,										// icon
		translator::creator,	
		(char*)ExportOptionsScript,									// script
		(char*)defaultExportOptions);
	if(!stat)
	{
		std::cout<<"Failed in loading the plugin of Unnamed Network."<<endl;
		std::cout<<"Copyright (C) 2007-2009 Unnamed Network. All Rights Reserved."<<endl;
	}
	else
	{
		std::cout<<"Succeeded in loading the plugin of Unnamed Network."<<endl;
		std::cout<<"Copyright (C) 2007-2009 Unnamed Network. All Rights Reserved."<<endl;
	}

	//注册非编导出转换器
	stat = plugin.registerFileTranslator(FB_STRING,"none",FBTranslator::creator,"","");
	if(!stat)
	{
		std::cout<<"Failed in loading the plugin of Unnamed Network."<<endl;
		std::cout<<"Copyright (C) 2007-2009 Unnamed Network. All Rights Reserved."<<endl;
	}
	else
	{
		std::cout<<"Succeeded in loading the plugin of Unnamed Network."<<endl;
		std::cout<<"Copyright (C) 2007-2009 Unnamed Network. All Rights Reserved."<<endl;
	}

	if(stat)stat = plugin.registerCommand( "unCreateParticleSystem", unCreateParticleSystem::creator );
	if(stat)stat = plugin.registerCommand( "unCreateRibbonSystem",unCreateRibbonSystem::creator );
	if(stat)stat = plugin.registerCommand( "unPreview",unPreview::creator );
	if(stat)stat = plugin.registerCommand( "unPreviewWithVertexColor",unPreviewWithVertexColor::creator);
	if(stat)stat = plugin.registerCommand( "unShowAvailableSystems",unShowAvailableSystems::creator );
	if(stat)stat = plugin.registerCommand( "unRefreshParticleSystem",unRefreshParticleSystem::creator );
	if(stat)stat = plugin.registerCommand( "unRefreshRibbonSystem",unRefreshRibbonSystem::creator );

	MGlobal::executeCommand("menu -l \"unFX\" -p \"MayaWindow\" unMenu;");

	MGlobal::executeCommand("menuItem -l \"Create Particle System\" -c \"unCreateParticleSystem\" unMenuParticleSystem;");
	MGlobal::executeCommand("menuItem -l \"Create Ribbon System\" -c \"unCreateRibbonSystem\" unMenuRibbonSystem;");

	MGlobal::executeCommand("menuItem -l \"Remove Particle System\" -c \"unCreateParticleSystem 0\" unMenuParticleSystemRemove;");
	MGlobal::executeCommand("menuItem -l \"Remove Ribbon System\" -c \"unCreateRibbonSystem 0\" unMenuRibbonSystemRemove;");

	MGlobal::executeCommand("menuItem -l \"Preview\" -c \"unPreview\" unMenuPreview;");
	MGlobal::executeCommand("menuItem -l \"Preview(Vertex Color,unlit+AlphaBlend\" -c \"unPreviewWithVertexColor\" unMenuPreviewWithVertexColor;");
	MGlobal::executeCommand("menuItem -l \"Show Available Systems\" -c \"unShowAvailableSystems\" unMenuShowAvailableSystems;");

	MGlobal::executeCommand("menuItem -l \"Refresh Particle System\" -c \"unRefreshParticleSystem\" unRefreshParticleSystem;");
	MGlobal::executeCommand("menuItem -l \"Refresh Ribbon System\" -c \"unRefreshRibbonSystem\" unRefreshRibbonSystem;");

	return stat;
};


extern "C" __declspec(dllexport) MStatus	uninitializePlugin(MObject obj) 
{
	MFnPlugin plugin( obj );
	MStatus stat = plugin.deregisterCommand("unCreateParticleSystem");
	stat = plugin.deregisterCommand("unCreateRibbonSystem");
	stat = plugin.deregisterCommand("unPreview");
	stat = plugin.deregisterCommand("unPreviewWithVertexColor");
	stat = plugin.deregisterFileTranslator(MZ_STRING);
	stat = plugin.deregisterFileTranslator(FB_STRING);
	if(!stat)
	{
		std::cout<<"Failed in unloading the plugin of Unnamed Network."<<endl;
		std::cout<<"MayaMzExport v 1.0    "<<endl;
		std::cout<<"Author: unnamed"<<endl;
		std::cout<<"Copyright (C) 2007-2009 Unnamed Network. All Rights Reserved."<<endl;
	}
	else
	{
		std::cout<<"Succeeded in unloading the plugin of Unnamed Network."<<endl;
		std::cout<<"MayaMzExport v 1.0    "<<endl;
		std::cout<<"Author: unnamed"<<endl;
		std::cout<<"Copyright (C) 2007-2009 Unnamed Network. All Rights Reserved.(c)  2007-2009"<<endl;
	}
	return stat;
}