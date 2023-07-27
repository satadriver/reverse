
#include "StdAfx.h"

#include "ChatingTalkBox.h"

chating::TalkBox::TalkBox(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::TalkBox::~TalkBox()
{
	
}

int chating::TalkBox::IsTalkBox(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("www.mytalkbox.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/talkbox/api/user?action=login") != -1 )  )
	{
		return kLoginM;
	}
	return 0;
}

int chating::TalkBox::Process(const char *packet,int action)
{
	if (action == kLoginM)
	{
		return LoginM(packet);
	}
	return 0;
}


int chating::TalkBox::LoginM(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string imei = ms_->GetValueFromMapByKey("udId");
	user_ = ms_->GetValueFromMapByKey("username");
	password_ = ms_->GetValueFromMapByKey("password");
	if (user_.empty() || password_.empty())
	{
		return 0;
	}

	output_text_ = 
		"imei=" + imei + "\r\n" + 
		"user=" + user_ + "\r\n" +
		"pass=" + password_ + "\r\n" +
		"state=µÇÂ¼\r\n";
	WriteIndexFile();
	return 1;
}

