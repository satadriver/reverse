#include "StdAfx.h"
#include "VideoActivityTuDou.h"

videoactivity::TuDouVideo::TuDouVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "土豆视频";	
}

videoactivity::TuDouVideo::~TuDouVideo()
{
	
}

int videoactivity::TuDouVideo::IsTuDouVideo(HttpSession *http_session)
{
	//网页搜索 1
	if((http_session->m_Requestline.m_Host.Find("www.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/t/nisearch")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/t/nisearch.do")==-1)) 
	{
		return kPcWebSearch;
	}
	//网页搜索 2
	if((http_session->m_Requestline.m_Host.Find("www.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/t/nisearch.do?kw=")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页评论 回复
	if((http_session->m_Requestline.m_Host.Find("www.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comments/itemnewcomment.srv?method=add")!=-1)) 
	{
		return kPcWebComment;
	}
	//网页弹幕 
	if((http_session->m_Requestline.m_Host.Find("service.danmu.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add?t")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//网页播放 1  新增记录
	if((http_session->m_Requestline.m_Host.Find("log.mmstat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/yt.gif?")!=-1)) 
	{
		return kPcWebPlay;
	}
	//网页播放 2  查看上一条记录
	if((http_session->m_Requestline.m_Host.Find("gm.mmstat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/yt/preclk")!=-1)) 
	{
		return kPcWebPlay;
	}
	//pc客户端搜索 
	if((http_session->m_Requestline.m_Host.Find("itudoupage.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/itudoupage/search.action?")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc客户端弹幕
	if((http_session->m_Requestline.m_Host.Find("service.danmu.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add?ver")!=-1)) 
	{
		return kPcClientDanMu;
	}
	//android搜索 (视频) /v4/search/spread?pidetwork=WIFI&keyword=%E9%82%A3%E5%A5%B3%E4%BA%BA%E7%9A%84%E9%80%89%E6%8B%A924
	if((http_session->m_Requestline.m_Host.Find("search.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4/search/spread")!=-1)) 
	{
		//return kAndroidSearch;
	}
	//android搜索 (自频道) /v4/search/spread?pidetwork=WIFI&keyword=%E9%82%A3%E5%A5%B3%E4%BA%BA%E7%9A%84%E9%80%89%E6%8B%A924
	if((http_session->m_Requestline.m_Host.Find("search.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4/search/spread")!=-1)) 
	{
		//return kAndroidSearch;
	}
	//android评论 回复 /v3_1/comment?pid=2bee25aa5485c2bb&_t_=1486533089&_e_=md5&_s_=383c1b65d80ae3c16c406edc244e5c33&guid=6347920e68ce220dc429967e72160ca6&ver=5.9&network=WIFI&itemCode=dDGzEnDSXzI&comment=hello
	if((http_session->m_Requestline.m_Host.Find("user.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3_1/comment")!=-1)) 
	{
		//return kAndroidComment;
	}
	//android弹幕 /v4_5/danmu/add?pid=2bee25aa5485c2bb&_t_=1486532241&_e_=md5&_s_=70420a583be4d46065acb865c87983f9&guid=6347920e68ce220dc429967e72160ca6&item_codes=dDGzEnDSXzI&playat=21580&propertis={"pos":3,"alpha":1,"size":1,"effect":0,"color":4294967295}&content=nihao&network=WIFI&ver=5.9
	if((http_session->m_Requestline.m_Host.Find("dm.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4_5/danmu/add")!=-1)) 
	{
		//return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::TuDouVideo::Process(const char *packet,int action)
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
	//pc客户端搜索
    if (action == kPcClientSearch)
    {
		return PcClientSearch(packet);
    }
	//pc客户端弹幕
	if (action == kPcClientDanMu)
	{
		return PcClientDanMu(packet);
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
int videoactivity::TuDouVideo::PcWebSearch(const char *packet)
{
	//从referer 中提取数据
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	string comment;
	if (uri.find("?f") == string::npos)
	{
		//内容
		comment = uri.substr(uri.find("kw=") + strlen("kw="));
	}
	if (uri.find("?f") != string::npos)
	{
		//内容
		comment = ms_->GetValueBetweenBeginEnd(uri,"nisearch/","?f",true);  
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
int videoactivity::TuDouVideo::PcWebComment(const char *packet)
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
int videoactivity::TuDouVideo::PcWebDanMu(const char *packet)
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
	//内容部分
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //昵称
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态				
	WriteIndexFile();
	return 1;
}
//网页播放
int videoactivity::TuDouVideo::PcWebPlay(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("title",true);  
	ms_->Replace(comment,"%u","");
	comment = ms_->UnicodeToGBK(comment);
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

//pc客户端搜索
int videoactivity::TuDouVideo::PcClientSearch(const char *packet)
{
	//从referer 中提取数据
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	string comment;
	if (uri.find("keyword=") != string::npos)
	{
		//内容
		comment = uri.substr(uri.find("keyword=") + strlen("keyword="));
		comment = ms_->UTF8ToGBK(UrlDecode(comment));
	}
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//pc客户端弹幕
int videoactivity::TuDouVideo::PcClientDanMu(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态				
	WriteIndexFile();
	return 1;
}

//Android搜索
int videoactivity::TuDouVideo::AndroidSearch(const char *packet)
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
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("keyword",true);  
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"nick="	 + 	  	nickname + "\r\n" + 		//58	[123] //昵称
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android评论 回复
int videoactivity::TuDouVideo::AndroidComment(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
AfxMessageBox(uri.c_str());
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//操作平台
	string app = ms_->GetValueFromMapByKey("app",true);
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	//判断 评论 回复
	string sourceCommentId = ms_->GetValueFromMapByKey("sourceCommentId",true);
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
int videoactivity::TuDouVideo::AndroidDanMu(const char *packet)
{
	//从cookie 中获得昵称
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("u");
	if (nickname.empty())
	{
		nickname = "优酷用户null";
	}
	//从数据部分获得内容
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//获取base64加密的信息
	string msg_base64 = ms_->GetValueFromMapByKey("msg");
	int out_byte = 0;
	string msg = ms_->Base64Decode(msg_base64,out_byte);
	string content;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(msg,value))
	{	
		//内容
		content = value["content"].asString();
		content = ms_->UTF8ToGBK(UrlDecode(content));
	}
	if (content.empty())
	{
		return 0;
	}
	output_text_ =			
		"nick="	 +		nickname + "\r\n" + 			//58	[123] //昵称
		"msg=" +		content + "\r\n" + 				//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" +	platformAndroid + "\r\n" +		//62	[126] //操作平台
		"state=弹幕\r\n";								//64	[19]  //状态
	WriteIndexFile();
	return 1;
}