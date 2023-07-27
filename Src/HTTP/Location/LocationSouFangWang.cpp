
#include "StdAfx.h"

#include "LocationSouFangWang.h"

location::SouFangWang::SouFangWang(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "ËÑ·¿Íø";
}

location::SouFangWang::~SouFangWang()
{
	
}

int location::SouFangWang::IsSouFangWang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("soufunapp.3g.fang.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/http/sfservice.jsp") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::SouFangWang::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::SouFangWang::DeviceLocation(const char *packet)
{
	char *p = (char *)packet;
	if (!p)
	{
		return 0;
	}
	int len = http_session_->m_Requestline.m_nseq - http_session_->m_Requestline.m_Ssequence;
	int i = 0;
	for (;i <= len ; i++,p++)
	{
		if ( 0 == *p)
		{
			*p = '&';
		}
	}
	string src_data(packet);
	
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"&&imei&&","&&user-agent&&");
	string phonenumber = ms_->GetValueBetweenBeginEnd(src_data,"&&%2B86","&&company&&");
	longitude_ = ms_->GetValueBetweenBeginEnd(src_data,"&&x1&&","&&app-name&&");
	latitude_ = ms_->GetValueBetweenBeginEnd(src_data,"&&y1&&","&&model&&");
	
	if (imei.empty() ||phonenumber.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"phonenumber=" + phonenumber + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
}