
#include "StdAfx.h"

#include "LocationDaMai.h"

location::DaMai::DaMai(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "´óÂó";
}

location::DaMai::~DaMai()
{
	
}

int location::DaMai::IsDaMai(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("mapi.damai.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Venue/getCityByXY") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::DaMai::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::DaMai::DeviceLocation(const char *packet)
{
	char *p = (char *)packet;
	if (!p)
	{
		return 0;
	}
	int len = http_session_->m_Requestline.m_nseq - http_session_->m_Requestline.m_Ssequence;
	int i = 0;
	for (;i <= len ; i++,p++)
	{
		if ( 0 == *p)
		{
			*p = '&';
		}
	}
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	imei_ = ms_->GetValueBetweenBeginEnd(src_data,"imei&&","&&",false);
	if (longitude_.empty() || latitude_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}