#include "StdAfx.h"
#include "VideoActivityIqiYi.h"

videoactivity::IQiYiVideo::IQiYiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "爱奇艺视频";	
}

videoactivity::IQiYiVideo::~IQiYiVideo()
{
	
}

int videoactivity::IQiYiVideo::IsIQiYiVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("s.360.cn")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/so/zz.gif")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论(泡泡评论)
	if((http_session->m_Requestline.m_Host.Find("api-t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/feed/publish")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		return kPcWebComment;
	}
	//用户回复(泡泡回复)
	if((http_session->m_Requestline.m_Host.Find("sns-comment.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v2/comment/publish.action")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		return kPcWebReply;
	}
    //用户评论(普通评论)
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qx_api/comment/publish")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		return kPcWebPublish;
	}
	//用户回复(普通回复)  (Get包无法组包)
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qx_api/comment/reply")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		//return kPcWebReplyGeneral;
	}
	//用户弹幕	
	if((http_session->m_Requestline.m_Host.Find("bar.i.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myna-api/publish")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("secure_p=GPhone") ==-1)) 
	{
		//return kPcWebDanmu;
	}
	//电脑客户端搜索
	if((http_session->m_Requestline.m_Host.Find("search.video.qiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/o?if=pps")!=-1)) 
	{
		return kPcSearch;
	}
	//手机客户端搜索
	if((http_session->m_Requestline.m_Host.Find("msg.71.am")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v5/alt/act")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidSearch;
	}
	//手机客户端评论
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/feed/publish")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidComment;
	}
	//手机客户端回复 
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/feed/comment")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidReply;
	}
	//手机客户端弹幕
	if((http_session->m_Requestline.m_Host.Find("bar.i.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myna-api/publish")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("secure_p=GPhone")!=-1))  
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::IQiYiVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//用户评论(泡泡评论)
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//用户评论(普通评论)
	if (action == kPcWebPublish)
	{
		return PcWebPublish(packet);
	}
	//用户回复(泡泡回复)
	if (action == kPcWebReply)
	{
		return PcWebReply(packet);
	}
	//用户回复(普通回复)
	if (action == kPcWebReplyGeneral)
	{
		return PcWebReplyGeneral(packet);
	}
	//用户弹幕
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//电脑客户端搜索
	if (action == kPcSearch)
	{
		return PcSearch(packet);
	}
	//手机客户端搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//手机客户端评论
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//手机客户端回复
	if (action == kAndroidReply)
	{
		return AndroidReply(packet);
	}
	//手机客户端弹幕
	if (action == kAndroidDanmu)
	{
		return AndroidDanmu(packet);
	}
	return 1;
}

//用户搜索
int videoactivity::IQiYiVideo::PcWebSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	string src_data = ms_->UTF8ToGBK(UrlDecode(uri));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"q_","?source");
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//用户评论(泡泡评论)
int videoactivity::IQiYiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("description",true);
	if (comment.empty())
	{
		return 1;
	}
	output_text_ =				
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户评论(普通评论)
int videoactivity::IQiYiVideo::PcWebPublish(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("title",true);
	string comment = ms_->GetValueFromMapByKey("text",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + title + "\r\n" + 				//56	[121] //视频名称
		"msg=" + comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户回复(泡泡回复)
int videoactivity::IQiYiVideo::PcWebReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("text",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  text + "\r\n" + 				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户回复(普通回复)
int videoactivity::IQiYiVideo::PcWebReplyGeneral(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("text",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));

	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户弹幕
int videoactivity::IQiYiVideo::PcWebDanmu(const char *packet)
{
	return 1;
}
//pc客户端搜索
int videoactivity::IQiYiVideo::PcSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("key",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机客户端搜索
int videoactivity::IQiYiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string keyword = ms_->GetValueFromMapByKey("s_rq",true);
	if (keyword.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  keyword + "\r\n" + 				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机客户端评论
int videoactivity::IQiYiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("description",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));
	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  content + "\r\n" + 				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机客户端回复
int videoactivity::IQiYiVideo::AndroidReply(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("text",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));

	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  content + "\r\n" + 				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机客户端弹幕
int videoactivity::IQiYiVideo::AndroidDanmu(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("content",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));

	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  content + "\r\n" + 				//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
