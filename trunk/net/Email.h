/********************************************************************
	author :	cpzhang
	created:	2012-1-17   16:55
*********************************************************************/
#ifndef __Email_h__
#define __Email_h__
#include "Common.h"
namespace Shannon
{
// 	bool fromString(const std::string &str, u8 &val)
// 	{
// 		char *end; long v; errno = 0; 
// 		v = strtol(str.c_str(), &end, 10);
// 		if (errno || v > UCHAR_MAX || v < 0 || end == str.c_str())
// 		{
// 			val = 0;
// 			return false; 
// 		} 
// 		else
// 		{
// 			val = (u8)v; 
// 			return true;
// 		} 
// 	}
// 
// 	bool fromString(const std::string &str, s8 &val)
// 	{
// 		char *end; long v; errno = 0; 
// 		v = strtol(str.c_str(), &end, 10);
// 		if (errno || v > SCHAR_MAX || v < SCHAR_MIN || end == str.c_str())
// 		{
// 			val = 0;
// 			return false; 
// 		} 
// 		else
// 		{
// 			val = (s8)v;
// 			return true;
// 		} 
// 	}
	
// 	bool fromString(const std::string &str, s32 &val)
// 	{
// 		char *end; long v; errno = 0;
// 		v = strtol(str.c_str(), &end, 10); 
// 		if (errno || v > INT_MAX || v < INT_MIN || end == str.c_str())
// 		{
// 			val = 0;
// 			return false;
// 		}
// 		else
// 		{
// 			val = (s32)v; 
// 			return true;
// 		}
// 	}
	class _NET_EXPORT_ Email
	{
	public:
		inline static bool fromString(const std::string &str, u32 &val)
		{
			size_t pos = str.find('-');
			//if (pos != std::string::npos) 
			if (pos != (size_t)(-1)) 
			{
				val = 0; 
				return false; 
			} 
			char *end; unsigned long v; errno = 0; 
			v = strtoul(str.c_str(), &end, 10); 
			if (errno || v > UINT_MAX || end == str.c_str())
			{
				val = 0; 
				return false;
			} 
			else
			{
				val = (u32)v; 
				return true; 
			}
		}
		/** Send an email
		* \param smtpServer must be a smtp email server.
		* \param from must be a valid email address. If empty, create a fake email address with anonymous@<ipaddress>.com
		* \param to must be a valid email address.
		* \param subject subject of the email. Can be empty.
		* \param body body of the email. Can be empty.
		* \param attachedFile a filename that will be send with the email. Can be empty.
		* \param onlyCheck If true, It'll not send the mail but only check if it could be send.
		*/

	static bool sendEmail (
		const std::string &smtpServer,
		u16 port,
		const std::string &from, 
		const std::string &to, 
		const std::string &subject, 
		const std::string &body, 
		const std::string &attachedFile = "", 
		bool onlyCheck = false
		);
	};
}

#endif //__Email_h__
