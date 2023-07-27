#include "StdAfx.h"
#include "VideoActivityBaoFengYingYin.h"

videoactivity::BaoFengVideo::BaoFengVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "暴风影音视频";	
}

videoactivity::BaoFengVideo::~BaoFengVideo()
{
	
}

int videoactivity::BaoFengVideo::IsBaoFengVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("eventlog.hd.baofeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/logger.php?ltype=search")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论 回复
	if((http_session->m_Requestline.m_Host.Find("comments.baofeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/push")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.baofeng.com")!=-1))  
	{
		return kPcWebComment;
	}
	//pc客户端搜索
	if((http_session->m_Requestline.m_Host.Find("search.moviebox.baofeng.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc客户端评论
	if((http_session->m_Requestline.m_Host.Find("comments.baofeng.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/push")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("moviebox.baofeng.net")!=-1)) 
	{
		return kPcClientComment;
	}
	//pc客户端弹幕
	if((http_session->m_Requestline.m_Host.Find("danmu.baofeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/d")!=-1)) 
	{
		return kPcClientDanmu;
	}
	//pc客户端播放
	if((http_session->m_Requestline.m_Host.Find("clicklog.moviebox.baofeng.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/click.php")!=-1)) 
	{
		return kPcClientPlay;
	}
	//手机搜索
	if ((http_session->m_Requestline.m_Host.Find("so.shouji.baofeng.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1))
	{
		return kAndroidSearch;
	}
	//手机评论
	if ((http_session->m_Requestline.m_Host.Find("comments.baofeng.net")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/push")!=-1)&&
		(http_session->m_Requestline.m_Referer.GetLength() == 0)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::BaoFengVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//用户评论 回复
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//pc客户端搜素
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//pc客户端评论
	if (action == kPcClientComment)
	{
		return PcClientComment(packet);
	}
	//pc客户端弹幕
	if (action == kPcClientDanmu)
	{
		return PcClientDanmu(packet);
	}
	//pc客户端播放
	if (action == kPcClientPlay)
	{
		return PcClientPlay(packet);
	}
	//手机搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//用户评论
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}

//用户搜索
int videoactivity::BaoFengVideo::PcWebSearch(const char *packet)
{
	//从uri 中获取数据
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("msg",true);
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string comment = value["keyword"].asString();
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//用户评论 回复
int videoactivity::BaoFengVideo::PcWebComment(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string title = ms_->GetValueFromMapByKey("xname",true);
	string comment = ms_->GetValueFromMapByKey("yestxt",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" +title +"\r\n" +
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//pc客户端搜索
int videoactivity::BaoFengVideo::PcClientSearch(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string comment = ms_->GetValueFromMapByKey("keyword",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//pc客户端评论
int videoactivity::BaoFengVideo::PcClientComment(const char *packet)
{
	//从cookie 中提取数据
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	ms_->SplitStringToMap(cookie,';','=');
	//用户ID
	string userid = ms_->GetValueFromMapByKey("bfuid");
	//用户名称
    string username = ms_->GetValueFromMapByKey("bfuname",true);
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//视频名称
	string title = ms_->GetValueFromMapByKey("xname",true);
    //内容
	string content = ms_->GetValueFromMapByKey("yestxt",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));

	if (userid.empty() || username.empty() || title.empty() || content.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + userid + "\r\n" + 				//54	[119] //用户ID --
		"onick=" + username + "\r\n" + 			//55	[120] //用户名 --
		"mid=" + title + "\r\n" + 				//56	[121] //视频名称
		"msg=" + content + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//pc客户端弹幕
int videoactivity::BaoFengVideo::PcClientDanmu(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	
	string uid = ms_->GetValueFromMapByKey("uid");
	string content = ms_->GetValueFromMapByKey("text",true);
	if (uid.empty() || content.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + uid + "\r\n" + 				//54	[119] //用户ID	
		"msg=" + content + "\r\n" + 			//63	[128] //内容
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//pc客户端播放
int videoactivity::BaoFengVideo::PcClientPlay(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string uid = ms_->GetValueFromMapByKey("uid");
	string title = ms_->GetValueFromMapByKey("title",true);
	if (uid.empty() || title.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + uid + "\r\n" + 				//54	[119] //用户ID	
		"mid=" + title + "\r\n" + 				//56	[121] //视频名称
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=播放\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//手机搜索
int videoactivity::BaoFengVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"query=","&");
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机评论
int videoactivity::BaoFengVideo::AndroidComment(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string title = ms_->GetValueFromMapByKey("xname",true);
	string comment = ms_->GetValueFromMapByKey("yestxt",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" + title + "\r\n" +
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
