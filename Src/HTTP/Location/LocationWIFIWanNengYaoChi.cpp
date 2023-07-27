
#include "StdAfx.h"

#include "LocationWIFIWanNengYaoChi.h"

location::WIFIWanNengYaoChi::WIFIWanNengYaoChi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "WIFIÍòÄÜÔ¿³×";
}

location::WIFIWanNengYaoChi::~WIFIWanNengYaoChi()
{
	
}

int location::WIFIWanNengYaoChi::IsWIFIWanNengYaoChi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("wifiapi02.51y5.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wifiapi/fa.cmd") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::WIFIWanNengYaoChi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::WIFIWanNengYaoChi::DeviceLocation(const char *packet)
{
	string src_data;
	char *hd = strstr(packet,"\r\n\r\n");
	if (hd)
	{
		string tmp(hd);
		src_data = tmp;
	}
	else
	{
		string tmp(packet);
		src_data = tmp;
	}
	ms_->SplitStringToMap(src_data,'&','=');
	imei_ = ms_->GetValueFromMapByKey("ii");
	longitude_ = ms_->GetValueFromMapByKey("lo");
	latitude_ = ms_->GetValueFromMapByKey("la");
	
	if (imei_.empty()  || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}