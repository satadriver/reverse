
#include "StdAfx.h"
#include "PhoneDeviceTouTiao.h"

phonedevice::PdTouTiao::PdTouTiao(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "今日头条";
}

phonedevice::PdTouTiao::~PdTouTiao()
{
	
}

int phonedevice::PdTouTiao::IsPdTouTiao(HttpSession *http_session)
{
	//手机设备信息 1
	if( http_session->m_Requestline.m_Host.Find("mon.snssdk.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/monitor/collect/") != -1)	
	{
		return kUploadPhoneDevice;
	}
	//手机设备信息 2 
	if (http_session->m_Requestline.m_Host.Find("dm.toutiao.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/get_domains/v4") != -1)
	{
		return kUploadPhoneDeviceSec;
	}
	//手机设备信息 3
	if (http_session->m_Requestline.m_Host.Find("isub.snssdk.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/2/user/info/") != -1)
	{
		return kUploadPhoneDeviceThi;
	}
	return 0;
}

int phonedevice::PdTouTiao::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	if (action == kUploadPhoneDeviceSec)
	{
		return UploadPhoneDeviceSec(packet);
	}
	if (action == kUploadPhoneDeviceThi)
	{
		return UploadPhoneDeviceThi(packet);
	}
	return 0;
}

//手机设备信息 1
int phonedevice::PdTouTiao::UploadPhoneDevice(const char *packet)
{
	//解压 函数
	string src_data;
	unsigned long out_length = 0;
	unsigned char* data_buff = NULL;
	vector<Byte *> data_vec;
	//解压函数
	if (-1 == Compress::Degzip2((unsigned char *)(packet + 10), (unsigned long)(packet - 10), data_vec, &out_length)) 
	{
		return 0;
	}	
	if (out_length < 10)
	{
		return -1;
	}	
	data_buff = new Bytef[out_length + 1];
	memset(data_buff, 0x00, out_length + 1);
	for(int i = 0; i < data_vec.size(); i++)
	{
		if (i == data_vec.size() - 1)
		{
			memcpy(data_buff + (i * 1024), data_vec[i], out_length - (i * 1024));
		}
		else
		{
			memcpy(data_buff + (i * 1024), data_vec[i], 1024);
		}
		delete data_vec[i];
		data_vec[i] = NULL;
	}
	src_data = string((char *)data_buff, out_length);
	delete []data_buff;
	data_buff = NULL;
	//判断是否为空
	if (src_data.empty())
	{
		return 0;
	}	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    //json提取数据
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	Json::Value header = value["header"];
    /*
	string display_name = header["display_name"].asString();
	string package = header["package"].asString();
	string app_version = header["app_version"].asString();
    string device_brand = header["device_brand"].asString();
    string resolution = header["resolution"].asString();
    string mc = header["mc"].asString();
	string access = header["access"].asString();
    */
    os_ = header["os"].asString();
	version_ = header["os_version"].asString();
	model_ = header["device_model"].asString();
    imei_ = header["udid"].asString();
	if (os_.empty() || version_.empty() || model_.empty() || imei_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();	
	WriteIndexFile();
	return 1;
}

//手机设备信息 2
int phonedevice::PdTouTiao::UploadPhoneDeviceSec(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->Replace(uri,"?","&");
    ms_->SplitStringToMap(uri,"&","=");
	longitude_ = ms_->GetValueFromMapByKey("longitude");
	latitude_ = ms_->GetValueFromMapByKey("latitude");
	os_ = ms_->GetValueFromMapByKey("device_platform");
	model_ = ms_->GetValueFromMapByKey("device_type");
	version_ = ms_->GetValueFromMapByKey("os_version");
	imei_ = ms_->GetValueFromMapByKey("uuid");
	if (longitude_.empty() || latitude_.empty() || imei_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

//手机设备信息 3
int phonedevice::PdTouTiao::UploadPhoneDeviceThi(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->Replace(uri,"?","&");
    ms_->SplitStringToMap(uri,"&","=");
	os_ = ms_->GetValueFromMapByKey("device_platform");
	model_ = ms_->GetValueFromMapByKey("device_type");
	version_ = ms_->GetValueFromMapByKey("os_version");
	imei_ = ms_->GetValueFromMapByKey("uuid");
	if (os_.empty() || model_.empty() || version_.empty() || imei_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}