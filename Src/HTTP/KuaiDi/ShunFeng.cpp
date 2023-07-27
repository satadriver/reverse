
#include "StdAfx.h"

#include "ShunFeng.h"

kuaidi::ShunFeng::ShunFeng(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::ShunFeng::~ShunFeng()
{
	
}

int kuaidi::ShunFeng::IsShunFeng(HttpSession *http_session)
{
	// pc 快递
	if((http_session->m_Requestline.m_Host.Find("www.sf-express.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("sf-service-owf-web/service/bills")!=-1))
	{
		return kpcshunfeng;
	}
	return 0;
}

int kuaidi::ShunFeng::Process(const char *packet,int action)
{
	if (action == kpcshunfeng)
	{
		return pcshunfeng(packet);
	}
	return 1;
}
int kuaidi::ShunFeng::pcshunfeng(const char *packet)
{
	string src_data(packet);
	hd = ms_->GetValueBetweenBeginEnd(src_data,"bills/","/routes");
	if (hd.empty())
	{
		return 0;
	}
	//多个单号  
	string srcSouce = hd;
	string split= ",";
	int start=0;
	int end = 0;
	string tmp;
	while(1)
	{
		end = srcSouce.find(split,start);
		if (end == string::npos)
		{
			if (start == string::npos)
			{
				break;
			}
			else
			{
				tmp = srcSouce.substr(start);
				start =string::npos;
				output_text_ = 
					"servername=" + tmp + "\r\n" +
					"state=查询\r\n";
				WriteIndexFile();
			}
		}
		else
		{
			tmp = srcSouce.substr(start,end-start);
			start = end + split.length();
			output_text_ = 
				"servername=" + tmp + "\r\n" +
				"state=查询\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}

