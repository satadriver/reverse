#include "StdAfx.h"
#include "BlogWangYiLofter.h"

blog::WangYiLofter::WangYiLofter(HttpSession *http_session, const string &pro_name) : Blog(http_session,pro_name)
{
	
}

blog::WangYiLofter::~WangYiLofter()
{
	
}

int blog::WangYiLofter::IsWangYiLofter(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("passport.www.lofter.com")!=-1 &&
	   (http_session->m_Requestline.m_URI.Find("/dl/l") != -1)))
	{
		return kPCWebLogin;
	}
	//文字
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/text/") != -1))	
	{
		return kWebNewTopic;
	}
	//图片
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/photo/") != -1))	
	{
		return kWebNewPicture;
	}
	//音乐
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/music/") != -1))	
	{
		return kWebNewMusic;
	}
	//视频
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/video/") != -1))	
	{
		return kWebNewVideo;
	}
	//长文章
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/new/long/") != -1))	
	{
		return kWebNewLong;
	}
	//评论 回复
	if( (http_session->m_Requestline.m_Host.Find("www.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/dwr/call/plaincall/PostBean.addPostResponse.dwr") != -1))	
	{
		return kWebPostComment;
	}
	//GOOGLE (imglf -- IE imglf0 -- Google)
	if( (http_session->m_Requestline.m_Host.Find("nos.netease.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/imglf") != -1))	
	{
		return KWebAttachFileGoogle;
	}
	//android文字 图片 视频
	if( (http_session->m_Requestline.m_Host.Find("api.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/v1.1/postNew.api?") != -1))	
	{
		return kAndroidNewTopicAll;
	}
	//android评论
	if( (http_session->m_Requestline.m_Host.Find("api.lofter.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/v1.1/commentAdd.api?") != -1))	
	{
		return kAndroidPostComment;
	}
	return 0;
}

int blog::WangYiLofter::Process(const char *packet,int action)
{
	//登录
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	//文字
	if (action == kWebNewTopic)
	{
		return WebNewTopic(packet);
	}
	//图片
	if (action == kWebNewPicture)
	{
		return WebNewPicture(packet);
	}
	//音乐
	if (action == kWebNewMusic)
	{
		return WebNewMusic(packet);
	}
	//视频
	if (action == kWebNewVideo)
	{
		return WebNewVideo(packet);
	}
	//评论
	if (action == kWebPostComment)
	{
		return WebPostComment(packet);
	}
	//长文章
	if (action == kWebNewLong)
	{
		return WebNewLong(packet);
	}
	//上传文件 Google IE
	if (action == KWebAttachFileGoogle)
	{
		return WebAttachFileGoogle(packet);
	}
	//android 文字 图片 视频
	if (action == kAndroidNewTopicAll)
	{
		return AndroidNewTopicAll(packet);
	}
	//android 评论
	if (action == kAndroidPostComment)
	{
		return AndroidPostComment(packet);
	}
	return 0;
}

