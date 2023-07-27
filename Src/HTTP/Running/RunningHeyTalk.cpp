
#include "StdAfx.h"

#include "RunningHeyTalk.h"

time_t running::HeyTalk::last_time_ = 0;

running::HeyTalk::HeyTalk(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "HeyTalk";
	version_ = "3.0.9.349+";
}

running::HeyTalk::~HeyTalk()
{
	
}

int running::HeyTalk::IsHeyTalk(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("push.heytell.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/VoxilateServer/http/NewMessages") != -1 ) )	
	{
		return 1;
	}
	return 0;
}


int running::HeyTalk::Process(const char *packet,int action)
{
	

	time_t now_time = time(0);
	if (now_time - last_time_ > 3 * 60)
	{
		output_text_ = 
			"webname=" + software_ + "\r\n" +
			"msgtype=" + version_ + "\r\n" + 
			"type=手机\r\n" + 
			"state=使用\r\n";

		WriteIndexFile();
		last_time_ = now_time;
	}
	return 1;
}