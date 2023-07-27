
#include "StdAfx.h"

#include "PhoneDeviceQuanMinKGe.h"

phonedevice::QuanMinKGe::QuanMinKGe(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{
	app_ = "ȫ��K��";
}

phonedevice::QuanMinKGe::~QuanMinKGe()
{
	
}

int phonedevice::QuanMinKGe::IsQuanMinKGe(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("wspeed.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/w.cgi") != -1 ) )	
	{
		return kUploadPhoneDevice;
	}
	return 0;
}

int phonedevice::QuanMinKGe::Process(const char *packet,int action)
{
	if (action == kUploadPhoneDevice)
	{
		return UploadPhoneDevice(packet);
	}
	return 0;
}


int phonedevice::QuanMinKGe::UploadPhoneDevice(const char *packet)
{
	string src_data;
	unsigned long out_length = 0;
	unsigned char* data_buff = NULL;
	vector<Byte *> data_vec;
	//��ѹ����
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
	//�ж��Ƿ�Ϊ��
	if (src_data.empty())
	{
		return 0;
	}	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");

	model_ = ms_->GetValueFromMapByKey("device",true);
	imei_ = ms_->GetValueBetweenBeginEnd(src_data,"imei=","&");
	version_ = ms_->GetValueFromMapByKey("os",true);
	os_ = GetOS();

	if (model_.empty() || imei_.empty() || version_.empty() || os_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

