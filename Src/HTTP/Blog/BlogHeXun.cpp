#include "StdAfx.h"
#include "BlogHeXun.h"

blog::HeXun::HeXun(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::HeXun::~HeXun()
{
	
}

int blog::HeXun::IsHeXun(HttpSession *http_session)
{
	//����
	if( (http_session->m_Requestline.m_Host.Find("post.blog.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/PostArticleHandler.ashx?blogname") != -1))	
	{
		return kWebNewTopic;
	}
	//���� 1
	if( (http_session->m_Requestline.m_Host.Find("blog.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/CommentCode_type0.html") != -1))	
	{
		return kWebPostComment;
	}
	//�ϴ��ļ� 1 ������ʱ�ϴ���ͼƬ--Ŀǰȡ���� 2017-07-05
	if( (http_session->m_Requestline.m_Host.Find("postnew.photo.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/upload/FlashUpload") != -1))	
	{
		return KWebAttachFile;
	}
	//�ϴ��ļ� 2 ������ϴ���ͼƬ    --Ŀǰȡ���� 2017-07-05
	if( (http_session->m_Requestline.m_Host.Find(".photo.hexun.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/PhotoAPI/Upload?userToken=") != -1))	
	{
		return KWebAttachFile;
	}
	return 0;
}

int blog::HeXun::Process(const char *packet,int action)
{
	//����
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//����
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//�ϴ��ļ�
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	return 0;
}


//����
int blog::HeXun::WebNewTopic(const char *packet)
{	
	//��cookie �л�����ڳ���
	string TOWN;
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
    if (cookie.find("TOWN=") != string::npos)
    {
		int pos = cookie.find("TOWN=");
		int tos = cookie.find(";",pos);
		//���ڳ��� (��׼��ַ��)
		TOWN = cookie.substr(pos + strlen("TOWN="),tos - pos - strlen("TOWN="));
    }
	//���ݲ���
	string src_data(packet);
	//�滻ת���ַ� < => &lt,> =>&gt,
	ms_->Replace(src_data,"&lt;","<");
	ms_->Replace(src_data,"&gt;",">");
	ms_->Replace(src_data,"&quot;","\"");
	ms_->Replace(src_data,"&amp;","&");
	ms_->Replace(src_data,"&nbsp;"," ");

	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string blogname,TitleTextbox,ContentSpaw,SourceUrlTextbox,weibo,action;
	//����ID
	blogname = ms_->GetValueFromMapByKey("blogname",true);
	//����
	TitleTextbox = ms_->GetValueFromMapByKey("TitleTextbox",true);
	//����
	ContentSpaw = ms_->GetValueFromMapByKey("ContentSpaw",true);
	//ת�ص�ַ (������Ϣ)
	SourceUrlTextbox = ms_->GetValueFromMapByKey("SourceUrlTextbox",true);

	//΢�� ͬ�� 1 / ��ͬ�� 0 (������Ϣ)
	weibo = ms_->GetValueFromMapByKey("weibo",true);
	//�ݸ� 1 / ��ʽ 0        (������Ϣ)
	action = ms_->GetValueFromMapByKey("action",true);

	if (blogname.empty() || TitleTextbox.empty() || ContentSpaw.empty())
	{
		return 0;
	}
	
	output_text_ =
		"webname==" + 	blogname + "\r\n" +	  //����ID
		"netmark=" + TitleTextbox + "\r\n" +  //����
		"msg=" + 	ContentSpaw + "\r\n" +	  //��Ϣ����
		"fileurl=" + TOWN + "\r\n" +		  //��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//����
int blog::HeXun::WebPostComment(const char *packet)
{
	//��cookie �л�����ڳ���
	string TOWN;
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
    if (cookie.find("TOWN=") != string::npos)
    {
		int pos = cookie.find("TOWN=");
		int tos = cookie.find(";",pos);
		//���ڳ��� (��׼��ַ��)
		TOWN = cookie.substr(pos + strlen("TOWN"),tos - pos - strlen("TOWN"));
    }
	//���ݲ���
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	
	string UserNameTextbox,UserUrlTextbox,CommentTitle,NoHtmlCommentContent;
	//����ID
	UserNameTextbox = ms_->GetValueFromMapByKey("UserID",true);
	//������ҳ(����)
	UserUrlTextbox = ms_->GetValueFromMapByKey("UserUrl",true);
	//����
	CommentTitle = ms_->GetValueFromMapByKey("Title",true);
	//����
	NoHtmlCommentContent = ms_->GetValueFromMapByKey("Content",true);
	
	if (CommentTitle.empty() || NoHtmlCommentContent.empty())
	{
		return 0;
	}
	output_text_ =
		"webname=" + 	UserNameTextbox + "\r\n" +	//����ID
		"netmark=" + CommentTitle + "\r\n" +		//����
		"msg=" + 	NoHtmlCommentContent + "\r\n" +	//��Ϣ����	
		"fileurl=" + TOWN + "\r\n" +				//��ַ
		"platform=���������\r\n"
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//�ϴ��ļ�
int blog::HeXun::WebAttachFile(const char *packet)
{
	//Ŀǰ��ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	user_ = ms_->GetValueFromMapByKey("email", true);
	if (user_.empty())
	{
		user_ = "null";
	}
	
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

