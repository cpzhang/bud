#pragma once

#include "ExportOptions.h"
#include "SkeletonDefine.h"

class skeleton
{
public:
	skeleton(void);
	virtual ~skeleton(void);

	void clear();
	MStatus load(MFnSkinCluster* pSkinCluster);
	MStatus loadAnims(MDagPath& jointDag,int jointId);
	MStatus loadAnims();
	MStatus loadClip(MString clipName,int start,int stop,int rate);
	void loadKeyframe(joint& j,int time,keyframeTranslation& translation,keyframeRotation& rotation,keyframeScale& scale);
	std::vector<joint>& getJoints();
	std::vector<animation>& getAnimations();

public:
	void loadJoint(MDagPath& jointDag, joint* parent);

	std::vector<joint> m_joints;
	std::vector<animation> m_animations;
};

