#include "StdAfx.h"
#include "VideoActivityPpTv.h"

videoactivity::PpTvVideo::PpTvVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "PPTV聚力视频";	
}

videoactivity::PpTvVideo::~PpTvVideo()
{
	
}

int videoactivity::PpTvVideo::IsPpTvVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("web.data.pplive.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pc/1.html?plt=web")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论 /sc/v3/pplive/ref/vod_25155189/feed/info?appplt=web&_method=post&_json={%22content%22:%22hello,%E4%BD%A0%E5%A5%BD%22,%22user_name%22:%22_2263853c6710030101c9@sina%22,%22type%22:0}&tk=K1X5vm9whs_nhhPGlGDvaXBOQTa2m9Hi7RnX0fp8Tv_GAlKCvGCVDLgdPkzsCsLiAOMGNrXLE1zE6VxLaFEfGb9puNHkef-sqDA2akGtgQpJygcw8mIl1rSBflyuxyqddmdCW8AGI1vbStwR8ZtZS4YCGeT8CZcVPC1rkPnaysc&from=web&version=1.0.0&format=jsonp&cb=pplive_callback_10
	if((http_session->m_Requestline.m_Host.Find("api.sc.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/sc/v3/pplive/ref")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("feed/info?appplt=web")!=-1)) 
	{
		//return kPcWebComment;
	}
	//用户回复 /sc/v3/pplive/ref/vod_25155189/feed/4032217233/reply?appplt=web&_method=post&_json={%22content%22:%22%E5%A5%BD%E7%9C%8Ben[PP%E5%A8%98%E5%BC%B9]%22,%22user_name%22:%22_2263853c6710030101c9@sina%22,%22at_id%22:%22%22,%22at_user_name%22:%22%22,%22at_nick_name%22:%22%E7%A7%BB%E5%8A%A8%E4%B8%93%E4%BA%AB%E4%BC%9A%E5%91%98%EF%BC%88%E9%98%BF%E9%87%8C%E4%BA%91OS%EF%BC%89%22}&tk=K1X5vm9whs_nhhPGlGDvaXBOQTa2m9Hi7RnX0fp8Tv_GAlKCvGCVDLgdPkzsCsLiAOMGNrXLE1zE6VxLaFEfGb9puNHkef-sqDA2akGtgQpJygcw8mIl1rSBflyuxyqddmdCW8AGI1vbStwR8ZtZS4YCGeT8CZcVPC1rkPnaysc&from=web&version=1.0.0&format=jsonp&cb=pplive_callback_12
	if((http_session->m_Requestline.m_Host.Find("api.sc.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/sc/v3/pplive/ref")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("reply?appplt=web")!=-1)) 
	{
		//return kPcWebComment;
	}
	//用户弹幕 
	if((http_session->m_Requestline.m_Host.Find("api.danmu.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/v1/pplive/danmu/info")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("appplt=web")!=-1)) 
	{
		return kPcWebDanmu;
	}
	//pc客户端搜索 
	if((http_session->m_Requestline.m_Host.Find("client.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/s/")!=-1)) 
	{
		return kPcClientSearch;
	} 
	//手机搜索
	if((http_session->m_Requestline.m_Host.Find("plt.data.pplive.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/1.html?isFFMode")!=-1)) 
	{
		return kAndroidSearch;
	}
	//手机评论	
	if((http_session->m_Requestline.m_Host.Find("api.sc.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/sc/v3/pplive/ref")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("appplt=aph")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("feed/info")!=-1)) 
	{
		return kAndroidComment;
	}
	//手机回复 
	if((http_session->m_Requestline.m_Host.Find("api.sc.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/sc/v3/pplive/ref")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("appplt=aph")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("reply")!=-1)) 
	{
		return kAndroidReply;
	}
	//手机弹幕  
	if((http_session->m_Requestline.m_Host.Find("api.danmu.pptv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/v1/pplive/danmu/info")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("appplt=aph")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::PpTvVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//用户弹幕
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//pc客户端搜索
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
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
int videoactivity::PpTvVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"kw=","&");
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户弹幕
int videoactivity::PpTvVideo::PcWebDanmu(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string comment = value["content"].asString();
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//pc客户端搜索
int videoactivity::PpTvVideo::PcClientSearch(const char *packet)
{
	//从referer 中提取数据
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
    string comment = ms_->GetValueFromMapByKey("kw");
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
//手机用户搜索
int videoactivity::PpTvVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("kw",true);
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
int videoactivity::PpTvVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string content = value["content"].asString();
	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + content + "\r\n" +     			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 	     		//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//手机用户回复
int videoactivity::PpTvVideo::AndroidReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string content = value["content"].asString();
	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + content + "\r\n" +     			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 	     		//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//手机弹幕
int videoactivity::PpTvVideo::AndroidDanmu(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string comment = value["content"].asString();
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}