#include "StdAfx.h"
#include "VideoActivityMiaoPai.h"

videoactivity::MiaoPaiVideo::MiaoPaiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "秒拍";	
}

videoactivity::MiaoPaiVideo::~MiaoPaiVideo()
{
	
}

int videoactivity::MiaoPaiVideo::IsMiaoPaiVideo(HttpSession *http_session)
{
	//手机搜索
	if((http_session->m_Requestline.m_Host.Find("mobile.api.hunantv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v6/search/getResult")!=-1)) 
	{
		return kAndroidSearch;
	}
	//手机评论回复--数据包 获取不到
	if ((http_session->m_Requestline.m_Host.Find("c.miaopai.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/m/comment.json")!=-1))
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::MiaoPaiVideo::Process(const char *packet,int action)
{
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


//手机搜索
int videoactivity::MiaoPaiVideo::AndroidSearch(const char *packet)
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
int videoactivity::MiaoPaiVideo::AndroidComment(const char *packet)
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