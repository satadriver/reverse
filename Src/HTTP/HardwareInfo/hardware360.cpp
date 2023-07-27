
#include "StdAfx.h"

#include "hardware360.h"

hardwareinfo::HardWare360::HardWare360(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "360安全卫士";
}

hardwareinfo::HardWare360::~HardWare360()
{
	
}

int hardwareinfo::HardWare360::IsHardWare360(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("s.360.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/instcomp.htm")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWare360::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWare360::hardwareinfomation(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("m",true);
	mid = mid.substr(0,32);
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