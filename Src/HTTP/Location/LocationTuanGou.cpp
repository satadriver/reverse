
#include "StdAfx.h"

#include "LocationTuanGou.h"

location::TuanGou::TuanGou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÍÅ¹º";
}

location::TuanGou::~TuanGou()
{
	
}

int location::TuanGou::IsTuanGou(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("m.api.dianping.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/rgc.bin") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::TuanGou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::TuanGou::DeviceLocation(const char *packet)
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
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"&&pragma-device&&","&&pragma-dpid");

	if (imei.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}