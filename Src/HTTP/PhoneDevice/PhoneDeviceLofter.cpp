
#include "StdAfx.h"

#include "PhoneDeviceLofter.h"

phonedevice::Lofter::Lofter(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "网易Lofter";
}

phonedevice::Lofter::~Lofter()
{
	
}

int phonedevice::Lofter::IsLofter(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("crash.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/client/api/uploadStartUpInfo.do") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::Lofter::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::Lofter::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	string src_dataEnd;
	if (src_data.rfind("data=") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.rfind("data="));
	}
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	string data = ms_->GetValueFromMapByKey("data");
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(data,value))
	{	
		version_ = value["osversion"].asString();
		model_ = value["model"].asString();
		imei_ = value["imei"].asString();
		os_ = value["os"].asString();
		//os_ = GetOS();
	}

	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{
	
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

