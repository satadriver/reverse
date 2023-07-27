
#include "StdAfx.h"

#include "RealNameQianChengWuYou.h"

realname::QianChengWuYou::QianChengWuYou(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "前程无忧";
}

realname::QianChengWuYou::~QianChengWuYou()
{
	
}

int realname::QianChengWuYou::IsQianChengWuYou(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("my.51job.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/cv/in/Resume/WizardAction.php") != -1 ) )	
	{
		return krealname;
	}
	return 0;
}

int realname::QianChengWuYou::Process(const char *packet,int action)
{
	if (action == krealname)
	{
		return RealNameUpdate(packet);
	}
	return 0;
}


int realname::QianChengWuYou::RealNameUpdate(const char *packet)
{	
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	name_ = ms_->GetValueFromMapByKey("Name",true);
	phone_ = ms_->GetValueFromMapByKey("Mobile");
	gender_ = ms_->GetValueFromMapByKey("Gender");
	id_ = ms_->GetValueFromMapByKey("IDNumber");

	if (name_.empty() || phone_.empty() || gender_.empty() || id_.empty())
	{
		return 0;
	}

	if (gender_ == "0")
	{
		gender_ = "男";
	}
	if (gender_ == "1")
	{
		gender_ = "女";
	}

	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}