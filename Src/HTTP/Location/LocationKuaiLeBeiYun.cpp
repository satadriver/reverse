
#include "StdAfx.h"

#include "LocationKuaiLeBeiYun.h"

location::KuaiLeBeiYun::KuaiLeBeiYun(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¿ìÀÖ±¸ÔÐ";
}

location::KuaiLeBeiYun::~KuaiLeBeiYun()
{
	
}

int location::KuaiLeBeiYun::IsKuaiLeBeiYun(HttpSession *http_session)
{
	
	if( ( http_session->m_Requestline.m_Host.Find("www.babytree.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/mobile_knowledge/sync_knowledge") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::KuaiLeBeiYun::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::KuaiLeBeiYun::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	imei_ = ms_->GetValueFromMapByKey("imei");
	
	if (longitude_.empty() || latitude_.empty()  || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}