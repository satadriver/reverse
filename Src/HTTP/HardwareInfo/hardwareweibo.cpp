
#include "StdAfx.h"

#include "hardwareweibo.h"

hardwareinfo::HardWareWeiBo::HardWareWeiBo(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "新浪微博";
}

hardwareinfo::HardWareWeiBo::~HardWareWeiBo()
{
	
}

int hardwareinfo::HardWareWeiBo::IsHardWareWeiBo(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("sdkapp.mobile.sina.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/interface/sdk/")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareWeiBo::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareWeiBo::hardwareinfomation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("aduserid",true);
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