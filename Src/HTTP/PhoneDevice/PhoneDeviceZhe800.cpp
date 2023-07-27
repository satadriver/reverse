
#include "StdAfx.h"

#include "PhoneDeviceZhe800.h"

phonedevice::Zhe800::Zhe800(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "уш800";
}

phonedevice::Zhe800::~Zhe800()
{
	
}

int phonedevice::Zhe800::IsZhe800(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("mauth.chinanetcenter.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/frontoffice/checkAuthority") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::Zhe800::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::Zhe800::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}

	model_ = value["model"].asString();
	version_ = value["platform"].asString();
	imei_ = value["imei"].asString();
	imsi_ = value["imsi"].asString();
	os_ = "Android";

	if (model_.empty() || version_.empty() || os_.empty() || imsi_.empty() || imei_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

