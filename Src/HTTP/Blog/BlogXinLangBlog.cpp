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
	//发表博文
	if( (http_session->m_Requestline.m_Host.Find("control.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/admin/article/article_post.php") != -1))	
	{
		return kWebNewTopic;
	}
	//评论
	if( (http_session->m_Requestline.m_Host.Find("control.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/admin/comment_new/cms_post.php?domain=1") != -1))	
	{
		return kWebPostComment;
	}
	//回复
	if( (http_session->m_Requestline.m_Host.Find("control.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/admin/comment_new/cms_usereply_post.php?domain=2") != -1))	
	{
		return kWebReply;
	}
	//上传文件 博文
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/interface/pic_upload.php?app=blog") != -1))	
	{
		return KWebAttachFile;
	}
	//上传文件 相册
	if( (http_session->m_Requestline.m_Host.Find("upload.photo.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/interface/pic_upload.php?") != -1) && 
		(http_session->m_Requestline.m_URI.Find("app=photo") != -1))	
	{
		return KWebAttachFile;
	}
	//android 发表博文
	if( (http_session->m_Requestline.m_Host.Find("app.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/article/article_add.php") != -1))	
	{
		return kAndroidNewTopic;
	}
	//android 评论 
	if( (http_session->m_Requestline.m_Host.Find("app.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/comment/comment_add.php") != -1))	
	{
		return kAndroidPostComment;
	}
	//android 回复
	if( (http_session->m_Requestline.m_Host.Find("app.blog.sina.com.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/api/comment/comment_reply.php") != -1))	
	{
		return kAndroidPostComment;
	}
	//android 上传图片 
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
	//发表博文
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//评论
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//回复
	if (action == kWebReply)
	{
		return WebReply(packet);
	}
	//上传文件 日志
	if (action == KWebAttachFile)
	{
		return WebAttachFile(packet);
	}
	//android 发表博文
	if (action == kAndroidNewTopic)
	{
		return AndroidNewTopic(packet);
	}
	//android 评论 回复
	if (action == kAndroidPostComment)
	{
		return AndroidPostComment(packet);
	}
	return 0;
}


//发表日志 
int blog::SinaBlog::WebNewTopic(const char *packet)
{	
	//从cookie 中得到 通行证号 
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//博客ID
	string blogId = ms_->GetValueFromMapByKey("_s_loginuid");     

	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

//1 判读是否是游记
	string is_travel_editor = ms_->GetValueFromMapByKey("is_travel_editor");
	if (is_travel_editor == "1")
	{
		//发表时间
		string date_time = ms_->GetValueFromMapByKey("date_pub") + " " + ms_->GetValueFromMapByKey("time");
		string travel_json = ms_->GetValueFromMapByKey("travel_json");
		//使用json 读取数据
		Json::Reader reader;
		Json::Value value;
		if (reader.parse(travel_json,value))
		{	
			for (int i = 0;i <value.size();i++)
			{
				//游玩地点
				title_ = value[i]["mdd_name"].asString();
				//正文
				content_ = value[i]["mdd_desc"].asString();	
				output_text_ =
					"webname=" + 	blogId + "\r\n" + //博客ID
					"netmark=" + 	title_ + "\r\n" + //标题
					"msg=" + 		content_ + "\r\n" + //信息内容
					"platform=电脑浏览器\r\n"
					"state=发表\r\n";
				WriteIndexFile();
			}
		}
	}
	else
	{
		//2 博文部分
		//发表时间
		string date_time = ms_->GetValueFromMapByKey("date_pub") + " " + ms_->GetValueFromMapByKey("time");
		//标题
		title_ = ms_->GetValueFromMapByKey("blog_title",true);
		//内容
		content_ = ms_->GetValueFromMapByKey("blog_body",true);
		output_text_ =
			"webname=" + 	blogId + "\r\n" + //博客ID
			"netmark=" + 	title_ + "\r\n" + //标题
			"msg=" + 		content_ + "\r\n" + //信息内容
			"platform=电脑浏览器\r\n"
			"state=发表\r\n";
		WriteIndexFile();	
	}
	return 1;
}

