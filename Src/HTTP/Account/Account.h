#ifndef HTTP_ACCOUNT_ACCOUNT_H_H_H
#define HTTP_ACCOUNT_ACCOUNT_H_H_H

#include "../HttpPro.h"

namespace account
{
	class Account : public HttpPro
	{
	public:
		Account(HttpSession *http_session, const string &pro_name);
		virtual ~Account();
		
		static int ProcessAccount(HttpSession *http_session ,const char *packet);
		
	protected:
		string GetOutputText();
		string user_;
		string password_;
		string software_;
		string state_;
	};
}

#endif