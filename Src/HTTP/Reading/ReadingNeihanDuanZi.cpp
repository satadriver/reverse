#include "StdAfx.h"
#include "ReadingNeihanDuanZi.h"

reading::NeiHanDuanZi::NeiHanDuanZi(HttpSession *http_session, const string &pro_name) : Reading(http_session, pro_name)
{

}

reading::NeiHanDuanZi::~NeiHanDuanZi()
{
	
}

int reading::NeiHanDuanZi::IsNeiHanDuanZi(HttpSession *http_session)
{
	//pc ��Ͷ��(���� ��ͼ ���� -- ��ʾ������)	
	if((http_session->m_Requestline.m_URI.Find("/api/publish_post/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return kPCWebNewPost;
	}
	//pc ��Ͷ�� (��ͼ��Դ)
	if((http_session->m_Requestline.m_URI.Find("/file/upload/v1/image/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return KPCWebUploadPicture;
	}
	//pc ��Ͷ�� ��Ƶ
	if((http_session->m_Requestline.m_URI.Find("/video/upload/1/duanzi/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 )) 
	{
		return KPCWebUploadVideo;
	}
	//pc ��Ͷ�� (��Ƶ·��)
	if((http_session->m_Requestline.m_URI.Find("/video/report_error/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return KPCWebUploadVideoPath;
	}
	//pc ������ �ظ�
	if((http_session->m_Requestline.m_URI.Find("/post_comment/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return kPCWebNewComment;
	}
	//android ��Ͷ��(���� ��ͼ ���� -- ��ʾ������) (�Ѳ��ɽ�2017-07-10)
	if((http_session->m_Requestline.m_URI.Find("/2/essay/zone/ugc/post/v2/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find(".snssdk.com") != -1 ))
	{
		return kAndroidNewPost;
	}
	//android ��Ͷ�� (��ͼ��Դ) 
	if((http_session->m_Requestline.m_URI.Find("/neihan/file/upload/v1/image/")!=-1) &&
		(http_session->m_Requestline.m_Host.Find("lf.snssdk.com") != -1))
	{
		return KAndroidUploadPicture;
	}
	//android ��Ͷ�� ��Ƶ (�Ѳ��ɽ�2017-07-10)
	if((http_session->m_Requestline.m_URI.Find("video/upload/1/duanzi/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find(".snssdk.com") != -1 )) 
	{
		return KAndroidUploadVideo;
	}
	//android ������	(�Ѳ��ɽ�2017-07-10)
	if((http_session->m_Requestline.m_URI.Find("/2/data/v2/post_message/?")!=-1) &&
		( http_session->m_Requestline.m_Host.Find(".snssdk.com") != -1 ))
	{
		return KAndroidNewComment;
	}
	return 0;
}

int reading::NeiHanDuanZi::Process(const char *packet,int action)
{
	//pc ��Ͷ��
	if (action == kPCWebNewPost)
	{
		return PCWebNewPost(packet);
	}
	//pc ��Ͷ�� ��ͼ
	if (action == KPCWebUploadPicture)
	{
		return PCWebUploadPicture(packet);
	}
	//pc ��Ͷ�� ��Ƶ
	if (action == KPCWebUploadVideo)
	{
		return PCWebUploadVideo(packet);
	}
	//pc ��Ͷ�� ��Ƶ ·��
	if (action == KPCWebUploadVideoPath)
	{
		return PCWebUploadVideoPath(packet);
	}
	//pc ������ �ظ�
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	//android ��Ͷ��
	if (action == kAndroidNewPost)
	{
		return AndroidNewPost(packet);
	}
	//android ��Ͷ�� ��ͼ
	if (action == KAndroidUploadPicture)
	{
		return AndroidUploadPicture(packet);
	}
	//android ��Ͷ�� ��Ƶ
	if (action == KAndroidUploadVideo)
	{
		return AndroidUploadVideo(packet);
	}
	//android ������
	if (action == KAndroidNewComment)
	{
		return AndroidNewComment(packet);
	}
	return 0;
}

//pc ��Ͷ��
int reading::NeiHanDuanZi::PCWebNewPost(const char *packet)
{
	//���ݲ��� 
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("tag") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("tag"));
	}
	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');

	//material_id   (���� ��    ��ͼ ��          ��Ƶ ��)
	//content_type  (���� 3		��ͼ 1			 ��Ƶ 2)
	//tag			(���� joke  ��ͼ funny_heavy ��Ƶ funnyvideo)
	string material_id,tag,content_type;
	string post_type;
    //��Դid
	material_id = ms_->GetValueFromMapByKey("material_id",true);
	//��ǩ 
	tag = ms_->GetValueFromMapByKey("tag",true);
	content_type = ms_->GetValueFromMapByKey("content_type",true);

	if (strcmp(content_type.c_str(),"1") == 0 && strcmp(tag.c_str(),"funny_heavy") == 0)
	{
		post_type = "��ͼ";
	}
	else if(strcmp(content_type.c_str(),"2") == 0 && strcmp(tag.c_str(),"funnyvideo") == 0)
	{
		post_type = "��Ƶ";
	}
	else if(strcmp(content_type.c_str(),"3") == 0 && strcmp(tag.c_str(),"joke") == 0)
	{
		post_type = "����";
	} 
	//Ͷ������
	content_ = ms_->GetValueFromMapByKey("text",true);

	if (post_type.empty() || content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);
	output_text_ =	
		"sex=" + post_type + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=����\r\n" +
		"state=Ͷ��\r\n";
	WriteIndexFile();
	return 1;
}

//pc ��Ͷ�� ��ͼ
int reading::NeiHanDuanZi::PCWebUploadPicture(const char *packet)
{
	//Ŀǰ��ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	from_ = ms_->GetValueFromMapByKey("email", true);
	if (from_.length() == 0)
	{
		from_ = "null";
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
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}
//pc ��Ͷ�� ��Ƶ
int reading::NeiHanDuanZi::PCWebUploadVideo(const char *packet)
{
	//Ŀǰ��ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	from_ = ms_->GetValueFromMapByKey("email", true);
	if (from_.length() == 0)
	{
		from_ = "null";
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
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}


//pc ��Ͷ�� ��Ƶ ·��
int reading::NeiHanDuanZi::PCWebUploadVideoPath(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//��Դid
    string id,data,f,ua;

	id = ms_->GetValueFromMapByKey("id",true);
	data = ms_->GetValueFromMapByKey("data",true);
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	//�ж�����ӻ��Ǹ���
	if (reader.parse(data,value))
	{	
		//�ļ�·��
		f = value["f"].asString();
		//ʹ�������
		ua = value["ua"].asString();	
	}
	if (f.empty() || ua.empty())
	{
		return 0;
	}
	output_text_ = 
		"lang=" + f + "\r\n" + 
		"state=�ϴ��ļ�\r\n";	
	WriteIndexFile();
	return 1;
}

//pc ������ �ظ�
int reading::NeiHanDuanZi::PCWebNewComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ж����� �ظ�
	string reply_to_comment_id = ms_->GetValueFromMapByKey("reply_to_comment_id");
	//��������
	string content_ = ms_->GetValueFromMapByKey("status",true);
	if (content_.empty())
	{
		return 0;
	}
	if (reply_to_comment_id.length() <= 0)
	{
		WriteCaseInfo(content_);
		output_text_ =
			"msg=" + content_ + "\r\n" +
			"platform=����\r\n" +
			"state=����\r\n";
		WriteIndexFile();
	}
	else
	{
		WriteCaseInfo(content_);
		output_text_ =
			"msg=" + content_ + "\r\n" +
			"platform=����\r\n" +
			"state=�ظ�\r\n";
		WriteIndexFile();
	}
	return 1;
}

//android ��Ͷ��
int reading::NeiHanDuanZi::AndroidNewPost(const char *packet)
{
	//���ݲ��� 
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("tag") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("tag"));
	}	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');  
	//Ͷ������
	content_ = ms_->GetValueFromMapByKey("text",true);
	if (content_.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ =	
		"msg=" + content_ + "\r\n" +
		"platform=�ֻ�\r\n" +
		"state=Ͷ��\r\n";
	WriteIndexFile();
	return 1;
}

//android ��Ͷ�� ��ͼ
int reading::NeiHanDuanZi::AndroidUploadPicture(const char *packet)
{
	//Ŀǰ��ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	from_ = ms_->GetValueFromMapByKey("email", true);
	if (from_.length() == 0)
	{
		from_ = "null";
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
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}


//android ��Ͷ�� ��Ƶ
int reading::NeiHanDuanZi::AndroidUploadVideo(const char *packet)
{
	//Ŀǰ��ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	from_ = ms_->GetValueFromMapByKey("email", true);
	if (from_.length() == 0)
	{
		from_ = "null";
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
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//android ������
int reading::NeiHanDuanZi::AndroidNewComment(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','='); 
	//��������
	content_ = ms_->GetValueFromMapByKey("text",true);
	if (content_.empty())
	{
		return 0;
	}
	WriteCaseInfo(content_);
	output_text_ =
		"msg=" + content_ + "\r\n" +
		"platform=�ֻ�\r\n" +
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}