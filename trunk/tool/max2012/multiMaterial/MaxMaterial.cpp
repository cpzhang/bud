#include "MaxMaterial.h"
#include "MaxMaterialClassDesc.h"
#include "MaxParamBlockDesc2s.h"

MaxMaterial::MaxMaterial(BOOL loading) 
{
	clearVariables();
	_pblock = NULL;

	if (!loading) 
		Reset();
}


void MaxMaterial::Reset() 
{
	GetMaxMaterialDesc()->Reset(this, TRUE);
	_ivalid.SetEmpty();
	GetMaxMaterialDesc()->MakeAutoParamBlocks(this);
	DeleteReference(eMaxMaterialRef_Texmap);
// 	DeleteReference(eMaxMaterialRef_Mtl);
// 	ReplaceReference(eMaxMaterialRef_Mtl, NewDefaultStdMat());
// 	GetCOREInterface()->AssignNewName(_mtl);
}

ParamDlg* MaxMaterial::CreateParamDlg(HWND hwMtlEdit, IMtlParams *imp) 
{
	IAutoMParamDlg* masterDlg = GetMaxMaterialDesc()->CreateParamDlgs(hwMtlEdit, imp, this);
	
	// TODO: Set param block user dialog if necessary
	return masterDlg;
}

BOOL MaxMaterial::SetDlgThing(ParamDlg* dlg)
{
	return FALSE;
}

Interval MaxMaterial::Validity(TimeValue t)
{
	Interval valid = FOREVER;		
	if (_texMap)
	{
		valid &= _texMap->Validity(t);
	}
// 	if (_mtl)
// 	{
// 		valid &= _mtl->Validity(t);
// 	}
	return valid;
}

/*===========================================================================*\
 |	Subanim & References support
\*===========================================================================*/

RefTargetHandle MaxMaterial::GetReference(int i) 
{
	switch(i)
	{
	case eMaxMaterialRef_IParamBlock2:
		{
			return _pblock;
		}break;
	case eMaxMaterialRef_Texmap:
		{
			return _texMap;
		}break;
// 	case eMaxMaterialRef_Mtl:
// 		{
// 			return _mtl;
// 		}break;
	}

	return NULL;
}

void MaxMaterial::SetReference(int i, RefTargetHandle rtarg) 
{
	switch(i)
	{
	case eMaxMaterialRef_IParamBlock2:
		{
			_pblock = (IParamBlock2 *)rtarg;
		}break;
	case eMaxMaterialRef_Texmap:
		{
			_texMap = (Texmap *)rtarg;
		}break;
// 	case eMaxMaterialRef_Mtl:
// 		{
// 			_mtl = (Mtl*)rtarg;
// 		}break;
	}
}

TSTR MaxMaterial::SubAnimName(int i) 
{
	switch(i)
	{
	case eMaxMaterialAnimation_IParamBlock2:
		{
			return "params";
		}break;
	case eMaxMaterialAnimation_Texmap:
		{
			return GetSubTexmapTVName(0);
		}break;
// 	case eMaxMaterialAnimation_Mtl:
// 		{
// 			return GetSubMtlSlotName(0);
// 		}break;
	}

	return _T("");
}

Animatable* MaxMaterial::SubAnim(int i) 
{
	switch(i)
	{
	case eMaxMaterialAnimation_IParamBlock2:
		{
			return _pblock;
		}break;
	case eMaxMaterialAnimation_Texmap:
		{
			return _texMap;
		}break;

// 	case eMaxMaterialAnimation_Mtl:
// 		{
// 			return _mtl;
// 		}break;
	}
	return NULL;
}

RefResult MaxMaterial::NotifyRefChanged(Interval changeInt, RefTargetHandle hTarget,PartID& partID, RefMessage message ) 
{
	switch (message) 
	{
		case REFMSG_CHANGE:
			_ivalid.SetEmpty();
			if (hTarget == _pblock)
			{
				ParamID changing_param = _pblock->LastNotifyParamID();
				tParamBlockDesc2.InvalidateUI(changing_param);
			}
			break;
	}
	return REF_SUCCEED;
}

