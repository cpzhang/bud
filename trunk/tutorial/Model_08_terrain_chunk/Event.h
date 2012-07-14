#pragma once
#include "base/singleton.h"
enum eEvent
{
	eEvent_CTreeDlgChangeFile,
	eEvent_Size,
};

class EventArgs
{
public:
	EventArgs(void) : handled(0) {}
	virtual ~EventArgs(void) {}

public:
    u32 handled;
};

class SlotFunctorBase
{
public:
	virtual ~SlotFunctorBase() {};
	virtual bool operator()(const EventArgs& args) = 0;
};
class FreeFunctionSlot : public SlotFunctorBase
{
public:
	//! Slot function type.
	typedef bool (SlotFunction)(const EventArgs&);

	FreeFunctionSlot(SlotFunction* func) :
	d_function(func)
	{}

	virtual bool operator()(const EventArgs& args)
	{
		return d_function(args);
	}

private:
	SlotFunction* d_function;
};
template<typename T>
class MemberFunctionSlot : public SlotFunctorBase
{
public:
	//! Member function slot type.
	typedef bool(T::*MemberFunctionType)(const EventArgs&);

	MemberFunctionSlot(MemberFunctionType func, T* obj) :
	d_function(func),
		d_object(obj)
	{}

	virtual bool operator()(const EventArgs& args)
	{
		return (d_object->*d_function)(args);
	}

private:
	MemberFunctionType d_function;
	T* d_object;
};
template<typename T>
class FunctorReferenceSlot : public SlotFunctorBase
{
public:
	FunctorReferenceSlot(T& functor) :
	  d_functor(functor)
	  {}

	  virtual bool operator()(const EventArgs& args)
	  {
		  return d_functor(args);
	  }

private:
	T& d_functor;
};
template<typename T>
class FunctorCopySlot : public SlotFunctorBase
{
public:
	FunctorCopySlot(const T& functor) :
	  d_functor(functor)
	  {}

	  virtual bool operator()(const EventArgs& args)
	  {
		  return d_functor(args);
	  }

private:
	T d_functor;
};
template<typename T>
class FunctorPointerSlot : public SlotFunctorBase
{
public:
	FunctorPointerSlot(T* functor) :
	  d_functor(functor)
	  {}

	  virtual bool operator()(const EventArgs& args)
	  {
		  return (*d_functor)(args);
	  }

private:
	T* d_functor;
};
template<typename T>
struct FunctorReferenceBinder
{
	FunctorReferenceBinder(T& functor) :
d_functor(functor)
{}

T& d_functor;
};
class SubscriberSlot
{
public:
    /*!
    \brief
        Default constructor.  Creates a SubscriberSlot with no bound slot.
    */
    SubscriberSlot()
	{

	}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a free function.
    */
    SubscriberSlot(FreeFunctionSlot::SlotFunction* func)
	{

	}

    /*!
    \brief
        Destructor.  Note this is non-virtual, which should be telling you not
        to sub-class!
    */
    ~SubscriberSlot()
	{

	}

    /*!
    \brief
        Invokes the slot functor that is bound to this Subscriber.  Returns
        whatever the slot returns, unless there is not slot bound when false is
        always returned.
    */
    bool operator()(const EventArgs& args) const
    {
        return (*d_functor_impl)(args);
    }

    // templatised constructors
    /*!
    \brief
        Creates a SubscriberSlot that is bound to a member function.
    */
    template<typename T>
    SubscriberSlot(bool (T::*function)(const EventArgs&), T* obj) :
        d_functor_impl(new MemberFunctionSlot<T>(function, obj))
    {}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a functor object reference.
    */
    template<typename T>
    SubscriberSlot(const FunctorReferenceBinder<T>& binder) :
        d_functor_impl(new FunctorReferenceSlot<T>(binder.d_functor))
    {}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a copy of a functor object.
    */
    template<typename T>
    SubscriberSlot(const T& functor) :
        d_functor_impl(new FunctorCopySlot<T>(functor))
    {}

    /*!
    \brief
        Creates a SubscriberSlot that is bound to a functor pointer.
    */
    template<typename T>
    SubscriberSlot(T* functor) :
        d_functor_impl(new FunctorPointerSlot<T>(functor))
    {}

private:
    //! Points to the internal functor object to which we are bound
    SlotFunctorBase* d_functor_impl;
};
class EventManager: public Buddha::SingletonEx<EventManager>
{
public:
	void subscribeEvent(eEvent e, SubscriberSlot slot)
	{
		mEventSubscriberSlots.insert(std::pair<eEvent, SubscriberSlot>(e, slot));
	}
	void fireEvent(eEvent e, EventArgs* args)
	{
		EventSubScriberSlotMap::iterator it;
		std::pair<EventSubScriberSlotMap::iterator, EventSubScriberSlotMap::iterator> ret;
		ret = mEventSubscriberSlots.equal_range(e);
		for (it=ret.first; it!=ret.second; ++it)
		{
			(*it).second(*args);
		}
	}
private:
	typedef std::multimap<eEvent, SubscriberSlot/*, EventComp*/> EventSubScriberSlotMap;
	EventSubScriberSlotMap mEventSubscriberSlots;
};
