
#include "StdAfx.h"

#include "LocationLaMaBang.h"

location::LaMaBang::LaMaBang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "À±Âè°ï";
}

location::LaMaBang::~LaMaBang()
{
	
}

int location::LaMaBang::IsLaMaBang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("open.lmbang.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/board/main") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::LaMaBang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::LaMaBang::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("long");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}