
#include "StdAfx.h"

#include "LocationZhiLianZhaoPin.h"

location::ZhiLianZhaoPin::ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÖÇÁªÕÐÆ¸";
}

location::ZhiLianZhaoPin::~ZhiLianZhaoPin()
{
	
}

int location::ZhiLianZhaoPin::IsZhiLianZhaoPin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("mi.zhaopin.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/android/Position/Search") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ZhiLianZhaoPin::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::ZhiLianZhaoPin::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string position = ms_->GetValueFromMapByKey("SF_2_100_12",true);

	latitude_ = position.substr(0,position.find(";"));
	longitude_ = position.substr(position.find(";") + 1);
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}