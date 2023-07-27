
#include "StdAfx.h"

#include "LocationAiLiao.h"

location::AiLiao::AiLiao(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°®ÁÄ";
}

location::AiLiao::~AiLiao()
{
	
}

int location::AiLiao::IsAiLiao(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("n.a.aliaotian.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/sns/userlist.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::AiLiao::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}

int location::AiLiao::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");

	
	if (longitude_.empty() || latitude_.empty() )
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();

	return 1;
}