

#include "StdAfx.h"

#include "XinLangWeiBo.h"

XinLangWeiBo::XinLangWeiBo()
{
	software_ = "新浪微博";
	pro_ = "wb_sina";
}

XinLangWeiBo::~XinLangWeiBo()
{
	
}

int XinLangWeiBo::Is(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("weibo.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/aj/mblog/add") != -1))	
	{
		return kAddWeiBo;
	}

	if( (http_session->m_Requestline.m_Host.Find("weibo.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/aj/v6/mblog/forward") != -1))	
	{
		return kForwardWeiBo;
	}

	if( (http_session->m_Requestline.m_Host.Find("weibo.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/aj/v6/comment/add") != -1))	
	{
		return kAddComment;
	}
	return 0;
}

int XinLangWeiBo::Process(const char *packet,int action)
{
	if (action == kAddWeiBo)
	{
		return AddWeiBo(packet);
	}
	if (action == kForwardWeiBo)
	{
		return ForwardWeiBo(packet);
	}
	if (action == kAddComment)
	{
		return AddComment(packet);
	}
	return 0;
}

int XinLangWeiBo::AddWeiBo(const char *packet)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->Replace(cookie,";","&");
	ms_->SplitStringToMap(cookie,'&','=');

	string user = ms_->GetValueFromMapByKey("name",true);
	string nick = ms_->GetValueFromMapByKey("nick",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("text",true);
	
	if (user.empty() || content.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + user + "\r\n" +  
		"nick=" + nick + "\r\n" +  
		"msg=" + content + "\r\n" +
		"msgtype=发表\r\n";

	WriteIndexFile();

	return 1;
}

int XinLangWeiBo::ForwardWeiBo(const char *packet)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->Replace(cookie,";","&");
	ms_->SplitStringToMap(cookie,'&','=');
	
	string user = ms_->GetValueFromMapByKey("name",true);
	string nick = ms_->GetValueFromMapByKey("nick",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string mid = ms_->GetValueFromMapByKey("mid",true);
	string content = ms_->GetValueFromMapByKey("reason",true);
	
	if (user.empty() || content.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + user + "\r\n" +  
		"nick=" + nick + "\r\n" +  
		"mid=" + mid + "\r\n" +
		"msg=" + content + "\r\n" +
		"msgtype=转载\r\n";
	WriteIndexFile();
	return 1;
}

int XinLangWeiBo::AddComment(const char *packet)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->Replace(cookie,";","&");
	ms_->SplitStringToMap(cookie,'&','=');
	
	string user = ms_->GetValueFromMapByKey("name",true);
	string nick = ms_->GetValueFromMapByKey("nick",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	string mid = ms_->GetValueFromMapByKey("mid",true);
	string content = ms_->GetValueFromMapByKey("content",true);

	if (user.empty() || content.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user + "\r\n" +  
		"nick=" + nick + "\r\n" +  
		"mid=" + mid + "\r\n" +
		"msg=" + content + "\r\n" +
		"msgtype=评论\r\n";
	WriteIndexFile();
	return 1;
}

