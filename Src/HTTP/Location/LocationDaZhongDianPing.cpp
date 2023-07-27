
#include "StdAfx.h"

#include "LocationDaZhongDianPing.h"

location::DaZhongDianPing::DaZhongDianPing(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "´óÖÚµãÆÀ";
}

location::DaZhongDianPing::~DaZhongDianPing()
{
	
}

int location::DaZhongDianPing::IsDaZhongDianPing(HttpSession *http_session)
{
	/*
	if( ( http_session->m_Requestline.m_Host.Find("l.api.dianping.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/rgc.bin") != -1 ) )	
	{
		return kDeviceLocation;
	}*/

	if( ( http_session->m_Requestline.m_URI.Find("/locate/rgc.bin") != -1 )  && 
		( http_session->m_Requestline.m_URI.Find("lat=") != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("lng=") != -1 ))	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::DaZhongDianPing::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::DaZhongDianPing::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,'&','=');
	
	string lng = ms_->GetValueFromMapByKey("lng");
	string lat = ms_->GetValueFromMapByKey("lat");

	
	latitude_ = lng;
	longitude_ = lat;
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}