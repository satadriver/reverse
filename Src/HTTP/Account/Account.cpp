#include "StdAfx.h"
#include "Account.h"

account::Account::Account(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

account::Account::~Account()
{
	
}

int account::Account::ProcessAccount(HttpSession *http_session ,const char *packet)
{	
	int action = -1;
	return 0;
}

string account::Account::GetOutputText()
{	
	return
		"user=" +	 user_ + "\r\n" + 
		"pass=" +	 password_ + "\r\n" + 
		"msgtype=" + software_ + "\r\n" +
		"state=" +	 state_ + "\r\n";
}