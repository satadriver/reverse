#include "StdAfx.h"

#include "ForumTianYa.h"

forum::TianYa::TianYa(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::TianYa::~TianYa()
{
	
}

int forum::TianYa::IsTianYa(HttpSession *http_session)
{
	//����
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.compose")!=-1))
	{
	return kPCWebNewPost;
	}
	//����
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.compose") == -1) &&
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.reply") == -1))
	{
		return kPCWebNewComment;
	}
	//�ظ�
	if((http_session->m_Requestline.m_Host.Find("bbs.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api?method=bbs.ice.reply")!=-1))
	{
		return kPCWebNewReply;
	}
	//android����
	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/compose")!=-1))
	{
		return kAndroidNewPost;
	}
	//android����
	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/comment")!=-1))
	{
		return kAndroidNewComment;
	}
	//android�ظ�
	if((http_session->m_Requestline.m_Host.Find("wireless.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/forumStand/reply")!=-1))
	{
		return kAndroidNewReply;
	}
	//�ϴ��ļ�	
	if((http_session->m_Requestline.m_Host.Find("photo.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/photo?act=uploadphoto")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/photo?act=uploadphoto&watermark=0&serverapp=mobile") == -1))
	{
		return KPCWebUploadDocument;
	}
	//android�ϴ��ļ� 
	if((http_session->m_Requestline.m_Host.Find("photo.tianya.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/photo?act=uploadphoto&watermark=0&serverapp=mobile")!=-1))
	{
		return KAndroidUploadDocument;
	}
	return 0;
}

int forum::TianYa::Process(const char *packet,int action)
{
	//����
	if (action == kPCWebNewPost)
	{
		return PCWebNewPost(packet);
	}
	//����
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	//�ظ�
	if (action == kPCWebNewReply)
	{
		return PCWebNewReply(packet);
	}
	//�ϴ��ļ�
	if (action == KPCWebUploadDocument)
	{
		return PCWebUploadDocument(packet);	
	}
	//android����
	if (action == kAndroidNewPost)
	{
		return AndroidNewPost(packet);
	}
	//android����
	if (action == kAndroidNewComment)
	{
		return AndroidNewComment(packet);
	}
	//android�ظ�
	if (action == kAndroidNewReply)
	{
		return AndroidNewReply(packet);	
	}
	//android�ϴ��ļ�
	if (action == KAndroidUploadDocument)
	{
		return AndroidUploadDocument(packet);	
	}
	return 0;
}

//����
int forum::TianYa::PCWebNewPost(const char *packet)
{
	//��cookie����ȡ����  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//�û���
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//�û�ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//ģ������
	string item = ms_->GetValueFromMapByKey("params.item",true);
	//����
	string title = ms_->GetValueFromMapByKey("params.title",true);
	//��Ϣ����
	string content = ms_->GetValueFromMapByKey("params.content",true);
	if ( title.empty() || content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//�û���	
		"userid=" + userid + "\r\n" + 	//�û�ID	
		"oid=" + item + "\r\n" +		//ģ������	
		"msgtype=" + title + "\r\n" + 	//����		
		"msg=" + content + "\r\n" + 	//��Ϣ����
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//����
int forum::TianYa::PCWebNewComment(const char *packet)
{
	//��cookie����ȡ����  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//�û���
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//�û�ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//ģ������
	string item = ms_->GetValueFromMapByKey("params.item",true);
	//��Ϣ����
	string content = ms_->GetValueFromMapByKey("params.content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//�û���	
		"userid=" + userid + "\r\n" + 	//�û�ID	
		"oid=" + item + "\r\n" +		//ģ������	
		"msg=" + content + "\r\n" + 	//��Ϣ����
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//�ظ�
int forum::TianYa::PCWebNewReply(const char *packet)
{
	//��cookie����ȡ����  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//�û���
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//�û�ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//ģ������
	string item = ms_->GetValueFromMapByKey("params.item",true);
	//���µ�ַ
	string preUrl = ms_->GetValueFromMapByKey("params.preUrl",true);
	//������
	string preUserName = ms_->GetValueFromMapByKey("params.preUserName",true);
	//����
	string title = ms_->GetValueFromMapByKey("params.title",true);
	//��Ϣ����
	string content = ms_->GetValueFromMapByKey("params.content",true);
	if ( title.empty() || content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +		//�û���	
		"userid=" + userid + "\r\n" + 		//�û�ID	
		"libname=" + preUserName + "\r\n" + //������
		"oid=" + item + "\r\n" +			//ģ������	
		"msgtype=" + title + "\r\n" + 		//����		
		"msg=" + content + "\r\n" + 		//��Ϣ����
		"nick=" + preUrl + "\r\n" +			//���µ�ַ
		"state=�ظ�\r\n";
	WriteIndexFile();
	return 1;
}


//�ϴ��ļ�
int forum::TianYa::PCWebUploadDocument(const char *packet)
{	
	//��cookie����ȡ����  user=w=xiaoxiaolong989&id=122840674&f=1
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);

	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"user=",";");
	//�û���
	string username = ms_->GetValueBetweenBeginEnd(userTemp,"w=","&");
	//�û�ID
	string userid = ms_->GetValueBetweenBeginEnd(userTemp,"id=","&");

	from_ = username;
	
	//�ļ�����
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
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	 
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//android����
int forum::TianYa::AndroidNewPost(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("userName",true);
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("userId",true);
	//ģ������
	string item = ms_->GetValueFromMapByKey("categoryId",true);
	//����
	string title = ms_->GetValueFromMapByKey("title",true);
	//��Ϣ����
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//�û���	
		"userid=" + userid + "\r\n" + 	//�û�ID	
		"oid=" + item + "\r\n" +		//ģ������	
		"msgtype=" + 	title + "\r\n" + //����	
		"msg=" + content + "\r\n" + 	//��Ϣ����
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//android����
int forum::TianYa::AndroidNewComment(const char *packet)
{	
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("userName",true);
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("userId",true);
	//ģ������
	string item = ms_->GetValueFromMapByKey("categoryId",true);
	//��Ϣ����
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//�û���	
		"userid=" + userid + "\r\n" + 	//�û�ID	
		"oid=" + item + "\r\n" +		//ģ������	
		"msg=" + content + "\r\n" + 	//��Ϣ����
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//android�ظ�
int forum::TianYa::AndroidNewReply(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//�û���
	string username = ms_->GetValueFromMapByKey("userName",true);
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("userId",true);
	//ģ������
	string item = ms_->GetValueFromMapByKey("categoryId",true);
	//��Ϣ����
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ = 
		"user=" + username + "\r\n" +	//�û���	
		"userid=" + userid + "\r\n" + 	//�û�ID	
		"oid=" + item + "\r\n" +		//ģ������	
		"msg=" + content + "\r\n" + 	//��Ϣ����
		"state=�ظ�\r\n";
	WriteIndexFile();
	return 1;
}

//android�ϴ��ļ�
int forum::TianYa::AndroidUploadDocument(const char *packet)
{	
	//��uri����ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	from_ = ms_->GetValueFromMapByKey("idWriter", true);
	
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"picture\"");          
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
