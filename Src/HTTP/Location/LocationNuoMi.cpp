
#include "StdAfx.h"

#include "LocationNuoMi.h"

location::NuoMi::NuoMi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Å´Ã×";
}

location::NuoMi::~NuoMi()
{
	
}

int location::NuoMi::IsNuoMi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("app.nuomi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/naserver/common/registerbind") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::NuoMi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::NuoMi::DeviceLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string position = ms_->GetValueFromMapByKey("location",true);
	string::size_type pos = position.find(",");
	if (pos != string::npos)
	{
		latitude_ = position.substr(0, pos);
		longitude_ = position.substr(pos + 1);
	}
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputString();
	
	Record(app_);
	WriteIndexFile();
	return 1;
}