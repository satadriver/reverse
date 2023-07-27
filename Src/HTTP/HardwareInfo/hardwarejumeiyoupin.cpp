
#include "StdAfx.h"

#include "hardwarejumeiyoupin.h"

hardwareinfo::HardWareJumeiYoupin::HardWareJumeiYoupin(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "聚美优品";
}

hardwareinfo::HardWareJumeiYoupin::~HardWareJumeiYoupin()
{
	
}

int hardwareinfo::HardWareJumeiYoupin::IsHardWareJumeiYoupin(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("mobile.jumei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/msapi/config/forbidden")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareJumeiYoupin::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareJumeiYoupin::hardwareinfomation(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->Replace(src_data,";","&");
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