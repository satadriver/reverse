
#include "StdAfx.h"
#include "Search.h"
#include "SearchChinaSo.h"
#include "SearchSoChina.h"
#include "SearchSina.h"
#include "SearchSo360.h"


search::Search::Search(HttpSession *http_session, const string &pro_name):HttpPro(http_session, pro_name)
{
	
}

search::Search::~Search()
{
	
}

int search::Search::ProcessSearch(HttpSession *http_session ,const char *packet)
{
	int action = -1;
	int n = 0;
	try
	{
		//中国搜索	1
		action = ChinaSo::IsChinaSo(http_session);
		if (action)
		{
			ChinaSo cs(http_session,"search_zhongsou");
			return cs.Process(packet,action);
		}
		//中华网搜索 2
		action = SoChina::IsSoChina(http_session);
		if (action)
		{
			SoChina sochina(http_session,"search_china");
			return sochina.Process(packet,action);
		}
		//新浪问答	3
		action = Sina::IsSina(http_session);
		if (action)
		{
			Sina s(http_session,"SEARCH_IASK");
			return s.Process(packet,action);
		}
		//360搜索	4
		action = So360::IsSo360(http_session);
		if (action)
		{
			So360 s(http_session,"SEARCH_360");
			return s.Process(packet,action);
		}
	}
	catch(...)
	{
		WriteLog("dce2","ProcessSearch Err : %d",n);
	}
	return 0;
}

string search::Search::PretreatURI(const string &src_uri)
{
	if (!src_uri.empty())
	{
		string result;
		string::size_type pos_front = src_uri.find("?");
		if (pos_front != string::npos)
		{
			result = src_uri.substr(pos_front + 1);
			string::size_type pos_back = result.rfind(" HTTP/1.1");
			if (pos_back != string::npos)
			{
				result = result.substr(0,pos_back);
				return result;
			}
			else
			{
				return result;
			}
		}
		else
		{
			return src_uri;
		}
	}
	else
	{
		return src_uri;
	}
	
}