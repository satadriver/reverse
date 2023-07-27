
#include "StdAfx.h"

#include "hardwareimooc.h"

hardwareinfo::HardWareImooc::HardWareImooc(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "爱慕课";	
}

hardwareinfo::HardWareImooc::~HardWareImooc()
{
	
}

int hardwareinfo::HardWareImooc::IsHardWareImooc(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("www.imooc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api3/userinfo")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareImooc::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareImooc::hardwareinfomation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("uuid",true);
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