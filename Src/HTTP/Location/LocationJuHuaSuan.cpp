
#include "StdAfx.h"

#include "LocationJuHuaSuan.h"

location::JuHuaSuan::JuHuaSuan(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¾Û»®Ëã";
}

location::JuHuaSuan::~JuHuaSuan()
{
	
}

int location::JuHuaSuan::IsJuHuaSuan(HttpSession *http_session)
{
	if(( http_session->m_Requestline.m_Host.Find("api.m.taobao.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/gw/mtop.ju.optionitem.get/1.0/") != -1 ) )	
	{
		 return kDeviceLocation;
	}
	return 0;
}

int location::JuHuaSuan::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::JuHuaSuan::DeviceLocation(const char *packet)
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
	ms_->UrlDecode(src_data);
	
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"&&m-e&&","&&m-t&&");
	string imsi = ms_->GetValueBetweenBeginEnd(src_data,"&&m-s&&","&&m-nettype");
	string postion = ms_->GetValueBetweenBeginEnd(src_data,"m-location&&","&&m-appkey");
	longitude_ = postion.substr(0,postion.find("%2C"));
	latitude_ = postion.substr(postion.find("%2C") + 3,postion.length());
	
	if (imei.empty() || imsi.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		
		"imsi=" + imsi+ "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
}