
#include "StdAfx.h"

#include "hardwaretengxunshipin.h"

hardwareinfo::HardWareTengxunShipin::HardWareTengxunShipin(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
		app_ = "腾讯视频";
}

hardwareinfo::HardWareTengxunShipin::~HardWareTengxunShipin()
{
	
}

int hardwareinfo::HardWareTengxunShipin::IsHardWareTengxunShipin(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("sdkconfig.video.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/getmfomat")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareTengxunShipin::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareTengxunShipin::hardwareinfomation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("device_id",true);
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