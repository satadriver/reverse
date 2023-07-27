
#include "StdAfx.h"

#include "LocationSuieXingWlan.h"

location::SuiEXingWLAN::SuiEXingWLAN(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ËæEÐÐWLAN";
}

location::SuiEXingWLAN::~SuiEXingWLAN()
{
	
}

int location::SuiEXingWLAN::IsSuiEXingWLAN(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("wlan.10086.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/interface/queryHotpointInfoByAround.service") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::SuiEXingWLAN::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::SuiEXingWLAN::DeviceLocation(const char *packet)
{
	string src_data(packet);
	//ms_->Replace(src_data,"\r\n","&");
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueBetweenBeginEnd(src_data,"<imei>","</imei>");
	imsi_ = ms_->GetValueBetweenBeginEnd(src_data,"<imsi>","</imsi>");
	longitude_ = ms_->GetValueBetweenBeginEnd(src_data,"<longitude>","</longitude>");
	latitude_ = ms_->GetValueBetweenBeginEnd(src_data,"<latitude>","</latitude>");
	
	if (imei_.empty() ||imsi_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}