/********************************************************************
	author :	cpzhang
	created:	2012Äê-2ÔÂ-13ÈÕ   11:43
*********************************************************************/
#ifndef __Mute_h__
#define __Mute_h__
#include "Common.h"
namespace Buddha
{
	class CFairMutex
	{
	public:

		/// Constructor
		CFairMutex(const std::string &name);

		/// Destructor
		~CFairMutex();

		void enter ();
		void leave ();

		std::string Name;
	private:
		CRITICAL_SECTION	_Cs;
	};
	
	template <class T>
	class CFairSynchronized
	{
	public:

		CFairSynchronized (const std::string &name) : _Cs(name) { }

		/**
		 * This class give you a thread safe access to the CFairSynchronized Value. Look at the example in CSynchronized.
		 */
		class CAccessor
		{
			CFairSynchronized<T> *Synchronized;
		public:

			/// get the mutex or wait
			CAccessor(CFairSynchronized<T> *cs)
			{
				Synchronized = cs;
				const_cast<CFairMutex&>(Synchronized->_Cs).enter();
			}

			/// release the mutex
			~CAccessor()
			{
				const_cast<CFairMutex&>(Synchronized->_Cs).leave();
			}

			/// access to the Value
			T &value()
			{
				return const_cast<T&>(Synchronized->_Value);
			}
		};

	private:

		friend class CFairSynchronized::CAccessor;

		/// The mutex of the synchronized value.
		volatile CFairMutex	_Cs;

		/// The synchronized value.
		volatile T			_Value;
	};

}

#endif //__Mute_h__
