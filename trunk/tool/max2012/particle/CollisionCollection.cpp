#include "CollisionCollection.h"

int CollisionCollection::MAX_COLLISIONS_PER_STEP = 10;

void CollisionCollection::Init(const Tab<CollisionObject*> &cobjs)
{
	m_cobjs.SetCount(cobjs.Count());
	for(int i=0; i<cobjs.Count(); i++)
	{
		m_cobjs[i] = cobjs[i];
	}
}

Object* CollisionCollection::GetSWObject()
{
	if (m_cobjs.Count()) 
	{
		return m_cobjs[0]->GetSWObject();
	}
	
	return NULL;
}

BOOL CollisionCollection::CheckCollision(TimeValue t,Point3 &pos, Point3 &vel, float dt,int index, float *ct, BOOL UpdatePastCollide)
{
	TimeValue nextTime, curTime = t;
	BOOL collide=FALSE, maybeStuck=FALSE;

	if (UpdatePastCollide)
	{
		for(int i=0; i<MAX_COLLISIONS_PER_STEP; i++)
		{
			if (FindClosestCollision(curTime, pos, vel, dt, index, ct))
			{
				collide = TRUE;
				nextTime = curTime + (TimeValue)ceil(*ct);
				dt -= nextTime - curTime;
				if (dt <= 0.0f) break; // time limit for the current integration step
				curTime = nextTime; // for the next micro-step
			}
			else break;
			// particle may still have a collision in the current integration step;
			// since particle reaches the limit of collision check per integration step,
			// we'd better hold on the particle movements for the current frame
			if (i==MAX_COLLISIONS_PER_STEP-1) maybeStuck = TRUE;
		}
		if ((dt > 0.0f) && (!maybeStuck)) // final interval without collisions
			pos += vel*dt;
	}
	else
		collide = FindClosestCollision(t, pos, vel, dt, index, ct);		

	return collide;
}

BOOL CollisionCollection::FindClosestCollision(TimeValue t,Point3 &pos, Point3 &vel, float dt,int index, float *ct)
{
	Point3 curPos, curVel, resPos, resVel;
	float curTime, minTime = dt+1.0f;
	BOOL collide = FALSE;

	for(int i=0; i<m_cobjs.Count(); i++)
	{
		curPos = pos; curVel = vel;
		if (m_cobjs[i]->CheckCollision(t, curPos, curVel, dt, index, &curTime, FALSE))
			if (curTime < minTime) // the collision is the closest one
			{
				collide = TRUE;
				minTime = curTime;
				resPos = curPos;
				resVel = curVel;
			}
	}
	if (collide)
	{
		pos = resPos;
		vel = resVel;
		*ct = minTime;
	}
	return collide;
}
