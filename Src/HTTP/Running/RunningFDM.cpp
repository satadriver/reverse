
#include "StdAfx.h"

#include "RunningFDM.h"


running::FDM::FDM(HttpSession *http_session, const string &pro_name) : Running(http_session,pro_name)
{
	software_ = "FDM";
}

running::FDM::~FDM()
{
	
}

int running::FDM::IsFDM(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("fdm.freedownloadmanager.org")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/fromfdm/url.") != -1 ) && 
		( http_session->m_Requestline.m_useragent.Find("FDM") != -1))	
	{
		return 1;
	}
	return 0;
}


int running::FDM::Process(const char *packet,int action)
{

	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	string::size_type pos = uri.rfind("/");
	if (pos != string::npos)
	{
		string content = uri.substr(pos);
		output_text_ = 
			"webname=" + software_ + "\r\n" +
			"msg=" + content + "\r\n" +
			"type=µÁƒ‘\r\n" + 
			"state= π”√\r\n";
		
		WriteIndexFile();
		return 1;
	}
	return 0;
}