/*===========================================================================*\
 |	SubMtl get and set
\*===========================================================================*/
Mtl* MaxMaterial::GetSubMtl(int i)
{
	//return _mtl;
	return NULL;
}

void MaxMaterial::SetSubMtl(int i, Mtl *m)
{
	// TODO: Set the material and update the UI	
// 	tParamBlockDesc2.InvalidateUI(eParamID_Mtl);
// 	ReplaceReference(eMaxMaterialRef_Mtl, m);
 	_ivalid.SetEmpty();
}

TSTR MaxMaterial::GetSubMtlSlotName(int i)
{
	// Return i'th sub-material name 
	//return _T("SubMaterial"); 
	return _T("");
}

TSTR MaxMaterial::GetSubMtlTVName(int i)
{
	return GetSubMtlSlotName(i);
}

/*===========================================================================*\
 |	Texmap get and set
 |  By default, we support none
\*===========================================================================*/

Texmap* MaxMaterial::GetSubTexmap(int i)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetSubTexmap(ID_DI);
// 	}
	return _texMap;
}

void MaxMaterial::SetSubTexmap(int i, Texmap *m)
{
	ReplaceReference(eMaxMaterialRef_Texmap, m);
	tParamBlockDesc2.InvalidateUI(eParamID_Color);
	_ivalid.SetEmpty();
}

TSTR MaxMaterial::GetSubTexmapSlotName(int i)
{
	return _T("Bitmap");
}

TSTR MaxMaterial::GetSubTexmapTVName(int i)
{
	// Return i'th sub-texture name 
	return GetSubTexmapSlotName(i);
}

/*===========================================================================*\
 |	Standard IO
\*===========================================================================*/

#define MTL_HDR_CHUNK 0x4000
#define MTL_DIY_CHUNK 0x4001
IOResult MaxMaterial::Save(ISave *isave) 
{ 
	IOResult res;
	isave->BeginChunk(MTL_HDR_CHUNK);
	res = MtlBase::Save(isave);
	if (res!=IO_OK) return res;
	isave->EndChunk();

	isave->BeginChunk(MTL_DIY_CHUNK);
	ulong nwrite = sizeof(bool);
	//
	isave->Write(&_isTwoSides, nwrite, &nwrite);
	isave->Write(&_isTcFlowU, nwrite, &nwrite);
	isave->Write(&_isTcFlowV, nwrite, &nwrite);
	isave->Write(&_isTcRotate, nwrite, &nwrite);
	isave->Write(&_isTcAngleReset, nwrite, &nwrite);
	isave->Write(&_isTcPositionReset, nwrite, &nwrite);
	isave->Write(&_dontCastShadow, nwrite, &nwrite);
	isave->Write(&_unLit, nwrite, &nwrite);
	isave->Write(&_noDepthCheck, nwrite, &nwrite);
	isave->Write(&_noDepthWrite, nwrite, &nwrite);
	isave->Write(&_caching, nwrite, &nwrite);
	isave->Write(&_additiveLayer, nwrite, &nwrite);
	//
	nwrite = sizeof(float);
	isave->Write(&_tcFlowSpeedU, nwrite, &nwrite);
	isave->Write(&_tcFlowSpeedV, nwrite, &nwrite);
	isave->Write(&_tcRotateSpeed, nwrite, &nwrite);
	isave->Write(&_unnamedTransparency, nwrite, &nwrite);
	isave->Write(&_translucenceCoeff, nwrite, &nwrite);
	isave->Write(&_diffuseReflectivity, nwrite, &nwrite);
	isave->Write(&_textureChangeInterval, nwrite, &nwrite);
	//
	nwrite = sizeof(int);
	isave->Write(&_textureRows, nwrite, &nwrite);
	isave->Write(&_textureCols, nwrite, &nwrite);
	//
	nwrite = sizeof(uchar);
	isave->Write(&_blendMode, nwrite, &nwrite);
	isave->Write(&_textureChannel, nwrite, &nwrite);
	isave->Write(&_textureChangeStyle, nwrite, &nwrite);
	isave->Write(&_textureResolution, nwrite, &nwrite);
	isave->Write(&_textureFilter, nwrite, &nwrite);
	isave->Write(&_nodeState, nwrite, &nwrite);
	//
	nwrite = sizeof(Point3);
	isave->Write(&_color, nwrite, &nwrite);
	isave->Write(&_incandescence, nwrite, &nwrite);

	isave->EndChunk();

	return IO_OK;
}	

