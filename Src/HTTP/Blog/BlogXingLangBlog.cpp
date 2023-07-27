#include "StdAfx.h"
#include "BlogXinLangBlog.h"

blog::SinaBlog::SinaBlog(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::SinaBlog::~SinaBlog()
{
	
}

int blog::SinaBlog::IsSinaBlog(HttpSession *http_session)
{
//pc
	//������
	if( (http_session->m_Requestline.m_Host.Find("control.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/admin/article/article_post.php") != -1))	
	{
		return kWebNewTopic;
	}
	//����
	if( (http_session->m_Requestline.m_Host.Find("control.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/admin/comment_new/cms_post.php?domain=1") != -1))	
	{
		return kWebPostComment;
	}
	//�ظ�
	if( (http_session->m_Requestline.m_Host.Find("control.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/admin/comment_new/cms_usereply_post.php?domain=2") != -1))	
	{
		return kWebReply;
	}
	//�ϴ��ļ� ����
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/interface/pic_upload.php?app=blog") != -1))	
	{
		return KWebAttachFile;
	}
	//�ϴ��ļ� ���
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/interface/pic_upload.php?") != -1) && 
		(http_session->m_Requestline.m_URI.Find("app=photo") != -1))	
	{
		return KWebAttachFile;
	}
	//android ������
	if( (http_session->m_Requestline.m_Host.Find("app.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/article/article_add.php") != -1))	
	{
		return kAndroidNewTopic;
	}
	//android ���� 
	if( (http_session->m_Requestline.m_Host.Find("app.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/comment/comment_add.php") != -1))	
	{
		return kAndroidPostComment;
	}
	//android �ظ�
	if( (http_session->m_Requestline.m_Host.Find("app.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/comment/comment_reply.php") != -1))	
	{
		return kAndroidPostComment;
	}
	//android �ϴ�ͼƬ 
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/interface/pic_upload.php") != -1))	
	{
		return KWebAttachFile;
	}
	return 0;
}

int blog::SinaBlog::Process(const char *packet,int action)
{
//pc
	//������
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//����
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//�ظ�
	if (action == kWebReply)
	{
		return WebReply(packet);
	}
	//�ϴ��ļ� ��־
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//android ������
	if (action == kAndroidNewTopic)
	{
		return AndroidNewTopic(packet);
	}
	//android ���� �ظ�
	if (action == kAndroidPostComment)
	{
		return AndroidPostComment(packet);
	}
	return 0;
}


//������־ 
int blog::SinaBlog::WebNewTopic(const char *packet)
{	
	//��cookie �еõ� ͨ��֤�� 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//����ID
	string blogId = ms_->GetValueFromMapByKey("_s_loginuid");     

	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

//1 �ж��Ƿ����μ�
	string is_travel_editor = ms_->GetValueFromMapByKey("is_travel_editor");
	if (is_travel_editor == "1")
	{
		//����ʱ��
		string date_time = ms_->GetValueFromMapByKey("date_pub") + " " + ms_->GetValueFromMapByKey("time");
		string travel_json = ms_->GetValueFromMapByKey("travel_json");
		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(travel_json,value))
		{	
			for (int i = 0;i <value.size();i++)
			{
				//����ص�
				title_ = value[i]["mdd_name"].asString();
				//����
				content_ = value[i]["mdd_desc"].asString();	
				output_text_ =
					"webname=" + 	blogId + "\r\n" + //����ID
					"netmark=" + 	title_ + "\r\n" + //����
					"msg=" + 		content_ + "\r\n" + //��Ϣ����
					"platform=���������\r\n"
					"msgtype=����\r\n";
				WriteIndexFile();
			}
		}
	}
	else
	{
		//2 ���Ĳ���
		//����ʱ��
		string date_time = ms_->GetValueFromMapByKey("date_pub") + " " + ms_->GetValueFromMapByKey("time");
		//����
		title_ = ms_->GetValueFromMapByKey("blog_title",true);
		//����
		content_ = ms_->GetValueFromMapByKey("blog_body",true);
				output_text_ =
					"webname=" + 	blogId + "\r\n" + //����ID
					"netmark=" + 	title_ + "\r\n" + //����
					"msg=" + 		content_ + "\r\n" + //��Ϣ����
					"platform=���������\r\n"
					"msgtype=����\r\n";
				WriteIndexFile();	
	}
	return 1;
}

//����
int blog::SinaBlog::WebPostComment(const char *packet)
{
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	nick_ = ms_->GetValueFromMapByKey("login_name",true);
	//����ID
	string blogId = ms_->GetValueFromMapByKey("uid",true);
	//����
	content_ = ms_->GetValueFromMapByKey("comment",true);

	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//�ǳ�
		"webname=" + blogId + "\r\n" + //����ID
		"msg=" + 	content_ + "\r\n" +	//��Ϣ����
		"platform=���������\r\n"
		"msgtype=����\r\n";
	WriteIndexFile();
	return 1;
}


