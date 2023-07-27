
#include "StdAfx.h"

#include "LocationLeDongLi.h"

location::LeDongLi::LeDongLi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÀÖ¶¯Á¦";
}

location::LeDongLi::~LeDongLi()
{
	
}

int location::LeDongLi::IsLeDongLi(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("core.api.ledongli.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/xq/io.ashx") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::LeDongLi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::LeDongLi::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	longitude_ = ms_->GetValueFromMapByKey("lon");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	imei_	= ms_->GetValueFromMapByKey("pc");
	if (longitude_.empty() || latitude_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}