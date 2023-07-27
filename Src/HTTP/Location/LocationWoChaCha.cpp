
#include "StdAfx.h"

#include "LocationWoChaCha.h"

location::WoChaCha::WoChaCha(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÎÒ²é²é";
}

location::WoChaCha::~WoChaCha()
{
	
}

int location::WoChaCha::IsWoChaCha(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("android.wochacha.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Newindex/slidebanner") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::WoChaCha::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::WoChaCha::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("udid");
	mac_ = ms_->GetValueFromMapByKey("mac",true);
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (imei_.empty() ||mac_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();

	Record(app_);
	WriteIndexFile();
	return 1;
}