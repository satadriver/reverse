#include "StdAfx.h"
#include "Email189.h"

email::EMail_189::EMail_189(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail_189::~EMail_189()
{
	
}

int email::EMail_189::IsEMail_189(HttpSession *http_session)
{
	//�ʼ�
	if((http_session->m_Requestline.m_Host.Find("webmail30.189.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/w2/mail/sendMail.do")!=-1))
	{
		return kPCWebSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("webmail30.189.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/w2/mail/upload.do")!=-1))
	{
		return kPCWebUploadAttachment;
	}
	//ʹ��IE ����������ʼ�
	if((http_session->m_Requestline.m_Host.Find("mail.189.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/sendMail.do")!=-1))
	{
		return kMWebSendMailIE;
	}
	//ʹ��IE ������ϴ�����
	if((http_session->m_Requestline.m_Host.Find("mail.189.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/upload")!=-1))
	{
		return kPCWebUploadAttachmentIE;
	}
	return 0;
}

int email::EMail_189::Process(const char *packet,int action)
{
	if (action == kPCWebSendMail)
	{
		return PCWebSendMail(packet);
	}
	if (action == kPCWebUploadAttachment)
	{
		return PCWebUploadAttachment(packet);
	}
	if (action == kMWebSendMailIE)
	{
		return PCWebSendMailIE(packet);
	}
	if (action == kPCWebUploadAttachmentIE)
	{
		return PCWebUploadAttachmentIE(packet);
	}
	return 0;
}

//���������
int email::EMail_189::PCWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));   	     
	ms_->SplitStringToMap(src_data, '&', '=');
	//������
	from_ = ms_->GetValueFromMapByKey("from", true);
    //�ռ���
	to_ = ms_->GetValueFromMapByKey("to", true);
    //����
	title_ = ms_->GetValueFromMapByKey("subject", true);
	//����
	content_ = ms_->GetValueFromMapByKey("content", true);
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	WriteVirtualIdentity(from_,"�����ʼ�");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail_189::PCWebUploadAttachment(const char *packet)
{	
	//������
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	ms_->SplitStringToMap(cookie,";","=");
	string user = ms_->GetValueFromMapByKey("189ACCOUNT", true);
	//���ݲ���
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary);
	
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"Filedata\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);
	//д����ʱ�ļ�
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
	
}
int email::EMail_189::PCWebSendMailIE(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));

	//boundary ֱ����ȡ
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	from_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"sender\"\r\n\r\n",boundary,true);
	to_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"to\"\r\n\r\n",boundary,true);
	title_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"subject\"\r\n\r\n",boundary,true);
	content_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"content_\"\r\n\r\n",boundary,true);

	WriteVirtualIdentity(from_,"�����ʼ�");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail_189::PCWebUploadAttachmentIE(const char *packet)
{
// 	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
// 	ms_->Replace(uri, "?", "&");
// 	ms_->SplitStringToMap(uri, "&", "=");
// 	string user = ms_->GetValueFromMapByKey("email", true);
// 	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
// 	//boundary �����ȡ
// 	BoundaryParser bp;
// 	bp.Parse(packet, http_session_->m_datalen, boundary);
// 	
// 	BoundaryValue *bv = NULL;
// 	string key;
// 	key = bp.GetBoundaryKey("name=\"Filedata\"");
// 	bv = bp.GetBoundaryValue(key);
// 	if (bv == NULL || key.empty())
// 	{
// 		return 0;
// 	}
// 	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);
// 
// 	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
// 	ConstructUploadAttachmentOuptputString();
// 	WriteIndexFile(real_file_name);

	StringManipulation sm;
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary);
	BoundaryValue *bv = NULL;
	string key = bp.GetBoundaryKey("name=\"Filedata\"");
	bv = bp.GetBoundaryValue(key);
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	string file_name = sm.GetValueBetweenBeginEnd(key,"filename=\"","\"",true);
	attachment_ = file_name;
	if (file_name.empty())
	{
		return 0;
	}
	string real_file_name = WriteTempRealFile(bv->ptr,file_name.c_str(),bv->len);
	output_text_ = 
		"from=" + from_ + "\r\n" + 
		"onick=" + attachment_ + "\r\n" + 
		"state=�ϴ�����\r\n";
	WriteIndexFile(real_file_name);
	return 1;
}

