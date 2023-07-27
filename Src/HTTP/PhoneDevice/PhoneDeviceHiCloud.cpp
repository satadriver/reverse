#include "StdAfx.h"
#include "PhoneDeviceHiCloud.h"

phonedevice::HiCloud::HiCloud(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "华为应用市场";
}

phonedevice::HiCloud::~HiCloud()
{
	
}

int phonedevice::HiCloud::IsHiCloud(HttpSession *http_session)
{
	if( http_session->m_Requestline.m_Host.Find("hispaceclt.hicloud.com")  != -1 && 
		http_session->m_Requestline.m_URI.Find("/hwmarket/api/storeApi3") != -1 )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::HiCloud::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::HiCloud::UploadPhoneDevice(const char *packet)
{
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
	ms_->SplitStringToMap(src_data,"&","=");
    
	model_ = ms_->GetValueFromMapByKey("phoneType");
	version_ = ms_->GetValueFromMapByKey("firmwareVersion");
    os_ = GetOS();	
	//备用
	string resolution = ms_->GetValueFromMapByKey("resolution");

	if (model_.empty() || version_.empty() || os_.empty())
	{	
		return 0;
	}
	output_text_ = GetOutputText();	
	WriteIndexFile();
	return 1;
}

