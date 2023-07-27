
#include "StdAfx.h"

#include "EMail21CN.h"

email::EMail21CN::EMail21CN(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail21CN::~EMail21CN()
{
	
}

int email::EMail21CN::IsEMail21CN(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/login.perform")!=-1))
	{
		return kPCWebLogin;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wapmail/logon.do")!=-1))
	{
		return kMWebLogin;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("sendMail")!=-1))
	{
		return kPCWebSendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wapmail/sendWapMail.do")!=-1))
	{
		return kMWebSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/upload.do")!=-1))
	{
		return kPCWebUploadAttachment;
	}
	
	return 0;
}

int email::EMail21CN::Process(const char *packet,int action)
{

	if ( action == kPCWebLogin )
	{
		return PCWebLogin(packet);
	}

	if ( action == kMWebLogin)
	{
		return MWebLogin(packet);
	}
	
	if (action == kPCWebSendMail)
	{
		version_ = "(µçÄÔä¯ÀÀÆ÷)";
		return PCWebSendMail(packet);
	}

	if (action == kMWebSendMail)
	{
		version_ = "(ÒÆ¶¯¶Ëä¯ÀÀÆ÷)";
		return MWebSendMail(packet);
	}

	if (action == kPCWebUploadAttachment)
	{
		return PCWebUploadAttachment(packet);
	}

	return 0;
}


int email::EMail21CN::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("UserName");
	pass_ = ms_->GetValueFromMapByKey("passwd");
		
	if (user_.empty() || pass_.empty())
	{
		return 0;
	}

	WriteVirtualIdentity(user_,"ÉÏÏß");

	output_text_ = "user=" + user_ + "\r\n" +
		"pass=" + pass_ + "\r\n" +
		"state=µÇÂ½\r\n";
	
	WriteIndexFile();
	return 1;
}

int email::EMail21CN::MWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("loginName",true);
	pass_ = ms_->GetValueFromMapByKey("passwd");
	
	if (user_.empty() || pass_.empty())
	{
		return 0;
	}
	output_text_ = "user=" + user_ + "\r\n" +
		"pass=" + pass_ + "\r\n" +
		"state=µÇÂ½\r\n";
	
	WriteIndexFile();
		return 1;
}

int email::EMail21CN::PCWebSendMail(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n--" + boundary;

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"sender\"\r\n\r\n",boundary);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"to\"\r\n\r\n",boundary);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"subject\"\r\n\r\n",boundary,true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"content\"\r\n\r\n",boundary,true);
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}


	WriteVirtualIdentity(from_,"·¢ËÍÓÊ¼þ");

	ConstructSendMailOutputString();
	WriteIndexFile();

	return 1;
}

int email::EMail21CN::MWebSendMail(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n--" + boundary;

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	from_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"sender\"\r\n\r\n",boundary);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"to\"\r\n\r\n",boundary);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"subject\"\r\n\r\n",boundary,true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"content\"\r\n\r\n",boundary,true);
	
	if (from_.empty() || to_.empty() /*|| title_.empty() || content_.empty()*/)
	{
		return 0;
	}
	/*
	string temp_content = ms_->GetValueBetweenBeginEnd(content_,"<div>","</div>");
	if (!temp_content.empty())
	{
		content_ = temp_content;
	}
	*/

	WriteVirtualIdentity(from_,"·¢ËÍÓÊ¼þ");

	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail21CN::PCWebUploadAttachment(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n--" + boundary;
	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));

	ms_->SplitStringToMap(cookie,';','=');
	from_ = ms_->GetValueFromMapByKey("ACCOUNT");
	
	if (from_.empty())
	{
		return 0;
	}

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"Filename\"\r\n\r\n",boundary,true);
	
	char *stream = strstr(packet,"Content-Type: application/octet-stream\r\n\r\n");
	if (stream)
	{
		int offset = strlen("Content-Type: application/octet-stream\r\n\r\n");
		int file_zise = GetRealFileSize(stream + offset,boundary.c_str());
		if ( (file_zise > 0) && (file_zise < real_file_max_size_) )
		{
			string real_file_name = WriteTempRealFile(stream + offset,attachment_.c_str(),file_zise);
			
			ConstructUploadAttachmentOuptputString();
			WriteIndexFile(real_file_name);
		}
		else
		{
			ConstructUploadAttachmentOuptputString();
			WriteIndexFile();
		}
		return 1;
	}

	return 0;
}
