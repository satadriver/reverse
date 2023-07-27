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
	//发表博文
	if( (http_session->m_Requestline.m_Host.Find("blog.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/usercp/index.php") != -1) &&
		(http_session->m_Requestline.m_URI.Find("op=editorUploadImg") == -1))	
	{
		return kWebNewTopic;
	}
	//评论　回复
	if( (http_session->m_Requestline.m_Host.Find("comment.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/post/?format=js&sid=") != -1) &&
		(http_session->m_Requestline.m_URI.Find("callback=sendCommentCallBack") != -1))	
	{
		return kWebPostComment;
	}
	//上传文件 博文
	if( (http_session->m_Requestline.m_Host.Find("blog.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/usercp/index.php?op=editorUploadImg") != -1))	
	{
		return KWebAttachFile;
	}
	//上传文件 相册
	if( (http_session->m_Requestline.m_Host.Find("blog.ifeng.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/public/index.php?param=albumUpload") != -1))	
	{
		return KWebAttachFileAlubm;
	}
	return 0;
}

int blog::FengHuangBlog::Process(const char *packet,int action)
{
	//发表博文
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//评论 回复
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//上传文件 日志
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//上传文件 相册
	if (action == KWebAttachFileAlubm)
	{
		return WebAttachFileAlubm(packet);
	}
	return 0;
}

//发表日志
int blog::FengHuangBlog::WebNewTopic(const char *packet)
{
	//通行证号，标题，内容，地址(省市区号)，IP地址
	string address,region_ip;
	//从cookie 中得到 通行证号 地址 IP地址
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
	user_ = ms_->GetValueFromMapByKey("IF_USER");      
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("prov") + "-" + ms_->GetValueFromMapByKey("city");
    //IP地址
	region_ip = ms_->GetValueFromMapByKey("region_ip");
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//标题
	title_ = ms_->GetValueFromMapByKey("subject",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("message",true);
	output_text_ =
		"mid=" + 	user_ + "\r\n" +		//通行证号
		"netmark=" + title_ + "\r\n" +		//标题
		"msg=" + 	content_ + "\r\n" +		//信息内容
		"fileurl=" + address + "\r\n" +		//地址
		"imsi=" + 	region_ip + "\r\n" +	//IP地址
		"platform=电脑浏览器\r\n"
		"state=发表\r\n";
	WriteIndexFile();
	return 1;
}

//评论 回复
int blog::FengHuangBlog::WebPostComment(const char *packet)
{
	//通行证号，文章标题，内容，文章地址，博客ID，地址(省市区号)，IP地址
	string address,region_ip;
	//从cookie 中得到 通行证号 地址 IP地址
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
	user_ = ms_->GetValueFromMapByKey("IF_USER");      
	//地址(省市 区号0531-0530)
	address = ms_->GetValueFromMapByKey("prov") + "-" + ms_->GetValueFromMapByKey("city");
    //IP地址
	region_ip = ms_->GetValueFromMapByKey("region_ip");

	//数据部分
	string url(http_session_->m_Requestline.m_URI.GetBuffer(0));
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');
	//文章标题
	title_ = ms_->GetValueFromMapByKey("docName"); 
	//内容
	content_ = ms_->GetValueFromMapByKey("content"); 
	//文章地址
    string docUrl = ms_->GetValueFromMapByKey("docUrl"); 
	//博客ID
	string blog_uid;
	string ext2 =  ms_->GetValueFromMapByKey("ext2"); 
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(ext2,value))
	{
		blog_uid = value["blog_uid"].asString();
	}
   	//目标博客ID 判断评论 回复
	string quoteId = ms_->GetValueFromMapByKey("quoteId"); 
	if (quoteId.empty())
	{
		output_text_ =		
			"mid=" + 		user_ + "\r\n" + //通行证号
			"netmark=" +	title_ + "\r\n" + //标题
			"msg=" + 		content_ + "\r\n" + //信息内容
			"clientmac=" + 	docUrl + "\r\n" +	//文章地址 
			"webname=" + 	blog_uid + "\r\n" + //博客ID
			"fileurl=" + 	address + "\r\n" +	//地址
			"imsi=" + 		region_ip + "\r\n" + //IP地址
			"platform=电脑浏览器\r\n"
			"state=评论\r\n";
		WriteIndexFile();
	}
	if (!quoteId.empty())
	{
		output_text_ =		
			"mid=" + 		user_ + "\r\n" +    //通行证号
			"netmark=" +	title_ + "\r\n" +   //标题
			"msg=" + 		content_ + "\r\n" + //信息内容
			"clientmac=" + 	docUrl + "\r\n" +	//文章地址 
			"webname=" + 	blog_uid + "\r\n" + //博客ID
			"oid=" + 		quoteId + "\r\n" + //目标博客ID
			"fileurl=" + 	address + "\r\n" +	//地址
			"imsi=" + 		region_ip + "\r\n" + //IP地址
			"platform=电脑浏览器\r\n"
			"state=回复\r\n";
		WriteIndexFile();
	}
	return 1;
}


//上传文件
int blog::FengHuangBlog::WebAttachFile(const char *packet)
{
	//从cookie 中获得user_  
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
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

 
//上传文件 相册
int blog::FengHuangBlog::WebAttachFileAlubm(const char *packet)
{
	//从cookie 中获得user_  
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//通行证号
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
