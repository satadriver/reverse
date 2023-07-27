
#include "StdAfx.h"

#include "SearchSina.h"

search::Sina::Sina(HttpSession *http_session, const string &pro_name) : Search(http_session, pro_name)
{
	
}

search::Sina::~Sina()
{
	
}

int search::Sina::IsSina(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("iask.sina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1))
	{
		return kWWW;
	}
	return 0;
}

int search::Sina::Process(const char *packet,int action)
{
	if (action == kWWW)
	{
		return WWW(packet);
	}
	return 0;
}

int search::Sina::WWW(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string keyword = ms_->GetValueFromMapByKey("cond",true);
	if (keyword.length() == 0)
	{
		return 0;
	}
	output_text_ = 
		"msg=" + keyword + "\r\n";	
	WriteIndexFile();
	return 1;
}