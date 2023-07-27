
#include "StdAfx.h"

#include "RealNameDongHang.h"

realname::DongHang::DongHang(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "¶«·½º½¿Õ";
}

realname::DongHang::~DongHang()
{
	
}

int realname::DongHang::IsDongHang(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("www.ceair.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/otabooking/paxinfo-input!checkDataNew.shtml") != -1 ) )	
	{
		return kBuyFlightTicket;
	}
	return 0;
}

int realname::DongHang::Process(const char *packet,int action)
{
	if (action == kBuyFlightTicket)
	{
		return BuyFlightTicket(packet);
	}
	return 0;
}


int realname::DongHang::BuyFlightTicket(const char *packet)
{	
	string data_buffer(packet);
	data_buffer = ms_->UrlDecode(data_buffer);
	ms_->SplitStringToMap(data_buffer,'&','=');

	string allPaxInfo = ms_->GetValueFromMapByKey("allPaxInfo",true);

	Json::Reader jreader;
	Json::Value jroot;
	if (jreader.parse(allPaxInfo,jroot))
	{
		int psz = jroot.size();
		for (int i = 0; i < psz; i++)
		{
			string paxName = jroot[i]["paxName"].asString();
			string idNo = jroot[i]["idNo"].asString();
			string mobile = jroot[i]["mobile"].asString();
		}
	}
	return 1;
}