IOResult MaxMaterial::Load(ILoad *iload)
{ 
	IOResult res;
	int id;
	while (IO_OK==(res=iload->OpenChunk())) 
	{
		switch(id = iload->CurChunkID())
		{
			case MTL_HDR_CHUNK:
				res = MtlBase::Load(iload);
				break;
			case MTL_DIY_CHUNK:
				{
					ulong nwrite = sizeof(bool);
					//
					iload->Read(&_isTwoSides, nwrite, &nwrite);
					iload->Read(&_isTcFlowU, nwrite, &nwrite);
					iload->Read(&_isTcFlowV, nwrite, &nwrite);
					iload->Read(&_isTcRotate, nwrite, &nwrite);
					iload->Read(&_isTcAngleReset, nwrite, &nwrite);
					iload->Read(&_isTcPositionReset, nwrite, &nwrite);
					iload->Read(&_dontCastShadow, nwrite, &nwrite);
					iload->Read(&_unLit, nwrite, &nwrite);
					iload->Read(&_noDepthCheck, nwrite, &nwrite);
					iload->Read(&_noDepthWrite, nwrite, &nwrite);
					iload->Read(&_caching, nwrite, &nwrite);
					iload->Read(&_additiveLayer, nwrite, &nwrite);
					//
					nwrite = sizeof(float);
					iload->Read(&_tcFlowSpeedU, nwrite, &nwrite);
					iload->Read(&_tcFlowSpeedV, nwrite, &nwrite);
					iload->Read(&_tcRotateSpeed, nwrite, &nwrite);
					iload->Read(&_unnamedTransparency, nwrite, &nwrite);
					iload->Read(&_translucenceCoeff, nwrite, &nwrite);
					iload->Read(&_diffuseReflectivity, nwrite, &nwrite);
					iload->Read(&_textureChangeInterval, nwrite, &nwrite);
					//
					nwrite = sizeof(int);
					iload->Read(&_textureRows, nwrite, &nwrite);
					iload->Read(&_textureCols, nwrite, &nwrite);
					//
					nwrite = sizeof(uchar);
					iload->Read(&_blendMode, nwrite, &nwrite);
					iload->Read(&_textureChannel, nwrite, &nwrite);
					iload->Read(&_textureChangeStyle, nwrite, &nwrite);
					iload->Read(&_textureResolution, nwrite, &nwrite);
					iload->Read(&_textureFilter, nwrite, &nwrite);
					iload->Read(&_nodeState, nwrite, &nwrite);
					//
					nwrite = sizeof(Point3);
					iload->Read(&_color, nwrite, &nwrite);
					iload->Read(&_incandescence, nwrite, &nwrite);
				}break;
		}
		iload->CloseChunk();
		if (res!=IO_OK) 
			return res;
	}

	return IO_OK;
	}


/*===========================================================================*\
 |	Updating and cloning
\*===========================================================================*/

