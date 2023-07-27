
#include "StdAfx.h"

#include "LocationAiBa.h"

location::AiBa::AiBa(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°®°É";
}

location::AiBa::~AiBa()
{
	
}

int location::AiBa::IsAiBa(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("api.aiba.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api_v5.7/mclient.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::AiBa::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::AiBa::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("lon");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	imei_ = ms_->GetValueFromMapByKey("deviceid");

	if (longitude_.empty() || latitude_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();

	return 1;
}