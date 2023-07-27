
#include "StdAfx.h"

#include "LocationQiuShiBaiKe.h"

location::QiuShiBaiKe::QiuShiBaiKe(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ôÜÊÂ°Ù¿Æ";
}

location::QiuShiBaiKe::~QiuShiBaiKe()
{
	
}

int location::QiuShiBaiKe::IsQiuShiBaiKe(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("nearby.qiushibaike.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/nearby/fetch?rqcnt=6") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::QiuShiBaiKe::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::QiuShiBaiKe::DeviceLocation(const char *packet)
{
	string src_data(packet);

	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	imei_ = ms_->GetValueFromMapByKey("DEVICEID");
	mac_ = ms_->GetValueFromMapByKey("MAC");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (imei_.empty() || mac_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}