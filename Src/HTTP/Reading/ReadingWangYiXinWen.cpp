
#include "StdAfx.h"

#include "ReadingWangYiXinWen.h"

reading::WangYiXinWen::WangYiXinWen(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::WangYiXinWen::~WangYiXinWen()
{
	
}

int reading::WangYiXinWen::IsWangYiXinWen(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("sdk.comment.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/v1/products") != -1 ) )	
	{
		return kAddCommentC;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("comment.api.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/v1/products") != -1 ) )	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::WangYiXinWen::Process(const char *packet,int action)
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


int reading::WangYiXinWen::AddCommentC(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');

	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);
	}
    //提取内容
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	content_ = ms_->GetValueFromMapByKey("content");
	//提不到了
	url_ = ms_->GetValueFromMapByKey("url");

	if (content_.empty()/* || url_.empty()*/)
	{
		return 0;
	}
	output_text_ =
		"user="	 + user_ + "\r\n" + 
		"oid=" + url_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int reading::WangYiXinWen::AddCommentM(const char *packet)
{		
	string src_data = GetPostData_tmp(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	user_ = ms_->GetValueFromMapByKey("userid",true);
	content_ = ms_->GetValueFromMapByKey("body",true);

	if (user_.empty() || content_.empty())
	{
		return 0;
	}

	output_text_ =
		"user="	 + user_ + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}