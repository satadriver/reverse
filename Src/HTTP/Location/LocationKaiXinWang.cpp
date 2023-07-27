
#include "StdAfx.h"

#include "LocationKaiXinWang.h"

location::KaiXinWang::KaiXinWang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¿ªÐÄÍø";
}

location::KaiXinWang::~KaiXinWang()
{
	
}

int location::KaiXinWang::IsKaiXinWang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("iweekapi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/m/weatherfromgps.php") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::KaiXinWang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::KaiXinWang::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	longitude_ =  ms_->GetValueFromMapByKey("x");
	latitude_ =  ms_->GetValueFromMapByKey("y");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}