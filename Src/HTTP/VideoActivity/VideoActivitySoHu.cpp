#include "StdAfx.h"
#include "VideoActivitySoHu.h"

videoactivity::SoHuVideo::SoHuVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "搜狐视频";	
}

videoactivity::SoHuVideo::~SoHuVideo()
{
	
}

int videoactivity::SoHuVideo::IsSoHuVideo(HttpSession *http_session)
{
	//用户搜索 1	
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mts?wd=")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户搜索 2
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mts?box")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论 回复 
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit?")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("tv.sohu.com")!=-1)) 
	{
		return kPcWebComment;
	}
	//弹幕
	if((http_session->m_Requestline.m_Host.Find("api.danmu.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu?act=dmfire_v2&dct=1")!=-1)) 
	{
		return kPcWebDanmu;
	}
	//pc客户端搜索	
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ifox/v5/search")!=-1)) 
	{
		return kPcClientSearch;
	}
	//手机用户搜索
	if((http_session->m_Requestline.m_Host.Find("m.so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/new/keyword")!=-1)) 
	{
		return kAndroidSearch;
	}
	//手机用户评论 回复
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit?")==-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidComment;
	}
	//手机用户弹幕
	if((http_session->m_Requestline.m_Host.Find("api.danmu.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/?act=dmfire_v2")!=-1) &&	
		(http_session->m_Requestline.m_URI.Find("/danmu?act=dmfire_v2&dct=1") == -1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::SoHuVideo::Process(const char *packet,int action)
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
	//弹幕
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//pc客户端搜索
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//手机用户搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//手机用户评论 回复
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//手机用户弹幕
	if (action == kAndroidDanmu)
	{
		return AndroidDanmu(packet);
	}
	return 1;
}

//用户搜索
int videoactivity::SoHuVideo::PcWebSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
  	
	string content = ms_->GetValueFromMapByKey("wd",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户评论 回复
int videoactivity::SoHuVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	//判断 评论 回复
	string sourceCommentId = ms_->GetValueFromMapByKey("reply_id",true);
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
//弹幕
int videoactivity::SoHuVideo::PcWebDanmu(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + comment + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";
	WriteIndexFile();
	return 1;
}
//pc客户端搜索
int videoactivity::SoHuVideo::PcClientSearch(const char *packet)
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
//手机用户搜索
int videoactivity::SoHuVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("key",true);
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";
	WriteIndexFile();
	return 1;
}

//手机用户评论 回复
int videoactivity::SoHuVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	//判断 评论 回复
	string sourceCommentId = ms_->GetValueFromMapByKey("reply_id",true);
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
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
			"state=回复\r\n";							//64	[19]  //状态
	}					
	WriteIndexFile();
	return 1;
}
//手机弹幕
int videoactivity::SoHuVideo::AndroidDanmu(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + comment + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";
	WriteIndexFile();
	return 1;
}