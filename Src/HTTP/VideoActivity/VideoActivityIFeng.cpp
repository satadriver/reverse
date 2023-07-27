#include "StdAfx.h"
#include "VideoActivityIFeng.h"

videoactivity::IFengVideo::IFengVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "凤凰视频";	
}

videoactivity::IFengVideo::~IFengVideo()
{
	
}

int videoactivity::IFengVideo::IsIFengVideo(HttpSession *http_session)
{
	//网页搜索 1
	if((http_session->m_Requestline.m_Host.Find("so.v.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/video")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页搜索 2
	if((http_session->m_Requestline.m_Host.Find("so.v.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.action")!=-1)) 
	{
		return kPcWebSearch;
	}
	//网页评论
	if((http_session->m_Requestline.m_Host.Find("comment.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/postv.php")!=-1)) 
	{
		return kPcWebComment;
	}
	//android搜索
	if((http_session->m_Requestline.m_Host.Find("vcis.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/searchWeMedia")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android评论 
	if((http_session->m_Requestline.m_Host.Find("comment.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wappost")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::IFengVideo::Process(const char *packet,int action)
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
	//Android评论 回复
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}

//网页搜索
int videoactivity::IFengVideo::PcWebSearch(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("q",true);
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
int videoactivity::IFengVideo::PcWebComment(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);  
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
int videoactivity::IFengVideo::AndroidSearch(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//用户ID 备用
	string userID = ms_->GetValueFromMapByKey("userID");
	//内容
	string comment = ms_->GetValueFromMapByKey("keyword",true);  
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + 	  	userID + "\r\n" + 			//54	[119] //用户ID
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";							//64	[19]  //状态
	WriteIndexFile();
	return 1;
}

//Android评论 
int videoactivity::IFengVideo::AndroidComment(const char *packet)
{
	//从uri 中获取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//视频名称
	string docName = ms_->GetValueFromMapByKey("docName",true);
	//内容
	string comment = ms_->GetValueFromMapByKey("content",true);
// 	//用户名 uri 截断
// 	string username;
// 	string ext2 = ms_->GetValueFromMapByKey("ext2",true);
// 	//使用json 读取数据
// 	Json::Reader reader;
// 	Json::Value value;
// 	if (reader.parse(ext2,value))
// 	{	
// 		username = value["nickname"].asString();	
// 	}
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"mid=" + 	  	docName + "\r\n" + 			//56	[121] //视频名称
		"msg=" + 		comment + "\r\n" + 			//63	[128] //内容	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126] //操作平台
		"state=评论或回复\r\n";					
	WriteIndexFile();
	return 1;
}
