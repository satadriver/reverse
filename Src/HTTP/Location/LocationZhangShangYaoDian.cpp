
#include "StdAfx.h"

#include "LocationZhangShangYaoDian.h"

map<string,string>location::ZhangShangYaoDian::uid_deviceinfo_ = map<string,string>();
location::ZhangShangYaoDian::ZhangShangYaoDian(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÕÆÉÏÒ©µê";
}

location::ZhangShangYaoDian::~ZhangShangYaoDian()
{
	
}

int location::ZhangShangYaoDian::IsZhangShangYaoDian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("imr.manle.com:1984")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/?app_id") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("phone.manle.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/yaodian.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ZhangShangYaoDian::Process(const char *packet,int action)
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

int location::ZhangShangYaoDian::Deviceinfo(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string info = ms_->GetValueFromMapByKey("device_info",true);

	if (info.empty())
	{
		return 0;
	}

	ms_->Replace(info,"{","");
	ms_->Replace(info,"}","");
	ms_->SplitStringToMap(info,',',':',"\"");
	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	string model = ms_->GetValueFromMapByKey("device_name");
	string uid = ms_->GetValueFromMapByKey("uid");

	if (imei.empty() || imsi.empty() || model.empty() || uid.empty())
	{
		return 0;
	}
	
	string deviceinfo = 	
						"imei=" + imei + "\r\n" +
						"nick=" + imsi + "\r\n" +
						"webname=" + model + "\r\n";
	uid_deviceinfo_.insert(std::make_pair(uid,deviceinfo));
	return 1;
}

int location::ZhangShangYaoDian::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	string uid = ms_->GetValueFromMapByKey("hwid");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (longitude_.empty() || latitude_.empty() || uid.empty())
	{
		return 0;
	}

	map<string,string>::iterator iter = uid_deviceinfo_.find(uid);
	if (iter == uid_deviceinfo_.end())
	{
		return 0;
	}
	
	string deviceinfo = iter->second;	
	uid_deviceinfo_.erase(uid);
	
	output_text_ = 
		deviceinfo + 
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	return 1;
}