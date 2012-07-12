/********************************************************************
**	file: 		MaxMaterial.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-15
**	purpose:	
*********************************************************************/
#ifndef __MaxMaterial_h__
#define __MaxMaterial_h__

#include "MaxCommon.h"
#include "IMaxMaterial.h"

class MaxMaterial :  public IMaxMaterial
{
public:
	// Parameter block
	IParamBlock2	*_pblock;	//ref 0
	//
	Texmap*			_texMap;	//ref 1
	//
	//Mtl*			_mtl;
	//
	BOOL			_mapOn;
	Interval		_ivalid;
	//
	bool			_isTwoSides;
	bool			_isTcFlowU;
	bool			_isTcFlowV;
	bool			_isTcRotate;
	bool			_isTcAngleReset;
	bool			_isTcPositionReset;
	bool			_dontCastShadow;
	bool			_unLit;
	bool			_noDepthCheck;
	bool			_noDepthWrite;
	bool			_caching;
	bool			_additiveLayer;
	//
	float			_tcFlowSpeedU;
	float			_tcFlowSpeedV;
	float			_tcRotateSpeed;
	float			_unnamedTransparency;
	float			_translucenceCoeff;
	float			_diffuseReflectivity;
	float			_textureChangeInterval;
	//
	int				_textureRows;
	int				_textureCols;
	//
	eBlendMode			_blendMode;
	eTexturedChannel	_textureChannel;
	eTextureChangeStyle	_textureChangeStyle;
	eTextureResolution	_textureResolution;
	eTextureFilter		_textureFilter;
	eNodeState			_nodeState;
	//
	//RGBA				_color;
	Point3				_color;
	Point3				_incandescence;
	//
	void invalidateUI(ParamID pid);
	void clearVariables();
	ParamDlg *CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp);
	void Update(TimeValue t, Interval& valid);
	Interval Validity(TimeValue t);
	void Reset();

	void NotifyChanged();

	// From MtlBase and Mtl
	void SetAmbient(Color c, TimeValue t);		
	void SetDiffuse(Color c, TimeValue t);		
	void SetSpecular(Color c, TimeValue t);
	void SetShininess(float v, TimeValue t);
	Color GetAmbient(int mtlNum=0, BOOL backFace=FALSE);
	Color GetDiffuse(int mtlNum=0, BOOL backFace=FALSE);
	Color GetSpecular(int mtlNum=0, BOOL backFace=FALSE);
	float GetXParency(int mtlNum=0, BOOL backFace=FALSE);
	float GetShininess(int mtlNum=0, BOOL backFace=FALSE);		
	float GetShinStr(int mtlNum=0, BOOL backFace=FALSE);
	float WireSize(int mtlNum=0, BOOL backFace=FALSE);
//	ULONG LocalRequirements(int subMtlNum);

	// Shade and displacement calculation
	BOOL  SupportsShaders();
	void Shade(ShadeContext& sc);
	bool IsOutputConst( ShadeContext& sc, int stdID );
	bool EvalColorStdChannel(ShadeContext& sc, int stdID, Color& outClr);
	bool EvalMonoStdChannel(ShadeContext& sc, int stdID, float& outVal);
	float EvalDisplacement(ShadeContext& sc); 
	Interval DisplacementValidity(TimeValue t); 	

	// SubMaterial access methods
	int NumSubMtls();
	Mtl* GetSubMtl(int i);
	void SetSubMtl(int i, Mtl *m);
	TSTR GetSubMtlSlotName(int i);
	TSTR GetSubMtlTVName(int i);

	// SubTexmap access methods
	int NumSubTexmaps();
	Texmap* GetSubTexmap(int i);
	void SetSubTexmap(int i, Texmap *m);
	TSTR GetSubTexmapSlotName(int i);
	TSTR GetSubTexmapTVName(int i);

	BOOL SetDlgThing(ParamDlg* dlg);
	MaxMaterial(BOOL loading);


	// Loading/Saving
	IOResult Load(ILoad *iload);
	IOResult Save(ISave *isave);

	//From Animatable
	Class_ID ClassID();
	SClass_ID SuperClassID();
	void GetClassName(TSTR& s);

	RefTargetHandle Clone( RemapDir &remap );
	RefResult NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget, PartID& partID,  RefMessage message);


	int NumSubs();
	Animatable* SubAnim(int i); 
	TSTR SubAnimName(int i);
	int SubNumToRefNum(int subNum);

	// TODO: Maintain the number or references here 
	int NumRefs();
	RefTargetHandle GetReference(int i);
	void SetReference(int i, RefTargetHandle rtarg);



	int	NumParamBlocks();
	IParamBlock2* GetParamBlock(int i);
	IParamBlock2* GetParamBlockByID(BlockID id);

	void DeleteThis();

	//
	BOOL SupportsRenderElements();
	ReshadeRequirements GetReshadeRequirements();
	void PreShade(ShadeContext& sc, IReshadeFragment* pFrag);
	void PostShade(ShadeContext& sc, IReshadeFragment* pFrag, int& nextTexIndex, IllumParams* ip);

	//
	void* GetInterface(ULONG id);
	//
	Sampler* GetPixelSampler(int mtlNum, BOOL backFace );
	//
	void SetMtlOn(int i, BOOL onOff);
	//
public:
	virtual bool isTwoSided();
	//
	virtual bool isTcFlowU();
	//
	virtual bool isTcFlowV();
	//
	virtual bool isTcRotate();
	//
	virtual bool isTcAngleReset();
	//
	virtual bool isTcPositionReset();
	//
	virtual float getTcFlowSpeedU(TimeValue time);
	//
	virtual float getTcFlowSpeedV(TimeValue time);
	//
	virtual float getTcRotateSpeed(TimeValue time);
	//
	virtual float getTransparency(TimeValue time);
	//
	virtual Point3 getColor(TimeValue time);
	//
	virtual int getRows();
	//
	virtual int getCols();
	//
	virtual int getChangeStyle(TimeValue time);
	//
	virtual int getChangeInterval();
	//
	virtual bool dontCastShadow();
	//
	virtual bool isUnlit();
	//
	virtual bool isNoDepthWrite();
	//
	virtual bool isNoDepthCheck();
	//
	virtual int isAdditivelayer();
	//
	virtual unsigned char getBlendMode();
};


#endif // __MaxMaterial_h__
