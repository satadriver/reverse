
#include "StdAfx.h"

#include "Location8684Bus.h"

map<string,string>location::Bus8684::ipagent_deviceinfo_ = map<string,string>();
location::Bus8684::Bus8684(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "8684¹«½»³µ";
}

location::Bus8684::~Bus8684()
{
	
}

int location::Bus8684::IsBus8684(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("update1.8684.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/checkupdate") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("api.8684.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/bus_api_new.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::Bus8684::Process(const char *packet,int action)
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

int location::Bus8684::Deviceinfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI);
	ms_->SplitStringToMap(src_data,'&','=');
	string imei = ms_->GetValueFromMapByKey("ime");
	
	if (imei.empty())
	{
		return 0;
	}

	string ip_agent = GetIpAgent();
	
	ipagent_deviceinfo_.insert(std::make_pair(ip_agent,imei));
	return 1;
}

int location::Bus8684::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
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
	
	imei_ = iter->second;	
	ipagent_deviceinfo_.erase(ip_agent);
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}