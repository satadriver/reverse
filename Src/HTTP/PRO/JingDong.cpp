
#include "StdAfx.h"

#include "JingDong.h"

JingDong::JingDong()
{
	
}

JingDong::~JingDong()
{
	
}

int JingDong::Is(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("trade.jd.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/dynamic/consignee/saveConsignee.action")!=-1))
	{
		return kSubmitOrderCW;
	}

	if((http_session->m_Requestline.m_Host.Find("gw.m.360buy.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("client.action?functionId=submitOrder")!=-1))
	{
		return kSubmitOrderM;
	}
	return 0;
}

int JingDong::Process(const char *packet,int action)
{
	
	if (action == kSubmitOrderCW)
	{
		return SubmitOrderCW(packet);
	}

	if (action == kSubmitOrderM)
	{
		return SubmitOrderM(packet);
	}
	
	return 0;
}

int JingDong::SubmitOrderCW(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');

	
	string user = ms_->GetValueFromMapByKey("consigneeParam.email");
	string name = ms_->GetValueFromMapByKey("consigneeParam.name",true);
	string phone = ms_->GetValueFromMapByKey("consigneeParam.mobile");

	
	string provice = ms_->GetValueFromMapByKey("consigneeParam.provinceName",true);
	string city = ms_->GetValueFromMapByKey("consigneeParam.cityName",true);
	string county = ms_->GetValueFromMapByKey("consigneeParam.countyName",true);
	string town = ms_->GetValueFromMapByKey("consigneeParam.townName",true);
	string detail = ms_->GetValueFromMapByKey("consigneeParam.address",true);

	string address = provice + city +  county + town + detail;
	
	if ( name.empty() || phone.empty() || address.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"nick=" + name + "\r\n" + 
		"oid=" + phone + "\r\n" + 
		"msg=" + address + "\r\n" + 
		"state=提交订单\r\n";
	
	WriteIndexFile();
	return 1;
}

int JingDong::SubmitOrderM(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	string address = ms_->GetValueBetweenBeginEnd(src_data,"\"Where\":\"","\"",true);
	string phone = ms_->GetValueBetweenBeginEnd(src_data,"\"Mobile\":\"","\"",true);
	string name = ms_->GetValueBetweenBeginEnd(src_data,"\"Name\":\"","\"",true);
	
	string fee = ms_->GetValueBetweenBeginEnd(src_data,"\"totalPrice\":\"","\"",true);

	if (name.empty() || phone.empty() || address.empty())
	{
		return 0;
	}

	output_text_ = 
		"nick=" + name + "\r\n" + 
		"oid=" + phone + "\r\n" + 
		"msg=" + address + "\r\n" + 
		"mid=" + fee + "\r\n" + 
		"state=提交订单\r\n";

	WriteIndexFile();
	return 1;
}

