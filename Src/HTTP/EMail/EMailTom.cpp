#include "StdAfx.h"
#include "EMailTom.h"

email::EMailTom::EMailTom(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMailTom::~EMailTom()
{
	
}

int email::EMailTom::IsEMailTom(HttpSession *http_session)
{
	//发送邮件
	if((http_session->m_Requestline.m_Host.Find("mail1.tom.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/writemail/sendmail.action")!=-1)) 
	{
		return kPCWebSendMail;
	}
	//上传附件
	if( (http_session->m_Requestline.m_Host.Find("mail1.tom.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/webmail/writemail/dfsuploadAttachment.do")!=-1))
	{
		return kPCWebUploadAttachment;
	}
	return 0;
}

int email::EMailTom::Process(const char *packet,int action)
{
	if (action == kPCWebSendMail)
	{
		version_ = "电脑浏览器";
		return PCWebSendMail(packet);
	}
	if (action == kPCWebUploadAttachment)
	{
		version_ = "电脑浏览器";
		return PCWebUploadAttachment(packet);
	}
	return 0;
}

//发邮件
int email::EMailTom::PCWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	ms_->SplitStringToMap(src_data, '&', '=');
	from_ = ms_->GetValueFromMapByKey("from", true);
	to_ = ms_->GetValueFromMapByKey("to", true);
	title_ = ms_->GetValueFromMapByKey("subject", true);
	content_ = ms_->GetValueFromMapByKey("text", true);

	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

//附件
int email::EMailTom::PCWebUploadAttachment(const char *packet)
{	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	if (boundary.empty())
	{
		return 0;
	}
	boundary = "\r\n--" + boundary;	
    //获取用户
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,',',':',"\'");
	from_ = ms_->GetValueFromMapByKey("loginname");
	if (from_.empty())
	{
		from_ = "Null";
	}
	//数据部分
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
    //文件大小
	string tmp_file_size = ms_->GetValueBetweenBeginEnd(src_data,"name=\"size\"\r\n\r\n",boundary);
	//附件名
	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"name\"\r\n\r\n",boundary,true);
	//文件部分
	if (!attachment_.empty() && !tmp_file_size.empty())
	{ 
		int file_size = ms_->ToInt(tmp_file_size);
		//判断文件大小小于5M
		if ((file_size >0) && (file_size < real_file_max_size_))
		{
			char *stream = strstr(packet,"Content-Type:");
			char *streamEnd = strstr(stream,"\r\n\r\n");
			if (streamEnd)
			{
				int offset = strlen("\r\n\r\n");
				string real_file_name = WriteTempRealFile(streamEnd + offset,attachment_.c_str(),file_size);
				
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

