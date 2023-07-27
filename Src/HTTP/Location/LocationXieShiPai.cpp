
#include "StdAfx.h"

#include "LocationXieShiPai.h"

location::XieShiPai::XieShiPai(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ð´Ê³ÅÉ";
}

location::XieShiPai::~XieShiPai()
{
	
}

int location::XieShiPai::IsXieShiPai(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("api.douguo.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/diaries/dnearby") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::XieShiPai::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::XieShiPai::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->Replace(src_data,"\r\n","&");
	ms_->Replace(src_data,":","=");
	// string tmp_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ = ms_->GetValueFromMapByKey("lon");
	latitude_ = ms_->GetValueFromMapByKey("lat");
 	string model = ms_->GetValueFromMapByKey("device",true);
 	string imei = ms_->GetValueFromMapByKey("imei");
	
	if (longitude_.empty() || latitude_.empty() || model.empty() || imei.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}