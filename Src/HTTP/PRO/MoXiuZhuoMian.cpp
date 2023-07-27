
#include "StdAfx.h"

#include "MoXiuZhuoMian.h"

MoXiuZhuoMian::MoXiuZhuoMian()
{
	software_ = "Ä§Ðã×ÀÃæ";
}

MoXiuZhuoMian::~MoXiuZhuoMian()
{
	
}

int MoXiuZhuoMian::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("weather.moxiu.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/json.php?do=Weather.Detail&addr") != -1 ) )	
	{
		return kDeviceGPSLocation;
	}
	return 0;
}

int MoXiuZhuoMian::Process(const char *packet,int action)
{
	if (action == kDeviceGPSLocation)
	{
		return DeviceGPSLocation(packet);
	}
	return 0;
}


int MoXiuZhuoMian::DeviceGPSLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	
	string imei = ms_->GetValueFromMapByKey("imei");
	string longitude = ms_->GetValueFromMapByKey("longitude");
	string latitude = ms_->GetValueFromMapByKey("latitude");
	
	if (imei.empty() || latitude.empty() || longitude.empty())
	{
		return 0;
	}

	output_text_ = 
		"oid=" + imei + "\r\n" +
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}