
#include "StdAfx.h"

#include "LocationWeiDaiJia.h"

location::WeiDaiJia::WeiDaiJia(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Î¢´ú¼Ý";
}

location::WeiDaiJia::~WeiDaiJia()
{
	
}

int location::WeiDaiJia::IsWeiDaiJia(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("www.weidaijia.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/data.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::WeiDaiJia::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::WeiDaiJia::DeviceLocation(const char *packet)
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
	
	imei_ = ms_->GetValueFromMapByKey("uuid");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	
	if (imei_.empty()  || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}