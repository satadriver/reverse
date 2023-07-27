
#include "StdAfx.h"

#include "PhoneDeviceWIFIWangNengYaoShi.h"

phonedevice::WiFiWangNengYaoShi::WiFiWangNengYaoShi(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "WIFIÍòÄÜÔ¿³×";
}

phonedevice::WiFiWangNengYaoShi::~WiFiWangNengYaoShi()
{
	
}

int phonedevice::WiFiWangNengYaoShi::IsWiFiWangNengYaoShi(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("a.wkanx.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/adx?") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::WiFiWangNengYaoShi::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::WiFiWangNengYaoShi::UploadPhoneDevice(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");

	model_ = ms_->GetValueFromMapByKey("dv",true);
	version_ = ms_->GetValueFromMapByKey("osv",true);
	os_ = ms_->GetValueFromMapByKey("os",true);
	imei_ = ms_->GetValueFromMapByKey("ci",true);
	if ( version_.empty() || model_.empty() || os_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

