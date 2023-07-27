#include "StdAfx.h"
#include "BlogTengXunWeiBo.h"

blog::TengXunWeiBo::TengXunWeiBo(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::TengXunWeiBo::~TengXunWeiBo()
{
	
}

int blog::TengXunWeiBo::IsTengXunWeiBo(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("api.t.qq.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/old/publish.php") != -1))	
	{
		return kAddWeiBo;
	}

	return 0;
}

int blog::TengXunWeiBo::Process(const char *packet,int action)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie,';','=');
	
	user_ = ms_->GetValueFromMapByKey("ptui_loginuin",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	content_ = ms_->GetValueFromMapByKey("content",true);

	if (user_.empty() || content_.empty())
	{
		return 0;
	}

	weibo_id_ = ms_->GetValueFromMapByKey("pId");
	if (weibo_id_ == "")
	{
		output_text_ = GetOutputText(kAddWeiBo);
		WriteIndexFile();
		return 1;
		
	}
	else
	{
		string type = ms_->GetValueFromMapByKey("type");
		//评论
		if (type == "5")
		{
			output_text_ = GetOutputText(kAddComment);
			WriteIndexFile();
			return 1;
		}
		//转播
		if (type == "1")
		{
			output_text_ = GetOutputText(kForwardWeiBo);
			WriteIndexFile();
			return 1;
		}
	}
	return 0;
}

//广播- 发表 转播-转载
string blog::TengXunWeiBo::GetOutputText(int state)
{
	if (state == kAddWeiBo)
	{
		return 
			"user=" + user_ + "\r\n" +  
			"mid=" + user_ + "\r\n" + 
			"msg=" + content_ + "\r\n" +
			"msgtype=广播\r\n";
	}
	if (state == kForwardWeiBo)
	{
		return 
			"user=" + user_ + "\r\n" + 
			"oid=" + weibo_id_ + "\r\n"+
			"mid=" + user_ + "\r\n" + 
			"msg=" + content_ + "\r\n" +
			"msgtype=转播\r\n";
	}
	if (state == kAddComment)
	{
		return 
			"user=" + user_ + "\r\n" + 
			"oid=" + weibo_id_ + "\r\n"+
			"mid=" + user_ + "\r\n" + 
			"msg=" + content_ + "\r\n" +
			"msgtype=评论\r\n";
	}
	return "";
}