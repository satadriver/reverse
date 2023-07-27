
#include "StdAfx.h"

#include "GOZhuoMian.h"

GOZhuoMian::GOZhuoMian()
{
	software_ = "Go×ÀÃæ";
}

GOZhuoMian::~GOZhuoMian()
{
	
}

int GOZhuoMian::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("goweatherex.3g.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/goweatherex/city/gps?") != -1 ) )	
	{
		return kDeviceGPSLocationM;
	}
	return 0;
}

int GOZhuoMian::Process(const char *packet,int action)
{
	if (action == kDeviceGPSLocationM)
	{
		return DeviceGPSLocationM(packet);
	}
	return 0;
}


int GOZhuoMian::DeviceGPSLocationM(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string position = ms_->GetValueFromMapByKey("latlng",true);

	string::size_type pos = position.find(",");
	string longitude;
	string latitude;
	if (pos != string::npos)
	{
		longitude = position.substr(pos + 1);
		latitude = position.substr(0, pos);
	}

	
	if (latitude.empty() || longitude.empty())
	{
		return 0;
	}

	output_text_ = 
		"from=" + longitude + "\r\n" +
		"to=" + latitude + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}