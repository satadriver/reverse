
#include "StdAfx.h"

#include "LocationXiMaLaYa.h"

map<string,string>location::XiMaLaYa::uid_deviceinfo_ = map<string,string>();
location::XiMaLaYa::XiMaLaYa(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ï²ÂíÀ­ÑÅÌýÊé";
}

location::XiMaLaYa::~XiMaLaYa()
{
	
}

int location::XiMaLaYa::IsXiMaLaYa(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("ad.ximalaya.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/adrecord/channel") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("mobile.ximalaya.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/mobile/pns/bind") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::XiMaLaYa::Process(const char *packet,int action)
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

int location::XiMaLaYa::Deviceinfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI);
	ms_->SplitStringToMap(src_data,'&','=');
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"deviceId=","&device",false);
	string imsi = ms_->GetValueFromMapByKey("imsi");

	if (imei.empty() || imsi.empty())
	{
		return 0;
	}
	string deviceinfo = "imei=" + imei + "\r\n" +
						"nick=" + imsi + "\r\n" ;
	string str(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	string uid = ms_->GetValueBetweenBeginEnd(str,"android&","&",false);

	uid_deviceinfo_.insert(std::make_pair(uid,deviceinfo));
	return 1;
}

int location::XiMaLaYa::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	string uid = ms_->GetValueBetweenBeginEnd(src_data,"android&","&",false);
	ms_->SplitStringToMap(src_data,';','=');
	string loc = ms_->GetValueFromMapByKey("SUP",true);
	int npos = loc.find(",");
	longitude_ = loc.substr(0,npos);
	latitude_ = loc.substr(npos + 1);
	
	map<string,string>::iterator iter = uid_deviceinfo_.find(uid);
	if (iter == uid_deviceinfo_.end())
	{
		return 0;
	}
	
	string deviceinfo = iter->second;	
	uid_deviceinfo_.erase(uid);
	output_text_ = deviceinfo + 
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	return 1;
}