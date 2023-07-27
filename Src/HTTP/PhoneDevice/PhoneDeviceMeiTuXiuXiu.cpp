
#include "StdAfx.h"

#include "PhoneDeviceMeiTuXiuXiu.h"

phonedevice::MeiTuXiuXiu::MeiTuXiuXiu(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÃÀÍ¼ÐãÐã";
}

phonedevice::MeiTuXiuXiu::~MeiTuXiuXiu()
{
	
}

int phonedevice::MeiTuXiuXiu::IsMeiTuXiuXiu(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("data.mobile.meitu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("install.php") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::MeiTuXiuXiu::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::MeiTuXiuXiu::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	version_ = ms_->GetValueFromMapByKey("ios");
	model_ = ms_->GetValueFromMapByKey("itype");
	imei_ = ms_->GetValueFromMapByKey("uid");
	os_ = GetOS();

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
	
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

