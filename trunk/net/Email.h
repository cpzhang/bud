/********************************************************************
	author :	cpzhang
	created:	2012-1-17   16:55
*********************************************************************/
#ifndef __Email_h__
#define __Email_h__
#include "Common.h"
namespace Shannon
{
	class _NET_EXPORT_ Email
	{
	public:
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
