
#include "StdAfx.h"

#include "LocationTaoBao.h"

location::TaoBao::TaoBao(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÌÔ±¦";
}

location::TaoBao::~TaoBao()
{
	
}

int location::TaoBao::IsTaoBao(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("api.m.taobao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/rest/api3.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::TaoBao::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::TaoBao::DeviceLocation(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');
	string data;
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	data = ms_->GetValueFromMapByKey("data",true);
	
	ms_->Replace(data,"\\","");
	ms_->SplitStringToMap(data,',',':',"\"");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (imei_.empty() || imsi_.empty() ||  longitude_.empty() || latitude_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	
	return 1;
}