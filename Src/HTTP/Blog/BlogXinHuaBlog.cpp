#include "StdAfx.h"
#include "BlogXinHuaBlog.h"

blog::XinHuaBlog::XinHuaBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::XinHuaBlog::~XinHuaBlog()
{
	
}

int blog::XinHuaBlog::IsXinHuaBlog(HttpSession *http_session)
{
	//��¼ 1 ��׼��¼
	if( (http_session->m_Requestline.m_Host.Find("login.home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/ilogin.do?") != -1))	
	{
		return kWebLogin;
	}
	//��¼ 2 ҳ���¼
	if( (http_session->m_Requestline.m_Host.Find("my.home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blog/ajaxLogin.do") != -1))	
	{
		return kWebLoginSecond;
	}
	//����
	if( (http_session->m_Requestline.m_Host.Find("my.xuan.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blogNew/articleAddSave.do") != -1))	
	{
		return kWebNewTopic;
	}
    //���� 2	
	if( (http_session->m_Requestline.m_Host.Find("my.home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blog/control/articlePost.do") != -1))	
	{
		return kWebNewTopic;
	}
	//���� 1
	if( (http_session->m_Requestline.m_Host.Find(".home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blog/commentAdd.do") != -1))	
	{
		return kWebPostComment;
	}
	//���� 2
	if( (http_session->m_Requestline.m_Host.Find(".home.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/portal/blog/addDiscuss/") != -1))	
	{
		return kWebPostComment;
	}
	//�ϴ��ļ� ����
	if( (http_session->m_Requestline.m_Host.Find("my.xuan.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/blogNew/articleImgSave.do") != -1))	
	{
		return KWebAttachFile;
	}
	//�ϴ��ļ� ���
	if( (http_session->m_Requestline.m_Host.Find("my.xuan.news.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/image/upload.do?") != -1))	
	{
		return KWebAttachFileTuyang;
	}
	return 0;
}

int blog::XinHuaBlog::Process(const char *packet,int action)
{
	//��¼ 1
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//��¼ 2
	if (action == kWebLoginSecond)
	{
		return WebLoginSecond(packet);
	}
	//���� ����
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//����
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//�ϴ��ļ� ����
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//�ϴ��ļ� ���
	if (action == KWebAttachFileTuyang)
	{
		return WebAttachFileTuyang(packet);
	}
	return 0;
}

//��¼ 1
int blog::XinHuaBlog::WebLogin(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string src_dataEnd = src_data.substr(src_data.find("loginID="));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//ͨ��֤��
	string username = ms_->GetValueFromMapByKey("loginID");
	//����
	string password = ms_->GetValueFromMapByKey("password");
	//������Ҫ�ı���
	int n = 0;
	//�û��� base64����
	username = ms_->Base64Decode(username,n);
	password = ms_->Base64Decode(password,n);

	if (username.empty() || password.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(username,"����");
	output_text_ =
		"mid=" + 	username + "\r\n" +		//ͨ��֤��
		"pass=" + 	password + "\r\n" +		//����
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}
 

//��¼ 2
int blog::XinHuaBlog::WebLoginSecond(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string src_dataEnd = src_data.substr(src_data.find("senderName="));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//ͨ��֤��
	string username = ms_->GetValueFromMapByKey("senderName");
	//����
	string password = ms_->GetValueFromMapByKey("password");
	
	if (username.empty() || password.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(username,"����");
	output_text_ =
		"mid=" + 	username + "\r\n" +		//ͨ��֤��
		"pass=" + 	password + "\r\n" +		//����
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//���� ����
int blog::XinHuaBlog::WebNewTopic(const char *packet)
{	
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("userName",true);
	username = ms_->Base64Decode(username,n);
	//���ݲ���
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	nick_ = ms_->GetValueFromMapByKey("title",true);
	//����
	content_ = ms_->GetValueFromMapByKey("contentText",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //ͨ��֤��
		"netmark=" + nick_ + "\r\n" +  //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}


//����
int blog::XinHuaBlog::WebPostComment(const char *packet)
{
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("userName",true);
	username = ms_->Base64Decode(username,n);
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//��Ϣ����
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //ͨ��֤��
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//�ϴ��ļ� ����
int blog::XinHuaBlog::WebAttachFile(const char *packet)
{
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("userName",true);
	user_ = ms_->Base64Decode(username,n);
	//uc����� û��
	if (user_.empty())
	{
		user_ = "null";
	}
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upfile\"");          
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

//�ϴ��ļ� ���
int blog::XinHuaBlog::WebAttachFileTuyang(const char *packet)
{
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("userName",true);
	user_ = ms_->Base64Decode(username,n);
	//uc����� û��
	if (user_.empty())
	{
		user_ = "null";
	}	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"imageFile\"");          
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