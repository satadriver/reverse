
#include "StdAfx.h"

#include "LocationJingDianTeJia.h"

location::JingDianTeJia::JingDianTeJia(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "景点特价门票";
}

location::JingDianTeJia::~JingDianTeJia()
{
	
}

int location::JingDianTeJia::IsJingDianTeJia(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.online.travel.meishixing.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/other/common/location/") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::JingDianTeJia::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::JingDianTeJia::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','='); 
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}