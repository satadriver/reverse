
#include "StdAfx.h"
#include "PhoneDeviceZte.h"

phonedevice::PdZte::PdZte(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "中兴应用商店";
}

phonedevice::PdZte::~PdZte()
{
	
}

int phonedevice::PdZte::IsPdZte(HttpSession *http_session)
{
	//手机设备信息
	if( http_session->m_Requestline.m_Host.Find("stat.ztems.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/dcsbap/DataCollection?cmd=saveData") != -1)	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::PdZte::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::PdZte::UploadPhoneDevice(const char *packet)
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
    //json提取数据
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string type = value["type"].asString();
	if (type != "launch")
	{
		return 0;
	}
	Json::Value device = value["device"];

	model_ = device["model"].asString();
	version_ = device["os_version"].asString();
    os_ = GetOS();
	/*
    string brand = device["brand"].asString();
	string mac = device["mac"].asString();
	string resolutions = device["resolution"].asString();
   */
	if (os_.empty() || version_.empty() || model_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

