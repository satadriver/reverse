#include "StdAfx.h"

#include "Discuz.h"

Discuz::Discuz()
{
	software_ = "Discuz";
}

Discuz::~Discuz()
{
	
}

int Discuz::Is(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_URI.Find("/member.php?mod=logging&action=login&loginsubmit=yes")!=-1))
	{
		return kLoginCW;
	}
	if((http_session->m_Requestline.m_URI.Find("/forum.php?mod=post&action=newthread")!=-1))
	{
		return kAddPostCW;
	}
	if((http_session->m_Requestline.m_URI.Find("/forum.php?mod=post")!=-1))
	{
		return kAddCommentCW;
	}
	return 0;
}

int Discuz::Process(const char *packet,int action)
{
	
	if (action == kLoginCW)
	{
		return LoginCW(packet);
	}
	if (action == kAddPostCW)
	{
		return AddPostCW(packet);
	}
	if (action == kAddCommentCW)
	{
		return AddCommentCW(packet);
	}
	return 0;
}

int Discuz::LoginCW(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string user = ms_->GetValueFromMapByKey("username",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	if ( user.empty() || password.empty())
	{
		return 0;
	}
	
	string host = http_session_->m_Requestline.m_Host.GetBuffer(0);
	WriteVirtualIdentity(user,"ÉÏÏß");
	output_text_ =
		"nick=" + host + "\r\n" + 
		"user=" + user + "\r\n" +
		"pass=" + password + "\r\n" +
		"state=µÇÂ¼\r\n";
	
	WriteIndexFile();
	return 1;
}

int Discuz::AddPostCW(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');

	string title = ms_->GetValueFromMapByKey("subject",true);
	string content = ms_->GetValueFromMapByKey("message",true);

	if (title.empty() || content.empty())
	{
		return 0;
	}
	
	string host = http_session_->m_Requestline.m_Host.GetBuffer(0);
	WriteCaseInfo(content);
	output_text_ =	
		"nick=" + host + "\r\n" +
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"state=·¢Ìû\r\n";

	WriteIndexFile();
	return 1;
}

int Discuz::AddCommentCW(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("message",true);
	if (content.empty())
	{
		return 0;
	}
	string host = http_session_->m_Requestline.m_Host.GetBuffer(0);
	WriteCaseInfo(content);
	output_text_ =
		"nick=" + host + "\r\n" + 
		"msg=" + content + "\r\n" +
		"state=¸úÌû\r\n";
	WriteIndexFile();
	return 1;
}