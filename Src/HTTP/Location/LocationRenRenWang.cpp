
#include "StdAfx.h"

#include "LocationRenRenWang.h"

location::RenRenWang::RenRenWang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÈËÈËÍø";
}

location::RenRenWang::~RenRenWang()
{
	
}

int location::RenRenWang::IsRenRenWang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.m.renren.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/lbs/locate") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::RenRenWang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::RenRenWang::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	longitude_ = ms_->GetValueFromMapByKey("base_station_longitude");
	latitude_ = ms_->GetValueFromMapByKey("base_station_latitude");
	
	if (imei.empty() ||imsi.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}