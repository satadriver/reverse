
#include "StdAfx.h"

#include "KuaiLeBeiYun.h"

KuaiLeBeiYun::KuaiLeBeiYun()
{
	software_ = "¿ìÀÖ±¸ÔÐ";
}

KuaiLeBeiYun::~KuaiLeBeiYun()
{
	
}

int KuaiLeBeiYun::Is(HttpSession *http_session)
{
	
	if( ( http_session->m_Requestline.m_Host.Find("www.babytree.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/mobile_knowledge/sync_knowledge") != -1 ) )	
	{
		return kDeviceGPSLocation;
	}
	return 0;
}

int KuaiLeBeiYun::Process(const char *packet,int action)
{
	if (action == kDeviceGPSLocation)
	{
		return DeviceGPSLocation(packet);
	}
	return 0;
}

int KuaiLeBeiYun::DeviceGPSLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');

	string longitude = ms_->GetValueFromMapByKey("longitude");
	string latitude = ms_->GetValueFromMapByKey("latitude");
	string imei = ms_->GetValueFromMapByKey("imei");
	
	if (longitude.empty() || latitude.empty()  || imei.empty())
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