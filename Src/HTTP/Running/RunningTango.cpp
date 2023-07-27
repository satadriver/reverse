
#include "StdAfx.h"

#include "RunningTango.h"

time_t running::Tango::last_time_ = 0;

running::Tango::Tango(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "Tango";
	version_ = "3.2.67659+";
}

running::Tango::~Tango()
{
	
}

int running::Tango::IsTango(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("configurator.tango.me:8080")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/configurator/property.xml") != -1 ) )	
	{
		return 1;
	}
	return 0;
}


int running::Tango::Process(const char *packet,int action)
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