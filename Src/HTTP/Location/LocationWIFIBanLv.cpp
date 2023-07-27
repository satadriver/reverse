
#include "StdAfx.h"

#include "LocationWifiBanLv.h"

location::WIFIBanLv::WIFIBanLv(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "WIFI°éÂÂ";
}

location::WIFIBanLv::~WIFIBanLv()
{
	
}

int location::WIFIBanLv::IsWIFIBanLv(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("ads.miidi.net:80")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/appscore4/active.bin") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::WIFIBanLv::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::WIFIBanLv::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	mac_ = ms_->GetValueFromMapByKey("mac_address",true);
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (imei_.empty() ||mac_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}