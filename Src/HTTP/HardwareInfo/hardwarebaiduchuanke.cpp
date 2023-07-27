
#include "StdAfx.h"

#include "hardwarebaiduchuanke.h"

hardwareinfo::HardWareBaiduChuanKe::HardWareBaiduChuanKe(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
		app_ = "百度传课";
}

hardwareinfo::HardWareBaiduChuanKe::~HardWareBaiduChuanKe()
{
	
}

int hardwareinfo::HardWareBaiduChuanKe::IsHardWareBaiduChuanKe(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("api2.sharesdk.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/conf3")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareBaiduChuanKe::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareBaiduChuanKe::hardwareinfomation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("device",true);
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