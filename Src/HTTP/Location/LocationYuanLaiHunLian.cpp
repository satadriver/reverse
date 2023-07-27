
#include "StdAfx.h"

#include "LocationYuanLaiHunLian.h"

location::YuanLaiHunLian::YuanLaiHunLian(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÔµÀ´»éÁµ";
}

location::YuanLaiHunLian::~YuanLaiHunLian()
{
	
}

int location::YuanLaiHunLian::IsYuanLaiHunLian(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("mobileapi.yuanlai.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/lbs/add-location-info.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::YuanLaiHunLian::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::YuanLaiHunLian::DeviceLocation(const char *packet)
{
	
	string src_data(packet);
	ms_->SplitStringToMap(src_data,';','=');
	string imei	= ms_->GetValueFromMapByKey("realDeviceId");
	string model = ms_->GetValueFromMapByKey("model",true);

	string tmp_data(packet);
	ms_->SplitStringToMap(tmp_data,'&','=');
	longitude_ 	= ms_->GetValueFromMapByKey("longitude");
	latitude_ 	= ms_->GetValueFromMapByKey("latitude");

	if (longitude_.empty() || latitude_.empty() || imei.empty() || model.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}