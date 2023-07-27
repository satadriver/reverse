
#include "StdAfx.h"

#include "LocationSinaWeiBo.h"

map<string,string>location::SinaWeiBo::uid_deviceinfo_ = map<string,string>();
location::SinaWeiBo::SinaWeiBo(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÐÂÀËÎ¢²©";
}

location::SinaWeiBo::~SinaWeiBo()
{
	
}

int location::SinaWeiBo::IsSinaWeiBo(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.weibo.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/interface/f/ttt/v3/lbsgetlocation.php") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("api.weibo.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("contacts/upload_vcard") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::SinaWeiBo::Process(const char *packet,int action)
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

int location::SinaWeiBo::Deviceinfo(const char *packet)
{
	string src_data(packet);

	string uid = ms_->GetValueBetweenBeginEnd(src_data,"\"uid\":\"","\",\"");
	string deviceinfo = ms_->GetValueBetweenBeginEnd(src_data,"\"imei_imsi\":\"","\"}");

	if (uid.empty() || deviceinfo.empty())
	{
		return 0;
	}

	uid_deviceinfo_.insert(std::make_pair(uid,deviceinfo));
	return 1;
}

int location::SinaWeiBo::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->SplitStringToMap(src_data,'&','=');

	string uid = ms_->GetValueFromMapByKey("uid");
	longitude_ = ms_->GetValueFromMapByKey("lon");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	string::size_type pos = latitude_.find(" ");
	if (pos != string::npos)
	{
		latitude_ = latitude_.substr(0, pos);
	}

	map<string,string>::iterator iter = uid_deviceinfo_.find(uid);
	if (iter == uid_deviceinfo_.end())
	{
		return 0;
	}

	string deviceinfo = iter->second;
	pos = deviceinfo.find("_");

	string imei,imsi;
	if (pos != string::npos)
	{
		imei = deviceinfo.substr(0, pos);
		imsi = deviceinfo.substr(pos + 1);
	}
	else
	{
		if (deviceinfo.size() == 31)
		{
			imei = deviceinfo.substr(0,15);
			imsi = deviceinfo.substr(16);
		}
		else
		{
			imei = deviceinfo;
			imsi = deviceinfo;
		}
		
	}
	

	uid_deviceinfo_.erase(uid);

	string agent(http_session_->m_Requestline.m_useragent.GetBuffer(0));
	pos = agent.find("_");
	string device;
	if (pos != string::npos)
	{
		device = agent.substr(0, pos);
	}
	else
	{
		device = agent;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"nick=" + imsi + "\r\n" +
		"webname=" + device + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";

	return 1;
}