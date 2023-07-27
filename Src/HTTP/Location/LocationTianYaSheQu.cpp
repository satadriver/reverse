
#include "StdAfx.h"

#include "LocationTianYaSheQu.h"

location::TianYaSheQu::TianYaSheQu(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÌìÑÄÉçÇø";
}

location::TianYaSheQu::~TianYaSheQu()
{
	
}

int location::TianYaSheQu::IsTianYaSheQu(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v/locate/setLocate") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::TianYaSheQu::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::TianYaSheQu::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("latitude");
	latitude_ = ms_->GetValueFromMapByKey("longitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}