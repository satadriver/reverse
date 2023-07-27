
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
	//��ҳ�˵�¼(��������) 2017-06-06
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webmail/login.perform")!=-1))
	{
		return kPCWebLogin;
	}
    //��ҳ�˷����ʼ�
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/w2/mail/sendMail.do")!=-1))
	{
		return kPCWebSendMail;
	}
	//��ҳ�˷����ʼ�
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/w2/mail/writeMail.do")!=-1))
	{
		return kPCWebSendMail;
	}
	//��ҳ���ϴ�����
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/w2/mail/upload.do")!=-1))
	{
		return kPCWebUploadAttachment;
	}
    //�ֻ��˵�¼(��������)2017-06-06
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wapmail/logon.do")!=-1))
	{
		return kMWebLogin;
	}
	//�ֻ��˷����ʼ�
	if((http_session->m_Requestline.m_Host.Find("mail.21cn.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wapmail/sendWapMail.do")!=-1))
	{
		return kMWebSendMail;
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
		version_ = "(���������)";
		return PCWebSendMail(packet);
	}
	if (action == kMWebSendMail)
	{
		version_ = "(�ƶ��������)";
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
	WriteVirtualIdentity(user_,"����");
	output_text_ = "user=" + user_ + "\r\n" +
		"pass=" + pass_ + "\r\n" +
		"state=��½\r\n";	
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
		"state=��½\r\n";	
	WriteIndexFile();
		return 1;
}

int email::EMail21CN::PCWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,'&','=');

	from_ = ms_->GetValueFromMapByKey("from",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);

	WriteVirtualIdentity(from_,"�����ʼ�");
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
	
	if (from_.empty() || to_.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(from_,"�����ʼ�");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

//�ϴ�����
int email::EMail21CN::PCWebUploadAttachment(const char *packet)
{
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
	return 0;
}