//�ظ�
int blog::SinaBlog::WebReply(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	nick_ = ms_->GetValueFromMapByKey("login_name",true);
	//����ID
	string blogId = ms_->GetValueFromMapByKey("uid",true);
	//����
	content_ = ms_->GetValueFromMapByKey("reply_content",true);
	//����(���±���)
	string blog_title = ms_->GetValueFromMapByKey("blog_title",true);
	//Ŀ�격��ID
	string source_uid = ms_->GetValueFromMapByKey("source_uid",true);
	//Ŀ�격���ǳ�
	string source_uname = ms_->GetValueFromMapByKey("source_uname",true);
	//��ַ(Ŀ�격�͸�����ҳ)
	string source_ulink= ms_->GetValueFromMapByKey("source_ulink",true);

	output_text_ =
		"nick=" + 		nick_ + "\r\n" +		//�ǳ�
		"webname=" +	blogId + "\r\n" +		//����ID
		"msg=" + 		content_ + "\r\n" +		//��Ϣ����
		"netmark=" +	blog_title + "\r\n" +	//����
		"oid=" + 		source_uid + "\r\n" +	//Ŀ�격��ID
		"clientmsg=" +	source_uname + "\r\n" + //Ŀ�격���ǳ�
		"fileurl=" + 	source_ulink + "\r\n" + //��ַ
		"platform=���������\r\n"
		"msgtype=�ظ�\r\n";
	WriteIndexFile();
	return 1;
}


//�ϴ��ļ�
int blog::SinaBlog::WebAttachFile(const char *packet)
{
	//user_ ��ȡ���� ��Ϊ null
	user_ = "Null";
	//���ݲ���
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"pic1\"");          
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


//android ������־ 
int blog::SinaBlog::AndroidNewTopic(const char *packet)
{	
// 	msg=<div abtype="abarticle"><img abtype="abhead" abdata="{&quot;auther_name&quot;:&quot;&quot;,&quot;
// 		poster_title&quot;:&quot;shark 2016-11-18&quot;,&quot;
// 		postersrc&quot;:&quot;http:\/\/s1.sinaimg.cn\/large\/006zaflTzy76vzvnjnb81&quot;}" 
// 		src="http://s1.sinaimg.cn/large/006zaflTzy76vzvnjnb81"><img abtype="abimg" abdata="{&quot;descrt&quot;:&quot;
// 		&quot;,&quot;location&quot;:&quot;||&quot;,&quot;src&quot;:&quot;http:\/\/s1.sinaimg.cn\/large\/006zaflTzy76vzvIxpYbe&quot;}" 
// 		src="http://s1.sinaimg.cn/large/006zaflTzy76vzvIxpYbe"><img abtype="abimg" abdata="{&quot;descrt&quot;:&quot;&quot;,
// 		&quot;location&quot;:&quot;||&quot;,&quot;src&quot;:&quot;http:\/\/s1.sinaimg.cn\/large\/006zaflTzy76vzvTeI078&quot;}" 
// 		src="http://s1.sinaimg.cn/large/006zaflTzy76vzvTeI078"><img abtype="abimg" abdata="{&quot;descrt&quot;:&quot;&quot;,&quot;
// 		location&quot;:&quot;||&quot;,&quot;src&quot;:&quot;http:\/\/s1.sinaimg.cn\/large\/tag_task_network_error&quot;}" 
// 		src="http://s1.sinaimg.cn/large/tag_task_network_error"><div abtype="abtext"><p dir="ltr">
// 		&#20170;&#22825;&#22825;&#27668;&#22909;&#26497;&#20102;,have a good time!!<b><i>
// 		<a href="http://www.baidu.com">&#30334;&#24230;&#22320;&#22336;</a></i></b></p>
// 		</div></div>

	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����ID
	string blogId = ms_->GetValueBetweenBeginEnd(src_data,"login_uid=","&",true);
	//����
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"article_title=","&",true); 
	//���� (������)
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"article_body=","&article_title",true);
	ms_->Replace(content_,"&quot;","\"");

	output_text_ =
		"webname=" + 	blogId + "\r\n" + //����ID
		"netmark=" + 	title_ + "\r\n" + //����
		"msg=" + 		content_ + "\r\n" + //��Ϣ����
		"platform=�ֻ�app\r\n"
		"msgtype=����\r\n";
	WriteIndexFile();	
	return 1;
}


//android ���� �ظ�
int blog::SinaBlog::AndroidPostComment(const char *packet)
{
	//���ݲ���
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����ID
	string blogId = ms_->GetValueFromMapByKey("login_uid",true);
	//����
	content_ = ms_->GetValueFromMapByKey("comment_content",true);
	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	if (uri.find("/api/comment/comment_add.php") != -1)
	{
		output_text_ =
			"webname=" + blogId + "\r\n" + //����ID
			"msg=" + 	content_ + "\r\n" +	//��Ϣ����
			"platform=���������\r\n"
			"msgtype=����\r\n";
		WriteIndexFile();
	}
	if (uri.find("/api/comment/comment_reply.php") != -1)
	{
		output_text_ =
			"webname=" + blogId + "\r\n" + //����ID
			"msg=" + 	content_ + "\r\n" +	//��Ϣ����
			"platform=���������\r\n"
			"msgtype=�ظ�\r\n";
		WriteIndexFile();
	}
	return 1;
}