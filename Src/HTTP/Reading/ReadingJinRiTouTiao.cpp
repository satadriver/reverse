
#include "StdAfx.h"
#include "ReadingJinRiTouTiao.h"

reading::JinRiTouTiao::JinRiTouTiao(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::JinRiTouTiao::~JinRiTouTiao()
{
	
}

int reading::JinRiTouTiao::IsJinRiTouTiao(HttpSession *http_session)
{
	//电脑端
	if( (http_session->m_Requestline.m_Host.Find("www.toutiao.com")  != -1 ) &&  
		(http_session->m_Requestline.m_URI.Find("/api/comment/post_comment/") != -1 ))	
	{
		return kAddCommentC;
	}
    //手机端
	if(  ( http_session->m_Requestline.m_Host.Find("isub.snssdk.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/2/data/post_message") != -1 ) )	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::JinRiTouTiao::Process(const char *packet,int action)
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


int reading::JinRiTouTiao::AddCommentC(const char *packet)
{
    //数据部分
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
    //内容
	content_ = ms_->GetValueFromMapByKey("content");
	if (content_.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + content_ + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

//android评论
int reading::JinRiTouTiao::AddCommentM(const char *packet)
{		
	string src_data = GetPostData_tmp(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	content_ = ms_->GetValueFromMapByKey("text",true);
	if (/*user_.empty() || */content_.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}