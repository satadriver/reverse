
#include "StdAfx.h"

#include "LocationBlued.h"

location::Blued::Blued(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Blued";
}

location::Blued::~Blued()
{
	
}

int location::Blued::IsBlued(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("s.blued.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/users?") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::Blued::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::Blued::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}