#include "StdAfx.h"
#include "BlogSohuBlog.h"

blog::SohuBlog::SohuBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::SohuBlog::~SohuBlog()
{
	
}

int blog::SohuBlog::IsSohuBlog(HttpSession *http_session)
{
	//������
	if( (http_session->m_Requestline.m_Host.Find("blog.sohu.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/manage/entry.do") != -1))	
	{
		return kWebNewTopic;
	}
	//���� �ظ�
	if( (http_session->m_Requestline.m_Host.Find(".blog.sohu.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/a/app/discuss/save.htm?_input_encode=UTF-8") != -1))	
	{
		return kWebPostComment;	
	}
	//�ϴ��ļ� ����
	if( (http_session->m_Requestline.m_Host.Find("upload.i.sohu.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/a/blog/upload") != -1))	
	{
		return KWebAttachFile;
	}
	return 0;
}

int blog::SohuBlog::Process(const char *packet,int action)
{
	//������
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//���� �ظ�
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//�ϴ��ļ� ��־
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	return 0;
}

//������־
int blog::SohuBlog::WebNewTopic(const char *packet)
{	
	//��cookie �еõ� ͨ��֤�� 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	string pp_login_time = ms_->GetValueFromMapByKey("pp_login_time");     
	int pos = pp_login_time.find("|",0);
	if (pos != string::npos)
	{
		int tos = pp_login_time.find("|",pos + 1);
		user_ = pp_login_time.substr(pos + strlen("|"),tos - pos - strlen("|"));
	}
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	title_ = ms_->GetValueFromMapByKey("entrytitle",true);
	//����
	content_ = ms_->GetValueFromMapByKey("entrycontent",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" + //ͨ��֤��
		"netmark=" + title_ + "\r\n" + //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//���� / �ظ�
int blog::SohuBlog::WebPostComment(const char *packet)
{
	//��cookie �еõ� ͨ��֤�� 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	string pp_login_time = ms_->GetValueFromMapByKey("pp_login_time");     
	int pos = pp_login_time.find("|",0);
	if (pos != string::npos)
	{
		int tos = pp_login_time.find("|",pos + 1);
		user_ = pp_login_time.substr(pos + strlen("|"),tos - pos - strlen("|"));
	}
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	content_ = ms_->GetValueFromMapByKey("content",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +	//ͨ��֤��
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//�ϴ��ļ�
int blog::SohuBlog::WebAttachFile(const char *packet)
{
	//��cookie �еõ� ͨ��֤�� 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	string pp_login_time = ms_->GetValueFromMapByKey("pp_login_time");     
	int pos = pp_login_time.find("|",0);
	if (pos != string::npos)
	{
		int tos = pp_login_time.find("|",pos + 1);
		user_ = pp_login_time.substr(pos + strlen("|"),tos - pos - strlen("|"));
	}
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"file\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	content_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,content_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

 