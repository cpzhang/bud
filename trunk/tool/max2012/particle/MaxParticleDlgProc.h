/********************************************************************
**	file: 		MaxParticleDlgProc.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-23
**	purpose:	
*********************************************************************/
#ifndef __MaxParticleDlgProc_h__
#define __MaxParticleDlgProc_h__

#include "MaxParticleCommon.h"
class GenParticle;

class ParticleDlgProc : public ParamMapUserDlgProc 
{
public:
	GenParticle *po;

	ParticleDlgProc(GenParticle *p) {po=p;}
	INT_PTR DlgProc(TimeValue t,IParamMap *map,HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam);
	void DeleteThis() {delete this;}
	void SetupMaxRate(HWND hWnd,IParamBlock *pblock,TimeValue t);
	void SetupConstant(HWND hWnd);
	void EnableTumble(HWND hWnd, IParamBlock *pblock, TimeValue t); // for Snow
};

#endif // __MaxParticleDlgProc_h__