//评论
int blog::SinaBlog::WebPostComment(const char *packet)
{
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	nick_ = ms_->GetValueFromMapByKey("login_name",true);
	//博客ID
	string blogId = ms_->GetValueFromMapByKey("uid",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("comment",true);

	output_text_ =
		"nick=" + 	nick_ + "\r\n" +	//昵称
		"webname=" + blogId + "\r\n" + //博客ID
		"msg=" + 	content_ + "\r\n" +	//信息内容
		"platform=电脑浏览器\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}


//回复
int blog::SinaBlog::WebReply(const char *packet)
{
	//数据部分
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	nick_ = ms_->GetValueFromMapByKey("login_name",true);
	//博客ID
	string blogId = ms_->GetValueFromMapByKey("uid",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("reply_content",true);
	//标题(文章标题)
	string blog_title = ms_->GetValueFromMapByKey("blog_title",true);
	//目标博客ID
	string source_uid = ms_->GetValueFromMapByKey("source_uid",true);
	//目标博客昵称
	string source_uname = ms_->GetValueFromMapByKey("source_uname",true);
	//地址(目标博客个人主页)
	string source_ulink= ms_->GetValueFromMapByKey("source_ulink",true);

	output_text_ =
		"nick=" + 		nick_ + "\r\n" +		//昵称
		"webname=" +	blogId + "\r\n" +		//博客ID
		"msg=" + 		content_ + "\r\n" +		//信息内容
		"netmark=" +	blog_title + "\r\n" +	//标题
		"oid=" + 		source_uid + "\r\n" +	//目标博客ID
		"clientmsg=" +	source_uname + "\r\n" + //目标博客昵称
		"fileurl=" + 	source_ulink + "\r\n" + //地址
		"platform=电脑浏览器\r\n"
		"state=回复\r\n";
	WriteIndexFile();
	return 1;
}


//上传文件
int blog::SinaBlog::WebAttachFile(const char *packet)
{
	//user_ 提取不到 设为 null
	user_ = "Null";
	//数据部分
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


//android 发表日志 
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

	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//博客ID
	string blogId = ms_->GetValueBetweenBeginEnd(src_data,"login_uid=","&",true);
	//标题
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"article_title=","&",true); 
	//内容 (待处理)//&#20170;&#22825;&#22825;&#27668;&#22909;&#26497;&#20102;,have a good time!!
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"<p dir=\"ltr\">","<b>",true);	
	ms_->Replace(content_,"&quot;","\"");	
	content_ = ms_->UnicodeToZhongWen(content_);
	output_text_ =

		"webname=" + 	blogId + "\r\n" + //博客ID
		"netmark=" + 	title_ + "\r\n" + //标题
		"msg=" + 		content_ + "\r\n" + //信息内容
		"platform=手机app\r\n"
		"state=发表\r\n";
	WriteIndexFile();	
	return 1;
}


//android 评论 回复
int blog::SinaBlog::AndroidPostComment(const char *packet)
{
	//数据部分
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//博客ID
	string blogId = ms_->GetValueFromMapByKey("login_uid",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("comment_content",true);
	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	if (uri.find("/api/comment/comment_add.php") != -1)
	{
		output_text_ =
			"webname=" + blogId + "\r\n" + //博客ID
			"msg=" + 	content_ + "\r\n" +	//信息内容
			"platform=电脑浏览器\r\n"
			"state=评论\r\n";
		WriteIndexFile();
	}
	if (uri.find("/api/comment/comment_reply.php") != -1)
	{
		output_text_ =
			"webname=" + blogId + "\r\n" + //博客ID
			"msg=" + 	content_ + "\r\n" +	//信息内容
			"platform=电脑浏览器\r\n"
			"state=回复\r\n";
		WriteIndexFile();
	}
	return 1;
}