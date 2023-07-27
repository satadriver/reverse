
#include "StdAfx.h"

#include "ReadingTengXunXinWen.h"

reading::TengXunXinWen::TengXunXinWen(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::TengXunXinWen::~TengXunXinWen()
{
	
}

int reading::TengXunXinWen::IsTengXunXinWen(HttpSession *http_session)
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

int reading::TengXunXinWen::Process(const char *packet,int action)
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

int reading::TengXunXinWen::AddCommentC(const char *packet)
{	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	user_ = ms_->GetValueFromMapByKey("ptui_loginuin");

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	content_ = ms_->GetValueFromMapByKey("content",true);

	if (user_.empty() || content_.empty())
	{
		return 0;
	}
	output_text_ =
		"user="	 + user_ + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int reading::TengXunXinWen::AddCommentM(const char *packet)
{	
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(uri,'&','=');
	string imei = ms_->GetValueFromMapByKey("devid");

	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');

	user_ = ms_->GetValueFromMapByKey("ptui_loginuin");
	if (user_.empty())
	{
		user_ = ms_->GetValueFromMapByKey("luin");
		ms_->Replace(user_,"o","");
	}

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	if (title_.empty() || content_.empty())
	{
		return 0;
	}
	
	output_text_ =
		"user="	 + user_ + "\r\n" + 
		"nick="	 + imei + "\r\n" +	  //IMEI
		"mid=" + title_ + "\r\n" +   //标题
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}