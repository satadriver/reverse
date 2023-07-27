
#include "StdAfx.h"

#include "LocationQuNaLvXing.h"

location::QuNaLvXing::QuNaLvXing(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "È¥ÄÄÂÃÐÐ";
}

location::QuNaLvXing::~QuNaLvXing()
{
	
}

int location::QuNaLvXing::IsQuNaLvXing(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("qde.qunar.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/html.ng/chan=hotel&pg=home&plateform=app&pos=bottom&site=app.qunar") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::QuNaLvXing::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::QuNaLvXing::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
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