//登录
int blog::WangYiLofter::PCWebLogin(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');

	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	//判断是添加还是更改
	if (reader.parse(src_data,value))
	{	
		user_ = value["un"].asString();
		nick_ = value["pw"].asString();	
	}
	if ( user_.empty() || nick_.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user_,"上线");
//原始数据
	output_text_ =
		"user=" + user_ + "\r\n" +
		"pass=" + nick_ + "\r\n" +
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//文字
int blog::WangYiLofter::WebNewTopic(const char *packet)
{	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//博客ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//标题
	title_ = ms_->GetValueFromMapByKey("title",true);
	//信息内容
	content_ = ms_->GetValueFromMapByKey("content");
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//昵称
		"webname=" + 	nick_ + "\r\n" +	//博客ID
		"netmark=" + 	title_ + "\r\n" +  //标题
		"msg=" + 		content_ + "\r\n" +	//信息内容
		"platform=电脑浏览器\r\n"
		"state=发表文字\r\n";
	WriteIndexFile();
	return 1;
}

//图片
int blog::WangYiLofter::WebNewPicture(const char *packet)
{
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//转义 空格
	ms_->Replace(src_data,"&nbsp;"," ");
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//博客ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("caption",true); 
	string titleGoogle;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	string photoInfo = ms_->GetValueFromMapByKey("photoInfo",true);
	if (reader.parse(photoInfo,value))
	{	
		for (int i = 0;i <value.size();i++)
		{
			//图片地址 IE (备用)
			title_ = value[i]["orign"].asString();
            //图片地址 Google (备用)
			titleGoogle = value[i]["raw"].asString();

			if (title_.empty() && !titleGoogle.empty())
			{
				output_text_ =
					"nick=" + 		user_ + "\r\n" +	//昵称
					"webname=" + 	nick_ + "\r\n" +	//博客ID
					"msg=" + 		content_ + "\r\n" +	//信息内容
					"platform=电脑浏览器\r\n"
					"state=发表图片\r\n";
				WriteIndexFile();
			}
			if (!title_.empty() && titleGoogle.empty())
			{
				output_text_ =
					"nick=" + 		user_ + "\r\n" +	//昵称
					"webname=" + 	nick_ + "\r\n" +	//博客ID
					"msg=" + 		content_ + "\r\n" +	//信息内容
					"platform=电脑浏览器\r\n"
					"state=发表图片\r\n";
				WriteIndexFile();
			}
		}
	}
	return 1;
}
//音乐
int blog::WangYiLofter::WebNewMusic(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//博客ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//内容
	content_ = ms_->GetValueFromMapByKey("caption",true);    
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	string musicInfo = ms_->GetValueFromMapByKey("embed",true);
	string song_name,artist_name,listenUrl;
	if (reader.parse(musicInfo,value))
	{	
		//歌曲名 (备用)
		song_name = value["song_name"].asString();
		//艺术家 (备用)
		artist_name = value["artist_name"].asString();
		//歌曲地址 (备用)
	    listenUrl = value["listenUrl"].asString();
	}
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//昵称
		"webname=" + 	nick_ + "\r\n" +	//博客ID
		"msg=" + 		content_ + "\r\n" +	//信息内容
		"platform=电脑浏览器\r\n"
		"state=发表音乐\r\n";
	WriteIndexFile();
	return 1;
}

//视频
int blog::WangYiLofter::WebNewVideo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//昵称
	user_ = ms_->GetValueBetweenBeginEnd(src_data,"blogName=","&content=",true);
	//博客ID
	nick_ = ms_->GetValueBetweenBeginEnd(src_data,"blogId=","&blogName=",true);
	//内容
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"caption=","&embed=",true);
	//视频信息
	string embed = ms_->GetValueBetweenBeginEnd(src_data,"embed=","&valCode=",true);

	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;	
	if (reader.parse(embed,value))
	{	
		//视频地址
		title_ = value["originUrl"].asString();
	}
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//昵称
		"webname=" + 	nick_ + "\r\n" +	//博客ID
		"msg=" + 		content_ + "\r\n" +	//信息内容
		"platform=电脑浏览器\r\n"
		"state=发表视频\r\n";
	WriteIndexFile();
	return 1;
}

