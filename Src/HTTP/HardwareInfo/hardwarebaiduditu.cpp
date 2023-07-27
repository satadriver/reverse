
#include "StdAfx.h"

#include "hardwarebaiduditu.h"

hardwareinfo::HardWareBaiduDitu::HardWareBaiduDitu(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "百度地图";	
}

hardwareinfo::HardWareBaiduDitu::~HardWareBaiduDitu()
{
	
}

int hardwareinfo::HardWareBaiduDitu::IsHardWareBaiduDitu(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("client.map.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/imap/ulog/")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareBaiduDitu::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareBaiduDitu::hardwareinfomation(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	mid = ms_->GetValueBetweenBeginEnd(src_data,"cuid=","|");
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