#include "StdAfx.h"
#include "PhoneDevice2345ZhuShou.h"

phonedevice::ZhuShou2345::ZhuShou2345(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "2345手机助手";
}

phonedevice::ZhuShou2345::~ZhuShou2345()
{
	
}

int phonedevice::ZhuShou2345::IsZhuShou2345(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("update.app.2345.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/index.php") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::ZhuShou2345::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::ZhuShou2345::UploadPhoneDevice(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("appkey=") != string::npos )
	{
		src_data = src_data.substr(src_data.rfind("appkey="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string data = ms_->GetValueFromMapByKey("terminal");
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse(data,value))
	{	
		return 0;
	}
	model_ = value["model"].asString();
	version_ = value["osVerName"].asString();
	imei_ = value["imei"].asString();
	imsi_ = value["imsi"].asString();
	os_ = GetOS();
	
	//备用
	string brand = value["brand"].asString();
	string mac = value["mac"].asString();
	string netinfo = value["netinfo"].asString();
	string resolution = value["resolution"].asString();
	string osVerCode = value["osVerCode"].asString();
	
	if (imei_.empty() || version_.empty() || model_.empty() || os_.empty())
	{	
		return 0;
	}
	output_text_ = GetOutputText();	
	WriteIndexFile();
	return 1;
}

