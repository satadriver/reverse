
#include "StdAfx.h"

#include "LocationMoXiuZhuoMian.h"

location::MoXiuZhuoMian::MoXiuZhuoMian(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Ä§Ðã×ÀÃæ";
}

location::MoXiuZhuoMian::~MoXiuZhuoMian()
{
	
}

int location::MoXiuZhuoMian::IsMoXiuZhuoMian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("weather.moxiu.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/json.php?do=Weather.Detail&addr") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::MoXiuZhuoMian::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::MoXiuZhuoMian::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (imei_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}