#include "StdAfx.h"

#include "ShoppingYiHaoDian.h"

shopping::YiHaoDian::YiHaoDian(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::YiHaoDian::~YiHaoDian()
{
	
}

int shopping::YiHaoDian::IsYiHaoDian(HttpSession *http_session)
{	
	if((http_session->m_Requestline.m_Host.Find("www.yhd.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/checkoutV3/receiver/saveReceiver.do")!=-1))
	{
		return kPCWebAddress;
	}
	return 0;
}

int shopping::YiHaoDian::Process(const char *packet,int action)
{	
	if (action == kPCWebAddress)
	{
		return PCWebAddAddress(packet);
	}	
	return 0;
}

int shopping::YiHaoDian::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');	
	name_ = ms_->GetValueFromMapByKey("receiver.receiverName",true);
	phone_ = ms_->GetValueFromMapByKey("receiver.mobile");
	address_ = ms_->GetValueFromMapByKey("receiver.detailAddress",true);
	string provice,city,county,town;	
	provice = ms_->GetValueFromMapByKey("receiver.provinceID",true);
	city = ms_->GetValueFromMapByKey("receiver.cityID",true);
	county = ms_->GetValueFromMapByKey("receiver.countyID",true);	
	address_ = provice + "." + city + "." + county + "." + address_;
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=ÃÌº”µÿ÷∑\r\n";
	WriteIndexFile();
	return 1;
}
