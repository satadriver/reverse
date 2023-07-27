
#include "StdAfx.h"

#include "ShoppingFanKe.h"

shopping::FanKe::FanKe(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::FanKe::~FanKe()
{
	
}

int shopping::FanKe::IsFanKe(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("android-api.vancl.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/address/add")!=-1))
	{
		return kPhoneAddAddress;
	}
	return 0;
}

int shopping::FanKe::Process(const char *packet,int action)
{
	
	if (action == kPhoneAddAddress)
	{
		return PhoneAddAddress(packet);
	}
	
	return 0;
}

int shopping::FanKe::PhoneAddAddress(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');
	
	name_ = ms_->GetValueFromMapByKey("fullname",true);
	phone_ = ms_->GetValueFromMapByKey("mobile");
	address_ = ms_->GetValueFromMapByKey("province",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("addressdetail",true) ;
	
	if ( name_.empty() || phone_.empty() || address_.empty() )
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


