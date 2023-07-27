#include "StdAfx.h"
#include "SearchSoChina.h"

search::SoChina::SoChina(HttpSession *http_session, const string &pro_name) : Search(http_session, pro_name)
{
	
}

search::SoChina::~SoChina()
{
	
}

string search::SoChina::GetType(string idType)
{
	string strType;
	if (idType == "0"){strType = "全部";}
	if (idType == "1"){strType = "全部";}
	if (idType == "2"){strType = "新闻";}
	if (idType == "3"){strType = "军事";}
	if (idType == "4"){strType = "汽车";}
	if (idType == "5"){strType = "游戏";}
	if (idType == "6"){strType = "科技";}
	if (idType == "7"){strType = "娱乐";}
	if (idType == "8"){strType = "社区";}
	if (idType == "9"){strType = "视频";}
	if (idType == "10"){strType = "文化";}
	return strType;
}

int search::SoChina::IsSoChina(HttpSession *http_session)
{
	//搜本站
	if((http_session->m_Requestline.m_Host.Find("so.china.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cse/search")!=-1))
	{
		return kWWW;
	}
	//搜全网
	if((http_session->m_Requestline.m_Host.Find("so.china.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/customsearch/click")!=-1))
	{
		return kWWW;
	}
	return 0;
}

int search::SoChina::Process(const char *packet,int action)
{
	if (action == kWWW)
	{
		return WWW(packet);
	}
	return 0;
}

int search::SoChina::WWW(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//搜索类型
	string nsid = ms_->GetValueFromMapByKey("nsid",true);
	type_ = GetType(nsid);
	//关键词
	keyword_ = ms_->GetValueFromMapByKey("q",true);	

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