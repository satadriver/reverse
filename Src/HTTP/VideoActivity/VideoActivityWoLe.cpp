#include "StdAfx.h"
#include "VideoActivityWoLe.h"

videoactivity::WoLeVideo::WoLeVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "56视频";	
}

videoactivity::WoLeVideo::~WoLeVideo()
{
	
}

int videoactivity::WoLeVideo::IsWoLeVideo(HttpSession *http_session)
{
	//网页搜索	(get数据包有时抓不到)
	if (http_session->m_Requestline.m_Host.Find("so.56.com") !=-1 && 
		http_session->m_Requestline.m_URI.Find("/all") !=-1 &&
		http_session->m_Requestline.m_Referer.Find("www.56.com/") != -1) 
	{
		return kPcWebSearch;
	}
	//网页搜索
	if (http_session->m_Requestline.m_Host.Find("click.hd.sohu.com.cn") != -1 &&
		http_session->m_Requestline.m_URI.Find("/x.gif?type=search_show") != -1 &&
		http_session->m_Requestline.m_Referer.Find("http://so.56.com/all/") != -1)
	{
		return kPcWebSearch;
	}
	//网页评论 回复
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.56.com")!=-1)) 
	{
		return kPcWebComment;
	}
	//网页弹幕 /dmpost?cid=6562626&aid=4066943&pid=2
	if((http_session->m_Requestline.m_Host.Find("interface.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/dmpost")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//网页播放 1 /v/web/web_page_view?mid=&fts=1486603842&url=http%253A%252F%252Fwww.bilibili.com%252Fvideo%252Fav8482736%252F&proid=1&ptype=1&module=main&title=%5BAMV%E6%A8%B1%E8%8A%B1%E9%A3%98%E8%90%BD%E7%9A%84%E7%9E%AC%E9%97%B4%2F%E5%8F%AF%E6%83%9C%E6%B2%A1%E5%A6%82%E6%9E%9C-%E7%A7%92%E4%BA%94%5D_%E5%8E%9F%E5%88%9B%E9%9F%B3%E4%B9%90_%E9%9F%B3%E4%B9%90_bilibili_%E5%93%94%E5%93%A9%E5%93%94%E5%93%A9%E5%BC%B9%E5%B9%95%E8%A7%86%E9%A2%91%E7%BD%91&ajaxtag=&ajaxid=&page_ref=http%3A%2F%2Fwww.bilibili.com%2Fvideo%2Fmusic-original-1.html&_=1486605859546         新增记录
	if((http_session->m_Requestline.m_Host.Find("data.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/web/web_page_view")!=-1)) 
	{
		//return kPcWebPlay;
	}
	//android搜索 /x/v2/search?duration=0&keyword=naruto%E6%80%A5&pn=1&ps=20&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&appkey=1d8b6e7d45233436&build=434000&mobi_app=android&platform=android
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wole/so")!=-1)) 
	{
		return kAndroidSearch;
	}
	return 0;
}

int videoactivity::WoLeVideo::Process(const char *packet,int action)
{
	//网页搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//网页评论 回复
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//网页弹幕
	if (action == kPcWebDanMu)
	{
		return PcWebDanMu(packet);
	}
	//网页播放
	if (action == kPcWebPlay)
	{
		return PcWebPlay(packet);
	}
	//android搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	return 1;
}

//网页搜索
int videoactivity::WoLeVideo::PcWebSearch(const char *packet)
{
	//从/all/naruto/?
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//网页搜索  
	string comment;
	if (uri.find("all/") != string::npos)
	{
		//内容
		comment = ms_->GetValueBetweenBeginEnd(uri,"all/","/?",true);
	}
	else if (uri.find("/x.gif?type=search_show") != -1)
	{
		//内容
		comment = ms_->GetValueBetweenBeginEnd(uri,"k=","&",true);
	}
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

//网页评论 回复
int videoactivity::WoLeVideo::PcWebComment(const char *packet)
{
	//获取内容
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	//判断 评论 回复
	string sourceCommentId = ms_->GetValueFromMapByKey("reply_id",true);
	if (comment.empty())
	{
		return 0;
	}
	if (sourceCommentId == "0")
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
			"state=评论\r\n";						//64	[19]  //状态
	}
	if (sourceCommentId != "0")
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

//网页弹幕
int videoactivity::WoLeVideo::PcWebDanMu(const char *packet)
{
	//提取数据
	string src_data(packet);
	if (src_data.find("fontsize=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("fontsize="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//内容
	string comment = ms_->GetValueFromMapByKey("message",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=弹幕\r\n";						//64	[19]  //状态				
	WriteIndexFile();
	return 1;
}
//网页播放
int videoactivity::WoLeVideo::PcWebPlay(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
//AfxMessageBox(uri.c_str());
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("title",true);  
//AfxMessageBox(comment.c_str());
	if (comment.empty())
	{
		return 0;
	}	
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //操作平台
		"state=播放\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android搜索
int videoactivity::WoLeVideo::AndroidSearch(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//内容
	comment = ms_->GetValueFromMapByKey("wd",true);  
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
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
