
#include "StdAfx.h"

#include "RunningAvast.h"

time_t running::Avast::last_time_ = 0;

running::Avast::Avast(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "Avast";
	version_ = "9.0.2108+";
}

running::Avast::~Avast()
{
	
}

int running::Avast::IsAvast(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("content.cdn.viber.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/stickers/notifications.json") != -1 ) )	
	{
		return 1;
	}
	return 0;
}

int running::Avast::Process(const char *packet,int action)
{
	time_t now_time = time(0);
	if (now_time - last_time_ > 3 * 60)
	{
		output_text_ = 
			"webname=" + software_ + "\r\n" +
			"msgtype=" + version_ + "\r\n" + 
			"type=µÁƒ‘\r\n" + 
			"state= π”√\r\n";
		
		WriteIndexFile();
		last_time_ = now_time;
	}
	
	return 1;
}

