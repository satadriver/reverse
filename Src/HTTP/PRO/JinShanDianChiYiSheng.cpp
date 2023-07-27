
#include "StdAfx.h"

#include "JinShanDianChiYiSheng.h"

JinShanDianChiYiSheng::JinShanDianChiYiSheng()
{
	software_ = "金山电池医生";
}

JinShanDianChiYiSheng::~JinShanDianChiYiSheng()
{
	
}

int JinShanDianChiYiSheng::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("dcys.ijinshan.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("imei") != -1 ) )	
	{
		return kUploadDeviceInfoM1;
	}
	return 0;
}

int JinShanDianChiYiSheng::Process(const char *packet,int action)
{
	if (action == kUploadDeviceInfoM1)
	{
		return UploadDeviceInfoM1(packet);
	}


	return 0;
}


int JinShanDianChiYiSheng::UploadDeviceInfoM1(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');

	string version = ms_->GetValueFromMapByKey("andver");
	string model = ms_->GetValueFromMapByKey("mode");
	string imei = ms_->GetValueFromMapByKey("imei");
	string os;
	if (src_data.find("androidid=") != string::npos)
	{
		os = "Android";
	}

	if (imei.empty() || version.empty() || model.empty() || os.empty())
	{
		return 0;
	}
		
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}


