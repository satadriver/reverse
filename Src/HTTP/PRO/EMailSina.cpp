
#include "StdAfx.h"

#include "EMailSina.h"


email::EMailSina::EMailSina(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMailSina::~EMailSina()
{
	
}


int email::EMailSina::IsEMailSina(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("mail.sina.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("send_mail")!=-1))
	{
		return kPCWebSendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.sina.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/classic/send.php")!=-1))
	{
		return kMWebSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("mail.sina.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/classic/uploadatt")!=-1))
	{
		return kPCWebUploadAttachment;
	}

	return 0;
}

int email::EMailSina::Process(const char *packet,int action)
{
	if (action == kMWebSendMail)
	{
		version_ = "ÒÆ¶¯¶Ëä¯ÀÀÆ÷";
		return MWebSendMail(packet);
	}
	
	if (action == kPCWebSendMail)
	{
		version_ = "µçÄÔä¯ÀÀÆ÷";
		return PCWebSendMail(packet);
	}

	if (action == kPCWebUploadAttachment)
	{
		return PCWebUploadAttachment(packet);
	}
	
	return 0;
}

int email::EMailSina::MWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	from_ = ms_->GetValueFromMapByKey("from",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("body",true);
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}

	WriteVirtualIdentity(from_,"·¢ËÍÓÊ¼þ");

	ConstructSendMailOutputString();
	WriteIndexFile();
	
	return 1;
}

int email::EMailSina::PCWebSendMail(const char *packet)
{

	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	ms_->Replace(boundary,"-","");
	if (boundary.empty())
	{
		return 0;
	}
	
	boundary = "\r\n------" + boundary;

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"sender\"\r\n\r\n",boundary);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"to\"\r\n\r\n",boundary);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"subj\"\r\n\r\n",boundary,true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"msgtxt\"\r\n\r\n",boundary,true);
	
	string att_temp = ms_->GetValueBetweenBeginEnd(src_data,"name=\"att_swf\"\r\n\r\n",boundary,true);

	if (!att_temp.empty())
	{
		ms_->SplitStringToMap(att_temp,',',':',"\"");
		attachment_ = ms_->GetValueFromMapByKey("name");
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

int email::EMailSina::PCWebUploadAttachment(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}

	boundary = "\r\n--" + boundary;
	
	from_ = ms_->GetValueBetweenBeginEnd(http_session_->m_Requestline.m_URI.GetBuffer(0),"email=","&");
	
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