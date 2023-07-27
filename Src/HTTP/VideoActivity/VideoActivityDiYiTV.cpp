#include "StdAfx.h"
#include "VideoActivityDiYiTV.h"

videoactivity::DiYiVideo::DiYiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "第一视频";	
}

videoactivity::DiYiVideo::~DiYiVideo()
{
	
}

int videoactivity::DiYiVideo::IsDiYiVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("www.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/getList")!=-1)) 
	{
		return kPcWebSearch;
	}
	//用户评论 
	if((http_session->m_Requestline.m_Host.Find("www.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/addComment")!=-1)) 
	{
		return kPcWebComment;
	}
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("i.apps.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/web/search/action")!=-1)) 
	{
		return kAndroidSearch;
	}
	//用户评论
	if((http_session->m_Requestline.m_Host.Find("dynamic.app.m.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/dynamic.php")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::DiYiVideo::Process(const char *packet,int action)
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
	return 1;
}

//用户搜索
int videoactivity::DiYiVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string::size_type pos = src_data.find("keyword=");
	if (pos == string::npos)
	{
		return 0;
	}
	string comment = src_data.substr(pos+strlen("keyword="));
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

//用户评论 
int videoactivity::DiYiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//手机用户搜索
int videoactivity::DiYiVideo::AndroidSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string content = ms_->GetValueFromMapByKey("keyword",true);
	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 	 content + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//手机评论
int videoactivity::DiYiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueFromMapByKey("comments",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}