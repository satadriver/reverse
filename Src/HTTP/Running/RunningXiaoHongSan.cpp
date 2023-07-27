
#include "StdAfx.h"

#include "RunningXiaoHongSan.h"

time_t running::XiaoHongSan::last_time_ = 0;

running::XiaoHongSan::XiaoHongSan(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "小红伞";
	version_ = "14.0.6.552+";
}

running::XiaoHongSan::~XiaoHongSan()
{
	
}

int running::XiaoHongSan::IsXiaoHongSan(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("personal.avira-update.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/update/wks_avira") != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("update.dll.gz") != -1 ))	
	{
		return 1;
	}
	return 0;
}


int running::XiaoHongSan::Process(const char *packet,int action)
{
	time_t now_time = time(0);
	if (now_time - last_time_ > 3 * 60)
	{
		output_text_ = 
			"webname=" + software_ + "\r\n" +
			"msgtype=" + version_ + "\r\n" + 
			"type=电脑\r\n" + 
			"state=使用\r\n";
		
		WriteIndexFile();
		last_time_ = now_time;
	}
	
	return 1;
}