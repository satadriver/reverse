

#include "StdAfx.h"

#include "BlogXinLangWeiBo.h"

blog::XinLangWeiBo::XinLangWeiBo(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::XinLangWeiBo::~XinLangWeiBo()
{
	
}

int blog::XinLangWeiBo::IsXinLangWeiBo(HttpSession *http_session)
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
	
	if ( (http_session->m_Requestline.m_Host.Find("api.weibo.cn") != -1) &&
		 (http_session->m_Requestline.m_URI.Find("/2/statuses/send") != -1))
	{
		return kAddWeiBoMA;
	}
	return 0;
}

int blog::XinLangWeiBo::Process(const char *packet,int action)
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

	if (action == kAddWeiBoMA)
	{
		return AddWeiBoMA(packet);
	}
	return 0;
}

int blog::XinLangWeiBo::AddWeiBo(const char *packet)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->Replace(cookie,";","&");
	ms_->SplitStringToMap(cookie,'&','=');

	user_ = ms_->GetValueFromMapByKey("name",true);
	nick_ = ms_->GetValueFromMapByKey("nick",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	content_ = ms_->GetValueFromMapByKey("text",true);
	
	if (user_.empty() || content_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText(kAddWeiBo);
	WriteIndexFile();

	return 1;
}

int blog::XinLangWeiBo::ForwardWeiBo(const char *packet)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->Replace(cookie,";","&");
	ms_->SplitStringToMap(cookie,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("name",true);
	nick_ = ms_->GetValueFromMapByKey("nick",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	mid_ = ms_->GetValueFromMapByKey("mid",true);
	content_ = ms_->GetValueFromMapByKey("reason",true);
	
	if (user_.empty() || content_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText(kForwardWeiBo);
	WriteIndexFile();
	return 1;
}

int blog::XinLangWeiBo::AddComment(const char *packet)
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->Replace(cookie,";","&");
	ms_->SplitStringToMap(cookie,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("name",true);
	nick_ = ms_->GetValueFromMapByKey("nick",true);

	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	mid_ = ms_->GetValueFromMapByKey("mid",true);
	content_ = ms_->GetValueFromMapByKey("content",true);

	if (user_.empty() || content_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText(kAddComment);
	WriteIndexFile();
	return 1;
}

int blog::XinLangWeiBo::AddWeiBoMA(const char *packet)
{

	BoundaryParser bp;
	bp.Parse(packet,http_session_->m_datalen,http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	BoundaryValue *bv = NULL;
	bv = bp.GetBoundaryValue("name=\"content\"");

	if (bv == NULL)
	{
		return 0;
	}

	content_ = ms_->UTF8ToGBK(ms_->UrlDecode(string(bv->ptr,bv->len)));
	ms_->Replace(content_,"\r","");
	ms_->Replace(content_,"\n","");
	
	bv = bp.GetBoundaryValue("name=\"imsi\"");
	
	if (bv == NULL)
	{
		return 0;
	}

	string imsi(bv->ptr,bv->len);

	output_text_ = 
		"imsi=" + imsi + "\r\n" + 
		"msg=" + content_ + "\r\n" + 
		"msgtype=发表(手机)\r\n";
	WriteIndexFile();
	return 1;
}



string blog::XinLangWeiBo::GetOutputText(int state)
{
	if (state == kAddWeiBo)
	{
		return 
			"user=" + user_ + "\r\n" +  
			"nick=" + nick_ + "\r\n" +  
			"mid=" + mid_ + "\r\n" +
			"msg=" + content_ + "\r\n" +
			"msgtype=发表\r\n";
	}
	if (state == kForwardWeiBo)
	{
		return 
			"user=" + user_ + "\r\n" +  
			"nick=" + nick_ + "\r\n" +  
			"mid=" + mid_ + "\r\n" +
			"msg=" + content_ + "\r\n" +
			"msgtype=转载\r\n";
	}
	if (state == kAddComment)
	{
		return 
			"user=" + user_ + "\r\n" +  
			"nick=" + nick_ + "\r\n" +  
			"mid=" + mid_ + "\r\n" +
			"msg=" + content_ + "\r\n" +
			"msgtype=评论\r\n";
	}
	return "";
}