//评论 回复
int blog::WangYiLofter::WebPostComment(const char *packet)
{
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');

	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	if (pos != string::npos)
	{
		user_ = tmp.substr(0, pos);        
	}
	//数据部分
	string src_data(packet);	
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));	
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"c0-e3=string:","\n",true);
	//判断评论 回复
	string replay = ms_->GetValueBetweenBeginEnd(src_data,"c0-e4=number:","\n",true);
	int panduan = ms_->ToInt(replay);
	if (panduan > 0)
	{
		output_text_ =
			"mid=" + 	user_ + "\r\n" +	//通行证号
			"msg=" + 	content_ + "\r\n" + //信息内容		
			"platform=电脑浏览器\r\n"
			"state=回复\r\n";
		WriteIndexFile();
	}
	else if (panduan == 0)
	{
		output_text_ =
			"mid=" + 	user_ + "\r\n" +	//通行证号
			"msg=" + 	content_ + "\r\n" + //信息内容		
			"platform=电脑浏览器\r\n"
			"state=评论\r\n";
		WriteIndexFile();
	}
	return 1;
}
//长文章
int blog::WangYiLofter::WebNewLong(const char *packet)
{	
    string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//昵称
	user_ = ms_->GetValueFromMapByKey("blogName",true);
	//博客ID
	nick_ = ms_->GetValueFromMapByKey("blogId",true);
	//标题
	title_ = ms_->GetValueFromMapByKey("title",true);
	//信息内容
	//content_ = ms_->GetValueFromMapByKey("content");
	content_ = ms_->GetValueFromMapByKey("longPostContent");
	output_text_ =
		"nick=" + 		user_ + "\r\n" +	//昵称
		"webname=" + 	nick_ + "\r\n" +	//博客ID
		"netmark=" + 	title_ + "\r\n" +	//标题
		"msg=" + 		content_ + "\r\n" +	//信息内容
		"platform=电脑浏览器\r\n"
		"state=发表长文章\r\n";
	WriteIndexFile();
	return 1;
}

