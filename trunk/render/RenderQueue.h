//==========================================================================
/**
* @file	  : RenderQueue.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-27   21:22
* purpose : 
*/
//==========================================================================

#ifndef __RenderQueue_h__
#define __RenderQueue_h__

#include "Common.h"
#include "Appearance.h"

namespace Euclid
{
	class IRenderable
	{
		virtual void  renderGeometry() = 0;
		
		virtual const Appearance& getAppearance() const = 0;
		
		virtual const Mat4& getWorldTransform() const = 0;
	};

	/** Class to manage the scene object rendering queue.
        @remarks
            Objects are grouped by material to minimise rendering state changes. The map from
            material to renderable object is wrapped in a class for ease of use.
        @par
            This class now includes the concept of 'queue groups' which allows the application
            adding the renderable to specifically schedule it so that it is included in 
            a discrete group. Good for separating renderables into the main scene,
            backgrounds and overlays, and also could be used in the future for more
            complex multipass routines like stenciling.
    */
	class RenderQueue
	{
	private:
		typedef std::vector<IRenderable*>	RenderableObjectVector;
		RenderableObjectVector				_renderArray[eRenderQueueOrder_Size]; /// 各种排序类别的渲染对象列表
	};
}

#endif // __RenderQueue_h__
 
