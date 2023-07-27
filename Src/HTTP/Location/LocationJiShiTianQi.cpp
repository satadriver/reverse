
#include "StdAfx.h"

#include "LocationJiShiTianQi.h"

location::JiShiTianQi::JiShiTianQi(HttpSession *http_session, const string &pro_name) : Location(http_session,pro_name)
{
	app_ = "¼´Ê±ÌìÆø";
}

location::JiShiTianQi::~JiShiTianQi()
{
	
}

int location::JiShiTianQi::IsJiShiTianQi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("weather.tv163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/service/detail.do") != -1 ) )	
	{
		return kDeviceLocation;
	}
	return 0;
}

int location::JiShiTianQi::Process(const char *packet,int action)
{
	if (action == kDeviceLocation)
	{
		return DeviceLocation(packet);
	}
	return 0;
}


int location::JiShiTianQi::DeviceLocation(const char *packet)
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
			*p = '/';
		}
	}
	string src_data(packet);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	string position = ms_->GetValueFromMapByKey("latlng");
	longitude_ = position.substr(position.find("/") + 1,position.length());
	latitude_ = position.substr(0,position.find("/"));
	
	if (latitude_.empty() || longitude_.empty())
	{
		return 0;
	}
	output_text_ = 
		"from=" + longitude_ + "\r\n" +
		"to=" + latitude_ + "\r\n" +
		"state=" + app_ + "\r\n";
	
	Record(app_);
	WriteIndexFile();
	return 1;
}