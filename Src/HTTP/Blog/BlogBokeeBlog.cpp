#include "StdAfx.h"
#include "BlogBokeeBlog.h"

blog::BokeeBlog::BokeeBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::BokeeBlog::~BokeeBlog()
{
	
}

int blog::BokeeBlog::IsBokeeBlog(HttpSession *http_session)
{
	//��¼
	if( (http_session->m_Requestline.m_Host.Find("sso.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/loginAjax?") != -1))	
	{
		return kWebLogin;
	}
	//���� ����
	if( (http_session->m_Requestline.m_Host.Find("user.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/user.php/blog/add/") != -1))	
	{
		return kWebNewTopic;
	}
	//������
	if( (http_session->m_Requestline.m_Host.Find("tuyang.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/admin/saveArticle") != -1))	
	{
		return kWebNewPaper;
	}
	//���� 
	if( (http_session->m_Requestline.m_Host.Find("home.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/home.php/comment/doadd") != -1))	
	{
		return kWebPostComment;
	}
	//���� 2
	if( (http_session->m_Requestline.m_Host.Find("tuyang.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/visitor/addArtComment/") != -1))	
	{
		return kWebPostCommentTuyang;
	}
	//�ϴ��ļ� ����
	if( (http_session->m_Requestline.m_Host.Find("upload.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/newbokee_user/imageUp.php") != -1))	
	{
		return KWebAttachFile;
	}
	//�ϴ��ļ� ͼ��
	if( (http_session->m_Requestline.m_Host.Find("tuyang.bokee.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/method/upload") != -1))	
	{
		return KWebAttachFileTuyang;
	}
	return 0;
}

int blog::BokeeBlog::Process(const char *packet,int action)
{
	//��¼
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//���� ����
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//���� ����
	if (action == kWebNewPaper)
	{
		return WebNewPaper(packet);
	}
	//����
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//����
	if (action == kWebPostCommentTuyang)
	{
		return WebPostCommentTuyang(packet);
	}
	//�ϴ��ļ� ����
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//�ϴ��ļ� ͼ��
	if (action == KWebAttachFileTuyang)
	{
		return WebAttachFileTuyang(packet);
	}
	return 0;
}

//��¼
int blog::BokeeBlog::WebLogin(const char *packet)
{
	//��uri �л�ȡ����
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	sm.Replace(uri, "?", "&");
	sm.SplitStringToMap(uri, '&', '=');
	//ͨ��֤��
	string username = sm.GetValueFromMapByKey("username");
	//����
	string password = sm.GetValueFromMapByKey("password");
	//IP��ַ
	string ipAddress = sm.GetValueFromMapByKey("ip");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(username,"����");
	output_text_ =
		"mid=" + 	username + "\r\n" +		//ͨ��֤��
		"pass=" + 	password + "\r\n" +		//����
		"imsi=" + 	ipAddress + "\r\n" +    //IP��ַ
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}
 
//���� ����
int blog::BokeeBlog::WebNewTopic(const char *packet)
{	
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	n = username.length();
	username = ms_->Base64Decode(username,n);

	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("sso_nickname",true);
	nickname = ms_->Base64Decode(nickname,n);
	nickname= ms_->UTF8ToGBK(UrlDecode(nickname));
	//����ID 
	string blogid = ms_->GetValueFromMapByKey("sso_uid",true);
	blogid = ms_->GetValueBetweenBeginEnd(blogid,"\"","\"",true);
	blogid = ms_->Base64Decode(blogid,n);
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string src_dataEnd = src_data.substr(src_data.find("subject="));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//����
	nick_ = ms_->GetValueFromMapByKey("subject",true);
	//����
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //ͨ��֤��
		"nick=" + 	nickname + "\r\n" + //�ǳ�
		"webname=" + blogid + "\r\n" + //����ID
		"netmark=" + nick_ + "\r\n" +  //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=������\r\n";
	WriteIndexFile();
	return 1;
}


//���� ����
int blog::BokeeBlog::WebNewPaper(const char *packet)
{	
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	username = ms_->Base64Decode(username,n);
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("sso_nickname",true);
	nickname = ms_->Base64Decode(nickname,n);
	nickname= ms_->UTF8ToGBK(UrlDecode(nickname));
	//����ID 
	string blogid = ms_->GetValueFromMapByKey("sso_uid",true);
	blogid = ms_->GetValueBetweenBeginEnd(blogid,"\"","\"",true);
	blogid = ms_->Base64Decode(blogid,n);
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	nick_ = ms_->GetValueBetweenBeginEnd(src_data,"title=","&sr",true); 
	//����
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"contentStr=","&tag",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //ͨ��֤��
		"nick=" + 	nickname + "\r\n" + //�ǳ�
		"webname=" + blogid + "\r\n" + //����ID
		"netmark=" + nick_ + "\r\n" +  //����
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=������\r\n";
	WriteIndexFile();
	return 1;
}

//����
int blog::BokeeBlog::WebPostComment(const char *packet)
{
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	username = ms_->Base64Decode(username,n);
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("sso_nickname",true);
	nickname = ms_->Base64Decode(nickname,n);
	nickname= ms_->UTF8ToGBK(UrlDecode(nickname));
	//����ID 
	string blogid = ms_->GetValueFromMapByKey("sso_uid",true);
	blogid = ms_->GetValueBetweenBeginEnd(blogid,"\"","\"",true);
	blogid = ms_->Base64Decode(blogid,n);
	//��uri �л�ȡ����
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);	
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	//Ŀ�격���ǳ�
	nick_ = ms_->GetValueBetweenBeginEnd(uri,"uname=","&",true);
	//��Ϣ����
	content_ = ms_->GetValueBetweenBeginEnd(uri,"content=","&",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //ͨ��֤��
		"nick=" + 	nickname + "\r\n" + //�ǳ�
		"webname=" + blogid + "\r\n" + //����ID
		"clientmsg=" + nick_ + "\r\n" + //Ŀ�격���ǳ�
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}


//����
int blog::BokeeBlog::WebPostCommentTuyang(const char *packet)
{
	//��uri �л�ȡ����
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);	
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');

	//ͨ���˺�
	string username = ms_->GetValueFromMapByKey("userName",true);
	//����id
	string blogid = ms_->GetValueFromMapByKey("userId");
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("commentator",true);
	//��Ϣ����
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	output_text_ =
		"mid=" + 	username + "\r\n" + //ͨ��֤��
		"nick=" + 	nickname + "\r\n" + //�ǳ�
		"webname=" + blogid + "\r\n" + //����ID
		"msg=" + 	content_ + "\r\n" + //��Ϣ����
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//�ϴ��ļ� ����
int blog::BokeeBlog::WebAttachFile(const char *packet)
{
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	user_ = ms_->Base64Decode(username,n);
	
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


//�ϴ��ļ� ͼ��
int blog::BokeeBlog::WebAttachFileTuyang(const char *packet)
{
	//��cookie �л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,';','=');
	//������Ҫ�ı���
	int n = 0;
    //ͨ��֤��
	string username = ms_->GetValueFromMapByKey("sso_username",true);
	user_ = ms_->Base64Decode(username,n);
	
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