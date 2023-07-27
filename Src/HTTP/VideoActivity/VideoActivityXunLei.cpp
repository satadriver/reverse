#include "StdAfx.h"
#include "VideoActivityXunLei.h"

videoactivity::XunLeiVideo::XunLeiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "迅雷视频";	
}

videoactivity::XunLeiVideo::~XunLeiVideo()
{
	
}

int videoactivity::XunLeiVideo::IsXunLeiVideo(HttpSession *http_session)
{
	//用户搜索 1
	if((http_session->m_Requestline.m_Host.Find("search.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户搜索 2
	if((http_session->m_Requestline.m_Host.Find("kkpgv2.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/?u=search_page")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论
	if((http_session->m_Requestline.m_Host.Find("backend.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/weibo_new.json")!=-1)) 
	{
		return kPcWebcomment;
	}
	//用户回复
	if((http_session->m_Requestline.m_Host.Find("backend.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment_new.json")!=-1)) 
	{
		return kPcWebReply;
	}
	//用户弹幕
	if((http_session->m_Requestline.m_Host.Find("backend3.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/point.json")!=-1)) 
	{
		return kPcWebDanmu;
	}
	//pc端搜索	
	if((http_session->m_Requestline.m_Host.Find("search.kkp.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/?word=")!=-1)) 
	{
		return kPcClientSearch;
	}
	//手机用户搜索
	if((http_session->m_Requestline.m_Host.Find("search.pad.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search4android.php")!=-1)) 
	{
		return kAndroidSearch;
	}
	//手机用户评论
	if((http_session->m_Requestline.m_Host.Find("backend.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/weibo_new_client.json")!=-1)) 
	{
		return kAndroidComment;
	}
	//手机用户弹幕
	if((http_session->m_Requestline.m_Host.Find("backend3.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/point_client.json")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::XunLeiVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//用户评论
	if (action == kPcWebcomment)
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
	//pc端搜索
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//手机用户搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//手机用户评论
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
int videoactivity::XunLeiVideo::PcWebSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string content;
	if (uri.find("/?u=search_page") != -1) 
	{
		ms_->Replace(uri,"?","&");
		ms_->SplitStringToMap(uri,'&','=');
		content = ms_->GetValueFromMapByKey("u1",true);
	}
	if (uri.find("/search.php") != -1)
	{
		ms_->Replace(uri,"?","&");
		ms_->SplitStringToMap(uri,'&','=');
		content = ms_->GetValueFromMapByKey("keyword",true);
	}
	if (content.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户评论
int videoactivity::XunLeiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("subject_title",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"mid=" + title + "\r\n" + 				//56	[121] //视频名称  
		"msg=" + comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户回复
int videoactivity::XunLeiVideo::PcWebReply(const char *packet)
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
		"msg=" + comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=回复\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//用户弹幕
int videoactivity::XunLeiVideo::PcWebDanmu(const char *packet)
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
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"netmark=" + app_ + "\r\n" +
		"state=弹幕\r\n";
	WriteIndexFile();
	return 1;
}
//pc端搜索
int videoactivity::XunLeiVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string content;
	if (uri.find("word=") != string::npos)
	{
		content = uri.substr(uri.find("word=") + strlen("word="));
		content = ms_->UTF8ToGBK(UrlDecode(content));
	}
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";
	WriteIndexFile();
	return 1;
}
//手机用户搜索
int videoactivity::XunLeiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string content = ms_->GetValueBetweenBeginEnd(src_data,"keyword=","&");
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";
	WriteIndexFile();
	return 1;
}
//手机用户评论
int videoactivity::XunLeiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string title = ms_->GetValueFromMapByKey("subject_title",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
		output_text_ = 
		"form=" + title + "\r\n" +
		"msg=" + comment + "\r\n"+
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"netmark=" + app_ + "\r\n" +
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}
//手机用户弹幕
int videoactivity::XunLeiVideo::AndroidDanmu(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}
		output_text_ = 
		"msg=" + content + "\r\n"+
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"netmark=" + app_ + "\r\n" +
		"state=弹幕\r\n";
	WriteIndexFile();
	return 1;
}
