
#include "StdAfx.h"

#include "LocationHeMap.h"

location::HeMap::HeMap(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ºÍµØÍ¼";
}

location::HeMap::~HeMap()
{
	
}

int location::HeMap::IsHeMap(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("nav7.mlocso.com:8003")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/DispatchServer/walk/resource") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::HeMap::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::HeMap::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("x");
	latitude_ = ms_->GetValueFromMapByKey("y");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}