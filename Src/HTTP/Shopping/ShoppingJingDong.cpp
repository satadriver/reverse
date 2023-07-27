
#include "StdAfx.h"

#include "ShoppingJingDong.h"

shopping::JingDong::JingDong(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::JingDong::~JingDong()
{
	
}

int shopping::JingDong::IsJingDong(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("trade.jd.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/dynamic/consignee/saveConsignee.action")!=-1))
	{
		return kPCWebNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("gw.m.360buy.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("client.action?functionId=submitOrder")!=-1))
	{
		return kPhoneNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("order.m.jd.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("client.action?functionId=submitOrder")!=-1))
	{
		return kPhoneNewOrder;
	}

	
	return 0;
}

int shopping::JingDong::Process(const char *packet,int action)
{
	
	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}

	if (action == kPhoneNewOrder)
	{
		return PhoneNewOrder(packet);
	}
	
	return 0;
}

int shopping::JingDong::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');

	
	user_ = ms_->GetValueFromMapByKey("consigneeParam.email");
	name_ = ms_->GetValueFromMapByKey("consigneeParam.name",true);
	phone_ = ms_->GetValueFromMapByKey("consigneeParam.mobile",true);
	string provice,city,county,town;
	
	provice = ms_->GetValueFromMapByKey("consigneeParam.provinceName",true);
	city = ms_->GetValueFromMapByKey("consigneeParam.cityName",true);
	county = ms_->GetValueFromMapByKey("consigneeParam.countyName",true);
	town = ms_->GetValueFromMapByKey("consigneeParam.townName",true);
	address_ = ms_->GetValueFromMapByKey("consigneeParam.address",true);

	address_ = provice + city +  county + town + address_;
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = "user=" + user_ + "\r\n" + 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=提交订单\r\n";
	
	WriteIndexFile();
	return 1;
}

int shopping::JingDong::PhoneNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	address_ = ms_->GetValueBetweenBeginEnd(src_data,"\"Where\":\"","\"",true);
	phone_ = ms_->GetValueBetweenBeginEnd(src_data,"\"Mobile\":\"","\"",true);
	name_ = ms_->GetValueBetweenBeginEnd(src_data,"\"Name\":\"","\"",true);
	
	string fee = ms_->GetValueBetweenBeginEnd(src_data,"\"totalPrice\":\"","\"",true);

	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}

	phone_ = ms_->UrlDecode(phone_);
	output_text_ = "nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"mid=" + fee + "\r\n" + 
		"state=提交订单\r\n";

	WriteIndexFile();
	return 1;
}

