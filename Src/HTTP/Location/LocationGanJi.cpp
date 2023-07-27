
#include "StdAfx.h"

#include "LocationGanJi.h"

location::GanJi::GanJi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¸Ï¼¯Éú»î";
}

location::GanJi::~GanJi()
{
	
}

int location::GanJi::IsGanJi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mobds.ganji.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/datashare/") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::GanJi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::GanJi::DeviceLocation(const char *packet)
{
	string src_data(packet);

	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (imei_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}