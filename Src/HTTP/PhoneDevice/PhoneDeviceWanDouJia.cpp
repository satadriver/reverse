
#include "StdAfx.h"

#include "PhoneDeviceWanDouJia.h"

phonedevice::WanDouJia::WanDouJia(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "Íã¶¹¼Ô";
}

phonedevice::WanDouJia::~WanDouJia()
{
	
}

int phonedevice::WanDouJia::IsWanDouJia(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("apis.wandoujia.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/five/v1/index") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::WanDouJia::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::WanDouJia::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
    
/*
	string cpu = ms_->GetValueFromMapByKey("deviceCPU",true);
	string br = ms_->GetValueFromMapByKey("deviceBrand",true);
	string ppi = ms_->GetValueFromMapByKey("resolution",true);
*/

	model_ = ms_->GetValueFromMapByKey("deviceModel",true);
	version_ = ms_->GetValueFromMapByKey("sdk");
	os_ = GetOS();
	
	if ( version_.empty() || model_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

