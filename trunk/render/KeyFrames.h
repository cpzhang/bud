#ifndef __KeyFrames_h__
#define __KeyFrames_h__

#include "Common.h"
#include "Interpolate.h"

namespace Euclid
{
	struct AnimationTime
	{
		AnimationTime()
		{
			start = 0;
			end = 0;
			current = 0;
		}

		s32 start;
		s32 end;
		s32 current;
	};

	template<class T> struct sKeyFrame
	{
		sKeyFrame() : time(0)
		{
			time = 0;
		}

		sKeyFrame(u32 time,const T& v)
		{
			this->time = time;
			this->v = v;
		}

		sKeyFrame(u32 time,const T& v,const T& in,const T& out)
		{
			this->time = time;
			this->v = v;
			//this->in = in;
			//this->out = out;
		}

		bool operator==(u32 time)
		{
			return this->time == time;
		}

		bool operator<(const sKeyFrame<T>& rhs)
		{
			return this->time < rhs.time;
		}

		u32 time;
		T v;
		//T in;
		//T out;
	};

	template<class T>
	inline bool kf_less(const sKeyFrame<T>& kf1, const sKeyFrame<T>& kf2)
	{
		return kf1.time < kf2.time;
	}

	typedef std::pair<int,int>		KeyFrameRange;
	typedef std::pair<u32,u32>	KeyFrameTimeRange;
// #	pragma pack( push, packing )
// #	pragma pack( 1 )

