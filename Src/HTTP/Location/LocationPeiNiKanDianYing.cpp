
#include "StdAfx.h"

#include "LocationPeiNiKanDianYing.h"

location::PeiNiKanDianYing::PeiNiKanDianYing(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÅãÄã¿´µçÓ°";
}

location::PeiNiKanDianYing::~PeiNiKanDianYing()
{
	
}

int location::PeiNiKanDianYing::IsPeiNiKanDianYing(HttpSession *http_session)
{

	if(  ( http_session->m_Requestline.m_Host.Find("42.121.87.199:8080")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mt/user/loginuser") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::PeiNiKanDianYing::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::PeiNiKanDianYing::DeviceLocation(const char *packet)
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
	
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("longitud");
	latitude_ = ms_->GetValueFromMapByKey("latitude");

	
	if (longitude_.empty() || latitude_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}