#include "StdAfx.h"

#include "ForumMaoPu.h"

forum::MaoPu::MaoPu(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::MaoPu::~MaoPu()
{
	
}

int forum::MaoPu::IsMaoPu(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("passport.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("ajax/login")!=-1))
	{
		return kPCWebLogin;
	}
	if((http_session->m_Requestline.m_Host.Find("m.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/client/account/signin.xml")!=-1))
	{
		return kAndroidLogin;
	}
	if((http_session->m_Requestline.m_Host.Find("dzh.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/audit/subject-add")!=-1))
	{
		return kPCWebNewPost;
	}
	if((http_session->m_Requestline.m_Host.Find("m.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/client/subject/update.xml")!=-1))
	{
		return kAndroidNewPost;
	}
	if((http_session->m_Requestline.m_Host.Find("dzh.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/reply/quick")!=-1))
	{
		return kPCWebNewComment;
	}
	if((http_session->m_Requestline.m_Host.Find("m.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/client/reply/update.xml")!=-1))
	{
		return kAndroidNewComment;
	}
	return 0;
}

int forum::MaoPu::Process(const char *packet,int action)
{
	
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	if (action == kPCWebNewPost)
	{
		return PCWebNewPost(packet);
	}
	if (action == kAndroidNewPost)
	{
		return AndroidNewPost(packet);
	}
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	if (action == kAndroidNewComment)
	{
		return AndroidNewComment(packet);
	}
	return 0;
}
//µçÄÔÍøÒ³µÇÂ¼
int forum::MaoPu::PCWebLogin(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("loginName",true);
	password_ = ms_->GetValueFromMapByKey("loginPasswd",true);
	if ( user_.empty() || password_.empty())
	{
		return 0;
	}
	

	output_text_ =	
		"user=" + user_ + "\r\n" +
		"pass=" + password_ + "\r\n" +
		"state=µÇÂ¼\r\n";
	
	WriteIndexFile();
	return 1;
}

//°²×¿ÊÖ»úÉÏÏß-µÇÂ¼
int forum::MaoPu::AndroidLogin(const char *packet)
{
	string src_data(packet);
	string::size_type pos = src_data.find("name");
	if (pos == string::npos)
	{
		return 0;
	}
	src_data = src_data.substr(pos);

	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("name",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	if ( user_.empty() || password_.empty())
	{
		return 0;
	}
	
	WriteVirtualIdentity(user_,"ÉÏÏß");
	output_text_ =	
		"user=" + user_ + "\r\n" +
		"pass=" + password_ + "\r\n" +
		"state=µÇÂ¼\r\n";
	
	WriteIndexFile();
	return 1;
}
//µçÄÔÍøÒ³·¢Ìû
int forum::MaoPu::PCWebNewPost(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');

	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("body",true);

	if (title_.empty() || content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);
	output_text_ =
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"state=·¢Ìû\r\n";

	WriteIndexFile();
	return 1;
}
//ÊÖ»úä¯ÀÀÆ÷·¢Ìû
int forum::MaoPu::AndroidNewPost(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("body",true);

	if (title_.empty() || content_.empty())
	{
		return 0;
	}

	WriteCaseInfo(content_);

	output_text_ =	
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"state=·¢Ìû\r\n";
	
	WriteIndexFile();
	return 1;
}
//µçÄÔä¯ÀÀÆ÷¸úÌû
int forum::MaoPu::PCWebNewComment(const char *packet)
{

	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	content_ = ms_->GetValueFromMapByKey("quickContent",true);
	if (content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);

	output_text_ = 
		"msg=" + content_ + "\r\n" +
		"state=¸úÌû\r\n";
	WriteIndexFile();
	return 1;
}
//ÊÖ»úä¯ÀÀÆ÷¸úÌû
int forum::MaoPu::AndroidNewComment(const char *packet)
{
	string src_data(packet);
	string::size_type pos = src_data.find("name");
	if (pos == string::npos)
	{
		return 0;
	}
	src_data = src_data.substr(pos);

	ms_->SplitStringToMap(src_data,'&','=');
	title_ = ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("body",true);

	if ( title_.empty() || content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);

	output_text_ = 
		"msgtype=" + title_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"state=¸úÌû\r\n";
	
	WriteIndexFile();
	return 1;
}