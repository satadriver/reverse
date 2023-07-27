#include "StdAfx.h"
#include "BlogFengHuangBlog.h"

blog::FengHuangBlog::FengHuangBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::FengHuangBlog::~FengHuangBlog()
{
	
}

int blog::FengHuangBlog::IsFengHuangBlog(HttpSession *http_session)
{
	//������
	if( (http_session->m_Requestline.m_Host.Find("blog.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/usercp/index.php") != -1) &&
		(http_session->m_Requestline.m_URI.Find("op=editorUploadImg") == -1))	
	{
		return kWebNewTopic;
	}
	//���ۡ��ظ�
	if( (http_session->m_Requestline.m_Host.Find("comment.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/post/?format=js&sid=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("callback=sendCommentCallBack") != -1))	
	{
		return kWebPostComment;
	}
	//�ϴ��ļ� ����
	if( (http_session->m_Requestline.m_Host.Find("blog.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/usercp/index.php?op=editorUploadImg") != -1))	
	{
		return KWebAttachFile;
	}
	//�ϴ��ļ� ���
	if( (http_session->m_Requestline.m_Host.Find("blog.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/public/index.php?param=albumUpload") != -1))	
	{
		return KWebAttachFileAlubm;
	}
	return 0;
}

int blog::FengHuangBlog::Process(const char *packet,int action)
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
	//�ϴ��ļ� ���
	if (action == KWebAttachFileAlubm)
	{
		return WebAttachFileAlubm(packet);
	}
	return 0;
}

//������־
int blog::FengHuangBlog::WebNewTopic(const char *packet)
{
	//ͨ��֤�ţ����⣬���ݣ���ַ(ʡ������)��IP��ַ
	string address,region_ip;
	//��cookie �еõ� ͨ��֤�� ��ַ IP��ַ
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	user_ = ms_->GetValueFromMapByKey("IF_USER");      
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("prov") + "-" + ms_->GetValueFromMapByKey("city");
    //IP��ַ
	region_ip = ms_->GetValueFromMapByKey("region_ip");
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	title_ = ms_->GetValueFromMapByKey("subject",true);
	//����
	content_ = ms_->GetValueFromMapByKey("message",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +		//ͨ��֤��
		"netmark=" + title_ + "\r\n" +		//����
		"msg=" + 	content_ + "\r\n" +		//��Ϣ����
		"fileurl=" + address + "\r\n" +		//��ַ
		"imsi=" + 	region_ip + "\r\n" +	//IP��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//���� �ظ�
int blog::FengHuangBlog::WebPostComment(const char *packet)
{
	//ͨ��֤�ţ����±��⣬���ݣ����µ�ַ������ID����ַ(ʡ������)��IP��ַ
	string address,region_ip;
	//��cookie �еõ� ͨ��֤�� ��ַ IP��ַ
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	user_ = ms_->GetValueFromMapByKey("IF_USER");      
	//��ַ(ʡ�� ����0531-0530)
	address = ms_->GetValueFromMapByKey("prov") + "-" + ms_->GetValueFromMapByKey("city");
    //IP��ַ
	region_ip = ms_->GetValueFromMapByKey("region_ip");

	//���ݲ���
	string url(http_session_->m_Requestline.m_URI.GetBuffer(0));
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');
	//���±���
	title_ = ms_->GetValueFromMapByKey("docName"); 
	//����
	content_ = ms_->GetValueFromMapByKey("content"); 
	//���µ�ַ
    string docUrl = ms_->GetValueFromMapByKey("docUrl"); 
	//����ID
	string blog_uid;
	string ext2 =  ms_->GetValueFromMapByKey("ext2"); 
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(ext2,value))
	{
		blog_uid = value["blog_uid"].asString();
	}
   	//Ŀ�격��ID �ж����� �ظ�
	string quoteId = ms_->GetValueFromMapByKey("quoteId"); 
	if (quoteId.empty())
	{
		output_text_ =		
			"mid=" + 		user_ + "\r\n" + //ͨ��֤��
			"netmark=" +	title_ + "\r\n" + //����
			"msg=" + 		content_ + "\r\n" + //��Ϣ����
			"clientmac=" + 	docUrl + "\r\n" +	//���µ�ַ 
			"webname=" + 	blog_uid + "\r\n" + //����ID
			"fileurl=" + 	address + "\r\n" +	//��ַ
			"imsi=" + 		region_ip + "\r\n" + //IP��ַ
			"platform=���������\r\n"
			"state=����\r\n";
		WriteIndexFile();
	}
	if (!quoteId.empty())
	{
		output_text_ =		
			"mid=" + 		user_ + "\r\n" +    //ͨ��֤��
			"netmark=" +	title_ + "\r\n" +   //����
			"msg=" + 		content_ + "\r\n" + //��Ϣ����
			"clientmac=" + 	docUrl + "\r\n" +	//���µ�ַ 
			"webname=" + 	blog_uid + "\r\n" + //����ID
			"oid=" + 		quoteId + "\r\n" + //Ŀ�격��ID
			"fileurl=" + 	address + "\r\n" +	//��ַ
			"imsi=" + 		region_ip + "\r\n" + //IP��ַ
			"platform=���������\r\n"
			"state=�ظ�\r\n";
		WriteIndexFile();
	}
	return 1;
}


//�ϴ��ļ�
int blog::FengHuangBlog::WebAttachFile(const char *packet)
{
	//��cookie �л��user_  
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	user_ = ms_->GetValueFromMapByKey("IF_USER");   

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
int blog::FengHuangBlog::WebAttachFileAlubm(const char *packet)
{
	//��cookie �л��user_  
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//ͨ��֤��
	user_ = ms_->GetValueFromMapByKey("IF_USER");   
	
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
	content_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,content_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}
