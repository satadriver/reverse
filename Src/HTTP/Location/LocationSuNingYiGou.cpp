
#include "StdAfx.h"

#include "LocationSuNingYiGou.h"

location::SuNingYiGou::SuNingYiGou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ËÕÄþÒ×¹º";
}

location::SuNingYiGou::~SuNingYiGou()
{
	
}

int location::SuNingYiGou::IsSuNingYiGou(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mobts.suning.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/suning-web-mobile/store/getNearStoreList.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::SuNingYiGou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::SuNingYiGou::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("gpsLongitude");
	latitude_ = ms_->GetValueFromMapByKey("gpsLatitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}