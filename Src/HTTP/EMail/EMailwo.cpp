
#include "StdAfx.h"

#include "EmailWo.h"
#include "../BoundaryParser.h"

email::EMailWo::EMailWo(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMailWo::~EMailWo()
{
	
}

int email::EMailWo::IsEMailWo(HttpSession *http_session)
{
	//邮件
	if((http_session->m_Requestline.m_Host.Find("mail.wo.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mail/emailSend.action")!=-1))
	{
		return kPCWebSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.wo.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/fileUpload.action")!=-1))
	{
		return kPCWebUploadAttachment;
	}
	return 0;
}

int email::EMailWo::Process(const char *packet,int action)
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

//电脑浏览器
int email::EMailWo::PCWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data, '&', '=');

	from_ = ms_->GetValueFromMapByKey("emailFrom", true);
	to_ = ms_->GetValueFromMapByKey("emailTo", true);
	title_ = ms_->GetValueFromMapByKey("emailSubject", true);
	content_ = ms_->GetValueFromMapByKey("emailContent", true);
		
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMailWo::PCWebUploadAttachment(const char *packet)
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
	from_ = ms_->GetValueFromMapByKey("loginUser");
	if (from_.empty())
	{
		from_ = "Null";
	}
	//数据部分
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
    //文件大小
	string tmp_file_size = ms_->GetValueBetweenBeginEnd(src_data,"name=\"filesize\"\r\n\r\n",boundary);
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

