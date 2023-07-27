
#include "StdAfx.h"

#include "RunningSkout.h"

time_t running::Skout::last_time_ = 0;
running::Skout::Skout(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "Skout";
	version_ = "4.8.26+";
}

running::Skout::~Skout()
{
	
}

int running::Skout::IsSkout(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("a20.skout.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/services/2.0/") != -1 ) )	
	{
		return 1;
	}
	return 0;
}


int running::Skout::Process(const char *packet,int action)
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