
#include "StdAfx.h"
#include "ComputerDevice360AnQuanWeiShi.h"

computerdevice::AnQuanWeiShi360::AnQuanWeiShi360(HttpSession *http_session, const string &pro_name) 
: ComputerDevice(http_session,pro_name)
{
	mid_from_ = "360°²È«ÎÀÊ¿";
}

computerdevice::AnQuanWeiShi360::~AnQuanWeiShi360()
{
	
}

int computerdevice::AnQuanWeiShi360::IsAnQuanWeiShi360(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("safe.matrix.uc.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/index.php")  != -1  ) &&
		( http_session->m_Requestline.m_URI.Find("mid=")  != -1  ))	
	{
		return 1;
	}
	return 0;
}

int computerdevice::AnQuanWeiShi360::Process(const char *packet,int action)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->SplitStringToMap(uri,'&','=');
	mid_ = ms_->GetValueFromMapByKey("mid");
	if (mid_.empty())
	{
		return 0;
	}
	
	string user_agent(http_session_->m_Requestline.m_useragent.GetBuffer(0));
	
	vector<string>item_list;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;
	while(1)
	{
		item_end_pos = user_agent.find(";",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = user_agent.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = user_agent.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + 1/*strlen(";")*/;
		}
		
		item_list.push_back(item);
		if (item_list.size() == 4)
		{
			string os = item_list.at(2);
			output_text_ = 
				"webname=" + os + "\r\n" + 
				"mid=" + mid_ + "\r\n" +
				"msg=" + mid_from_ + "\r\n";
			WriteIndexFile();
			return 1;
		}
		
	}
	return 0;
}
