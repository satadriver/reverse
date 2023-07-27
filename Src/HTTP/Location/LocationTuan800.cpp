
#include "StdAfx.h"

#include "LocationTuan800.h"

location::Tuan800::Tuan800(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ме800";
}

location::Tuan800::~Tuan800()
{
	
}

int location::Tuan800::IsTuan800(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("m.api.tuan800.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v4/nearby") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::Tuan800::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::Tuan800::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');

	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	string decvice(http_session_->m_Requestline.m_useragent.GetBuffer(0));
	string imei = decvice.substr(decvice.find("tuan800|") + 8,decvice.find("|Android") - decvice.find("tuan800|") - 8);
	if (imei.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}