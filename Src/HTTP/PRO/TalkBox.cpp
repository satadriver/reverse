
#include "StdAfx.h"

#include "TalkBox.h"

TalkBox::TalkBox()
{
	
}

TalkBox::~TalkBox()
{
	
}

int TalkBox::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("www.mytalkbox.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/talkbox/api/user?action=login") != -1 )  )
	{
		return kLoginM;
	}
	return 0;
}

int TalkBox::Process(const char *packet,int action)
{
	if (action == kLoginM)
	{
		return LoginM(packet);
	}
	return 0;
}


int TalkBox::LoginM(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string imei = ms_->GetValueFromMapByKey("udId");
	string user = ms_->GetValueFromMapByKey("username");
	string password = ms_->GetValueFromMapByKey("password");
	if (user.empty() || password.empty())
	{
		return 0;
	}

	output_text_ = 
		"imei=" + imei + "\r\n" + 
		"user=" + user + "\r\n" +
		"pass=" + password + "\r\n" +
		"state=µÇÂ¼\r\n";
	WriteIndexFile();
	return 1;
}

