
#include "StdAfx.h"

#include "EMail126.h"

email::EMail126::EMail126(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail126::~EMail126()
{

}

int email::EMail126::IsEMail126(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("entry.mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/corEMail/fcg/ntesdoor2")!=-1))
	{
		return kLogin;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("compose/compose")!=-1))
	{
		return kPCWebJY6SendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("compose/compose")!=-1))
	{
		return kPCWebJY5SendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose&")!=-1))
	{
		return kPCWeb6SendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose&")!=-1))
	{
		return kPCWeb5SendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
	{
		return kMWebSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("ver=js")!=-1))
	{
		return kPCWebJSUploadAttachment;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("ver=jy")!=-1))
	{
		return kPCWebJYUploadAttachment;
	}
	return 0;
}

int email::EMail126::Process(const char *packet,int action)
{
	switch (action)
	{
	case kPCWebJY6SendMail :
		version_ = "(电脑浏览器简约6.0版)";
		return PCWebJY6SendMail(packet);
	case kPCWeb6SendMail :
		version_ = "(电脑浏览器6.0版 默认版本)";
		return PCWeb6SendMail(packet);
	case kPCWebJY5SendMail :
		version_ = "(电脑浏览器简约5.0版)";
		return PCWebJY5SendMail(packet);
	case kPCWeb5SendMail :
		version_ = "(电脑浏览器5.0版)";
		return PCWeb5SendMail(packet);
	case kMWebSendMail :
		return MWebSendMail(packet);
	case kPCWebJSUploadAttachment :
//		return PCWebJSUploadAttachment(packet);
	case kPCWebJYUploadAttachment :
//		return PCWebJYUploadAttachment(packet);
	default:
		break;
	}
	return 0;
}


int email::EMail126::PCWebJY6SendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWeb6SendMail(const char *packet)
{
	string src_data = ms_->UrlDecode(packet);
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"to\"><string>","</string>",true);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}

	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWebJY5SendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}

	
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWeb5SendMail(const char *packet)
{
	string src_data = ms_->UrlDecode(packet);
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"to\"><string>","</string>",true);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::MWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}

	string::size_type pos_begin = content_.find("\n\n");
	if (pos_begin != string::npos)
	{
		content_ = content_.substr(pos_begin);
	}
	string::size_type pos_end = content_.find("\n\n\n--");
	if (pos_end != string::npos)
	{
		content_ = content_.substr(0,pos_end);
	}

	WriteVirtualIdentity(from_,"发送邮件");

	ConstructSendMailOutputString();
	WriteIndexFile();

	return 1;
}


int email::EMail126::PCWebJSUploadAttachment(const char *packet)
{
	ms_->SplitStringToMap(http_session_->m_Requestline.m_URI.GetBuffer(0),'&','=');
	from_ = ms_->GetValueFromMapByKey("uid");
	
	if (from_.empty())
	{
		return 0;
	}
	int file_size = http_session_->m_databufsize;
	
	if ( (file_size > 0 )&& (file_size < real_file_max_size_) )
	{
		attachment_ = "unknown_file_name";
		string real_file_name = WriteTempRealFile(packet,attachment_.c_str(),http_session_->m_databufsize);
		

		ConstructUploadAttachmentOuptputString();
		WriteIndexFile(real_file_name);
	}
	else
	{
		// 由于又没文件名又没文件实体故略去不写
		//	ConstructUploadAttachmentOuptputString();
		//	WriteIndexFile();
	}
	
	return 1;
}

int email::EMail126::PCWebJYUploadAttachment(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n--" + boundary;
	
	ms_->SplitStringToMap(http_session_->m_Requestline.m_URI.GetBuffer(0),'&','=');
	from_ = ms_->GetValueFromMapByKey("uid");
	
	if (from_.empty())
	{
		return 0;
	}


	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	string tmp_file_size = ms_->GetValueBetweenBeginEnd(src_data,"name=\"size\"\r\n\r\n",boundary);
	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"name\"\r\n\r\n",boundary,true);
	
	
	if (!attachment_.empty() && !tmp_file_size.empty())
	{ 
		int file_size = ms_->ToInt(tmp_file_size);
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			char *stream = strstr(packet,"Content-Type: application/octet-stream\r\n\r\n");
			if (stream)
			{
				int offset = strlen("Content-Type: application/octet-stream\r\n\r\n");
				string real_file_name = WriteTempRealFile(stream + offset,attachment_.c_str(),file_size);
				
				ConstructUploadAttachmentOuptputString();
				WriteIndexFile(real_file_name);
			}
			else
			{
				ConstructUploadAttachmentOuptputString();
				WriteIndexFile();
			}
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
