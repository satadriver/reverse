
#include "StdAfx.h"

#include "LocationMianBaoLvXing.h"

location::MianBaoLvXing::MianBaoLvXing(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ãæ°üÂÃÐÐ";
}

location::MianBaoLvXing::~MianBaoLvXing()
{
	
}

int location::MianBaoLvXing::IsMianBaoLvXing(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.breadtrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/hasexperience/") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::MianBaoLvXing::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::MianBaoLvXing::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
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