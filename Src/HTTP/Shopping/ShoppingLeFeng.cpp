#include "StdAfx.h"

#include "ShoppingLeFeng.h"

shopping::LeFeng::LeFeng(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::LeFeng::~LeFeng()
{
	
}

int shopping::LeFeng::IsLeFeng(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find(".lefeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cart/cart_shippingInfo.jsp")!=-1))
	{
		return kPCWebAddAddress;
	}

	if((http_session->m_Requestline.m_Host.Find("mapi.dangdang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("action=add_address")!=-1))
	{
		return kAndroidAddAddress;
	}
	if((http_session->m_Requestline.m_Host.Find("v5.lefeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mall/user/login.jsp")!=-1))
	{
		return kAndroidLogin;
	}
	return 0;
}

int shopping::LeFeng::Process(const char *packet,int action)
{
	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}

	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}

	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}

	
	return 0;
}

int shopping::LeFeng::PCWebAddAddress(const char *packet)
{
	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("email",true);
	name_ = ms_->GetValueFromMapByKey("receiver",true);
	phone_ = ms_->GetValueFromMapByKey("mobile");
	address_ = ms_->GetValueFromMapByKey("address",true);
	string city,county;
	
	city = ms_->GetValueFromMapByKey("city",true);
	county = ms_->GetValueFromMapByKey("county",true);
	
	address_ = city + county + address_;
	if ( user_.empty() || name_.empty() || phone_.empty() || address_.empty())
	{ 
		return 0;
	}
	
	output_text_ = "user=" + user_ + "\r\n" +
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 1;
}


int shopping::LeFeng::AndroidAddAddress(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI);	
	ms_->SplitStringToMap(src_data,'&','=');
	
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	phone_ = ms_->GetValueFromMapByKey("mobile");
	address_ = ms_->GetValueFromMapByKey("address_detail",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 0;
}

int shopping::LeFeng::AndroidLogin(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');
	string user,pass;
	user = ms_->GetValueFromMapByKey("loginname",true);
	pass = ms_->GetValueFromMapByKey("password");
	
	if ( user.empty() || pass.empty())
	{
		return 0;
	}

	output_text_ =  
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=登录\r\n";
	
	WriteIndexFile();
	return 1;
}

