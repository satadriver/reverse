
#include "StdAfx.h"

#include "LocationHuangLiTianQi.h"

map<string,string>location::HuangLiTianQi::ipagent_deviceinfo_ = map<string,string>();
location::HuangLiTianQi::HuangLiTianQi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "»ÆÀúÌìÆø";
}

location::HuangLiTianQi::~HuangLiTianQi()
{
	
}

int location::HuangLiTianQi::IsHuangLiTianQi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.weather.rj.99.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getpopupad") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("api.weather.rj.99.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/othertools") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::HuangLiTianQi::Process(const char *packet,int action)
{
	if (action == kDeviceInfo)
	{
		return Deviceinfo(packet);
	}
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::HuangLiTianQi::Deviceinfo(const char *packet)
{
	char *p = strstr(packet,"\r\n\r\n");
	if (!p)
	{
		return 0;
	}
	string src_data(p);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	string imei = ms_->GetValueFromMapByKey("localid");

	if (imei.empty())
	{
		return 0;
	}

	string ip_agent = GetIpAgent();
	
	
	ipagent_deviceinfo_.insert(std::make_pair(ip_agent,imei));
	return 1;
}

int location::HuangLiTianQi::DeviceLocation(const char *packet)
{
	char *p = strstr(packet,"\r\n\r\n");
	if (!p)
	{
		return 0;
	}
	string src_data(p);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	longitude_ = ms_->GetValueFromMapByKey("jd");
	latitude_ = ms_->GetValueFromMapByKey("wd");
	
	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	
	string ip_agent = GetIpAgent();

	map<string,string>::iterator iter = ipagent_deviceinfo_.find(ip_agent);
	if (iter == ipagent_deviceinfo_.end())
	{
		return 0;
	}
	
	string imei = iter->second;	
	ipagent_deviceinfo_.erase(ip_agent);
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
}