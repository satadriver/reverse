#include "StdAfx.h"
#include "LocationZaker.h"

location::Zaker::Zaker(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Zaker";
}

location::Zaker::~Zaker()
{
	
}

int location::Zaker::IsZaker(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("iphone.myzaker.com") != -1) && 
		(http_session->m_Requestline.m_URI.Find("/zaker/app_style.php?") != -1 ))	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::Zaker::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::Zaker::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	longitude_ = ms_->GetValueFromMapByKey("_lng");
	latitude_ = ms_->GetValueFromMapByKey("_lat");

	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputString();	
	Record(app_);
	WriteIndexFile();	
	return 1;
}