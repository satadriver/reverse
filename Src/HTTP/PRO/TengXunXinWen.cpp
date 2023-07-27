
#include "StdAfx.h"

#include "TengXunXinWen.h"

TengXunXinWen::TengXunXinWen()
{
	software_ = "腾讯新闻";
	pro_ = "phoneread_newstx";
	
}

TengXunXinWen::~TengXunXinWen()
{
	
}

int TengXunXinWen::Is(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("w.coral.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/article/comment/") != -1 ) )	
	{
		return kAddCommentC;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("w.inews.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/shareQQNewsMulti") != -1 ) )
	{
		return kAddCommentM;
	}
	return 0;
}

int TengXunXinWen::Process(const char *packet,int action)
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


int TengXunXinWen::AddCommentC(const char *packet)
{	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	string user = ms_->GetValueFromMapByKey("ptui_loginuin");

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("content",true);

	if (user.empty() || content.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";;
	WriteIndexFile();
	return 1;
}

int TengXunXinWen::AddCommentM(const char *packet)
{	
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	string imei = ms_->GetValueFromMapByKey("devid");

	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');

	string user = ms_->GetValueFromMapByKey("ptui_loginuin");
	if (user.empty())
	{
		user = ms_->GetValueFromMapByKey("luin");
		ms_->Replace(user,"o","");
	}

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("content",true);
	
	if (title.empty() || content.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"mid=" + title + "\r\n" + 
		"platform=手机\r\n"
		"state=评论\r\n";
		

	WriteIndexFile();
	return 1;
}

