#include "StdAfx.h"
#include "SearchChinaSo.h"

search::ChinaSo::ChinaSo(HttpSession *http_session, const string &pro_name) : Search(http_session, pro_name)
{
	
}

search::ChinaSo::~ChinaSo()
{
	
}

int search::ChinaSo::IsChinaSo(HttpSession *http_session)
{
    //新闻
	if((http_session->m_Requestline.m_Host.Find("news.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1))
	{
		return kNews;
	}
    //网页
	if((http_session->m_Requestline.m_Host.Find("www.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/pagesearch.htm")!=-1))
	{
		return kWeb;
	}
	//图片
	if((http_session->m_Requestline.m_Host.Find("image.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so")!=-1))
	{
		return kPicture;
	}
	//视频
	if((http_session->m_Requestline.m_Host.Find("v.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/so")!=-1))
	{
		return kVideo;
	}
	//百科
	if((http_session->m_Requestline.m_Host.Find("baike.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wiki/search")!=-1))
	{
		return kBaike;
	}
    //地图
 	if((http_session->m_Requestline.m_Host.Find("service.map.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/region/query")!=-1))
	{
		return kMap;
	}
    //学术
	if((http_session->m_Requestline.m_Host.Find("scholar.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/searchwenxian/index.html")!=-1))
	{
		return kScholar;
	}
	//图书
	if((http_session->m_Requestline.m_Host.Find("book.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("kw")!=-1))
	{
		return kBook;
	}
	return 0;
}

int search::ChinaSo::Process(const char *packet,int action)
{
	string key;
	if (action == kNews)
	{
		type_ = "新闻";
		key = "wd";
	}
	if (action == kWeb)
	{
		type_ = "网页";
		key = "q";
	}
	if (action == kPicture)
	{
		type_ = "图片";
		key = "q";
	}
	if (action == kVideo)
	{
		type_ = "视频";
		key = "q";
	}
	if (action == kBaike)
	{
		type_ = "百科";
		key = "q";
	}
	if (action == kMap)
	{
		type_ = "地图";
		key = "keyword";
	}
	if (action == kScholar)
	{
		type_ = "学术";
		key = "q";
	}
	if (action == kBook)
	{
		type_ = "图书";
		key = "kw";
	}
	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string src_data(PretreatURI(uri));	
	ms_->SplitStringToMap(src_data,'&','=');
	keyword_ = ms_->GetValueFromMapByKey(key,true);	
	if (keyword_.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + keyword_ + "\r\n" +
		"state=" + type_ + "\r\n";
	WriteIndexFile();
	return 1;
}
