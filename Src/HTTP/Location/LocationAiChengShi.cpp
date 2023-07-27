
#include "StdAfx.h"

#include "LocationAiChengShi.h"

location::AiChengShi::AiChengShi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "°®³ÇÊÐ";
}

location::AiChengShi::~AiChengShi()
{
	
}

int location::AiChengShi::IsAiChengShi(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("www.153.cn:8081")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/service/encrypt/login.action") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::AiChengShi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::AiChengShi::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (imei_.empty() || imsi_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}