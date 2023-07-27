
#include "StdAfx.h"

#include "Blued.h"

Blued::Blued()
{
	software_ = "Blued";
}

Blued::~Blued()
{
	
}

int Blued::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("s.blued.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/users?") != -1 ) )	
	{
		return kDeviceGPSLocation;
	}
	return 0;
}

int Blued::Process(const char *packet,int action)
{
	if (action == kDeviceGPSLocation)
	{
		return DeviceGPSLocation(packet);
	}
	return 0;
}


int Blued::DeviceGPSLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string longitude = ms_->GetValueFromMapByKey("longitude");
	string latitude = ms_->GetValueFromMapByKey("latitude");
	
	if (latitude.empty() || longitude.empty())
	{
		return 0;
	}

	output_text_ = 
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}