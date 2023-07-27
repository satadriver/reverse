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
	//ÓÊ¼þ
	if((http_session->m_Requestline.m_Host.Find("mail.sina.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/classic/send.php")!=-1))
	{
		return kPCWebSendMail;
	}
    //¸½¼þ
	if((http_session->m_Requestline.m_Host.Find("mail.sina.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/classic/uploadatt.php")!=-1)) 
	{
		return kPCWebUploadAttachment;
	}
	//Phone
	if((http_session->m_Requestline.m_Host.Find("mail.sina.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wa.php?a=send_mail")!=-1))
	{
		return kMWebSendMail;
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

	StringManipulation sm;
	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary);
	
	
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"sender\"");
	bv = bp.GetBoundaryValue(key);
	

	if (bv == NULL || key.empty())
	{
		return 0;
	}
	
	from_ = ms_->UTF8ToGBK(sm.UrlDecode(string(bv->ptr, bv->len)));
	

	key = bp.GetBoundaryKey("name=\"to\"");
	bv = bp.GetBoundaryValue(key);
	
	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	
	to_ = ms_->UTF8ToGBK(sm.UrlDecode(string(bv->ptr, bv->len)));

	key = bp.GetBoundaryKey("name=\"subj\"");
	bv = bp.GetBoundaryValue(key);
	
	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	
	title_ = ms_->UTF8ToGBK(sm.UrlDecode(string(bv->ptr, bv->len)));

	key = bp.GetBoundaryKey("name=\"msgtxt\"");
	bv = bp.GetBoundaryValue(key);
	
	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	
	content_ = ms_->UTF8ToGBK(sm.UrlDecode(string(bv->ptr, bv->len)));

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
	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	string user = ms_->GetValueFromMapByKey("email", true);

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
	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}
