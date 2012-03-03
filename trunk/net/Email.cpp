#include "Email.h"
#include "TCPSock.h"
namespace Shannon
{
	bool sendEMailCommand (TCPSocket &sock, const std::string &command, u32 code = 250)
	{
		std::string buffer = command + "\r\n";
		u32 size = (u32)buffer.size();
		if(!command.empty())
		{
			if (sock.send ((u8 *)buffer.c_str(), size) != Sock::eSockResult_Ok)
			{
				Warning("EMAIL: Can't send data to the server");
				return false;
			}
		}

		std::string res;
		char c;
		for(;;)
		{
			size = 1;
			if (sock.receive((u8*)&c, size, false) == Sock::eSockResult_Ok)
			{
				res += c;
				if (c == '\n')
				{
					u32 c;
					Email::fromString(res, c);
					if (c != code)
					{
						Warning ("EMAIL: EMail command '%s' returned '%s' instead of code %d on sock %s", command.substr(0, 20).c_str(), res.substr(0, res.size()-2).c_str(), code, sock.remoteAddr().asString().c_str());
						return false;
					}
					return true;
				}
			}
			else
			{
				Warning ("EMAIL: EMail connection closed before end of line, command '%s' returned '%s' on sock %s (code %d)", command.substr(0, 20).c_str(), res.c_str(), sock.remoteAddr().asString().c_str(), code);
				return false;
			}
		}

		return true;
	}
	bool Email::sendEmail( const std::string &smtpServer, u16 port, const std::string &from, const std::string &to, const std::string &subject, const std::string &body, const std::string &attachedFile /*= ""*/, bool onlyCheck /*= false */ )
	{
		bool ok = false;
		TCPSocket sock;
		Address ad(smtpServer, port);
		sock.connect(ad);
		if (sock.connected())
		{
			// we must skip the first line
			std::string formatedBody = "\r\n";
			if(!sendEMailCommand (sock, "", 220)) goto end;
			if(onlyCheck)
			{
				if(!sendEMailCommand (sock, "HELO localhost")) goto end;
				if(!sendEMailCommand (sock, "MAIL FROM: " + from)) goto end;
				if(!sendEMailCommand (sock, "RCPT TO: " + to)) goto end;
				if(!sendEMailCommand (sock, "QUIT", 221)) goto end;

				ok = true;
			}
			else
			{
				if(!sendEMailCommand (sock, "HELO localhost")) goto end;
				//if(!sendEMailCommand (sock, "STARTTLS", 220)) goto end;
				//if(!sendEMailCommand (sock, "AUTH PLAIN AHVzZXIAc2VjcmV0")) goto end;
				if(!sendEMailCommand (sock, "AUTH LOGIN", 334)) goto end;
				if(!sendEMailCommand (sock, "bccr001")) goto end;
				if(!sendEMailCommand (sock, "MAIL FROM: " + from)) goto end;
				if(!sendEMailCommand (sock, "RCPT TO: " + to)) goto end;
				if(!sendEMailCommand (sock, "DATA", 354)) goto end;

				std::string buffer =
					"From: " + from + "\r\n"
					"To: " + to + "\r\n"
					"Subject: " + subject + "\r\n"
					+ formatedBody + "\r\n.";

				if(!sendEMailCommand (sock, buffer)) goto end;
				if(!sendEMailCommand (sock, "QUIT", 221)) goto end;

				ok = true;
			}
			end:
			sock.close();
		}
		else
		{
			MessageBox(NULL, smtpServer.c_str(), "woops", MB_OK);
		}
		return ok;
	}
}