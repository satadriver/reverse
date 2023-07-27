
#include "StdAfx.h"

#include "PhoneDeviceYouKu.h"

phonedevice::YouKu::YouKu(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "се©А";
}

phonedevice::YouKu::~YouKu()
{
	
}

int phonedevice::YouKu::IsYouKu(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.mobile.youku.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/openapi-wireless/initial") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::YouKu::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::YouKu::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	os_ = ms_->GetValueFromMapByKey("os");
	version_ = ms_->GetValueFromMapByKey("os_ver");
	model_ = ms_->GetValueFromMapByKey("btype");
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	
	if (os_.empty() ||  imei_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

