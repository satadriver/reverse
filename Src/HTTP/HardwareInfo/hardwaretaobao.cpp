
#include "StdAfx.h"

#include "hardwaretaobao.h"

hardwareinfo::HardWareTaoBao::HardWareTaoBao(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "淘宝";	
}

hardwareinfo::HardWareTaoBao::~HardWareTaoBao()
{
	
}

int hardwareinfo::HardWareTaoBao::IsHardWareTaoBao(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("amdc.m.taobao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/amdc/mobileDispatch")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareTaoBao::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareTaoBao::hardwareinfomation(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("deviceId",true);
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