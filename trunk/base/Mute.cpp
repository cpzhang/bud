#include "Mute.h"
#include "Logger.h"
namespace Buddha
{
	CFairMutex::CFairMutex( const std::string &name )
	{
		Name = name;
#if (_WIN32_WINNT >= 0x0500)
		DWORD dwSpinCount = 0x80000000; // set high-order bit to use preallocation
		if ( ! InitializeCriticalSectionAndSpinCount( &_Cs, dwSpinCount ) )
		{
			Error( "Error entering critical section" );
		}
#else
		InitializeCriticalSection( &_Cs );
#endif
	}

	CFairMutex::~CFairMutex()
	{
		DeleteCriticalSection( &_Cs );
	}
	void CFairMutex::enter()
	{
		EnterCriticalSection( &_Cs );
	}

	void CFairMutex::leave()
	{
		LeaveCriticalSection( &_Cs );
	}
}