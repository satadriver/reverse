
#include "StdAfx.h"

#include "EMS.h"

kuaidi::EMS::EMS(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::EMS::~EMS()
{
	
}

int kuaidi::EMS::IsEms(HttpSession *http_session)
{
	//pc 单个查询
	if((http_session->m_Requestline.m_Host.Find("www.ems.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ems/order/singleQuery_t")!=-1))
	{
		return kpcems;
	}
	//pc 多个查询
	if((http_session->m_Requestline.m_Host.Find("www.ems.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ems/order/multiQuery_tn")!=-1))
	{
		return kpcemsduo;
	}
	//Android 
	if((http_session->m_Requestline.m_Host.Find("shipping.ems.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ems-mobile/")!=-1))
	{
		return kandroidems;
	}
	return 0;
}

int kuaidi::EMS::Process(const char *packet,int action)
{
	//pc 单个查询
	if (action == kpcems)
	{
		return pcems(packet);
	}
	//pc 多个查询
	if (action == kpcemsduo)
	{
		return pcemsduo(packet);
	}
	//android 查询
	if (action ==kandroidems)
	{
		return androidems(packet);
	}
	return 1;
}

//pc 单个查询
int kuaidi::EMS::pcems(const char *packet)
{
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	hd = ms_->GetValueBetweenBeginEnd(src_data,"mailNum=","&");
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

//pc 多个查询
int kuaidi::EMS::pcemsduo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	hd = ms_->GetValueBetweenBeginEnd(src_data,"muMailNum=","&");
	ms_->Replace(hd,"\r\n",",");
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

//android 查询
int kuaidi::EMS::androidems(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	string emsquery="emsquery/";
	int n = uri.find(emsquery);
	if (n==string::npos)
	{
		return 0;
	}
	hd = uri.substr(n+emsquery.length());
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