#include "StdAfx.h"
#include "VideoActivityTouTiao.h"

videoactivity::TouTiaoVideo::TouTiaoVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "头条视频";	
}

videoactivity::TouTiaoVideo::~TouTiaoVideo()
{
	
}

int videoactivity::TouTiaoVideo::IsTouTiaoVideo(HttpSession *http_session)
{

	//android搜索
	if((http_session->m_Requestline.m_Host.Find("ic.snssdk.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/wap/search/")!=-1)) 
	{
		return kAndroidSearch;
	}
	return 0;
}

int videoactivity::TouTiaoVideo::Process(const char *packet,int action)
{
	//android搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	return 1;
}

//Android搜索
int videoactivity::TouTiaoVideo::AndroidSearch(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("keyword",true);  
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
