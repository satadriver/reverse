
#include "StdAfx.h"

#include "SearchSo360.h"

search::So360::So360(HttpSession *http_session, const string &pro_name) : Search(http_session, pro_name)
{
	
}

search::So360::~So360()
{
	
}
		
int search::So360::IsSo360(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("news.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ns?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kNews;
	}
	if((http_session->m_Requestline.m_Host.Find("wenda.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kWenDa;
	}
	if((http_session->m_Requestline.m_Host.Find("www.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/s?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kWeb;
	}
	if((http_session->m_Requestline.m_Host.Find("video.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("v?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kVideo;
	}
	if((http_session->m_Requestline.m_Host.Find("image.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/i?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kImage;
	}
	if((http_session->m_Requestline.m_Host.Find("music.haosou.com/")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/s?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kMusic;
	}	
	if((http_session->m_Requestline.m_Host.Find("map.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("citycode=")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("keyword")!=-1))
	{
		return kMap;
	}
	if((http_session->m_Requestline.m_Host.Find("baike.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("search")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q")!=-1))
	{
		return kBaiKe;
	}	
	if((http_session->m_Requestline.m_Host.Find("ly.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/s?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kLiangYi;
	}
	if((http_session->m_Requestline.m_Host.Find("soft.haosou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kSoft;
	}
	if((http_session->m_Requestline.m_Host.Find("s8.etao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/s?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("q=")!=-1))
	{
		return kETao;
	}	
	return 0;
}

int search::So360::Process(const char *packet,int action)
{

	string key;
	if (action == kNews)
	{
		type_ = "新闻";
		key = "q";
	}
	if (action == kWenDa)
	{
		type_ = "问答";
		key = "q";
	}
	if (action == kWeb)
	{
		type_ = "网页";
		key = "q";
	}
	if (action == kVideo)
	{
		type_ = "视频";
		key = "q";
	}
	if (action == kImage)
	{
		type_ = "图片";
		key = "q";
	}
	if (action == kMusic)
	{
		type_ = "音乐";
		key = "q";
	}
	if (action == kMap)
	{
		type_ = "地图";
		key = "keyword";
	}
	if (action == kBaiKe)
	{
		type_ = "百科";
		key = "q";
	}
	if (action == kLiangYi)
	{
		type_ = "良医";
		key = "q";
	}
	if (action == kSoft)
	{
		type_ = "软件";
		key = "q";
	}
	if (action == kETao)
	{
		type_ = "购物";
		key = "q";
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

