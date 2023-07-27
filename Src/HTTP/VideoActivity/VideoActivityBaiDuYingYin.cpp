#include "StdAfx.h"
#include "VideoActivityBaiDuYingYin.h"

videoactivity::BaiDuYingYinVideo::BaiDuYingYinVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "百度影音视频";	
}

videoactivity::BaiDuYingYinVideo::~BaiDuYingYinVideo()
{
	
}

int videoactivity::BaiDuYingYinVideo::IsBaiDuYingYinVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("v.baidu.com")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/v")!=-1)) 
	{
		return kPcWebSearch;
	}
	//android搜素
	if((http_session->m_Requestline.m_Host.Find("app.video.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/app")!=-1)) 
	{
		return kAndroidSearch;
	}
	return 0;
}

int videoactivity::BaiDuYingYinVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//android搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	return 1;
}

//用户搜索
int videoactivity::BaiDuYingYinVideo::PcWebSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	string content = ms_->GetValueFromMapByKey("word",true);
	if (content.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}


//用户搜索
int videoactivity::BaiDuYingYinVideo::AndroidSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	string content = ms_->GetValueFromMapByKey("word",true);
	if (content.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 			//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //来源
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}