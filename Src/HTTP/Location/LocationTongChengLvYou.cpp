#include "StdAfx.h"

#include "LocationTongChengLvYou.h"

location::TongChengLvYou::TongChengLvYou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Í¬³ÌÂÃÓÎ";
}

location::TongChengLvYou::~TongChengLvYou()
{
	
}

int location::TongChengLvYou::IsTongChengLvYou(HttpSession *http_session)
{

	if(  ( http_session->m_Requestline.m_Host.Find("tcmobileapi.17usoft.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Common/general/RecommendHandler.ashx") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::TongChengLvYou::Process(const char *packet,int action)
{

	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::TongChengLvYou::DeviceLocation(const char *packet)
{	
	string src_data(packet);
	ms_->Replace(src_data,"{",",");
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	string ipagent = GetIpAgent();
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	if (longitude_.empty() || latitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}