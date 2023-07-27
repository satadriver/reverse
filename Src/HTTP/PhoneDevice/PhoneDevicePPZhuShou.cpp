#include "StdAfx.h"
#include "PhoneDevicePPZhuShou.h"

phonedevice::ZhuShouPP::ZhuShouPP(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "PP手机助手";
}

phonedevice::ZhuShouPP::~ZhuShouPP()
{
	
}

int phonedevice::ZhuShouPP::IsZhuShouPP(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("sjzs-api.25pp.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/resource.app.increaseAmount") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::ZhuShouPP::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::ZhuShouPP::UploadPhoneDevice(const char *packet)
{

	string src_data(packet);
	string src_dataEnd;
	if (src_data.rfind("{\"sign") != string::npos )
	{
		src_data = src_data.substr(src_data.rfind("{\"sign"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse(src_data,value))
	{	
		return 0;
	}
	Json::Value client = value["client"];
	Json::Value ex = client["ex"];

	model_ = ex["model"].asString();
    imei_ = ex["imei"].asString();
    os_ = "Android";

	//备用	
	string mac = ex["mac"].asString();
	string net = ex["net"].asString();
	string brand = ex["brand"].asString();
	
	if (imei_.empty() || model_.empty() || os_.empty())
	{	
		return 0;
	}
	output_text_ = GetOutputText();	
	WriteIndexFile();
	return 1;
}

