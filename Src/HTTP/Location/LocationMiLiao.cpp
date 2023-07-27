
#include "StdAfx.h"

#include "LocationMiLiao.h"

location::MiLiao::MiLiao(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "цвад";
}

location::MiLiao::~MiLiao()
{
	
}

int location::MiLiao::IsMiLiao(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("123.129.202.148")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/find/v3/user/467963491/nearby/user") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::MiLiao::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::MiLiao::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}