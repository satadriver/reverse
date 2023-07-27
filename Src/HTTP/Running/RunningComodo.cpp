
#include "StdAfx.h"

#include "RunningComodo.h"

time_t running::Comodo::last_time_ = 0;

running::Comodo::Comodo(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "Comodo";
	version_ = "7.0.317799.4142+";
}

running::Comodo::~Comodo()
{
	
}

int running::Comodo::IsComodo(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("download.comodo.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/av/updatesurl/versioninfo.ini") != -1 ) )	
	{
		return 1;
	}
	return 0;
}


int running::Comodo::Process(const char *packet,int action)
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