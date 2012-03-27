#pragma once

#include "ExportOptions.h"
#include "SkeletonDefine.h"

class feibian
{
public:
	feibian(void);
	virtual ~feibian(void);

	void clear();
	MStatus load(MFnSkinCluster* pSkinCluster);
	MStatus loadAnims(MDagPath& jointDag,int jointId);
	MStatus loadAnims();
	MStatus loadClip(MString clipName,int start,int stop,int rate);
	void loadKeyframe(joint& j,int time,keyframeTranslation& translation,keyframeRotation& rotation,keyframeScale& scale);
	void loadJoint(MDagPath& jointDag, joint* parent);

public:
	std::vector<joint> m_joints;
	animation	m_animation;
};