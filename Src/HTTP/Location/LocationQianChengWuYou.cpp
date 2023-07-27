
#include "StdAfx.h"

#include "LocationQianChengWuYou.h"

location::QianChengWuYou::QianChengWuYou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ç°³ÌÎÞÓÇ";
}

location::QianChengWuYou::~QianChengWuYou()
{
	
}

int location::QianChengWuYou::IsQianChengWuYou(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.51job.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/util/get_location.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::QianChengWuYou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::QianChengWuYou::DeviceLocation(const char *packet)
{
	string src_data;
	char *hd = strstr(packet,"\r\n\r\n");
	if (hd)
	{
		string tmp(hd);
		src_data = tmp;
	}
	else
	{
		string tmp(packet);
		src_data = tmp;
	}

	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("baiduloc_longitude");
	latitude_ = ms_->GetValueFromMapByKey("baiduloc_latitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}