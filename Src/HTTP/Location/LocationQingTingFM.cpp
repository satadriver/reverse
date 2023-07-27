
#include "StdAfx.h"

#include "LocationQingTingFM.h"

location::QingTingFM::QingTingFM(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "òßòÑFM";
}

location::QingTingFM::~QingTingFM()
{
	
}

int location::QingTingFM::IsQingTingFM(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("a1.easemob.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/qingting/qingting/devices") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::QingTingFM::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::QingTingFM::DeviceLocation(const char *packet)
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
	
	ms_->SplitStringToMap(src_data,',',':',"\"");
	imei_	= ms_->GetValueFromMapByKey("imei");
	longitude_ = ms_->GetValueFromMapByKey("loc.lng");
	latitude_ = ms_->GetValueFromMapByKey("loc.lat");
	
	if (longitude_.empty() || latitude_.empty() || imei_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}