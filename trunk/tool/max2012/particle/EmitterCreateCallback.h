/********************************************************************
**	file: 		EmitterCreateCallback.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __EmitterCreateCallback_h__
#define __EmitterCreateCallback_h__

#include "MaxParticleCommon.h"

class EmitterCreateCallback : public CreateMouseCallBack
{
public:
	GenParticle *rain;
	Point3 p0,p1;
	IPoint2 sp0, sp1;
	int proc( ViewExp *vpt,int msg, int point, int flags, IPoint2 m, Matrix3& mat)
	{
#ifdef _OSNAP
		if (msg == MOUSE_FREEMOVE)
		{
#ifdef _3D_CREATE
			vpt->SnapPreview(m,m,NULL, SNAP_IN_3D);
#else
			vpt->SnapPreview(m,m,NULL, SNAP_IN_PLANE);
#endif
		}
#endif

		if (msg == MOUSE_POINT || msg == MOUSE_MOVE)
		{
			switch(point)  
			{
			case 0:
				{
					sp0 = m;
#ifdef _3D_CREATE	
					p0 = vpt->SnapPoint(m, m, NULL, SNAP_IN_3D);
#else	
					p0 = vpt->SnapPoint(m,m,NULL,SNAP_IN_PLANE);
#endif
					mat.SetTrans(p0);
					rain->pblock->SetValue(eParamUID_EmitterWidth, 0, 0.01f);
					rain->pblock->SetValue(eParamUID_EmitterHeight, 0, 0.01f);
					rain->pmapParam->Invalidate();
				}break;

			case 1: 
				{
					mat.IdentityMatrix();
					sp1 = m;
	#ifdef _3D_CREATE	
					p1 = vpt->SnapPoint(m,m,NULL,SNAP_IN_3D);
	#else	
					p1 = vpt->SnapPoint(m,m,NULL,SNAP_IN_PLANE);
	#endif
					Point3 center = (p0 + p1) / float(2);
					mat.SetTrans(center);
					rain->pblock->SetValue(eParamUID_EmitterWidth, 0,
						(float)fabs(p1.x - p0.x));
					rain->pblock->SetValue(eParamUID_EmitterHeight, 0,
						(float)fabs(p1.y-p0.y));
					rain->pmapParam->Invalidate();

					if (msg == MOUSE_POINT)
					{
						if (Length(m-sp0)<3 || Length(p1-p0)<0.1f)
						{						
							return CREATE_ABORT;
						} 
						else 
						{
							return CREATE_STOP;
						}
					}
				}break;
			}
		}
		else
		{
			if (msg == MOUSE_ABORT)
			{
				return CREATE_ABORT;
			}
		}

		return 1;
	}
};

static EmitterCreateCallback tEmitterCreateCallback;

#endif // __EmitterCreateCallback_h__
