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
	//pc 端投稿(段子 囧图 视屏 -- 显示的内容)	
	if((http_session->m_Requestline.m_URI.Find("/api/publish_post/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return kPCWebNewPost;
	}
	//pc 端投稿 (囧图资源)
	if((http_session->m_Requestline.m_URI.Find("/file/upload/v1/image/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return KPCWebUploadPicture;
	}
	//pc 端投稿 视频
	if((http_session->m_Requestline.m_URI.Find("/video/upload/1/duanzi/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 )) 
	{
		return KPCWebUploadVideo;
	}
	//pc 端投稿 (视频路径)
	if((http_session->m_Requestline.m_URI.Find("/video/report_error/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return KPCWebUploadVideoPath;
	}
	//pc 端评论 回复
	if((http_session->m_Requestline.m_URI.Find("/post_comment/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find("neihanshequ.com") != -1 ))
	{
		return kPCWebNewComment;
	}
	//android 端投稿(段子 囧图 视屏 -- 显示的内容) (已不可解2017-07-10)
	if((http_session->m_Requestline.m_URI.Find("/2/essay/zone/ugc/post/v2/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find(".snssdk.com") != -1 ))
	{
		return kAndroidNewPost;
	}
	//android 端投稿 (囧图资源) 
	if((http_session->m_Requestline.m_URI.Find("/neihan/file/upload/v1/image/")!=-1) &&
		(http_session->m_Requestline.m_Host.Find("lf.snssdk.com") != -1))
	{
		return KAndroidUploadPicture;
	}
	//android 端投稿 视频 (已不可解2017-07-10)
	if((http_session->m_Requestline.m_URI.Find("video/upload/1/duanzi/")!=-1) &&
		( http_session->m_Requestline.m_Host.Find(".snssdk.com") != -1 )) 
	{
		return KAndroidUploadVideo;
	}
	//android 端评论	(已不可解2017-07-10)
	if((http_session->m_Requestline.m_URI.Find("/2/data/v2/post_message/?")!=-1) &&
		( http_session->m_Requestline.m_Host.Find(".snssdk.com") != -1 ))
	{
		return KAndroidNewComment;
	}
	return 0;
}

int reading::NeiHanDuanZi::Process(const char *packet,int action)
{
	//pc 端投稿
	if (action == kPCWebNewPost)
	{
		return PCWebNewPost(packet);
	}
	//pc 端投稿 囧图
	if (action == KPCWebUploadPicture)
	{
		return PCWebUploadPicture(packet);
	}
	//pc 端投稿 视频
	if (action == KPCWebUploadVideo)
	{
		return PCWebUploadVideo(packet);
	}
	//pc 端投稿 视频 路径
	if (action == KPCWebUploadVideoPath)
	{
		return PCWebUploadVideoPath(packet);
	}
	//pc 端评论 回复
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	//android 端投稿
	if (action == kAndroidNewPost)
	{
		return AndroidNewPost(packet);
	}
	//android 端投稿 囧图
	if (action == KAndroidUploadPicture)
	{
		return AndroidUploadPicture(packet);
	}
	//android 端投稿 视频
	if (action == KAndroidUploadVideo)
	{
		return AndroidUploadVideo(packet);
	}
	//android 端评论
	if (action == KAndroidNewComment)
	{
		return AndroidNewComment(packet);
	}
	return 0;
}

//pc 端投稿
int reading::NeiHanDuanZi::PCWebNewPost(const char *packet)
{
	//数据部分 
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("tag") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("tag"));
	}
	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');

	//material_id   (段子 无    囧图 有          视频 有)
	//content_type  (段子 3		囧图 1			 视频 2)
	//tag			(段子 joke  囧图 funny_heavy 视频 funnyvideo)
	string material_id,tag,content_type;
	string post_type;
    //资源id
	material_id = ms_->GetValueFromMapByKey("material_id",true);
	//标签 
	tag = ms_->GetValueFromMapByKey("tag",true);
	content_type = ms_->GetValueFromMapByKey("content_type",true);

	if (strcmp(content_type.c_str(),"1") == 0 && strcmp(tag.c_str(),"funny_heavy") == 0)
	{
		post_type = "囧图";
	}
	else if(strcmp(content_type.c_str(),"2") == 0 && strcmp(tag.c_str(),"funnyvideo") == 0)
	{
		post_type = "视频";
	}
	else if(strcmp(content_type.c_str(),"3") == 0 && strcmp(tag.c_str(),"joke") == 0)
	{
		post_type = "段子";
	} 
	//投稿内容
	content_ = ms_->GetValueFromMapByKey("text",true);

	if (post_type.empty() || content_.empty())
	{
		return 0;
	}
	
	WriteCaseInfo(content_);
	output_text_ =	
		"sex=" + post_type + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=电脑\r\n" +
		"state=投稿\r\n";
	WriteIndexFile();
	return 1;
}

//pc 端投稿 囧图
int reading::NeiHanDuanZi::PCWebUploadPicture(const char *packet)
{
	//目前获取不到
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
//pc 端投稿 视频
int reading::NeiHanDuanZi::PCWebUploadVideo(const char *packet)
{
	//目前获取不到
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


//pc 端投稿 视频 路径
int reading::NeiHanDuanZi::PCWebUploadVideoPath(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//资源id
    string id,data,f,ua;

	id = ms_->GetValueFromMapByKey("id",true);
	data = ms_->GetValueFromMapByKey("data",true);
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	//判断是添加还是更改
	if (reader.parse(data,value))
	{	
		//文件路径
		f = value["f"].asString();
		//使用浏览器
		ua = value["ua"].asString();	
	}
	if (f.empty() || ua.empty())
	{
		return 0;
	}
	output_text_ = 
		"lang=" + f + "\r\n" + 
		"state=上传文件\r\n";	
	WriteIndexFile();
	return 1;
}

//pc 端评论 回复
int reading::NeiHanDuanZi::PCWebNewComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//判断评论 回复
	string reply_to_comment_id = ms_->GetValueFromMapByKey("reply_to_comment_id");
	//评论内容
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
			"platform=电脑\r\n" +
			"state=评论\r\n";
		WriteIndexFile();
	}
	else
	{
		WriteCaseInfo(content_);
		output_text_ =
			"msg=" + content_ + "\r\n" +
			"platform=电脑\r\n" +
			"state=回复\r\n";
		WriteIndexFile();
	}
	return 1;
}

//android 端投稿
int reading::NeiHanDuanZi::AndroidNewPost(const char *packet)
{
	//数据部分 
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("tag") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("tag"));
	}	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');  
	//投稿内容
	content_ = ms_->GetValueFromMapByKey("text",true);
	if (content_.empty())
	{
		return 0;
	}	
	WriteCaseInfo(content_);
	output_text_ =	
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n" +
		"state=投稿\r\n";
	WriteIndexFile();
	return 1;
}

//android 端投稿 囧图
int reading::NeiHanDuanZi::AndroidUploadPicture(const char *packet)
{
	//目前获取不到
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


//android 端投稿 视频
int reading::NeiHanDuanZi::AndroidUploadVideo(const char *packet)
{
	//目前获取不到
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

//android 端评论
int reading::NeiHanDuanZi::AndroidNewComment(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','='); 
	//评论内容
	content_ = ms_->GetValueFromMapByKey("text",true);
	if (content_.empty())
	{
		return 0;
	}
	WriteCaseInfo(content_);
	output_text_ =
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n" +
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}