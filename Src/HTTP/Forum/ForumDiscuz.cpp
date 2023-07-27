#include "StdAfx.h"

#include "ForumDiscuz.h"

forum::Discuz::Discuz(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	host_ = http_session->m_Requestline.m_Host.GetBuffer(0);
}

forum::Discuz::~Discuz()
{
	
}

int forum::Discuz::IsDiscuz(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_URI.Find("/member.php?mod=logging&action=login&loginsubmit=yes")!=-1))
	{
		return kPCWebLogin;
	}
	if((http_session->m_Requestline.m_URI.Find("/forum.php?mod=post&action=newthread")!=-1))
	{
		return kPCWebNewPost;
	}
	if((http_session->m_Requestline.m_URI.Find("/forum.php?mod=post")!=-1))
	{
		return kPCWebNewComment;
	}
	return 0;
}

int forum::Discuz::Process(const char *packet,int action)
{
	
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	if (action == kPCWebNewPost)
	{
		return PCWebNewPost(packet);
	}
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	return 0;
}

int forum::Discuz::PCWebLogin(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	if ( user_.empty() || password.empty())
	{
		return 0;
	}
	
	WriteVirtualIdentity(user_,"ÉÏÏß");
	output_text_ =
		"nick=" + host_ + "\r\n" + 
		"user=" + user_ + "\r\n" +
		"pass=" + password + "\r\n" +
		"state=µÇÂ¼\r\n";
	WriteIndexFile();
	return 1;
}

int forum::Discuz::PCWebNewPost(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');

	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("message",true);

	if (title_.empty() || content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);
	output_text_ =	
		"nick=" + host_ + "\r\n" +
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"state=·¢Ìû\r\n";
	WriteIndexFile();
	return 1;
}


int forum::Discuz::PCWebNewComment(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	content_ = ms_->GetValueFromMapByKey("message",true);
	if (content_.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ =
		"nick=" + host_ + "\r\n" + 
		"msg=" + content_ + "\r\n" +
		"state=¸úÌû\r\n";
	WriteIndexFile();
	return 1;
}