
#include "StdAfx.h"

#include "ZhiLianZhaoPin.h"

ZhiLianZhaoPin::ZhiLianZhaoPin()
{
	software_ = "ÖÇÁªÕÐÆ¸";
}

ZhiLianZhaoPin::~ZhiLianZhaoPin()
{
	
}

int ZhiLianZhaoPin::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("my.zhaopin.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/MYZHAOPIN/resume_baseinfo_save.asp") != -1 ) )	
	{
		return kSaveResumeCW;
	}
	return 0;
}

int ZhiLianZhaoPin::Process(const char *packet,int action)
{
	if (action == kSaveResumeCW)
	{
		return SaveResumeCW(packet);
	}
	return 0;
}


int ZhiLianZhaoPin::SaveResumeCW(const char *packet)
{	
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);
	
	string name = ms_->GetValueBetweenBeginEnd(src_data,"name=\"username\"\r\n\r\n",boundary,true);
	string gender = ms_->GetValueBetweenBeginEnd(src_data,"name=\"gender\"\r\n\r\n",boundary);
	string id = ms_->GetValueBetweenBeginEnd(src_data,"name=\"id_number\"\r\n\r\n",boundary);
	string phone = ms_->GetValueBetweenBeginEnd(src_data,"name=\"contact_num0\"\r\n\r\n",boundary);

	if (name.empty() || gender.empty() || phone.empty())
	{
		return 0;
	}

	if (gender == "1")
	{
		gender = "ÄÐ";
	}
	if (gender == "2")
	{
		gender = "Å®";
	}
	output_text_ = 
		"user=" + name + "\r\n" +
		"from=" + gender + "\r\n" +
		"to=" + id + "\r\n" +
		"webname=" + phone + "\r\n" +
		"state=" + software_ + "\r\n";

	WriteIndexFile();
	return 1;
}