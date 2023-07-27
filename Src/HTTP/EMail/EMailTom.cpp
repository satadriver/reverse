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
	//�����ʼ�
	if((http_session->m_Requestline.m_Host.Find("mail1.tom.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/writemail/sendmail.action")!=-1)) 
	{
		return kPCWebSendMail;
	}
	//�ϴ�����
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
		version_ = "���������";
		return PCWebSendMail(packet);
	}
	if (action == kPCWebUploadAttachment)
	{
		version_ = "���������";
		return PCWebUploadAttachment(packet);
	}
	return 0;
}

//���ʼ�
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
	WriteVirtualIdentity(from_,"�����ʼ�");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

//����
int email::EMailTom::PCWebUploadAttachment(const char *packet)
{	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	if (boundary.empty())
	{
		return 0;
	}
	boundary = "\r\n--" + boundary;	
    //��ȡ�û�
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,',',':',"\'");
	from_ = ms_->GetValueFromMapByKey("loginname");
	if (from_.empty())
	{
		from_ = "Null";
	}
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
    //�ļ���С
	string tmp_file_size = ms_->GetValueBetweenBeginEnd(src_data,"name=\"size\"\r\n\r\n",boundary);
	//������
	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"name\"\r\n\r\n",boundary,true);
	//�ļ�����
	if (!attachment_.empty() && !tmp_file_size.empty())
	{ 
		int file_size = ms_->ToInt(tmp_file_size);
		//�ж��ļ���СС��5M
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

