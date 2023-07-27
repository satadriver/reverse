
#include "StdAfx.h"

#include "LocationWangXin.h"

location::WangXin::WangXin(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÍúÐÅ";
}

location::WangXin::~WangXin()
{
	
}

int location::WangXin::IsWangXin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("loc1.ditu.aliyun.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/getlocation.html") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::WangXin::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::WangXin::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	longitude_ = ms_->GetValueFromMapByKey("posx");
	latitude_ = ms_->GetValueFromMapByKey("posy");
	
	if (imei_.empty() ||imsi_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}