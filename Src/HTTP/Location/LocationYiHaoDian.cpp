
#include "StdAfx.h"

#include "LocationYiHaoDian.h"

location::YiHaoDian::YiHaoDian(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "1ºÅµê";
}

location::YiHaoDian::~YiHaoDian()
{
	
}

int location::YiHaoDian::IsYiHaoDian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("interface.m.yhd.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/centralmobile/servlet/CentralMobileFacadeServlet") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::YiHaoDian::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::YiHaoDian::DeviceLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}