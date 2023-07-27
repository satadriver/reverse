
#include "StdAfx.h"

#include "PhoneDeviceZaker.h"

phonedevice::Zaker::Zaker(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "Zaker";
}

phonedevice::Zaker::~Zaker()
{
	
}

int phonedevice::Zaker::IsZaker(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("iphone.myzaker.com") != -1) && 
		(http_session->m_Requestline.m_URI.Find("/zaker/app_style.php?") != -1 ))
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::Zaker::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::Zaker::UploadPhoneDevice(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	version_ = ms_->GetValueFromMapByKey("_lng");
	model_ = ms_->GetValueFromMapByKey("_lat");
	imei_ = ms_->GetValueFromMapByKey("_lat");
	os_ = ms_->GetValueFromMapByKey("_lat");
	
	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
	
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

