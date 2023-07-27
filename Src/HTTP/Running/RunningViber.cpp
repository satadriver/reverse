
#include "StdAfx.h"

#include "RunningViber.h"

time_t running::Viber::last_time_ = 0;

running::Viber::Viber(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "Viber";
	version_ = "1.16.0005+";
}

running::Viber::~Viber()
{
	
}

int running::Viber::IsViber(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("content.cdn.viber.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/stickers/notifications.json") != -1 ) )	
	{
		return 1;
	}
	return 0;
}

int running::Viber::Process(const char *packet,int action)
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