#include "StdAfx.h"
#include "VideoActivityBiLiBiLi.h"

videoactivity::BiLiBiLiVideo::BiLiBiLiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "哔哩哔哩弹幕视频";	
}

videoactivity::BiLiBiLiVideo::~BiLiBiLiVideo()
{
	
}

int videoactivity::BiLiBiLiVideo::IsBiLiBiLiVideo(HttpSession *http_session)
{
	//网页搜索	
	if((http_session->m_Requestline.m_Host.Find("search.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/all")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页评论 回复
	if((http_session->m_Requestline.m_Host.Find("www.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comments/itemnewcomment.srv?method=add")!=-1)) 
	{
		//return kPcWebComment;
	}
	//网页弹幕 
	if((http_session->m_Requestline.m_Host.Find("interface.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/dmpost")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//网页播放 1 /v/web/web_page_view?mid=&fts=1486603842&url=http%253A%252F%252Fwww.bilibili.com%252Fvideo%252Fav8482736%252F&proid=1&ptype=1&module=main&title=%5BAMV%E6%A8%B1%E8%8A%B1%E9%A3%98%E8%90%BD%E7%9A%84%E7%9E%AC%E9%97%B4%2F%E5%8F%AF%E6%83%9C%E6%B2%A1%E5%A6%82%E6%9E%9C-%E7%A7%92%E4%BA%94%5D_%E5%8E%9F%E5%88%9B%E9%9F%B3%E4%B9%90_%E9%9F%B3%E4%B9%90_bilibili_%E5%93%94%E5%93%A9%E5%93%94%E5%93%A9%E5%BC%B9%E5%B9%95%E8%A7%86%E9%A2%91%E7%BD%91&ajaxtag=&ajaxid=&page_ref=http%3A%2F%2Fwww.bilibili.com%2Fvideo%2Fmusic-original-1.html&_=1486605859546         新增记录
	if((http_session->m_Requestline.m_Host.Find("data.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/web/web_page_view")!=-1)) 
	{
		//return kPcWebPlay;
	}
	//android搜索 /x/v2/search?duration=0&keyword=naruto%E6%80%A5&pn=1&ps=20&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&appkey=1d8b6e7d45233436&build=434000&mobi_app=android&platform=android
	if((http_session->m_Requestline.m_Host.Find("app.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/x/v2/search?duration")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android评论 回复 /x/v2/reply/add?_device=android&_hwid=440b7ad385b965a8&_ulv=5000&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&appkey=1d8b6e7d45233436&build=434000&mobi_app=android&platform=android&sign=eba0a80697b01d493e86b2d8e261d0ff
	if((http_session->m_Requestline.m_Host.Find("api.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/x/v2/reply/add")!=-1)) 
	{
		return kAndroidComment;
	}
	//android弹幕 /comment/post?_device=android&_ulv=5000&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&aid=4270788&appkey=1d8b6e7d45233436&build=434000&cid=6907160&mobi_app=android&pid=88935161&platform=android&sign=4ffb15b41d86a17b10324a5633d69575
	if((http_session->m_Requestline.m_Host.Find("api.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/post")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::BiLiBiLiVideo::Process(const char *packet,int action)
{
	//网页搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//网页评论 回复
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//网页弹幕
	if (action == kPcWebDanMu)
	{
		return PcWebDanMu(packet);
	}
	//网页播放
	if (action == kPcWebPlay)
	{
		return PcWebPlay(packet);
	}
	//android搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//Android评论 回复
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//Android弹幕
	if (action == kAndroidDanmu)
	{
		return AndroidDanMu(packet);
	}
	return 1;
}
//网页搜索
int videoactivity::BiLiBiLiVideo::PcWebSearch(const char *packet)
{
	//从referer 中提取数据	/all?keyword
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	string comment;
	if (uri.find("keyword") != string::npos)
	{
		//内容
		comment = uri.substr(uri.find("keyword") + strlen("keyword"));
		comment = ms_->UTF8ToGBK(UrlDecode(comment));
	}
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//网页评论 回复
int videoactivity::BiLiBiLiVideo::PcWebComment(const char *packet)
{
	//从cookie 中获得昵称
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("u_nick");
	if (nickname.empty())
	{
		nickname = "土豆用户null";
	}
	//获取内容
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	//判断 评论 回复
	string sourceCommentId = ms_->GetValueFromMapByKey("parent",true);
	if (comment.empty())
	{
		return 0;
	}
	if (sourceCommentId == "0")
	{
		output_text_ =				
			"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //昵称
			"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
			"state=评论\r\n";						//64	[19]  //状态
	}
	if (sourceCommentId != "0")
	{
		output_text_ =				
			"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //昵称
			"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
			"state=回复\r\n";						//64	[19]  //状态
	}					
	WriteIndexFile();
	return 1;
}

//网页弹幕
int videoactivity::BiLiBiLiVideo::PcWebDanMu(const char *packet)
{
	//提取数据
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("message",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态				
	WriteIndexFile();
	return 1;
}
//网页播放
int videoactivity::BiLiBiLiVideo::PcWebPlay(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
//AfxMessageBox(uri.c_str());
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("title",true);  
//AfxMessageBox(comment.c_str());
	if (comment.empty())
	{
		return 0;
	}	
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //操作平台
		"state=播放\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android搜索
int videoactivity::BiLiBiLiVideo::AndroidSearch(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("keyword",true);  
	comment = ms_->UTF8ToGBK(comment);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android评论 回复
int videoactivity::BiLiBiLiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	//提取数据
	if (src_data.find("parent=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("parent="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("message",true);
	//判断 评论 回复
	string sourceCommentId = ms_->GetValueFromMapByKey("parent",true);
	
	if (comment.empty())
	{
		return 0;
	}
	if (sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
			"state=评论\r\n";							//64	[19]  //状态
	}
	if (!sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
			"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126] //操作平台
			"state=回复\r\n";							//64	[19]  //状态
	}					
	WriteIndexFile();
	return 1;
}

//Android弹幕
int videoactivity::BiLiBiLiVideo::AndroidDanMu(const char *packet)
{
	//从数据部分获得内容
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("msg");
	if (content.empty())
	{
		return 0;
	}
	output_text_ =			
		"msg=" +		content + "\r\n" + 				//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" +	platformAndroid + "\r\n" +		//62	[126] //操作平台
		"state=弹幕\r\n";								//64	[19]  //状态
	WriteIndexFile();
	return 1;
}