	template<class T> class sKeyFrames
	{
	public:
		typedef std::vector<sKeyFrame<T> >	KeyFrameList;
		typedef std::vector<KeyFrameRange>	KeyFrameRangeList;

		KeyFrameList			_keyFrames;
		KeyFrameRangeList		_keyFrameRanges;
		eInterpolationType		_interpolationType;
		typename KeyFrameList::iterator	_it ;

		/// Static
		bool					_bStatic;
		T						_staticData;

		/// GlobalSequence
		int						_globalSequenceId;

	public:
		void clear()
		{
			_keyFrames.clear();
			_keyFrameRanges.clear();
		}
		void setInterpolationType(eInterpolationType interpolationType)
		{
			_interpolationType = interpolationType;
		}

		eInterpolationType getInterpolationType()
		{
			return _interpolationType;
		}

		int addKeyFrameRange(const KeyFrameRange& keyFrameRange)
		{
			size_t size = _keyFrameRanges.size();
			_keyFrameRanges.push_back(keyFrameRange);
			return size;
		}

		int addKeyFrameRange(const KeyFrameTimeRange& keyFrameTimeRange)
		{
			int first = -1;
			int second = -1;

			size_t size = _keyFrames.size();
			for(size_t i = 0;i < size;i++)
			{
				sKeyFrame<T>& keyFrame = _keyFrames[i];
				if(keyFrame.time == keyFrameTimeRange.first)
					first = i;
				if(keyFrame.time == keyFrameTimeRange.second)
					second = i;

				if(first != -1 && second != -1)break;
			}

			KeyFrameRange keyFrameRange;
			keyFrameRange.first = first;
			keyFrameRange.second = second;

			return addKeyFrameRange(keyFrameRange);
		}

		void setStaticData(const T& data)
		{
			_staticData = data;
			_bStatic = true;
		}

		void addKeyFrame(const sKeyFrame<T>& keyFrame)
		{
			_keyFrames.push_back(keyFrame);
			_bStatic = false;
		}

		// 添加一组有序的关键帧
		void addKeyFrames(const sKeyFrames<T>& keyFrames)
		{
			_keyFrames.insert(_keyFrames.end(), keyFrames._keyFrames.begin(), keyFrames._keyFrames.end());
			_bStatic = false;
		}

		// 插入一组有序的关键帧
		void insertKeyFrames(sKeyFrames<T>& keyFrames)
		{
			sKeyFrame<T>* kf = keyFrames.getKeyFrame(0);
			KeyFrameList::iterator insert_pos = std::upper_bound(_keyFrames.begin(), _keyFrames.end(), *kf, &kf_less<T>);
			_keyFrames.insert(insert_pos, keyFrames._keyFrames.begin(), keyFrames._keyFrames.end());
			_bStatic = false;
		}

		void reserve(size_t n)
		{
			_keyFrames.reserve(n);
		}

		void sort()
		{
			if (_keyFrames.size() > 1)
				std::sort(_keyFrames.begin(), _keyFrames.end());
		}

		size_t numKeyFrames()
		{
			return _keyFrames.size();
		}

		T getFrame(const AnimationTime *pTime)
		{
			if(_bStatic)return _staticData;
// 
			if (_keyFrames.empty())
			{
				return _staticData;
			}
 			size_t size = _keyFrames.size();
			u32 lastTime = _keyFrames[size - 1].time;
			u32 time = pTime->current;
			//if(lastTime)time %= lastTime;

			//时间在第一帧之前
			if(time < _keyFrames[0].time)
				return _staticData;

			//在最后一帧之后
			if(time >= _keyFrames[size - 1].time)return _keyFrames[size - 1].v;

			sKeyFrame<T> *pKeyFrame1 = 0;
			sKeyFrame<T> *pKeyFrame2 = 0;

			KeyFrameRange range;
			range.first = 0;
			range.second = size - 1;

			return getFrame(range,pTime);
		}

		T getFrame(int keyFrameRangeIndex,const AnimationTime *pTime)
		{
			if(_bStatic)return _staticData;

			if(keyFrameRangeIndex == -1)return getFrame(pTime);
// 			KeyFrameRange range = _keyFrameRanges[keyFrameRangeIndex];
// 
// 			return getFrame(range,pTime);
		}

		sKeyFrame<T>* getKeyFrame(size_t index)
		{
			return &_keyFrames[index];
		}

		void setGlobalSequenceId(int globalSequeneId)
		{
			_globalSequenceId = globalSequeneId;
		}

		int getGlobalSequenceId()
		{
			return _globalSequenceId;
		}

		sKeyFrames<T>& operator=(const sKeyFrames<T>& keyFrames)
		{
			_keyFrames = keyFrames._keyFrames;
			//_keyFrameRanges = keyFrames._keyFrameRanges;
			_interpolationType = keyFrames._interpolationType;
			_bStatic = keyFrames._bStatic;
			_staticData = keyFrames._staticData;
			_globalSequenceId = keyFrames._globalSequenceId;

			return *this;
		}

		size_t getMemorySize()
		{
			return sizeof(*this) + _keyFrames.size() * sizeof(sKeyFrame<T>)/* + _keyFrameRanges.size() * sizeof(KeyFrameRange)*/;
		}

	private:
		T interpolateHermite(float r,sKeyFrame<T> *pKeyFrame1,sKeyFrame<T> *pKeyFrame2)
		{
			//T t = rkt::interpolateHermite<T>(r,pKeyFrame1->v,pKeyFrame2->v,pKeyFrame2->in,pKeyFrame1->out);
			T t = InterpolateHermite<T>(r,pKeyFrame1->v,pKeyFrame2->v,pKeyFrame2->v,pKeyFrame1->v);
			return t;
		}

		T interpolateBezier(float r,sKeyFrame<T> *pKeyFrame1,sKeyFrame<T> *pKeyFrame2)
		{
			//T t = rkt::interpolateBezier<T>(r,pKeyFrame1->v,pKeyFrame2->v,pKeyFrame2->in,pKeyFrame1->out);
			T t = InterpolateBezier<T>(r,pKeyFrame1->v,pKeyFrame2->v,pKeyFrame2->v,pKeyFrame1->v);
			return t;
		}

		T getFrame(KeyFrameRange range,const AnimationTime *pTime)
		{
			size_t lastIndex = _keyFrames.size() - 1;
			u32 time = pTime->current % _keyFrames[lastIndex].time;

			while(range.first < range.second - 1)
			{
				int middle = ((range.first + range.second) >> 1);
				if(time >= _keyFrames[range.first].time && time <= _keyFrames[middle].time)
				{
					range.second = middle;
				}
				else
				{
					range.first = middle;
				}
			}

			sKeyFrame<T> *pKeyFrame1 = &_keyFrames[range.first];
			sKeyFrame<T> *pKeyFrame2 = &_keyFrames[range.second];

			if(pKeyFrame1->time < pTime->start)
			{
				if(pKeyFrame2->time > pTime->end)
				{
					return _staticData;
				}
				else
				{
					return pKeyFrame2->v;
				}
			}

			if(pKeyFrame2->time > pTime->end)
			{
				return pKeyFrame1->v;
			}

			if(pKeyFrame1->time == time)return pKeyFrame1->v;
			if(pKeyFrame2->time == time)return pKeyFrame2->v;

			if(_interpolationType == eInterpolationType_None)return pKeyFrame1->v;

			float r = (time - pKeyFrame1->time) / (float)(pKeyFrame2->time - pKeyFrame1->time);

			T t;

			switch(_interpolationType)
			{
			case eInterpolationType_Bezier_Linear:
				t = InterpolateBezier(r,pKeyFrame1->v,pKeyFrame2->v);
				break;
			case INTERPOLATION_HERMITE:
				//t = interpolate(r,pKeyFrame1->v,pKeyFrame2->v);
				t = interpolateHermite(r,pKeyFrame1,pKeyFrame2);
				break;
			case eInterpolationType_Bezier_Cubic:
				//t = interpolate(r,pKeyFrame1->v,pKeyFrame2->v);
				t = interpolateBezier(r,pKeyFrame1,pKeyFrame2);
				break;
			}

			return t;
		}
	public:
		sKeyFrames() : _interpolationType(eInterpolationType_None),_bStatic(true),_globalSequenceId(-1)
		{
			//_keyFrames.reserve(1000);
		}
	};
}

#endif // __KeyFrames_h__