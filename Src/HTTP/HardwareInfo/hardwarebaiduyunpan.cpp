
#include "StdAfx.h"

#include "hardwarebaiduyunpan.h"

hardwareinfo::HardWareBaiDuYunPan::HardWareBaiDuYunPan(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "百度云盘";
}

hardwareinfo::HardWareBaiDuYunPan::~HardWareBaiDuYunPan()
{
	
}

int hardwareinfo::HardWareBaiDuYunPan::IsHardWareBaiDuYunPan(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("update.pan.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/statistics")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareBaiDuYunPan::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareBaiDuYunPan::hardwareinfomation(const char *packet)
{

	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	mid = ms_->GetValueFromMapByKey("devuid",true);
	string temp="BDIMXV2-O_";
	string::size_type startpos = 0;
	startpos = src_data.find(temp);
	if (startpos==string::npos)
	{
		return 0;
	}
	mid = src_data.substr(startpos+temp.length(),32);
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