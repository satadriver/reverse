#include "StdAfx.h"

#include "MaoPu.h"

MaoPu::MaoPu()
{
	software_ = "Ã¨ÆË";
}

MaoPu::~MaoPu()
{
	
}

int MaoPu::Is(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("passport.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("ajax/login")!=-1))
	{
		return kLoginCW;
	}
	if((http_session->m_Requestline.m_Host.Find("m.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/client/account/signin.xml")!=-1))
	{
		return kLoginM;
	}
	if((http_session->m_Requestline.m_Host.Find("dzh.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/audit/subject-add")!=-1))
	{
		return kAddPostCW;
	}
	if((http_session->m_Requestline.m_Host.Find("m.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/client/subject/update.xml")!=-1))
	{
		return kAddPostM;
	}
	if((http_session->m_Requestline.m_Host.Find("dzh.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/reply/quick")!=-1))
	{
		return kAddCommentCW;
	}
	if((http_session->m_Requestline.m_Host.Find("m.mop.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/client/reply/update.xml")!=-1))
	{
		return kAddCommentM;
	}
	return 0;
}

int MaoPu::Process(const char *packet,int action)
{
	
	if (action == kLoginCW)
	{
		return LoginCW(packet);
	}
	if (action == kLoginM)
	{
		return LoginM(packet);
	}
	if (action == kAddPostCW)
	{
		return AddPostCW(packet);
	}
	if (action == kAddPostM)
	{
		return AddPostM(packet);
	}
	if (action == kAddCommentCW)
	{
		return AddCommentCW(packet);
	}
	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}

int MaoPu::LoginCW(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	ms_->SplitStringToMap(src_data,'&','=');
	string user = ms_->GetValueFromMapByKey("loginName",true);
	string password = ms_->GetValueFromMapByKey("loginPasswd",true);

	if ( user.empty() || password.empty())
	{
		return 0;
	}
	

	output_text_ =	
		"user=" + user + "\r\n" +
		"pass=" + password + "\r\n" +
		"state=µÇÂ¼\r\n";
	
	WriteIndexFile();
	return 1;
}

int MaoPu::LoginM(const char *packet)
{
	string src_data(packet);
	string::size_type pos = src_data.find("name");
	if (pos == string::npos)
	{
		return 0;
	}
	src_data = src_data.substr(pos);

	ms_->SplitStringToMap(src_data,'&','=');
	string user = ms_->GetValueFromMapByKey("name",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	if ( user.empty() || password.empty())
	{
		return 0;
	}
	
	WriteVirtualIdentity(user,"ÉÏÏß");
	output_text_ =	
		"user=" + user + "\r\n" +
		"pass=" + password + "\r\n" +
		"state=µÇÂ¼\r\n";
	
	WriteIndexFile();
	return 1;
}

int MaoPu::AddPostCW(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');

	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("body",true);

	if (title.empty() || content.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content);
	output_text_ =
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"state=·¢Ìû\r\n";

	WriteIndexFile();
	return 1;
}

int MaoPu::AddPostM(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("body",true);

	if (title.empty() || content.empty())
	{
		return 0;
	}

	WriteCaseInfo(content);

	output_text_ =	
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"state=·¢Ìû\r\n";
	
	WriteIndexFile();
	return 1;
}

int MaoPu::AddCommentCW(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("quickContent",true);
	if (content.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content);

	output_text_ = 
		"msg=" + content + "\r\n" +
		"state=¸úÌû\r\n";
	WriteIndexFile();
	return 1;
}

int MaoPu::AddCommentM(const char *packet)
{
	string src_data(packet);
	string::size_type pos = src_data.find("name");
	if (pos == string::npos)
	{
		return 0;
	}
	src_data = src_data.substr(pos);

	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("body",true);

	if ( title.empty() || content.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content);

	output_text_ = 
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"state=¸úÌû\r\n";
	
	WriteIndexFile();
	return 1;
}