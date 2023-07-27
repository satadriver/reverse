
#include "StdAfx.h"

#include "LocationBaiXingWang.h"

location::BaiXingWang::BaiXingWang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°ÙÐÕÍø";
}

location::BaiXingWang::~BaiXingWang()
{
	
}

int location::BaiXingWang::IsBaiXingWang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("w.m.taobao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/q?") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::BaiXingWang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::BaiXingWang::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("device_id");
	mac_ = ms_->GetValueFromMapByKey("mc",true);
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (imei_.empty() ||mac_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}