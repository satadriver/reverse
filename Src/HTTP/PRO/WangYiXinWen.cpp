
#include "StdAfx.h"

#include "WangYiXinWen.h"

WangYiXinWen::WangYiXinWen()
{
	software_ = "网易新闻";
	pro_ = "phoneread_newswy";
}

WangYiXinWen::~WangYiXinWen()
{
	
}

int WangYiXinWen::Is(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("comment.news.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/reply/dopost.jsp") != -1 ) )	
	{
		return kAddCommentC;
	}

	if(  ( http_session->m_Requestline.m_Host.Find("comment.api.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/jsonp/post/insert") != -1 ) )	
	{
		return kAddCommentM;
	}

	if( ( http_session->m_Requestline.m_Host.Find("m.analytics.126.net")!= -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/news/c") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int WangYiXinWen::Process(const char *packet,int action)
{
	if (action == kAddCommentC)
	{
		return AddCommentC(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}

	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int WangYiXinWen::AddCommentC(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');

	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	string::size_type pos = tmp.find("|");
	string user;
	if (pos != string::npos)
	{
		user = tmp.substr(0, pos);
	}

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string content = ms_->GetValueFromMapByKey("body");
	string url = ms_->GetValueFromMapByKey("url");

	if (content.empty() || url.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"oid=" + url + "\r\n" +
		"platform=电脑\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int WangYiXinWen::AddCommentM(const char *packet)
{		
	string src_data = GetPostData_tmp(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user = ms_->GetValueFromMapByKey("userid",true);
	string content = ms_->GetValueFromMapByKey("body",true);

	if (user.empty() || content.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user + "\r\n" +
		"msg=" + content + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}

int WangYiXinWen::UploadDeviceInfoM(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,',',':',"\"");
	
	string version = ms_->GetValueFromMapByKey("o");
	string model = ms_->GetValueFromMapByKey("m",true);
	string imei = ms_->GetValueFromMapByKey("u");
	
	DeviceInfo device_info;
	string os = device_info.GetOS(model,version);
	
	if (imei.empty() || version.empty() || model.empty() || os.empty())
	{
		
		return 0;
	}
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}
