
#include "StdAfx.h"

#include "RunningLine.h"

time_t running::Line::last_time_ = 0;

running::Line::Line(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "Line";
	version_ = "4.50+";
}

running::Line::~Line()
{
	
}

int running::Line::IsLine(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("terms.line.me")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("line_terms_chinese/sp") != -1 ) )	
	{
		return 1;
	}
	return 0;
}


int running::Line::Process(const char *packet,int action)
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