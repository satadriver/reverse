#include "StdAfx.h"
#include "VideoActivityLeShi.h"

videoactivity::LeShiVideo::LeShiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "乐视视频";	
}

videoactivity::LeShiVideo::~LeShiVideo()
{
	
}
int videoactivity::LeShiVideo::IsLeShiVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("dc.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qy/")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论 
	if((http_session->m_Requestline.m_Host.Find("api.my.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/vcm/api/add")!=-1)) 
	{
		return kPcWebComment;
	}
	//用户回复 
	if((http_session->m_Requestline.m_Host.Find("api.my.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/vcm/api/reply")!=-1)) 
	{
		return kPcWebReply;
	}
	//用户弹幕	/danmu/add?vid=27639136&cid=2&pid=10022394&start=1122.965&txt=%E4%B8%89%E7%94%9F%E4%B8%89%E4%B8%96&font=m&position=4&tn=0.07311636907979846&type=txt&isIdentify=1&from=1&color=FFFFFF
	if((http_session->m_Requestline.m_Host.Find("hd.my.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/add")!=-1)) 
	{
		//return kPcWebDanmu;
	}
	//pc客户端搜索 
	if((http_session->m_Requestline.m_Host.Find("client.pc.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc客户端弹幕 
	if((http_session->m_Requestline.m_Host.Find("hd.my.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/add")!=-1)) 
	{
		//return kPcClientDanMu;
	}
	//手机用户搜索
	if((http_session->m_Requestline.m_Host.Find("dc.so.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/data_collect.so?action_code")!=-1)) 
	{
		return kAndroidSearch;
	}
	//手机评论
	if((http_session->m_Requestline.m_Host.Find("api.mob.app.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/add")!=-1)) 
	{
		return kAndroidComment;
	}
	//手机回复
	if((http_session->m_Requestline.m_Host.Find("api.mob.app.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/reply")!=-1)) 
	{
		return kAndroidReply;
	}
	//手机弹幕
	if((http_session->m_Requestline.m_Host.Find("dynamic.meizi.app.m.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/android/dynamic.php")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::LeShiVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//用户评论
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//用户回复
	if (action == kPcWebReply)
	{
		return PcWebReply(packet);
	}
	//用户弹幕
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//电脑客户端搜索
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//电脑客户端弹幕
	if (action == kPcClientDanMu)
	{
		return PcClientDanMu(packet);
	}
	//手机搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//手机评论
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//手机回复
	if (action == kAndroidReply)
	{
		return AndroidReply(packet);
	}
	//手机弹幕
	if (action == kAndroidDanmu)
	{
		return AndroidDanmu(packet);
	}
	return 1;
}

//用户搜索
int videoactivity::LeShiVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("query",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//用户评论
int videoactivity::LeShiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	string title = ms_->GetValueFromMapByKey("title",true);
	if (comment.empty() || title.empty())
	{
		return 0;
	}
	output_text_ =	
		"from" + title + "\r\n" +
		"msg=" + comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//回复
int videoactivity::LeShiVideo::PcWebReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  comment + "\r\n" +     		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//用户弹幕
int videoactivity::LeShiVideo::PcWebDanmu(const char *packet)
{
	return 1;
}
//电脑客户端搜索
int videoactivity::LeShiVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	
	string content = ms_->GetValueFromMapByKey("wd",true);
	content = ms_->UrlDecode(content);
	
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";
	WriteIndexFile();
	return 1;
}
//电脑客户端弹幕
int videoactivity::LeShiVideo::PcClientDanMu(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	
	string content = ms_->GetValueFromMapByKey("txt",true);
	content = ms_->UrlDecode(content);
	
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";
	WriteIndexFile();
	return 1;
}
//手机用户搜索
int videoactivity::LeShiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("query",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机评论
int videoactivity::LeShiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("content",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机回复
int videoactivity::LeShiVideo::AndroidReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("content",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机弹幕
int videoactivity::LeShiVideo::AndroidDanmu(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string txt = ms_->GetValueFromMapByKey("txt",true);
	if (txt.empty())
	{
		return 0;
	}	
	output_text_ =				
		"msg=" + txt + "\r\n" +     				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}