
#include "StdAfx.h"

#include "LocationShengHuoRiLi.h"

location::ShengHuoRiLi::ShengHuoRiLi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "生活日历";
}

location::ShengHuoRiLi::~ShengHuoRiLi()
{
	
}

int location::ShengHuoRiLi::IsShengHuoRiLi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("iweekapi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/m/weatherfromgps.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ShengHuoRiLi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::ShengHuoRiLi::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ =  ms_->GetValueFromMapByKey("lon");
	latitude_ =  ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}