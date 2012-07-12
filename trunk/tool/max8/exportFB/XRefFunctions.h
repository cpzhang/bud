/********************************************************************
**	file: 		XRefFunctions.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-5-19
**	purpose:	
*********************************************************************/
#ifndef __XRefFunctions_h__
#define __XRefFunctions_h__

#include "RocketCommon.h"

/** This class contains static helper functions to handle XRefs*/
    class XRefFunctions
    {
	public:

		/** Checks, if @a object is a XRef object.
		@return True, if @a object is a XRef object, false if not.*/
		static bool isXRefObject(Animatable* object);

		/** Checks, if @a object is a XRef item.
		@return True, if @a object is a XRef item, false if not.*/
		static bool isXRefItem(Animatable* object);

		/** Checks, if @a material is a XRef object.
		@return True, if @a material is a XRef object, false if not.*/
		static bool isXRefMaterial(Animatable* material);

		/** Recursively resolves the XRef object, i.e. it returns the object that is referenced.
		@return The resolved object. It is garanteed to be not an Xref object.*/
		static Object* getXRefObjectSource(Object* xRefObject, Tab< Modifier * > * modifiers = 0);

		/** Recursively resolves the XRef object, i.e. it returns the object that is referenced.
		@return The resolved object. It is garanteed to be not an Xref object.*/
		static Object* getXRefItemSource(Object* xRefObject);

		/** Recursively resolves the XRef material, i.e. it returns the object that is referenced.
		@return The resolved material. It is garanteed to be not an Xref material.*/
		static Mtl* getXRefMaterialSource(Mtl* xRefMaterial);
	};

#endif // __XRefFunctions_h__
