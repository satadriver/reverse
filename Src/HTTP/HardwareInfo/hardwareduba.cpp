
#include "StdAfx.h"
#include "hardwareduba.h"

hardwareinfo::HardWareDuBa::HardWareDuBa(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "金山毒霸";
}

hardwareinfo::HardWareDuBa::~HardWareDuBa()
{
	
}

int hardwareinfo::HardWareDuBa::IsHardWareDuBa(HttpSession *http_session)
{
	//GET /cv?uuid=6fcb5516dbd07cebbfce0f2668547bde&tc=8659163&p=kxescore.exe&c=1335 Host: cv.duba.net
	if((http_session->m_Requestline.m_Host.Find("cv.duba.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cv?uuid=")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareDuBa::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareDuBa::hardwareinfomation(const char *packet)
{
	//URL 中得到数据
	//数据部分
	string url(http_session_->m_Requestline.m_URI.GetBuffer(0));
	url = ms_->UrlDecode(url);
	if (url.find("uuid=") != string::npos)
	{
		url = url.substr(url.find("uuid="));
	}
	ms_->SplitStringToMap(url,'&','=');
	mid = ms_->GetValueFromMapByKey("uuid"); 
	if (mid.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + mid + "\r\n" +
		"nick=" + app_ + "\r\n" +
		"state=硬件特征信息\r\n";
	WriteIndexFile();
	return 1;
}