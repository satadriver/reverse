
#include "StdAfx.h"

#include "LocationSouHuGouFangZhuShou.h"

location::SouHuGouFangZhuShou::SouHuGouFangZhuShou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ËÑºü¹º·¿ÖúÊÖ";
}

location::SouHuGouFangZhuShou::~SouHuGouFangZhuShou()
{
	
}

int location::SouHuGouFangZhuShou::IsSouHuGouFangZhuShou(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("focus-xinfang-app.sohusce.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v4/house/newsearch/pos") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::SouHuGouFangZhuShou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::SouHuGouFangZhuShou::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("device_token");
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