RefTargetHandle MaxMaterial::Clone(RemapDir &remap) 
{
	MaxMaterial *mnew = new MaxMaterial(FALSE);
	*((MtlBase*)mnew) = *((MtlBase*)this); 
	mnew->ReplaceReference(eBlockID_UnnamedTextureAttibutes, remap.CloneRef(_pblock));

	mnew->_ivalid.SetEmpty();	
	mnew->_blendMode = _blendMode;
	mnew->_isTcAngleReset = _isTcAngleReset;
	mnew->_isTcFlowU = _isTcFlowU;
	mnew->_isTcFlowV = _isTcFlowV;
	mnew->_isTcPositionReset = _isTcPositionReset;
	mnew->_isTcRotate = _isTcRotate;
	mnew->_isTwoSides = _isTwoSides;
	mnew->_tcFlowSpeedU = _tcFlowSpeedU;
	mnew->_tcFlowSpeedV = _tcFlowSpeedV;
	mnew->_tcRotateSpeed = _tcRotateSpeed;
	mnew->_mapOn = _mapOn;
	mnew->_additiveLayer = _additiveLayer;
	mnew->_caching = _caching;
	mnew->_color = _color;
	mnew->_unnamedTransparency = _unnamedTransparency;
	mnew->_diffuseReflectivity = _diffuseReflectivity;
	mnew->_dontCastShadow = _dontCastShadow;
	mnew->_incandescence = _incandescence;

	if (_texMap)
	{
		mnew->ReplaceReference(eMaxMaterialRef_Texmap, remap.CloneRef(_texMap));
	}
// 	if (_mtl)
// 	{
// 		mnew->ReplaceReference(eMaxMaterialRef_Mtl, remap.CloneRef(_mtl));
// 	}
	BaseClone(this, mnew, remap);
	return (RefTargetHandle)mnew;
}

void MaxMaterial::NotifyChanged() 
{
	NotifyDependents(FOREVER, PART_ALL, REFMSG_CHANGE);
}

void MaxMaterial::Update(TimeValue t, Interval& valid) 
{	
	_ivalid = FOREVER;
	valid &= _ivalid;
// 	if (_mtl)
// 	{
// 		_mtl->Update(t, valid);
// 	}
}

/*===========================================================================*\
 |	Determine the characteristics of the material
\*===========================================================================*/

void MaxMaterial::SetAmbient(Color c, TimeValue t) {}		
void MaxMaterial::SetDiffuse(Color c, TimeValue t) {}		
void MaxMaterial::SetSpecular(Color c, TimeValue t) {}
void MaxMaterial::SetShininess(float v, TimeValue t) {}
				
Color MaxMaterial::GetAmbient(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetAmbient(mtlNum, backFace);
// 	}
	return Color(0,0,0);
}

Color MaxMaterial::GetDiffuse(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetDiffuse(mtlNum, backFace);
// 	}
	return Color(0,0,0);
}

Color MaxMaterial::GetSpecular(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetSpecular(mtlNum, backFace);
// 	}
	return Color(0,0,0);
}

float MaxMaterial::GetXParency(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetXParency(mtlNum, backFace);
// 	}
	return 0.0f;
}

float MaxMaterial::GetShininess(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetShininess(mtlNum, backFace);
// 	}
	return 0.0f;
}

float MaxMaterial::GetShinStr(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetShinStr(mtlNum, backFace);
// 	}
	return 0.0f;
}

float MaxMaterial::WireSize(int mtlNum, BOOL backFace)
{
// 	if (_mtl)
// 	{
// 		return _mtl->WireSize(mtlNum, backFace);
// 	}
	return 0.0f;
}

float MaxMaterial::EvalDisplacement(ShadeContext& sc)
{
	return 0.0f;
}

Interval MaxMaterial::DisplacementValidity(TimeValue t)
{
	Interval iv; 
	iv.SetInfinite();

	return iv;	
}

void MaxMaterial::clearVariables()
{
	//
	_isTwoSides = true;
	_isTcFlowU = false;
	_isTcFlowV = false;
	_isTcRotate = false;
	_isTcAngleReset = false;
	_isTcPositionReset = false;
	_dontCastShadow = false;
	_unLit = false;
	_noDepthCheck = false;
	_noDepthWrite = false;
	_caching = false;
	_additiveLayer = false;
	//
	_tcFlowSpeedU = 0.0f;
	_tcFlowSpeedV = 0.0f;
	_tcRotateSpeed = 0.0f;
	_unnamedTransparency = 1.0f;
	_translucenceCoeff = 0.0f;
	_diffuseReflectivity = 0.0f;
	_textureChangeInterval = 0.0f;
	//
	_textureRows = 1;
	_textureCols = 1;
	//
	_blendMode = eBlendMode_Opaque;
	_textureChannel = eTexturedChannel_None;
	_textureChangeStyle = eTextureChangeStyle_Sequence;
	_textureResolution = eTextureResolution_Default;
	_textureFilter = eTextureFilter_UseGlobalSetting;
	_nodeState = eNodeState_Normal;
	//
	_texMap = NULL;
	//
	//_mtl = NULL;
	//
	_color = Point3(1,1,1);
	_incandescence = Point3(1,1,1);
}

