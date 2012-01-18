#include "Email.h"
#include "TCPSock.h"
namespace Shannon
{
	bool Email::sendEmail( const std::string &smtpServer, const std::string &from, const std::string &to, const std::string &subject, const std::string &body, const std::string &attachedFile /*= ""*/, bool onlyCheck /*= false */ )
	{
		TCPSocket sock;
		Address ad(smtpServer, 80);
		sock.connect(ad);
		if (sock.connected())
		{
			MessageBox(NULL, "sent email ok", "woops", MB_OK);
			sock.close();
		}
		else
		{
			MessageBox(NULL, smtpServer.c_str(), "woops", MB_OK);
		}
		return true;
	}
}