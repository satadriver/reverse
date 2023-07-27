
#include "StdAfx.h"

#include "hardwaremangguo.h"

hardwareinfo::HardWareMangGuo::HardWareMangGuo(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "芒果旅游";
}

hardwareinfo::HardWareMangGuo::~HardWareMangGuo()
{
	
}

int hardwareinfo::HardWareMangGuo::IsHardWareMangGuo(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("sg2.mangocity.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("ctsp/staticdata/uploadDeviceInfo")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareMangGuo::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareMangGuo::hardwareinfomation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	int startpos = src_data.find("{");
	src_data = src_data.substr(startpos);
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		mid = value["deviceId"].asString();
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