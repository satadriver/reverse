
#include "StdAfx.h"

#include "hardwareboke.h"

hardwareinfo::HardWareBoKe::HardWareBoKe(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "新浪博客";
}

hardwareinfo::HardWareBoKe::~HardWareBoKe()
{
	
}

int hardwareinfo::HardWareBoKe::IsHardWareBoKe(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("m.beacon.sina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mstat")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareBoKe::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareBoKe::hardwareinfomation(const char *packet)
{
	char *ptem=NULL;
	ptem = strstr(packet,"\r\n\r\n");
	if (ptem==NULL)
	{
		return 0;
	}
	ptem = ptem+4;
	//解压数据
 	string src_data;
 	unsigned long out_length = 0;
 	unsigned char* data_buff = NULL;
	vector<Byte *> data_vec;
 	if (-1 == Compress::Degzip2((unsigned char *)(ptem + 10), (unsigned long)(ptem - 10), data_vec, &out_length)) 
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
	//利用json提取数据
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value header =value["header"];
		mid = header["device_id"].asString();
	}

	if (mid.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + mid + "\r\n" +
		"nick=" + app_ + "\r\n" +
		"state=硬件特征信息\r\n";
	WriteIndexFile();
  	return 1;
}