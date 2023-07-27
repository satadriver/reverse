
#include "StdAfx.h"

#include "LocationZhouMoQuNa.h"

location::ZhouMoQuNa::ZhouMoQuNa(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÖÜÄ©È¥ÄÄ";
}

location::ZhouMoQuNa::~ZhouMoQuNa()
{
	
}

int location::ZhouMoQuNa::IsZhouMoQuNa(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("www.wanzhoumo.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wanzhoumo?UUID=") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::ZhouMoQuNa::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::ZhouMoQuNa::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	latitude_ = ms_->GetValueFromMapByKey("lon");
	longitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}