
#include "StdAfx.h"
#include "PhoneDeviceYiRenDai.h"

phonedevice::PdYiRenDai::PdYiRenDai(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "宜人贷";
}

phonedevice::PdYiRenDai::~PdYiRenDai()
{
	
}

int phonedevice::PdYiRenDai::IsPdYiRenDai(HttpSession *http_session)
{
	//手机设备信息
	if( http_session->m_Requestline.m_Host.Find("app-normal.yirendai.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/init/initVersion") != -1)	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::PdYiRenDai::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::PdYiRenDai::UploadPhoneDevice(const char *packet)
{

    string src_data(packet);
	string division = "system=";

	if (src_data.rfind(division) != -1)
	{
		src_data = src_data.substr(src_data.rfind(division) + division.length());
	}
	//base64解密
	int out_byte = 0;
	string msg = ms_->Base64Decode(src_data,out_byte);
	//json提取数据
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(msg,value))
	{
		return 0;
	}
	imei_ = value["singleCode"].asString();
	model_ = value["systemModel"].asString();
	os_ = value["systemPhone"].asString();
	version_ = value["systemVersion"].asString();
	if (imei_.empty() || model_.empty() || os_.empty() || version_.empty())
	{
		return 0; 
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

