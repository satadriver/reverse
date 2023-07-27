
#include "StdAfx.h"

#include "LocationMaMaQuan.h"

map<string,string>location::MaMaQuan::uid_postion_ = map<string,string>();
location::MaMaQuan::MaMaQuan(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÂèÂèÈ¦";
}

location::MaMaQuan::~MaMaQuan()
{
	
}

int location::MaMaQuan::IsMaMaQuan(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("statis1.gzmama.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/app_visit.php") != -1 ) )	
	{
		return kDeviceInfo;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("um.mama.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/app/api/user_upgeohash_v1.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::MaMaQuan::Process(const char *packet,int action)
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

int location::MaMaQuan::Deviceinfo(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	string uid = ms_->GetValueFromMapByKey("uid");
	string imei = ms_->GetValueFromMapByKey("mobileID");

	map<string,string>::iterator iter = uid_postion_.find(uid);
	if (iter == uid_postion_.end())
	{
		return 0;
	}
	string postion = iter->second;
	latitude_ = postion.substr(0,postion.find("-"));
	longitude_ = postion.substr(postion.find("-") + 1,postion.length());
	uid_postion_.erase(uid);
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";	
	Record(app_);
	WriteIndexFile();

	
	return 1;
}

int location::MaMaQuan::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string uid = ms_->GetValueFromMapByKey("uid");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	if (uid.empty() || longitude_.empty() || latitude_.empty())
	{
		return 0;
	}
	string postion = latitude_ + "-" +longitude_; 
	uid_postion_.insert(std::make_pair(uid,postion));
	return 1;
}