
#include "StdAfx.h"
#include "ComputerDeviceUserAgent.h"

computerdevice::UserAgent::UserAgent(HttpSession *http_session, const string &pro_name) 
: ComputerDevice(http_session,pro_name)
{
	
}

computerdevice::UserAgent::~UserAgent()
{
	
}

int computerdevice::UserAgent::IsUserAgent(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("www.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI == "/" ) )	
	{
		return kBaidu;
	}
	return 0;
}

int computerdevice::UserAgent::Process(const char *packet,int action)
{
	if (action == kBaidu)
	{
		return BaiDu(packet);
	}
	return 0;
}

int computerdevice::UserAgent::BaiDu(const char *packet)
{
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
				"webname=" + os + "\r\n";
			WriteIndexFile();
			return 1;
		}
	}
	return 0;
}