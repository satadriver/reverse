
#include "StdAfx.h"

#include "ZhongTong.h"

kuaidi::ZhongTong::ZhongTong(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::ZhongTong::~ZhongTong()
{
	
}

int kuaidi::ZhongTong::IsZhongTong(HttpSession *http_session)
{
	// pc中通快递
	if((http_session->m_Requestline.m_Host.Find("www.zto.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/GuestService/BillNew")!=-1))
	{
		return kzhongtong;
	}
	//Android 
	if((http_session->m_Requestline.m_Host.Find("usercenterapi.zto.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/ComInterface")!=-1))
	{
		return kAndroidZT;
	}
	return 0;
}

int kuaidi::ZhongTong::Process(const char *packet,int action)
{
	if (action == kzhongtong)
	{
		return zhongtong(packet);
	}

	if (action ==kAndroidZT)
	{
		return AndroidZT(packet);
	}
	return 1;
}
int kuaidi::ZhongTong::zhongtong(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	if (uri.find("txtbill") != string::npos)
	{
		int n=uri.find("txtbill=");
		if (n==string::npos)
		{
			return 0;
		}
		hd = uri.substr(n+ strlen("txtbill="));
		if (hd.empty())
		{
			return 0;
		}
		output_text_ = 
			"servername=" + hd + "\r\n" +
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (uri.find("txtbill") == string::npos)
	{
		string src_data(packet);
		//精确提取数据
		if (src_data.rfind("\r\n") != 0)
		{
			src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
		}
		hd = src_data.substr(src_data.find("txtbill=") + strlen("txtbill="));
		if (hd.empty())
		{
			return 0;
		}
		output_text_ = 
			"servername=" + hd + "\r\n" +
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}

int kuaidi::ZhongTong::AndroidZT(const char *packet)
{
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		hd = value["data"].asString();
	}
	if (hd.empty())
	{
		return 0;
	}
	ms_->Replace(hd,"[\"","");
	ms_->Replace(hd,"\"]","");
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=查询\r\n";
	WriteIndexFile();
	return 1;
}