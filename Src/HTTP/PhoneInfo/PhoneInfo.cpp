#include "StdAfx.h"
#include "PhoneInfo.h"

phoneinfo::PhoneInfo::PhoneInfo(HttpSession *http_session, const string &pro_name) : HttpPro(http_session,pro_name)
{
	
}

phoneinfo::PhoneInfo::~PhoneInfo()
{
	
}

string phoneinfo::PhoneInfo::GetOutputText()
{
	return 
		"state=" + app_ + "\r\n";
}