
#include "StdAfx.h"

#include "InstalledAFanTi.h"

installed::AFanTi::AFanTi(HttpSession *http_session, const string &pro_name) : Installed(http_session,pro_name)
{
	app_ = "阿凡题";
}

installed::AFanTi::~AFanTi()
{
	
}

int installed::AFanTi::IsAFanTi(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("tdcv3.talkingdata.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/g/d") != -1 ) )	
	{
		return kSoftwareUpdate;
	}
	return 0;
}

int installed::AFanTi::Process(const char *packet,int action)
{
	if (action == kSoftwareUpdate)
	{
		return SoftwareUpdate(packet);
	}
	return 0;
}


int installed::AFanTi::SoftwareUpdate(const char *packet)
{	
	//解压缩处理
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
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"\"extra6\":","}");
	ms_->Replace(imei,"\""," ");
	string::size_type pos = src_data.find("\xA0\xA0\xA0\xC2");
	if (pos==string::npos)
	{
		return 0;
	}
	string::size_type end = src_data.find("\x5D",pos+8);
	if (end==string::npos)
	{
		return 0;
	}
	src_data = src_data.substr(pos+8,end-(pos+8-1));
	delete []data_buff;
	data_buff = NULL;
	//判断是否为空
	if (src_data.empty())
	{
		return 0;
	}	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	while(src_data.find("com.")!=string::npos)
	{
		string::size_type begin = src_data.find("com.");
		string::size_type end = src_data.find(",",begin);
		string result  = src_data.substr(begin,end-begin);
		src_data = src_data.substr(end);
		output_text_ =
			"imei=" + imei + "\r\n" +
			"webname=" + result + "\r\n" +
			"type=手机\r\n" +
			"msg=" + app_ + "\r\n" +
			"state=安装\r\n";
			WriteIndexFile();
	}

	return 1;
}