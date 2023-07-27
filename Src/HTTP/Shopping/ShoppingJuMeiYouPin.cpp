#include "StdAfx.h"

#include "ShoppingJuMeiYouPin.h"

shopping::JuMeiYouPin::JuMeiYouPin(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::JuMeiYouPin::~JuMeiYouPin()
{
	
}

int shopping::JuMeiYouPin::IsJuMeiYouPin(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("cart.jumei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Confirmation/AddAddress")!=-1))
	{
		return kPCWebAddAddress;
	}

	if((http_session->m_Requestline.m_Host.Find("www.jumei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/i/account/ajax_login")!=-1))
	{
		return kPCWebLogin;
	}

	if((http_session->m_Requestline.m_Host.Find("mobile.jumei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/msapi/address/add.json")!=-1))
	{
		return kAndroidAddAddress;
	}

	return 0;
}

int shopping::JuMeiYouPin::Process(const char *packet,int action)
{
	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}

	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}

	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}

	
	return 0;
}

int shopping::JuMeiYouPin::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	
	name_ = ms_->GetValueFromMapByKey("recipient_name",true);
	phone_ = ms_->GetValueFromMapByKey("recipient_hp");
	string provice,city,county,street;
	
	provice = ms_->GetValueFromMapByKey("recipient_province",true);
	city = ms_->GetValueFromMapByKey("recipient_city",true);
	county = ms_->GetValueFromMapByKey("recipient_dist",true);
	street = ms_->GetValueFromMapByKey("recipient_street",true);
	address_ = provice + city + county + street;
	
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 1;
}

int shopping::JuMeiYouPin::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	user_ = ms_->GetValueFromMapByKey("email",true);
	password_ = ms_->GetValueFromMapByKey("password",true);

	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	WriteVirtualIdentity(user_,"上线");

	output_text_ = 
		"user=" + user_ + "\r\n" +  
		"password=" + password_ + "\r\n" + 
		"state=登录\r\n";
	
	WriteIndexFile();
	return 1;
}

int shopping::JuMeiYouPin::AndroidAddAddress(const char *packet)
{
	char *p = strstr(packet,"\r\n\r\n");
	if (!p)
	{
		return 0;
	}
	string src_data1(p);
	ms_->SplitStringToMap(src_data1,'&','=',"\r\n");
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	phone_ = ms_->GetValueFromMapByKey("mobile");
	address_ = ms_->GetValueFromMapByKey("address",true);
	
	string src_data2(http_session_->m_Requestline.m_Cookie.GetBuffer(0));	
	ms_->SplitStringToMap(src_data2,';','=');

	user_ = ms_->GetValueFromMapByKey("nickname",true);
	string imei = ms_->GetValueFromMapByKey("imei");
	string imsi = ms_->GetValueFromMapByKey("imsi");
	
	if ( name_.empty() || phone_.empty() || address_.empty() || user_.empty() || imei.empty() || imsi.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"imei=" + imei + "\r\n" + 
		"imsi=" + imsi + "\r\n" + 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 1;
}