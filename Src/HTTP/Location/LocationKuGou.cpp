
#include "StdAfx.h"

#include "LocationKuGou.h"

location::KuGou::KuGou(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¿á¹·ÒôÀÖ";
}

location::KuGou::~KuGou()
{
	
}

int location::KuGou::IsKuGou(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("y.service.kugou.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/geo/postneighbor") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::KuGou::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::KuGou::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}