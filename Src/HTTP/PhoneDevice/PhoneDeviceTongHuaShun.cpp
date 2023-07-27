
#include "StdAfx.h"

#include "PhoneDeviceTongHuaShun.h"

phonedevice::TongHuaShun::TongHuaShun(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "Í¬»¨Ë³";
}

phonedevice::TongHuaShun::~TongHuaShun()
{
	
}

int phonedevice::TongHuaShun::IsTongHuaShun(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("stat.cbas.myhexin.com:8080")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/razor/index.php") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::TongHuaShun::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::TongHuaShun::UploadPhoneDevice(const char *packet)
{
    string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}

	src_data = src_data.substr(src_data.find("content=") + strlen("content="));
	
	Json::Value vale;
	Json::Reader reader;
	if (!reader.parse(src_data,vale))
	{
		return 0;
	}

	imei_ = vale["imsi"].asString();
	model_ = vale["devicename"].asString();
	version_ = vale["os_version"].asString();
	os_ = GetOS();
	if (model_.empty() || version_.empty() || os_.empty() )
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

