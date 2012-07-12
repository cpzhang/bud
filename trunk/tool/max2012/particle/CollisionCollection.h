/********************************************************************
**	file: 		CollisionCollection.h
**	author:		cpzhang <chengpengzhang@gmail.com>
**	created:	2011-4-25
**	purpose:	
*********************************************************************/
#ifndef __CollisionCollection_h__
#define __CollisionCollection_h__

#include "MaxParticleCommon.h"
//--- For multiple collision per integration step (frame) --------

class CollisionCollection: public CollisionObject {
public:
	static int MAX_COLLISIONS_PER_STEP;

	// CollisionObject methods
	BOOL CheckCollision(TimeValue t,Point3 &pos, Point3 &vel, float dt,int index, float *ct, BOOL UpdatePastCollide);
	Object *GetSWObject();

	// CollisionCollection methods
	void Init(const Tab<CollisionObject*> &cobjs);
	BOOL FindClosestCollision(TimeValue t,Point3 &pos, Point3 &vel, float dt,int index, float *ct);

private:
	Tab<CollisionObject*> m_cobjs;
};


#endif // __CollisionCollection_h__
