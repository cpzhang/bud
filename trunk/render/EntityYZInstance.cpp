#include "stdafx.h"
#include "EntityYZInstance.h"
#include "EntityYZ.h"
#include "SubEntityYZInstance.h"
#include "Skeleton.h"
#include "VertexBuffer.h"

namespace Euclid
{
	EntityYZInstance::EntityYZInstance(EntityYZInstancePara* p)
		:_para(NULL)
	{
		_para = new EntityYZInstancePara(p);
		_name = _para->_name;
		_isRecycle = true;
		clear();
	}

	EntityYZInstance::~EntityYZInstance()
	{

	}

	bool EntityYZInstance::create()
	{
		_ib = _para->_entityYZ->_ib;
		_vb = _para->_entityYZ->cloneVB();

		SubMeshMap::iterator it = _para->_entityYZ->_sm.begin();
		for (; it != _para->_entityYZ->_sm.end(); ++it)
		{
			SubEntityYZInstancePara p;
			p._entityYZInstance = this;
			p._subEntityName = it->second->_name;
			SubEntityYZInstance* se = new SubEntityYZInstance(&p);
			if (!se->create())
			{
				return false;
			}
			_subEntityInstances.push_back(se);
		}

		return true;
	}

	void EntityYZInstance::destroy()
	{
		safeDelete(_para);
	}

	void EntityYZInstance::preRender()
	{

	}

	void EntityYZInstance::postRender()
	{

	}

	void EntityYZInstance::update( u32 current, u32 delta )
	{
		if (_vb == NULL)
		{
			return;
		}
		// update vertex
		POSITION_TEXTURE* data = (POSITION_TEXTURE*)_vb->lock(0, 0, 0);

		if (data == NULL)
		{
			return;
		}

		if (!_para->_entityYZ->_animationSet.empty() || !_para->_entityYZ->_am.empty())
		{
			updateClock(delta);
			// update bones
			for (size_t i = 0; i < _para->_entityYZ->_skeletons.size(); ++i)
			{
				Skeleton* b = _para->_entityYZ->_skeletons[i];
				b->update(_clock);
			}

			for (size_t i = 0; i < _para->_entityYZ->_vertices.size(); ++i)
			{
				sVertexEx* e = _para->_entityYZ->_vertices[i];

				data[i]._pos = Vec3::ZERO;

				bool gotValue =false;
				for (size_t k = 0; k < 4; ++k)
				{
					if(e->weights[k] > 0)
					{
						if (e->bones[k] < _para->_entityYZ->_bones.size())
						{
							gotValue = true;
							Vec3 v;
							sBone* b = _para->_entityYZ->_bones[e->bones[k]];
							Mat4 m = b->getFinalMatrix();
							v = m * e->pos;
							v *= (float)e->weights[k];

							data[i]._pos += v;
						}
					}
				}

				if (!gotValue)
				{
					data[i]._pos = e->pos;
				}
			}
		}
		
		_vb->unLock();
		//
		SubEntityYZInstanceSet::iterator it = _subEntityInstances.begin();
		for (; it != _subEntityInstances.end(); ++it)
		{
			(*it)->update(current, delta);
		}
	}

	void EntityYZInstance::renderNormal()
	{
		SubEntityYZInstanceSet::iterator it = _subEntityInstances.begin();
		for (; it != _subEntityInstances.end(); ++it)
		{
			(*it)->renderNormal();
		}
	}

	void EntityYZInstance::renderWithEffect()
	{
		SubEntityYZInstanceSet::iterator it = _subEntityInstances.begin();
		for (; it != _subEntityInstances.end(); ++it)
		{
			(*it)->renderWithEffect();
		}
	}

	void EntityYZInstance::updateClock( u32 delta )
	{
		_clock.current += delta * _speed;

		if (_clock.current > _clock.end)
		{
			if (_isRecycle)
			{
				_clock.current = _clock.start;
			}
			else
			{
				_clock.current = _clock.end;
			}
		}
	}

	bool EntityYZInstance::setAnimation( const std::string& name )
	{
		AnimationMap::iterator it = _para->_entityYZ->_am.begin();
		if (it == _para->_entityYZ->_am.end())
		{
			return false;
		}

		_clock.start = it->second->_time_start;
		_clock.end = it->second->_time_end;
		_clock.current = it->second->_time_start;

		return true;
	}

	void EntityYZInstance::setSpeed( Real speed )
	{
		_speed = speed;
	}

	size_t EntityYZInstance::getSubEntityYZInstanceNum()
	{
		return _subEntityInstances.size();
	}

	SubEntityYZInstance* EntityYZInstance::getSubEntityInstance( size_t i )
	{
		return _subEntityInstances[i];
	}

	void EntityYZInstance::setRecycle( bool b )
	{
		_isRecycle = b;
	}

	void EntityYZInstance::clear()
	{
		_isRenderingBone = false;
		_bonesObject = NULL;
	}

	bool EntityYZInstance::isRenderingBone()
	{
		return _isRenderingBone;
	}

	void EntityYZInstance::setRenderingBone( bool b )
	{
		_isRenderingBone = b;
	}
}
