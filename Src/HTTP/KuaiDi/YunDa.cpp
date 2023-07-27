
#include "StdAfx.h"

#include "YunDa.h"

kuaidi::YunDa::YunDa(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::YunDa::~YunDa()
{
	
}

int kuaidi::YunDa::IsYunDa(HttpSession *http_session)
{
	// pc 快递
	if((http_session->m_Requestline.m_Host.Find("www.yundaex.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cn/data/waybill_search")!=-1))
	{
		return kpcyunda;
	}
	//Android 
	if((http_session->m_Requestline.m_Host.Find("mapp.yundasys")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/appserver/interface.do")!=-1))
	{
		return kandroidyunda;
	}
	return 0;
}

int kuaidi::YunDa::Process(const char *packet,int action)
{
	if (action == kpcyunda)
	{
		return pcyunda(packet);
	}

	if (action ==kandroidyunda)
	{
		return androidyunda(packet);
	}
	return 1;
}
int kuaidi::YunDa::pcyunda(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	hd = ms_->GetValueBetweenBeginEnd(src_data,"waybill=","&");
	if (hd.empty())
	{
		return 0;
	}
	//多个单号  aaa,bbbb,bbbb
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

int kuaidi::YunDa::androidyunda(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("data",true);
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		hd = value["mailNo"].asString();
	}
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=查询\r\n";
	WriteIndexFile();
	return 1;
}