/********************************************************************
**	file: 		MaxGUP.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-27
**	purpose:	
*********************************************************************/
#ifndef __MaxGUP_h__
#define __MaxGUP_h__

#include "MaxGUPCommon.h"

class MaxGUP : public GUP
{
public:

	static HWND hParams;

	// GUP Methods
	DWORD Start	();
	void Stop();
	DWORD_PTR Control(DWORD parameter);

	// Loading/Saving
	IOResult Save(ISave *isave);
	IOResult Load(ILoad *iload);

	//Constructor/Destructor
	MaxGUP();
	~MaxGUP();		
};


#endif // __MaxGUP_h__