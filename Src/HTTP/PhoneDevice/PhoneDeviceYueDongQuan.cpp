
#include "StdAfx.h"

#include "PhoneDeviceYueDongQuan.h"

phonedevice::YueDongQuan::YueDongQuan(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ÔÃ¶¯È¦";
}

phonedevice::YueDongQuan::~YueDongQuan()
{
	
}

int phonedevice::YueDongQuan::IsYueDongQuan(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("api.51yund.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/banner/get_openad_banner_v2") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::YueDongQuan::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::YueDongQuan::UploadPhoneDevice(const char *packet)
{

	string src_data(packet);

	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}

	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");

	imei_ = ms_->GetValueFromMapByKey("deviceId",true);
	version_ = ms_->GetValueFromMapByKey("osVersion",true);
	model_ = ms_->GetValueFromMapByKey("phone_type",true);
	os_ = GetOS();
	longitude_ = ms_->GetValueFromMapByKey("longitude",true);
	latitude_ = ms_->GetValueFromMapByKey("latitude",true);

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty() || longitude_.empty() || latitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

