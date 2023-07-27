
#include "StdAfx.h"

#include "LocationAnJuKe.h"

location::AnJuKe::AnJuKe(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°²¾Ó¿Í";
}

location::AnJuKe::~AnJuKe()
{
	
}

int location::AnJuKe::IsAnJuKe(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.anjuke.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mobile/1.3/location.getCity") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::AnJuKe::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::AnJuKe::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("i");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (imei_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}