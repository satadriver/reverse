
#include "StdAfx.h"

#include "TianTiankuaidi.h"

kuaidi::TianTian::TianTian(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::TianTian::~TianTian()
{
	
}

int kuaidi::TianTian::IsTianTian(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("www.ttkdex.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("ttkdweb/query")!=-1))
	{
		return ktiantiangw;
	}
	return 0;
}

int kuaidi::TianTian::Process(const char *packet,int action)
{
	if (action == ktiantiangw)
	{
		return tiantiangw(packet);
	}
	return 1;
}

int kuaidi::TianTian::tiantiangw(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	hd = ms_->GetValueFromMapByKey("wen_no",true);
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=≤È—Ø\r\n";
	WriteIndexFile();
	return 1;
}
