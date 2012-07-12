#include "MaxParticleDlgProc.h"


void ParticleDlgProc::SetupMaxRate(HWND hWnd,IParamBlock *pblock,TimeValue t)
{
}

void ParticleDlgProc::SetupConstant(HWND hWnd)
{
}

void ParticleDlgProc::EnableTumble(HWND hWnd, IParamBlock *pblock, TimeValue t) // for Snow
{
}

INT_PTR ParticleDlgProc::DlgProc(TimeValue t,IParamMap *map,HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	switch (msg) {
		case WM_INITDIALOG:
			SetupMaxRate(hWnd,(IParamBlock*)map->GetParamBlock(),t);
			SetupConstant(hWnd);
			EnableTumble(hWnd,(IParamBlock*)map->GetParamBlock(),t);
			break;			

		case CC_SPINNER_CHANGE:
			{
			
			}break;

		case WM_COMMAND:
			break;	
	}
	return FALSE;
}



