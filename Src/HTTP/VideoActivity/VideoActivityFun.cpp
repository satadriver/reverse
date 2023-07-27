#include "StdAfx.h"
#include "VideoActivityFun.h"

videoactivity::FunVideo::FunVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "风行视频";	
}

videoactivity::FunVideo::~FunVideo()
{
	
}

int videoactivity::FunVideo::IsFunVideo(HttpSession *http_session)
{
	//网页搜索 1
	if((http_session->m_Requestline.m_Host.Find("api1.fun.tv")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/guess_like/search")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页评论 
	if((http_session->m_Requestline.m_Host.Find("api1.fun.tv")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/update")!=-1)) 
	{
		return kPcWebComment;
	}
    //pc搜索 
	if((http_session->m_Requestline.m_Host.Find("ps.funshion.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v7/search/video")!=-1)) 
	{
		return kPcClientSerch;
	}
	//android搜索 
	if((http_session->m_Requestline.m_Host.Find("ps.funshion.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v5/search/video")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android评论  
	if((http_session->m_Requestline.m_Host.Find("pinter.funshion.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::FunVideo::Process(const char *packet,int action)
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
	//pc搜索
	if (action == kPcClientSerch)
	{
		return PcClientSearch(packet);
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
int videoactivity::FunVideo::PcWebSearch(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
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
int videoactivity::FunVideo::PcWebComment(const char *packet)
{
	//从cookie 中获得昵称
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("funshion_username",true);
	if (nickname.empty())
	{
		nickname = "风行用户null";
	} 
	//获取内容
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =			
		"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //昵称	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=评论\r\n";						//64	[19]  //状态			
	WriteIndexFile();
	return 1;
}

//pc搜索
int videoactivity::FunVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("q",true);  
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformPC + "\r\n" +		//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android搜索
int videoactivity::FunVideo::AndroidSearch(const char *packet)
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
int videoactivity::FunVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != -1)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
    src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
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
