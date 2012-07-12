/********************************************************************
**	file: 		RocketDescriptor.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-3-15
**	purpose:	
*********************************************************************/
#ifndef __RocketDescriptor_h__
#define __RocketDescriptor_h__

#include "RocketCommon.h"
//extern HINSTANCE hInstance;

class RocketDescriptor : public ClassDesc2//, public Buddha::SingletonEx<RocketDescriptor>
{
public:
	RocketDescriptor();
	virtual ~RocketDescriptor();

public:  
		// If the plug-in can be picked and assigned by the user, as is	usually the case, return TRUE.
		int         IsPublic();

		// 3ds Max calls this method when it needs a pointer to a new instance of the plug-in class.
		void*        Create(BOOL loading = FALSE);

		// the name of the class. This name appears in the button for the plug-in in the 3ds Max user interface.
		const TCHAR* ClassName();

		// a system defined constant describing the class this plug-in class was derived from.
		SClass_ID    SuperClassID();

		// ID for the object
		// The ClassID consists of two unsigned 32-bit quantities
		// A developer should use the random Class_ID generator	to avoid conflicts
		Class_ID     ClassID();

		// a string describing the category a plug-in fits into. The category is usually selected in the drop down list in the create, or utility branch of the command panel.
		const TCHAR* Category();

		virtual const TCHAR* InternalName() 			{ return _T("MaxExporterFB"); }	// returns fixed parsable name (scripter-visible name)
		virtual HINSTANCE HInstance() 					{ return gInstance; }					// returns owning module handle
};

static RocketDescriptor tRocketDescriptor;
RocketDescriptor* GetRocketDescriptor();

#endif // __RocketDescriptor_h__
