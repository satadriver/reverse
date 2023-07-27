
#include "StdAfx.h"

#include "XinLangXinWen.h"

XinLangXinWen::XinLangXinWen()
{
	software_ = "新浪新闻";
	pro_ = "phoneread_newsxl";
}

XinLangXinWen::~XinLangXinWen()
{
	
}

int XinLangXinWen::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("comment5.news.sina.com.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/cmnt/submit") != -1 ) )	
	{
		return kAddCommentC;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("api.sina.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/sinago/comment.json") != -1 ) )	
	{
		return kAddCommentM;
	}
	return 0;
}

int XinLangXinWen::Process(const char *packet,int action)
{
	if (action == kAddCommentC)
	{
		return AddCommentC(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}


int XinLangXinWen::AddCommentC(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, '&','=');

	string user = ms_->GetValueFromMapByKey("user",true);

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string content = ms_->GetValueFromMapByKey("content",true);
	string url = ms_->GetValueFromMapByKey("share_url");

	if (content.empty() || url.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"oid=" + url + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int XinLangXinWen::AddCommentM(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	string imei = ms_->GetValueFromMapByKey("imei");

	string src_data = GetPostData_tmp(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("content",true);
	string url = ms_->GetValueFromMapByKey("link",true);
	
	if (title.empty() || content.empty() || url.empty())
	{
		return 0;
	}

	output_text_ = 
		"imei=" + imei + "\r\n" +
		"msg=" + content + "\r\n" +
		"mid=" + title + "\r\n" + 
		"oid=" + url + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
		
	WriteIndexFile();
	return 1;
}

