
#include "StdAfx.h"

#include "ReadingFengHuangXinWen.h"

reading::FengHuangXinWen::FengHuangXinWen(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::FengHuangXinWen::~FengHuangXinWen()
{
	
}

int reading::FengHuangXinWen::IsFengHuangXinWen(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("comment.ifeng.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/post.php?format=new") != -1 ) )
	{
		return kAddCommentC;
	}
	/*
	if( ( http_session->m_Requestline.m_Host.Find("icomment.ifeng.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wappost.php") != -1 ) )
	{
		return kAddCommentM;
	}
	*/
	return 0;
}

int reading::FengHuangXinWen::Process(const char *packet,int action)
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


int reading::FengHuangXinWen::AddCommentC(const char *packet)
{	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	user_ = ms_->GetValueFromMapByKey("userid");

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	title_ =  ms_->GetValueFromMapByKey("docName",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	url_ = ms_->GetValueFromMapByKey("docUrl");
	
	if (title_.empty() || content_.empty() || url_.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + user_ + "\r\n" +
		"mid=" + title_ + "\r\n" + 
		"oid=" + url_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int reading::FengHuangXinWen::AddCommentM(const char *packet)
{		
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	title_ =  ms_->GetValueFromMapByKey("docName",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	url_ = ms_->GetValueFromMapByKey("docUrl",true);
	if (title_.empty() || content_.empty() || url_.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + user_ + "\r\n" +
		"mid=" + title_ + "\r\n" + 
		"oid=" + url_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}