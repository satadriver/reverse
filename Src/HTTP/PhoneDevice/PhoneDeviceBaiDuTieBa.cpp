
#include "StdAfx.h"

#include "PhoneDeviceBaiDuTieBa.h"

phonedevice::BaiDuTieBa::BaiDuTieBa(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "°Ù¶ÈÌù°É";
}

phonedevice::BaiDuTieBa::~BaiDuTieBa()
{
	
}

int phonedevice::BaiDuTieBa::IsBaiDuTieBa(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("c.tieba.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/c/s/inpv") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::BaiDuTieBa::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::BaiDuTieBa::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);

	ms_->SplitStringToMap(src_data,'&','=');

	version_ = ms_->GetValueFromMapByKey("os_version");
	model_ = ms_->GetValueFromMapByKey("model",true);
	imei_ = ms_->GetValueFromMapByKey("_phone_imei");
	os_ = GetOS();

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

