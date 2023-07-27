
#include "StdAfx.h"

#include "LocationJiaoTongWeiZhangChaXun.h"

location::JiaoTongWeiZhangChaXun::JiaoTongWeiZhangChaXun(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "全国交通违章查询";
}

location::JiaoTongWeiZhangChaXun::~JiaoTongWeiZhangChaXun()
{
	
}

int location::JiaoTongWeiZhangChaXun::IsJiaoTongWeiZhangChaXun(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("api.shijia.kakamobi.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/open/home/index.htm") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::JiaoTongWeiZhangChaXun::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::JiaoTongWeiZhangChaXun::DeviceLocation(const char *packet)
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
	ms_->Replace(src_data,",","&");
	ms_->SplitStringToMap(src_data,'&','=');
	
	string imei = ms_->GetValueFromMapByKey("_imei");
	string mac = ms_->GetValueFromMapByKey("_mac");
	longitude_ = ms_->GetValueFromMapByKey("_longitude");
	latitude_ = ms_->GetValueFromMapByKey("_latitude");
	
	if (imei.empty() ||mac.empty() || latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"mac=" + mac + "\r\n" +
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
}