
#include "StdAfx.h"

#include "hardwareliantong.h"

hardwareinfo::HardWareLianTong::HardWareLianTong(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "联通营业厅";
}

hardwareinfo::HardWareLianTong::~HardWareLianTong()
{
	
}

int hardwareinfo::HardWareLianTong::IsHardWareLianTong(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("m.client.10010.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mobileService")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareLianTong::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareLianTong::hardwareinfomation(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("deviceCode",true);
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