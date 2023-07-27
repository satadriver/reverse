
#include "StdAfx.h"

#include "ChatingWeiXin.h"

map<string, time_t> chating::WeiXin::unrepeated_ = map<string , time_t>();

chating::WeiXin::WeiXin(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::WeiXin::~WeiXin()
{
	
}

int chating::WeiXin::IsWeiXin(HttpSession *http_session)
{
    //手机首次联网 微信连接网络 
	if (http_session->m_Requestline.m_Host.Find("dns.weixin.qq.com") != -1 && 
		http_session->m_Requestline.m_URL.Find("/cgi-bin/micromsg-bin/newgetdns") != -1)
	{
		return kGetDns;
	}
	//登录
	if (((http_session->m_Requestline.m_Host.Find("short.weixin.qq.com") != -1) &&
		(http_session->m_Requestline.m_URL.Find("/cgi-bin/micromsg-bin/getcdndns") != -1)) || 
		((http_session->m_Requestline.m_Host.Find("szextshort.weixin.qq.com") != -1) &&
		(http_session->m_Requestline.m_URL.Find("/cgi-bin/micromsg-bin/getcdndns") != -1)))
	{
		return kLogin;
	}
  
	if ((http_session->m_Requestline.m_Host.Find("wx.qlogo.cn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/mmhead/ver") != -1) )
	{
		return kUIN;
	}

	if ((http_session->m_Requestline.m_Host.Find("mp.weixin.qq.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/mp/appmsgreport?") != -1) )
	{
		return kWXID;
	}
	return 0;
}

int chating::WeiXin::Process(const char *packet,int action)
{
	//获取dns
	if (action == kGetDns)
	{
		return GetDns(packet);
	}
	if (action == kLogin)
	{
		return Login(packet);
	}
	if (action == kUIN)
	{
		return UIN(packet);
	}
	if (action == kWXID)
	{
		return WXID(packet);
	}
	return 0;
}

//获取DNS
int chating::WeiXin::GetDns(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,"&","=");
	string uin = ms_->GetValueFromMapByKey("uin");
	if (uin.empty())
	{
		return 0;
	}
	if (Unrepeated(unrepeated_,uin))
	{
		output_text_ = 
			"user=" + uin + "\r\n";
		WriteIndexFile();
	}
	return 1;
}

int chating::WeiXin::Login(const char *packet)
{
	output_text_ = "state=上线\r\n";
	WriteIndexFile();
	return 1;
}

int chating::WeiXin::UIN(const char *packet)
{
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	ms_->Replace(referer,"?","&");
	ms_->SplitStringToMap(referer,'&','=');
	string uin = ms_->GetValueFromMapByKey("uin");
	if (uin.empty())
	{
		return 0;
	}
	if (Unrepeated(unrepeated_,uin))
	{
		output_text_ = 
			"user=" + uin + "\r\n";
		WriteIndexFile();
	}
	return 1;
}

int chating::WeiXin::WXID(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));

	ms_->Replace(uri,";","&");
	ms_->SplitStringToMap(uri,'&','=');
	string uin_base64 = ms_->GetValueFromMapByKey("uin");

	int out_byte = 0;

	string uin = ms_->Base64Decode(uin_base64,out_byte);
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));

	ms_->SplitStringToMap(cookie,';','=');

	string wxid;
	string wxid1 = ms_->GetValueFromMapByKey("mmusername");
	string wxid2 = ms_->GetValueFromMapByKey("wcid_c");
	if (!wxid1.empty())
	{
		wxid = wxid1;
	}
	else
	{
		wxid = wxid2;
	}
	string phone = ms_->GetValueFromMapByKey("authm_c");

	ms_->Replace(phone,"+86","");
	string nickname = ms_->GetValueFromMapByKey("wx_nickname");

	if ( !wxid.empty() || !phone.empty())
	{
		output_text_ =
			"webname=" + wxid + "\r\n" +
			"contact=" + phone + "\r\n" +
			"nick=" + nickname + "\r\n" + 
			"state=账号信息\r\n" + 
			"user=" + uin + "\r\n";
		WriteIndexFile();
		return 1;
	}
	return 0;
}
