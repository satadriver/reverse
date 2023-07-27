
#include "StdAfx.h"

#include "RealNameZhiLianZhaoPin.h"

realname::ZhiLianZhaoPin::ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "ÖÇÁªÕÐÆ¸";
}

realname::ZhiLianZhaoPin::~ZhiLianZhaoPin()
{
	
}

int realname::ZhiLianZhaoPin::IsZhiLianZhaoPin(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("my.zhaopin.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/MYZHAOPIN/resume_baseinfo_save.asp") != -1 ) )	
	{
		return krealname;
	}
	return 0;
}

int realname::ZhiLianZhaoPin::Process(const char *packet,int action)
{
	if (action == krealname)
	{
		return RealNameUpdate(packet);
	}
	return 0;
}


int realname::ZhiLianZhaoPin::RealNameUpdate(const char *packet)
{	
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;
	
	string src_data(packet);
	
	name_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"username\"\r\n\r\n",boundary,true);
	gender_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"gender\"\r\n\r\n",boundary);
	id_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"id_number\"\r\n\r\n",boundary);
	phone_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"contact_num0\"\r\n\r\n",boundary);

	if (name_.empty() || gender_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}

	if (gender_ == "1")
	{
		gender_ = "ÄÐ";
	}
	if (gender_ == "2")
	{
		gender_ = "Å®";
	}
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}