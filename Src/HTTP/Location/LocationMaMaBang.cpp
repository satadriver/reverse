
#include "StdAfx.h"

#include "LocationMaMaBang.h"

location::MaMaBang::MaMaBang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÂèÂè°ï";
}

location::MaMaBang::~MaMaBang()
{
	
}

int location::MaMaBang::IsMaMaBang(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("mobile.mmbang.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api4/") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::MaMaBang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::MaMaBang::DeviceLocation(const char *packet)
{
	string src_data;
	char *hd = strstr(packet,"\r\n\r\n");
	if (hd)
	{
		string tmp(hd + strlen("\r\n\r\n"));
		src_data = tmp;
	}
	else
	{
		string tmp(packet);
		src_data = tmp;
	}

	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
 	imei_ = ms_->GetValueFromMapByKey("device_id");
	
	if (longitude_.empty() || latitude_.empty()  || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}