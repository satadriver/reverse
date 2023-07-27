
#include "StdAfx.h"

#include "ReadingXinLangXinWen.h"

reading::XinLangXinWen::XinLangXinWen(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::XinLangXinWen::~XinLangXinWen()
{
	
}

int reading::XinLangXinWen::IsXinLangXinWen(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("comment5.news.sina.com.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/cmnt/submit") != -1 ) )	
	{
		return kAddCommentC;
	}
	//android	
	if(  ( http_session->m_Requestline.m_Host.Find("newsapi.sina.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("resource=comment/post") != -1 ) )	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::XinLangXinWen::Process(const char *packet,int action)
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


int reading::XinLangXinWen::AddCommentC(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, '&','=');

	user_ = ms_->GetValueFromMapByKey("user",true);

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	content_ = ms_->GetValueFromMapByKey("content",true);
	url_ = ms_->GetValueFromMapByKey("share_url");

	if (content_.empty() || url_.empty())
	{
		return 0;
	}

	output_text_ =
		"user=" + user_ + "\r\n" +
		"oid=" + url_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//android
int reading::XinLangXinWen::AddCommentM(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	string imei = ms_->GetValueFromMapByKey("imei");

	string src_data = GetPostData_tmp(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	url_ = ms_->GetValueFromMapByKey("link",true);
	
	if (title_.empty() || content_.empty() || url_.empty())
	{
		return 0;
	}

	output_text_ =
		"nick="	 + imei + "\r\n" + 
		"oid=" + url_ + "\r\n" +
		"mid=" + title_ + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}