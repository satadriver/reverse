
#include "StdAfx.h"

#include "Location58TongCheng.h"

location::TongCheng58::TongCheng58(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "58Í¬³Ç";
}

location::TongCheng58::~TongCheng58()
{
	
}

int location::TongCheng58::IsTongCheng58(HttpSession *http_session)
{
	
	if(  ( http_session->m_Requestline.m_Host.Find("app.58.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/log/api/unity/locate") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::TongCheng58::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::TongCheng58::DeviceLocation(const char *packet)
{	
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,'&','=');
	
	string lotid = ms_->GetValueFromMapByKey("lotid");
	string latid = ms_->GetValueFromMapByKey("latid");
	
	
	latitude_ = lotid;
	longitude_ = latid;
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}