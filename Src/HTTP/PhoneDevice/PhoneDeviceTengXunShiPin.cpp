
#include "StdAfx.h"

#include "PhoneDeviceTengXunShiPin.h"

phonedevice::TengXunShiPin::TengXunShiPin(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÌÚÑ¶ÊÓÆµ";
}

phonedevice::TengXunShiPin::~TengXunShiPin()
{
	
}

int phonedevice::TengXunShiPin::IsTengXunShiPin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("sdkconfig.video.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getmfomat") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::TengXunShiPin::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::TengXunShiPin::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	version_ = ms_->GetValueFromMapByKey("sysver");
	model_ = ms_->GetValueFromMapByKey("model");
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	
	if (imei_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();

	WriteIndexFile();
	return 1;
}

