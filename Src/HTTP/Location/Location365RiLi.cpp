
#include "StdAfx.h"

#include "Location365RiLi.h"

location::RiLi365::RiLi365(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "365ÈÕÀú";
}

location::RiLi365::~RiLi365()
{
	
}

int location::RiLi365::IsRiLi365(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("weather.365rili.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/get_city_info.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::RiLi365::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::RiLi365::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	longitude_ =  ms_->GetValueFromMapByKey("lng");
	latitude_ =  ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}