
#include "StdAfx.h"

#include "LocationDouBan.h"

location::DouBan::DouBan(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¶¹°ê";
}

location::DouBan::~DouBan()
{
	
}

int location::DouBan::IsDouBan(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("api.douban.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v2/movie/cinemas/nearby") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::DouBan::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::DouBan::DeviceLocation(const char *packet)
{
	/*char *p = (char *)packet;
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
	}*/
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");

	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}