void MaxMaterial::invalidateUI( ParamID pid )
{
	tParamBlockDesc2.InvalidateUI(pid);
}

bool MaxMaterial::isTwoSided()
{
	return _isTwoSides;
}

int MaxMaterial::NumSubMtls()
{
	return tNumSubMtls;
}

int MaxMaterial::NumSubTexmaps()
{
	return tNumSubTexMaps;
}

Class_ID MaxMaterial::ClassID()
{
	return MaxMaterial_CLASS_ID;
}

SClass_ID MaxMaterial::SuperClassID()
{
	return MATERIAL_CLASS_ID;
}

void MaxMaterial::GetClassName( TSTR& s )
{
	s = GetString(IDS_CLASS_NAME);
}

int MaxMaterial::NumSubs()
{
	return eMaxMaterialAnimation_Size;
}

int MaxMaterial::NumParamBlocks()
{
	return eBlockID_Size;
}

IParamBlock2* MaxMaterial::GetParamBlock( int i )
{
	return _pblock;
}

IParamBlock2* MaxMaterial::GetParamBlockByID( BlockID id )
{
	if (_pblock && _pblock->ID() == id)
	{
		return _pblock;
	}
	return NULL;
}

void MaxMaterial::DeleteThis()
{
	delete this;
}

int MaxMaterial::NumRefs()
{
	return eMaxMaterialRef_Size;
}

int MaxMaterial::SubNumToRefNum( int subNum )
{
	return subNum;
}

BOOL MaxMaterial::SupportsRenderElements()
{
	return TRUE;
}

void* MaxMaterial::GetInterface( ULONG id )
{
	switch(id)
	{
	case IID_IReshading:
		{
			return (IReshading*)( this );	
		}break;

	case IID_IValidityToken:
		{
			return (IValidityToken*)( this );	
		}break;
	}
	
	return Mtl::GetInterface(id);
}

Sampler* MaxMaterial::GetPixelSampler( int mtlNum, BOOL backFace )
{
// 	if (_mtl)
// 	{
// 		return _mtl->GetPixelSampler(mtlNum, backFace);
// 	}
	return NULL;
}

bool MaxMaterial::isTcFlowU()
{
	return _isTcFlowU;
}

bool MaxMaterial::isTcFlowV()
{
	return _isTcFlowV;
}

bool MaxMaterial::isTcRotate()
{
	return _isTcRotate;
}

bool MaxMaterial::isTcAngleReset()
{
	return _isTcAngleReset;
}

bool MaxMaterial::isTcPositionReset()
{
	return _isTcPositionReset;
}
#define Stupid_Error
float MaxMaterial::getTcFlowSpeedU( TimeValue time )
{
	if (_pblock)
	{
		Interval il;
		_pblock->GetValue(eParamID_TcFlowSpeedU, time, _tcFlowSpeedU, il);
	}
	
	return _tcFlowSpeedU;
}

float MaxMaterial::getTcFlowSpeedV( TimeValue time )
{
	if (_pblock)
	{
		Interval il;
		_pblock->GetValue(eParamID_TcFlowSpeedV, time, _tcFlowSpeedV, il);
	}
	return _tcFlowSpeedV;
}

float MaxMaterial::getTcRotateSpeed( TimeValue time )
{
	if (_pblock)
	{
		Interval il;
		_pblock->GetValue(eParamID_TcRotateSpeed, time, _tcRotateSpeed, il);
	}
	return _tcRotateSpeed;
}

float MaxMaterial::getTransparency(TimeValue time)
{
#ifdef Stupid_Error
	if (_pblock)
	{
		Interval il;
		_pblock->GetValue(eParamID_UnnamedTransparency, time, _unnamedTransparency, il);
	}
#endif
	return _unnamedTransparency;
}

