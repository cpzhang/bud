#include "XRefFunctions.h"


//---------------------------------------------------------------
bool XRefFunctions::isXRefObject(Animatable* object)
{
	return IXRefObject8::Is_IXRefObject8(* object);
}


//---------------------------------------------------------------
bool XRefFunctions::isXRefItem(Animatable* object)
{
	return IXRefItem::IsIXRefItem(* object);
}


//---------------------------------------------------------------
bool XRefFunctions::isXRefMaterial(Animatable* material)
{
	return IXRefMaterial::Is_IXRefMaterial(*material);
}

//---------------------------------------------------------------
Object* XRefFunctions::getXRefObjectSource(Object* xRefObject, Tab< Modifier * > * modifiers)
{
	if ( !isXRefObject(xRefObject) ) 
		return xRefObject; // this is the source

	IXRefObject8* xRefObjectInterface = IXRefObject8::GetInterface(* xRefObject);
	if ( !xRefObjectInterface ) 
		return 0;

	// resolve nested
	Object* source = xRefObjectInterface->GetSourceObject(false, modifiers);

	return source;
}


//---------------------------------------------------------------
Object* XRefFunctions::getXRefItemSource(Object* xRefObject)
{
	if ( !isXRefItem(xRefObject) ) 
		return (IDerivedObject*) xRefObject; // this is the source

	IXRefItem * xRefObjectInterface = IXRefItem::GetInterface(* xRefObject);
	if ( !xRefObjectInterface ) 
		return 0;

	// resolve nested
	IDerivedObject* source = (IDerivedObject*) xRefObjectInterface->GetSrcItem(false);

	INode* bb1 = source->GetWorldSpaceObjectNode();


	int h1h = 5;

	return source;
}

//---------------------------------------------------------------
Mtl* XRefFunctions::getXRefMaterialSource(Mtl* xRefMaterial)
{
	if (!isXRefMaterial(xRefMaterial))
	{
		// this is the source
		return xRefMaterial;
	}

	IXRefMaterial* xRefMaterialInterface = IXRefMaterial::GetInterface(* xRefMaterial);
	if ( !xRefMaterialInterface )
		return 0;

	// resolve nested
	Mtl* source = xRefMaterialInterface->GetSourceMaterial(true);
	return source;
}