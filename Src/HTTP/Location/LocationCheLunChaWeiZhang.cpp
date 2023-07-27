
#include "StdAfx.h"

#include "LocationCheLunChaWeiZhang.h"

location::CheLunChaWeiZhang::CheLunChaWeiZhang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "³µÂÖ²éÎ¥ÕÂ";
}

location::CheLunChaWeiZhang::~CheLunChaWeiZhang()
{
	
}

int location::CheLunChaWeiZhang::IsCheLunChaWeiZhang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("ex.puata.info")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/q?layout_type") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::CheLunChaWeiZhang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::CheLunChaWeiZhang::DeviceLocation(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("device_id");
	mac_ = ms_->GetValueFromMapByKey("mc",true);
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if(imei_.empty() ||mac_.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}