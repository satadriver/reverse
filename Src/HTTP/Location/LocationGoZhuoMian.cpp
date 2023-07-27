
#include "StdAfx.h"

#include "LocationGOZhuoMian.h"

location::GOZhuoMian::GOZhuoMian(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "Go×ÀÃæ";
}

location::GOZhuoMian::~GOZhuoMian()
{
	
}

int location::GOZhuoMian::IsGOZhuoMian(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("goweatherex.3g.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/goweatherex/city/gps?") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::GOZhuoMian::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::GOZhuoMian::DeviceLocation(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string position = ms_->GetValueFromMapByKey("latlng",true);

	string::size_type pos = position.find(",");
	if (pos != string::npos)
	{
		longitude_ = position.substr(pos + 1);
		latitude_ = position.substr(0, pos);
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