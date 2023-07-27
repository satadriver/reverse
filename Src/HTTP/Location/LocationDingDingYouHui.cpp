
#include "StdAfx.h"

#include "LocationDingDingYouHui.h"

location::DingDingYouHui::DingDingYouHui(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¶¡¶¡ÓÅ»Ý";
}

location::DingDingYouHui::~DingDingYouHui()
{
	
}

int location::DingDingYouHui::IsDingDingYouHui(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("msv.ddmap.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/rest/v1/map/getcitybyxy.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::DingDingYouHui::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::DingDingYouHui::DeviceLocation(const char *packet)
{

	string src_data(packet);
	ms_->Replace(src_data,"\r\n","&");
	ms_->SplitStringToMap(src_data,'&',':');
	
	imei_ = ms_->GetValueFromMapByKey("primarykey");
	mac_ = ms_->GetValueFromMapByKey("macaddr");
	longitude_ = ms_->GetValueFromMapByKey("Lon");
	latitude_ = ms_->GetValueFromMapByKey("Lat");
	
	if (imei_.empty() ||mac_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}