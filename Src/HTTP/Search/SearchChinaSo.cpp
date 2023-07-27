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
    //����
	if((http_session->m_Requestline.m_Host.Find("news.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1))
	{
		return kNews;
	}
    //��ҳ
	if((http_session->m_Requestline.m_Host.Find("www.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/pagesearch.htm")!=-1))
	{
		return kWeb;
	}
	//ͼƬ
	if((http_session->m_Requestline.m_Host.Find("image.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so")!=-1))
	{
		return kPicture;
	}
	//��Ƶ
	if((http_session->m_Requestline.m_Host.Find("v.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/so")!=-1))
	{
		return kVideo;
	}
	//�ٿ�
	if((http_session->m_Requestline.m_Host.Find("baike.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wiki/search")!=-1))
	{
		return kBaike;
	}
    //��ͼ
 	if((http_session->m_Requestline.m_Host.Find("service.map.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/region/query")!=-1))
	{
		return kMap;
	}
    //ѧ��
	if((http_session->m_Requestline.m_Host.Find("scholar.chinaso.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/searchwenxian/index.html")!=-1))
	{
		return kScholar;
	}
	//ͼ��
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
		type_ = "����";
		key = "wd";
	}
	if (action == kWeb)
	{
		type_ = "��ҳ";
		key = "q";
	}
	if (action == kPicture)
	{
		type_ = "ͼƬ";
		key = "q";
	}
	if (action == kVideo)
	{
		type_ = "��Ƶ";
		key = "q";
	}
	if (action == kBaike)
	{
		type_ = "�ٿ�";
		key = "q";
	}
	if (action == kMap)
	{
		type_ = "��ͼ";
		key = "keyword";
	}
	if (action == kScholar)
	{
		type_ = "ѧ��";
		key = "q";
	}
	if (action == kBook)
	{
		type_ = "ͼ��";
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
