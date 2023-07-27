
#include "StdAfx.h"
#include "PhoneDeviceLianJia.h"

phonedevice::PdLianJia::PdLianJia(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "链家地产";
}

phonedevice::PdLianJia::~PdLianJia()
{
	
}

int phonedevice::PdLianJia::IsPdLianJia(HttpSession *http_session)
{
	//手机设备信息
	if( http_session->m_Requestline.m_Host.Find("www.lianjia.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/client/log") != -1)	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::PdLianJia::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::PdLianJia::UploadPhoneDevice(const char *packet)
{
    string src_data(packet);
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
         imei_ = value["uuid"].asString();
		 version_ = value["os_version"].asString();
		 model_ = value["device_type"].asString();
		 os_ = GetOS();
		 if (imei_.empty() || version_.empty() || model_.empty())
		 {
			 return 0;
		 }
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

