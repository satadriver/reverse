
#include "StdAfx.h"

#include "EMailQQ.h"

map<string,string>email::EMailQQ::filemd5_fileinfo_ = map<string,string>();

email::EMailQQ::EMailQQ(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{

}

email::EMailQQ::~EMailQQ()
{

}

int email::EMailQQ::IsEMailQQ(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("i.mail.qq.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/compose_send")!=-1))
	{
		return kAndroidPhoneSendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("en.mail.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/compose_send")!=-1))
	{
		return kPCWebEnglishSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("w.mail.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/cgi_redirect")!=-1))
	{
		return kPCWebBaseSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("w.mail.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/compose_send")!=-1))
	{
		return kMWebSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/compose_send")!=-1))
	{
		return kPCWebDefaultSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/uploadfile")!=-1))
	{
		return kPCWebUploadAttachment;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/uploadunite")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("CreateFile")!=-1))
	{
		return kPCWebUploadAttachmentInfo;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.ftn.qq.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ftn_handler/")!=-1))
	{
		return kPCWebUploadAttachmentBody;
	}
	return 0;
}


int email::EMailQQ::Process(const char *packet,int action)
{
	switch (action)
	{
	case kAndroidPhoneSendMail :
		version_ = "(ÊÖ»ú°²×¿App°æ)";
		return AndroidPhoneSendMail(packet);
	case kMWebSendMail :
		version_ = "(ÊÖ»úä¯ÀÀÆ÷°æ)";
		return MWebSendMail(packet);
	case kPCWebEnglishSendMail :
		version_ = "(µçÄÔä¯ÀÀÆ÷Ó¢ÎÄ°æ)";
		return PCWebEnglishSendMail(packet);
	case kPCWebBaseSendMail :
		version_ = "(µçÄÔä¯ÀÀÆ÷»ù±¾°æ)";
		return PCWebBaseSendMail(packet);
	case kPCWebDefaultSendMail:
		version_ = "(µçÄÔä¯ÀÀÆ÷Ä¬ÈÏ°æ)";
		return PCWebDefaultSendMail(packet);
	case kPCWebUploadAttachment:
		return PCWebUploadAttachment(packet);
	case kPCWebUploadAttachmentInfo:
		return PCWebUploadAttachmentInfo(packet);
	case kPCWebUploadAttachmentBody:
		return PCWebUploadAttachmentBody(packet);
	default:
		break;
	}

	return 0;
}

int email::EMailQQ::AndroidPhoneSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("sendmailname");
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	

	attachment_ = ms_->GetValueFromMapByKey("attachlist_log",true);
	if (!attachment_.empty())
	{
		int pos = attachment_.find("?");
		if (pos != string::npos)
		{
			attachment_ = attachment_.substr(0,pos);
		}
	}

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
	ConstructSendMailOutputString();
	WriteIndexFile();

	return 1;
}

int email::EMailQQ::MWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("sendmailname");
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
		
	attachment_ = ms_->GetValueFromMapByKey("attachlist_log",true);
	if (!attachment_.empty())
	{
		int pos = attachment_.find("?");
		if (pos != string::npos)
		{
			attachment_ = attachment_.substr(0,pos);
		}
	}
	
	if (from_.empty())
	{
		string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
		ms_->SplitStringToMap(cookie,';','=');
		from_ = ms_->GetValueFromMapByKey("ptui_loginuin");
	}
	
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
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMailQQ::PCWebEnglishSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("sendmailname");
	to_ = ms_->GetValueFromMapByKey("to",true);	
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content__html",true);

	
	attachment_ = ms_->GetValueFromMapByKey("attachlist_log",true);
	if (!attachment_.empty())
	{
		int pos = attachment_.find("?");
		if (pos != string::npos)
		{
			attachment_ = attachment_.substr(0,pos);
		}
	}
	
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
				
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMailQQ::PCWebBaseSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("sendmailname");
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);

	attachment_ = ms_->GetValueFromMapByKey("attachlist_log",true);
	if (!attachment_.empty())
	{
		int pos = attachment_.find("?");
		if (pos != string::npos)
		{
			attachment_ = attachment_.substr(0,pos);
		}
	}
	
	if (from_.empty())
	{
		string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
		ms_->SplitStringToMap(cookie,';','=');
		from_ = ms_->GetValueFromMapByKey("ptui_loginuin");
	}
	
	
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
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMailQQ::PCWebDefaultSendMail(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("sendmailname");
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);

	content_ = ms_->GetValueFromMapByKey("content__html",true);
	if (content_.empty())
	{
		content_ = ms_->GetValueFromMapByKey("content",true);
	}

	attachment_ = ms_->GetValueFromMapByKey("attachlist_log",true);
	if (!attachment_.empty())
	{
		string::size_type pos = attachment_.find("?");
		if (pos != string::npos)
		{
			attachment_ = attachment_.substr(0,pos);
		}
		else
		{
			pos = attachment_.find("(");
			if (pos != string::npos)
			{
				attachment_ = attachment_.substr(0,pos);
				ms_->Trim(attachment_);
			}
		}
	}
	

	if (from_.empty() || to_.empty() )
	{
		return 0;
	}


	WriteVirtualIdentity(from_,"·¢ËÍÓÊ¼þ");

	ConstructSendMailOutputString();
	WriteIndexFile();
	
	return 1;
}

int email::EMailQQ::PCWebUploadAttachment(const char *packet)
{

	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n--" + boundary;

	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	
	ms_->SplitStringToMap(cookie,';','=');
	from_ = ms_->GetValueFromMapByKey("ptui_loginuin");
	
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

int email::EMailQQ::PCWebUploadAttachmentInfo(const char *packet)
{
	string src_data(packet);
	ms_->Replace(src_data,"{","");
	ms_->Replace(src_data,"}","");
	ms_->Replace(src_data,"\"","");
	
	ms_->SplitStringToMap(src_data,',',':');

	string filemd5 = ms_->GetValueFromMapByKey("md5");
	string filename = ms_->GetValueFromMapByKey("path");
	string filesize = ms_->GetValueFromMapByKey("size");
	string fileinfo = filename + "&" + filesize;
	filemd5_fileinfo_.insert(std::make_pair(filemd5,fileinfo));
	return 0;
}

int email::EMailQQ::PCWebUploadAttachmentBody(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));

	string filemd5 = ms_->GetValueBetweenBeginEnd(src_data,"/ftn_handler/","?");
	
	map<string,string>::iterator iter = filemd5_fileinfo_.find(filemd5);
	if (iter != filemd5_fileinfo_.end())
	{
		string fileinfo = iter->second;
		string::size_type pos = fileinfo.find("&");
		if (pos != string::npos)
		{
			string file_name = fileinfo.substr(0,pos);
			string file_size = fileinfo.substr(pos + 1);
			int size = ms_->ToInt(file_size);

			string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
			ms_->SplitStringToMap(cookie,';','=');

			from_ = ms_->GetValueFromMapByKey("ptui_loginuin");
			attachment_ = ms_->UTF8ToGBK(file_name);
			string real_file_name = WriteTempRealFile(packet + 16 * 23 - 4,attachment_.c_str(),size);

			ConstructUploadAttachmentOuptputString();
			WriteIndexFile(real_file_name);
			filemd5_fileinfo_.erase(filemd5);
			return 1;
		}
	}

	return 0;
}