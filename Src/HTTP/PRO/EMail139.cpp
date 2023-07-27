
#include "StdAfx.h"

#include "EMail139.h"

email::EMail139::EMail139(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail139::~EMail139()
{
	
}

int email::EMail139::IsEMail139(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("appmail.mail.10086.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("func=mbox:compose")!=-1))
	{
		return kPCWebSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("html5.mail.10086.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("func=mbox:compose")!=-1))
	{
		return kMWebSendMail;
	}

	if((http_session->m_Requestline.m_Host.Find("appmail.mail.10086.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("func=attach:upload")!=-1))
	{
		return kPCWebUploadAttachment;
	}

	
	return 0;
}

int email::EMail139::Process(const char *packet,int action)
{

	if (action == kPCWebSendMail)
	{
		return PCWebSendMail(packet);
	}

	if (action == kMWebSendMail)
	{
		return MWebSendMail(packet);
	}

	if (action == kPCWebUploadAttachment)
	{
		return PCWebUploadAttachment(packet);
	}
	return 0;
	
}


int email::EMail139::PCWebSendMail(const char *packet)
{
	string src_data = ms_->UrlDecode(packet);
	
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"\"to\">","</string>",true);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);

	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"\"fileName\">","</string>",true);
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}

	WriteVirtualIdentity(from_,"发送邮件");

	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}


int email::EMail139::MWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	string::size_type pos = src_data.find("{\"attrs\":{");
	if (pos == string::npos)
	{
		return 0;
	}
	else
	{
		src_data = src_data.substr(pos + strlen("{\"attrs\":{"));
	}
	
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	from_ = ms_->GetValueFromMapByKey("account");
	to_ = ms_->GetValueFromMapByKey("to");
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	if (from_.empty() || to_.empty() /*|| title_.empty() || content_.empty()*/)
	{
		return 0;
	}
	
	WriteVirtualIdentity(from_,"发送邮件");

	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}


int email::EMail139::PCWebUploadAttachment(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}

	boundary = "\r\n--" + boundary;
	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));

	ms_->SplitStringToMap(cookie,',',':',"\'");
	from_ = ms_->GetValueFromMapByKey("loginname");

	if (from_.empty())
	{
		return 0;
	}


	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	string tmp_file_size = ms_->GetValueBetweenBeginEnd(src_data,"name=\"filesize\"\r\n\r\n",boundary);
	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"Filename\"\r\n\r\n",boundary,true);
	

	if (!attachment_.empty() && !tmp_file_size.empty())
	{ 
		int file_size = ms_->ToInt(tmp_file_size);
		if (  (file_size > 0 ) && (file_size < real_file_max_size_) )
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