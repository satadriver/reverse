#include "StdAfx.h"
#include "VideoActivityKuLiu.h"

videoactivity::KuLiuVideo::KuLiuVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "ku6视频";	
}

videoactivity::KuLiuVideo::~KuLiuVideo()
{
	
}

int videoactivity::KuLiuVideo::IsKuLiuVideo(HttpSession *http_session)
{
	//网页搜索  
	if((http_session->m_Requestline.m_Host.Find("video.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v?fr=ex_ku6")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页评论 
	if((http_session->m_Requestline.m_Host.Find("comment.ku6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/web_save.jhtm")!=-1)) 
	{
		//return kPcWebComment;
	}
	//android搜索 
	if((http_session->m_Requestline.m_Host.Find("search.ku6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/iosSearch.htm")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android评论 
	if((http_session->m_Requestline.m_Host.Find("comment.ku6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/save.jhtm")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::KuLiuVideo::Process(const char *packet,int action)
{
	//网页搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//网页评论 
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//android搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//Android评论 
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}
//网页搜索
int videoactivity::KuLiuVideo::PcWebSearch(const char *packet)
{
	//uri 中提取数据
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("word",true);
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

//网页评论
int videoactivity::KuLiuVideo::PcWebComment(const char *packet)
{
	//uri 中提取数据
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("comment.commentContent",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态		
	WriteIndexFile();
	return 1;
}
//Android搜索
int videoactivity::KuLiuVideo::AndroidSearch(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("q",true);  
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
//Android评论 
int videoactivity::KuLiuVideo::AndroidComment(const char *packet)
{
	//uri 中提取数据
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("comment.commentContent",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";							//64	[19]  //状态		
	WriteIndexFile();				
	return 1;
}
