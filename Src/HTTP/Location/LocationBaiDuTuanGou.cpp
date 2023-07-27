
#include "StdAfx.h"

#include "LocationBaiDuTuanGou.h"

location::BaiDuTuanGou::BaiDuTuanGou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°Ù¶ÈÍÅ¹º";
}

location::BaiDuTuanGou::~BaiDuTuanGou()
{
	
}

int location::BaiDuTuanGou::IsBaiDuTuanGou(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("api.m.taobao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/gw/mtop.auks.mc.synconfig/1.0/") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::BaiDuTuanGou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::BaiDuTuanGou::DeviceLocation(const char *packet)
{
	
	string src_data(packet);

	imei_ = ms_->GetValueBetweenBeginEnd(src_data,"m-e:","m-t");
	imsi_ = ms_->GetValueBetweenBeginEnd(src_data,"m-s:","m-nettype");
	string postion = ms_->GetValueBetweenBeginEnd(src_data,"m-location:","m-appkey");
	longitude_ = postion.substr(0,postion.find("%2C"));
	latitude_ = postion.substr(postion.find("%2C") + 3,postion.length());
	
	if (imei_.empty() || imsi_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}