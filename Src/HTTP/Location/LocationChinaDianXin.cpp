
#include "StdAfx.h"

#include "LocationChinaDianXin.h"

location::ChinaDianXin::ChinaDianXin(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "中国电信";
}

location::ChinaDianXin::~ChinaDianXin()
{
	
}

int location::ChinaDianXin::IsChinaDianXin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("116.228.55.155:6060")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/dataquery/query") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ChinaDianXin::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::ChinaDianXin::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("cenx");
	latitude_ = ms_->GetValueFromMapByKey("ceny");
	
	if ( latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}