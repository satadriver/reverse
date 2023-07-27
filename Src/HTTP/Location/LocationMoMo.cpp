
#include "StdAfx.h"

#include "LocationMoMo.h"

location::MoMo::MoMo(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "İİ";
}

location::MoMo::~MoMo()
{
	
}

int location::MoMo::IsMoMo(HttpSession *http_session)
{

	if( ( http_session->m_Requestline.m_useragent.Find("MomoChat")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/config?version") != -1 ))	
	{
		return kDeviceLocationI;
	}
	return 0;
}


int location::MoMo::Process(const char *packet,int action)
{
	if (action == kDeviceLocationI)
	{
		return DeviceLocationI(packet);
	}
	return 0;
}

int location::MoMo::DeviceLocationA(const char *packet)
{
	return 0;

}

int location::MoMo::DeviceLocationI(const char *packet)
{
	
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','='); 
	
	longitude_ = ms_->GetValueFromMapByKey("lng");
	latitude_ = ms_->GetValueFromMapByKey("lat");
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputString() + 
		"platform=iOs\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
	
}