Point3 MaxMaterial::getColor( TimeValue time )
{
	if (_pblock)
	{
		Interval il;
		_pblock->GetValue(eParamID_UnnamedColor, time, _color, il);
	}

	return _color;
}

int MaxMaterial::getRows()
{
	return _textureRows;
}

int MaxMaterial::getCols()
{
	return _textureCols;
}

int MaxMaterial::getChangeStyle( TimeValue time )
{
	if (_pblock)
	{
		Interval il;
		int temp = 0;
		_pblock->GetValue(eParamID_TextureChangeStyle, time, temp, il);
		_textureChangeStyle = (eTextureChangeStyle)temp;
	}
	return _textureChangeStyle;
}

int MaxMaterial::getChangeInterval()
{
	return _textureChangeInterval;
}

bool MaxMaterial::dontCastShadow()
{
	return _dontCastShadow;
}

bool MaxMaterial::isNoDepthWrite()
{
	return _noDepthWrite;
}

bool MaxMaterial::isNoDepthCheck()
{
	return _noDepthCheck;
}

int MaxMaterial::isAdditivelayer()
{
	return _additiveLayer;
}

bool MaxMaterial::isUnlit()
{
	return _unLit;
}

BOOL  MaxMaterial::SupportsShaders()
{
	return TRUE;
}

bool MaxMaterial::EvalColorStdChannel( ShadeContext& sc, int stdID, Color& outClr )
{
	if (sc.out.t.r < 0.78f && sc.out.t.g < 0.78f && sc.out.t.b < 0.78f)
	{
		outClr = outClr;
	}
	else
	{
		if (_texMap)
		{
			AColor c = _texMap->EvalColor(sc);
			outClr = c;
		}
	}
	return true;
}
		
/*===========================================================================*\
 |	Actual shading takes place
\*===========================================================================*/
void MaxMaterial::Shade(ShadeContext& sc) 
{
	if (gbufID)
	{
		sc.SetGBufferID(gbufID);
	}
	if (_texMap)
	{
		AColor c = _texMap->EvalColor(sc);
		sc.out.c = c;
		sc.out.t = c;
	}
	else
	{
		sc.out.c.Black();
		sc.out.t.Black();
	}
// 	if (_mtl)
// 	{
// 		_mtl->Shade(sc);
// 	}
}

bool MaxMaterial::EvalMonoStdChannel( ShadeContext& sc, int stdID, float& outVal )
{
	if (sc.out.t.r < 0.78f && sc.out.t.g < 0.78f && sc.out.t.b < 0.78f)
	{
		outVal = 0.0f;
	}
	else
	{
		outVal = 1.0f;
	}
	return true;
}

void MaxMaterial::PreShade( ShadeContext& sc, IReshadeFragment* pFrag )
{
	if (sc.out.t.r < 0.78f && sc.out.t.g < 0.78f && sc.out.t.b < 0.78f)
	{
	}
	else
	{
	}
}

void MaxMaterial::PostShade( ShadeContext& sc, IReshadeFragment* pFrag, int& nextTexIndex, IllumParams* ip )
{
	if (sc.out.t.r < 0.78f && sc.out.t.g < 0.78f && sc.out.t.b < 0.78f)
	{
	}
	else
	{
	}
}

IReshading::ReshadeRequirements MaxMaterial::GetReshadeRequirements()
{
	ReshadeRequirements r = RR_NeedReshade;
	return r;
}

void MaxMaterial::SetMtlOn( int i, BOOL onOff )
{
	_mapOn = onOff;
}

bool MaxMaterial::IsOutputConst( ShadeContext& sc, int stdID )
{
	if (_texMap/* || _mtl*/)
	{
		return false;
	}
	return true;
}

unsigned char MaxMaterial::getBlendMode()
{
	return _blendMode;
}

// 
// ULONG MaxMaterial::LocalRequirements( int subMtlNum )
// {
// 	return MTLREQ_2SIDE | MTLREQ_ADDITIVE_TRANSP;
// }
