
#include "StdAfx.h"

#include "LocationRenMaiTong.h"

location::RenMaiTong::RenMaiTong(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ÈËÂöÍ¨";
}

location::RenMaiTong::~RenMaiTong()
{
	
}

int location::RenMaiTong::IsRenMaiTong(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("a.9tong.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/user/saveGPS.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::RenMaiTong::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::RenMaiTong::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}