
#include "StdAfx.h"

#include "hardwarekugou.h"

hardwareinfo::HardWareKuGou::HardWareKuGou(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "酷狗音乐";
}

hardwareinfo::HardWareKuGou::~HardWareKuGou()
{
	
}

int hardwareinfo::HardWareKuGou::IsHardWareKuGou(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("install2.kugou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mobileuc/usercount.aspx")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareKuGou::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareKuGou::hardwareinfomation(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("machineid",true);
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