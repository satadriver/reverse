
#include "StdAfx.h"

#include "ShoppingNuoMi.h"

shopping::NuoMi::NuoMi(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::NuoMi::~NuoMi()
{
	
}

int shopping::NuoMi::IsNuoMi(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("app.nuomi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/naserver/search/nearitem?")!=-1))
	{
		return kAndroidLocation;
	}
	return 0;
}

int shopping::NuoMi::Process(const char *packet,int action)
{
	
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 0;
}

int shopping::NuoMi::AndroidLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI);
	ms_->SplitStringToMap(src_data,'&','=');
	string location,lon,lat;
	location = ms_->GetValueFromMapByKey("location",true);
	int nposbeg = location.find(',');
	lon = location.substr(0,nposbeg);
	lat = location.substr(nposbeg + 1,location.length() - nposbeg - 1);
	if ( lon.empty() || lat.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"lon=" + lon + "\r\n" + 
		"lat=" + lat + "\r\n" + 
		"state=¶¨Î»\r\n";
	
//	WriteIndexFile();
	return 1;
}

