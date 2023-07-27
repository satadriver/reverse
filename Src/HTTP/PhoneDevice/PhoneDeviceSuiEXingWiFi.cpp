
#include "StdAfx.h"

#include "PhoneDeviceSuiEXingWiFi.h"

phonedevice::SuiEXingWiFi::SuiEXingWiFi(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ËæeÐÐwifi";
}

phonedevice::SuiEXingWiFi::~SuiEXingWiFi()
{
	
}

int phonedevice::SuiEXingWiFi::IsSuiEXingWiFi(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("wlan.10086.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/interface/batchUpload.service") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::SuiEXingWiFi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::SuiEXingWiFi::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	model_ = ms_->GetValueBetweenBeginEnd(src_data,"<ua>","</ua>");
	version_ = ms_->GetValueBetweenBeginEnd(src_data,"<osVersion>","</osVersion>");
	os_ = ms_->GetValueBetweenBeginEnd(src_data,"<osPlatform>","</osPlatform>");

	imei_ = ms_->GetValueBetweenBeginEnd(src_data,"<imei>","</imei>");
	if (model_.empty() || version_.empty() || os_.empty() || imei_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

