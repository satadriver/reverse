
#include "StdAfx.h"

#include "LocationChangWuXian.h"

location::ChangWuXian::ChangWuXian(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "³©ÎÞÏß";
}

location::ChangWuXian::~ChangWuXian()
{
	
}

int location::ChangWuXian::IsChangWuXian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("ap.wlanplus.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/loc/myNearPoi") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ChangWuXian::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);    
	}
	return 0;
}


int location::ChangWuXian::DeviceLocation(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("lon");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if ( latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}