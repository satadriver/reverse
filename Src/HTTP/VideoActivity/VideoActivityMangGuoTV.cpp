#include "StdAfx.h"
#include "VideoActivityMangGouTV.h"

videoactivity::MangGuoVideo::MangGuoVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "芒果TV视频";	
}

videoactivity::MangGuoVideo::~MangGuoVideo()
{
	
}

int videoactivity::MangGuoVideo::IsMangGuoVideo(HttpSession *http_session)
{
	//用户搜索1
	if((http_session->m_Requestline.m_Host.Find("so.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so/k")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户搜索2
	if((http_session->m_Requestline.m_Host.Find("so.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so?k")!=-1)) 
	{
		return kPcWebSearcht;
	}
	//用户评论 回复
	if((http_session->m_Requestline.m_Host.Find("comment.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/video_comment")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.mgtv.com") != -1)) 
	{
		return kPcWebComment;
	}
	//pc客户端搜索
	if((http_session->m_Requestline.m_Host.Find("pc.api.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v1/html/so/")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc客户端评论 回复
	if((http_session->m_Requestline.m_Host.Find("comment.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/video_comment")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("pc.api.mgtv.com") != -1)) 
	{
		return kPcClientComment;
	}
	//pc客户端播放
	if((http_session->m_Requestline.m_Host.Find("pc.da.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pc/player")!=-1)) 
	{
		//return kPcClientPlay;
	}
	//手机搜索
	if((http_session->m_Requestline.m_Host.Find("mobile.api.hunantv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v6/search/getResult")!=-1)) 
	{
		return kAndroidSearch;
	}
	//手机评论回复
	if ((http_session->m_Requestline.m_Host.Find("comment.mgtv.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/comment/write")!=-1))
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::MangGuoVideo::Process(const char *packet,int action)
{
	//用户搜索1
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//用户搜索2
	if (action == kPcWebSearcht)
	{
		return PcWebSearcht(packet);
	}
	//用户评论 回复
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//pc端搜索
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//pc端评论 回复
	if (action == kPcClientComment)
	{
		return PcClientComment(packet);
	}
	//pc端播放
	if (action == kPcClientPlay)
	{
		return PcClientPlay(packet);
	}
	//手机搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//手机评论回复
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}

//用户搜索
int videoactivity::MangGuoVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string::size_type pos;
	pos = src_data.find("/so/k");
	if (pos == string::npos)
	{
		return 0;
	}
	string comment = src_data.substr(pos + strlen("/so/k")+1);
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
//用户搜索2
int videoactivity::MangGuoVideo::PcWebSearcht(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"/so?k=","&");
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
int videoactivity::MangGuoVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string type = ms_->GetValueFromMapByKey("fid",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (type.empty() || comment.empty())
	{
		return 0;
	}
	if (type == "0")
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		    "state=评论\r\n";						//64	[19]  //状态
	}else
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		    "state=回复\r\n";						 //64	[19]  //状态
	}
	WriteIndexFile();
	return 1;
}

//pc端搜索
int videoactivity::MangGuoVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("name",true);
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
//pc端评论 回复
int videoactivity::MangGuoVideo::PcClientComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string type = ms_->GetValueFromMapByKey("fid",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (type.empty() || comment.empty())
	{
		return 0;
	}
	if (type == "0")
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
			"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
			"state=评论\r\n";						//64	[19]  //状态
	}else
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
			"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
			"state=回复\r\n";						 //64	[19]  //状态
	}
	WriteIndexFile();
	return 1;
}
//pc端播放
int videoactivity::MangGuoVideo::PcClientPlay(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string v = ms_->GetValueFromMapByKey("v",true);
	string title;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(v,value))
	{	
		//视频名称
		title   = value["title"].asString();
	}
	if (title.empty())
	{
		return 0;
	}
	output_text_ =	
		"mid=" + 	  	title + "\r\n" + 		//56	[121] //视频名称	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
//手机搜索
int videoactivity::MangGuoVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("name",true);
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
//手机评论回复
int videoactivity::MangGuoVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.find("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string type = ms_->GetValueFromMapByKey("commentId",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	if (type.empty())
	{
		output_text_ =	
			"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
			"state=手机评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	}else
	{
		output_text_ =	
			"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
			"state=手机回复\r\n";						//64	[19]  //状态
	WriteIndexFile();
	}
	return 1;
}