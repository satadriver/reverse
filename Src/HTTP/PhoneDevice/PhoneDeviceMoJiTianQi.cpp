
#include "StdAfx.h"

#include "PhoneDeviceMoJiTianQi.h"

phonedevice::MoJiTianQi::MoJiTianQi(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "Ä«¼£ÌìÆø";
}

phonedevice::MoJiTianQi::~MoJiTianQi()
{
	
}

int phonedevice::MoJiTianQi::IsMoJiTianQi(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("register.moji001.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/weather/RegisterUser") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::MoJiTianQi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::MoJiTianQi::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	os_ = ms_->GetValueFromMapByKey("Platform");
	version_ = ms_->GetValueFromMapByKey("SdkVer");
	model_ = ms_->GetValueFromMapByKey("Model",true);
	imei_ = ms_->GetValueFromMapByKey("IMEI");


	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
	
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();

	return 1;
}

