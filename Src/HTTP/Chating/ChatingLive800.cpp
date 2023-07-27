
#include "StdAfx.h"

#include "ChatingLive800.h"

chating::Live::Live(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::Live::~Live()
{
	
}

int chating::Live::IsLive(HttpSession *http_session)
{
	//百家乐www.3.casino

	if( (http_session->m_Requestline.m_URI.Find("/chat/ChaterServer?cmd=203") != -1) &&
		(http_session->m_Requestline.m_URI.Find("msgContent=") != -1) &&
		(http_session->m_Requestline.m_Host.Find("chat.livechatvalue.com") != -1))	
	{
		return kBaiJiaLe;
	}

	//新世纪娱乐城

	if( (http_session->m_Requestline.m_URI.Find("/chat/ChaterServer?cmd=203") != -1) &&
		(http_session->m_Requestline.m_URI.Find("msgContent=") != -1) &&
		(http_session->m_Requestline.m_Host.Find("szzero.livechatvalue.com") != -1))	
	{
		return kXinShiJiYuLeCheng;
	}

	//申博太阳城或银河国际

	if( (http_session->m_Requestline.m_URI.Find("/live800/ChaterServer?cmd=203") != -1) &&
		(http_session->m_Requestline.m_URI.Find("msgContent=") != -1) &&
		(http_session->m_Requestline.m_Host.Find("chat56.live800.com") != -1))	
	{
		return kShenBoTaiYangCheng;
	}
	//同升国际

	if ((http_session->m_Requestline.m_URI.Find("/chat/ChaterServer?cmd=203") != -1) &&
		(http_session->m_Requestline.m_URI.Find("msgContent=") != -1) &&
		(http_session->m_Requestline.m_Host.Find("chat6.livechatvalue.com") != -1))
	{
		return kTongShengGuoJi;
	}
	return 0;
}

int chating::Live::Process(const char *packet,int action)
{
	if (action == kBaiJiaLe)
	{
		return BaiJiaLe(packet);
	}
	if (action==kXinShiJiYuLeCheng)
	{
		return XinShiJiYuLeCheng(packet);
	}
	if (action == kShenBoTaiYangCheng)
	{
		return ShenBoTaiYangCheng(packet);
	}
	if (action==kTongShengGuoJi)
	{
		return TongShengGuoJi(packet);
	}
	return 0;
}


int chating::Live::BaiJiaLe(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');
	from_ = ms_->GetValueFromMapByKey("msgContent",true);
	if (from_.empty())
	{
		return 0;
	}
	output_text_ = 
		"from=" + from_ + "\r\n" +
		"state=百家乐消息\r\n";
		WriteIndexFile();
	return 1;
}
int chating::Live::XinShiJiYuLeCheng(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');
	from_ = ms_->GetValueFromMapByKey("msgContent",true);
	if (from_.empty())
	{
		return 0;
	}
	output_text_ = 
		"from=" + from_ + "\r\n" +
		"state=新世纪娱乐城消息\r\n";
	WriteIndexFile();
	return 1;
}

int chating::Live::ShenBoTaiYangCheng(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');
	from_ = ms_->GetValueFromMapByKey("msgContent",true);
	if (from_.empty())
	{
		return 0;
	}
	output_text_ = 
		"from=" + from_ + "\r\n" +
		"state=申博太阳城消息\r\n";
	WriteIndexFile();
	return 1;
}
//同升国际
int chating::Live::TongShengGuoJi(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));

	ms_->SplitStringToMap(uri,'&','=');
	from_ = ms_->GetValueFromMapByKey("msgContent",true);
	if (from_.empty())
	{
		return 0;
	}
	output_text_ = 
		"from=" + from_ + "\r\n" +
		"state=同升国际消息\r\n";
	WriteIndexFile();
	return 1;

}
