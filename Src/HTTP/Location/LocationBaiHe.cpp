
#include "StdAfx.h"

#include "LocationBaiHe.h"

location::BaiHe::BaiHe(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°ÙºÏÍø";
}

location::BaiHe::~BaiHe()
{
	
}

int location::BaiHe::IsBaiHe(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("mi.baihe.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/index.php?act=logon") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::BaiHe::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::BaiHe::DeviceLocation(const char *packet)
{
	char *p = strstr(packet,"\r\n\r\n");
	if (!p)
	{
		return 0;
	}
	string src_data(p);
	ms_->SplitStringToMap(src_data,'&','=',"\r\n");
	
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	imei_ = ms_->GetValueFromMapByKey("device");

	
	if (longitude_.empty() || latitude_.empty() || imei_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}