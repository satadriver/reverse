
#include "StdAfx.h"

#include "YouKu.h"

YouKu::YouKu()
{
	software_ = "се©А";
}

YouKu::~YouKu()
{
	
}

int YouKu::Is(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.mobile.youku.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/openapi-wireless/initial") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int YouKu::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int YouKu::UploadDeviceInfoM(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	string os = ms_->GetValueFromMapByKey("os");
	string version = ms_->GetValueFromMapByKey("os_ver");
	string model = ms_->GetValueFromMapByKey("btype");
	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	
	if (os.empty() ||  imei.empty() || version.empty() || model.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"imsi=" + imsi + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

