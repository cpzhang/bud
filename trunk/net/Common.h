/********************************************************************
	author :	cpzhang
	created:	2011-29-12   15:55
*********************************************************************/
#ifndef __Common_h__
#define __Common_h__
//
#if defined(net_EXPORTS) // inside DLL
#   define _NET_EXPORT_   __declspec(dllexport)
#else // outside DLL
#   define _NET_EXPORT_   __declspec(dllimport)
#endif

//
#include "base/Buddha.h"

//
#include<winsock2.h>
#include<ws2tcpip.h>

namespace Shannon
{
	class Address;
	class ESocket;
	class Sock;
}

#endif //__Common_h__