//上传文件 GOOGLE
int blog::WangYiLofter::WebAttachFileGoogle(const char *packet)
{
	//目前获取不到
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	string user = ms_->GetValueFromMapByKey("email", true);
	
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

//android 文字 图片 视频
int blog::WangYiLofter::AndroidNewTopicAll(const char *packet)
{
	//(博客ID，内容，图片地址，位置)  
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//判断类型
	string type = ms_->GetValueFromMapByKey("type");
	if (type == "Text") //文字
	{
		//博客ID
		nick_ = ms_->GetValueFromMapByKey("blogid");
		//个人主页地址
		blogdomain_ = ms_->GetValueFromMapByKey("blogdomain");
		//标题
		title_ = ms_->GetValueFromMapByKey("title");
		//内容 
		content_ = ms_->GetValueFromMapByKey("content");
		//地理位置
		string PhotoUrl,Address,latitude,longitude;
		string location = ms_->GetValueFromMapByKey("location",true);
		Json::Reader readerLocation;
		Json::Value valueLocation;
		if (readerLocation.parse(location,valueLocation))
		{	
			//地址信息
			Address = valueLocation["city"].asString() + valueLocation["addr"].asString() + valueLocation["name"].asString();
			//经度
			CString strtemp;
			double a = valueLocation["longitude"].asDouble();
			strtemp.Format("%lf",a);
			longitude = strtemp.GetBuffer(0);
			double b = valueLocation["latitude"].asDouble();
			strtemp.Format("%lf",b);
			latitude = strtemp.GetBuffer(0);	
		}
		output_text_ =
			"webname=" + nick_ + "\r\n" +    //博客ID
			"netmark=" + title_ + "\r\n" + 	 //标题	
			"msg=" + 	content_ + "\r\n" +  //信息内容
			"from=" + 	longitude + "\r\n" + //经度
			"to=" + latitude + "\r\n" +		 //纬度	
			"fileurl=" + Address + "\r\n" +  //地址
			"imsi=" + 	blogdomain_ + "\r\n" +   //IP地址(个人主页地址)
			"platform=手机app\r\n"
			"state=发表文字\r\n";
		WriteIndexFile();
	}
	if (type == "Photo") //图片
	{
		//博客ID
		nick_ = ms_->GetValueFromMapByKey("blogid",true);
		//个人主页地址
		blogdomain_ = ms_->GetValueFromMapByKey("blogdomain");
		//内容
		content_ = ms_->GetValueFromMapByKey("caption",true); 
		//图片地址
		string PhotoUrl,Address,latitude,longitude;
		//使用json 读取数据
		Json::Reader readerPhoto;
		Json::Value valuePhoto;
		string photoInfo = ms_->GetValueFromMapByKey("photoInfo",true);
		if (readerPhoto.parse(photoInfo,valuePhoto))
		{	
			//图片地址
			int i = 0;
			PhotoUrl = valuePhoto[i]["raw"].asString();
		}
		//地理位置
		string location = ms_->GetValueFromMapByKey("location",true);
		Json::Reader readerLocation;
		Json::Value valueLocation;
		if (readerLocation.parse(location,valueLocation))
		{	
			//地址信息
			Address = valueLocation["city"].asString() + valueLocation["addr"].asString() + valueLocation["name"].asString();
			//经度
			CString strtemp;
			double a = valueLocation["longitude"].asDouble();
			strtemp.Format("%lf",a);
			longitude = strtemp.GetBuffer(0);
			double b = valueLocation["latitude"].asDouble();
			strtemp.Format("%lf",b);
			latitude = strtemp.GetBuffer(0);	
		}
		output_text_ =
			"webname=" + nick_ + "\r\n" +    //博客ID
			"msg=" + 	content_ + "\r\n" +  //信息内容
			"state=" +  PhotoUrl + "\r\n" +  //图片地址 
			"from=" + 	longitude + "\r\n" + //经度
			"to=" + latitude + "\r\n" +		 //纬度	
			"fileurl=" + Address + "\r\n" +  //地址
			"imsi=" + 	blogdomain_ + "\r\n" +   //IP地址(个人主页地址)
			"platform=手机app\r\n"
			"state=发表图片\r\n";
		WriteIndexFile();
	}
	if (type == "Video") //视频
	{
		//博客ID
		nick_ = ms_->GetValueFromMapByKey("blogid",true);
		//个人主页地址
		blogdomain_ = ms_->GetValueFromMapByKey("blogdomain");
		//内容
		content_ = ms_->GetValueFromMapByKey("caption",true); 
		//地理位置
		string PhotoUrl,Address,latitude,longitude;
		string location = ms_->GetValueFromMapByKey("location",true);
		Json::Reader readerLocation;
		Json::Value valueLocation;
		if (readerLocation.parse(location,valueLocation))
		{	
			//地址信息
			Address = valueLocation["city"].asString() + valueLocation["addr"].asString() + valueLocation["name"].asString();
			//经度
			CString strtemp;
			double a = valueLocation["longitude"].asDouble();
			strtemp.Format("%lf",a);
			longitude = strtemp.GetBuffer(0);
			double b = valueLocation["latitude"].asDouble();
			strtemp.Format("%lf",b);
			latitude = strtemp.GetBuffer(0);	
		}
		output_text_ =
			"webname=" + nick_ + "\r\n" +    //博客ID
			"msg=" + 	content_ + "\r\n" +  //信息内容
			"from=" + 	longitude + "\r\n" + //经度
			"to=" + latitude + "\r\n" +		 //纬度	
			"fileurl=" + Address + "\r\n" +  //地址
			"imsi=" + 	blogdomain_ + "\r\n" +   //IP地址(个人主页地址)
			"platform=手机app\r\n"
			"state=发表视频\r\n";
		WriteIndexFile();
	}
	return 1;
}

//android评论 回复
int blog::WangYiLofter::AndroidPostComment(const char *packet)
{
	//数据部分
	string src_data(packet);	
	string src_dataEnd = src_data.substr(src_data.find("commentjson=") + strlen("commentjson="));
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));	
	int replyToUserId = 0;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_dataEnd,value))
	{	
		//判断评论 回复
		replyToUserId = value["replyToUserId"].asInt();
		content_ = value["content"].asString();	
	}
	if (replyToUserId > 0)
	{
		output_text_ =
			"msg=" + 	content_ + "\r\n" + //信息内容
			"state=回复\r\n";
		WriteIndexFile();
	}
	else
	{
		output_text_ =
			"msg=" + 	content_ + "\r\n" + //信息内容
			"state=评论\r\n";
		WriteIndexFile();
	}
	return 1;
}
