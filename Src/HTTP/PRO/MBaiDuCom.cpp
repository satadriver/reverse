
#include "StdAfx.h"

#include "MBaiDuCom.h"

MBaiDuCom::MBaiDuCom()
{
	software_ = "m.baidu.com";
}

MBaiDuCom::~MBaiDuCom()
{
	
}

int MBaiDuCom::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("aladdin.wapmap.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/plugin") != -1 ) )	
	{
		return kGPSLocation;
	}
	return 0;
}

int MBaiDuCom::Process(const char *packet,int action)
{
	if (action == kGPSLocation)
	{
		return GPSLocation(packet);
	}
	return 0;
}


int MBaiDuCom::GPSLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string position = ms_->GetValueFromMapByKey("xyr");

	string longitude;
	string latitude;
	
	string::size_type pos2 = position.rfind("_");

	if (pos2 != string::npos)
	{
		position = position.substr(0,pos2);
		string::size_type pos1 = position.find("_");
		if (pos1 != string::npos)
		{
			longitude = position.substr(0, pos1);
			latitude = position.substr(pos1 + 1);
		}
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