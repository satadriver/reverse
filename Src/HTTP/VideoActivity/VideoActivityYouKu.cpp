#include "StdAfx.h"
#include "VideoActivityYouKu.h"

videoactivity::YouKuVideo::YouKuVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "优酷视频";	
}

videoactivity::YouKuVideo::~YouKuVideo()
{
	
}

int videoactivity::YouKuVideo::IsYouKuVideo(HttpSession *http_session)
{
	//网页搜索
	//pc客户端搜索
	if((http_session->m_Requestline.m_Host.Find("www.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/passport")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页评论 回复
	if((http_session->m_Requestline.m_Host.Find("p.comments.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ycp/comment/pc/add")!=-1)) 
	{
		return kPcWebComment;
	}
	//网页弹幕 
	if((http_session->m_Requestline.m_Host.Find("service.danmu.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add?t")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//pc客户端弹幕
	if((http_session->m_Requestline.m_Host.Find("service.danmu.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("add")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("add?") ==-1)) 
	{
		return kPcClientDanMu;
	}
	//android搜索 
	if((http_session->m_Requestline.m_Host.Find("api.appsdk.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/i/s?")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android评论 回复 
	if((http_session->m_Requestline.m_Host.Find("p.comments.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ycp/comment/mobile/add")!=-1)) 
	{
		return kAndroidComment;
	}
	//android弹幕
	if((http_session->m_Requestline.m_Host.Find("dmapp.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/common/danmu/send")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::YouKuVideo::Process(const char *packet,int action)
{
	//网页搜索
	//pc客户端搜索
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
//pc客户端搜索
int videoactivity::YouKuVideo::PcWebSearch(const char *packet)
{
	//从referer 中提取数据
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	//网页搜索  
	if (referer.find("search_video") != string::npos)  
	{
		string comment;
		if (referer.find("?f") == string::npos)
		{
			//内容
			comment = referer.substr(referer.find("q_") + strlen("q_"));
			comment = ms_->UTF8ToGBK(UrlDecode(comment));
		}
		if (referer.find("?f") != string::npos)
		{
			//内容
			comment = ms_->GetValueBetweenBeginEnd(referer,"q_","?f",true);  
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
	}
	//pc客户端搜索
	if (referer.find("search_ikuvideo") != string::npos)  
	{
		string comment;
		if (referer.find("?f") == string::npos)
		{
			//内容
			comment = referer.substr(referer.find("q_") + strlen("q_"));
			comment = ms_->UTF8ToGBK(UrlDecode(comment));
		}
		if (referer.find("?f") != string::npos)
		{
			//内容
			comment = ms_->GetValueBetweenBeginEnd(referer,"q_","?f",true);  
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
	}
	return 1;
}

//网页评论 回复
int videoactivity::YouKuVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
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
			"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
			"state=评论\r\n";						//64	[19]  //状态
	}
	if (!sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
			"state=回复\r\n";						//64	[19]  //状态
	}					
	WriteIndexFile();
	return 1;
}

//网页弹幕
int videoactivity::YouKuVideo::PcWebDanMu(const char *packet)
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
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态				
	WriteIndexFile();
	return 1;
}

//pc客户端弹幕
int videoactivity::YouKuVideo::PcClientDanMu(const char *packet)
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
int videoactivity::YouKuVideo::AndroidSearch(const char *packet)
{
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
	string platform = "Android端";

	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android评论 回复
int videoactivity::YouKuVideo::AndroidComment(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
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
int videoactivity::YouKuVideo::AndroidDanMu(const char *packet)
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