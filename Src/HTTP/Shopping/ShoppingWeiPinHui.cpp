#include "StdAfx.h"

#include "ShoppingWeiPinHui.h"

shopping::WeiPinHui::WeiPinHui(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::WeiPinHui::~WeiPinHui()
{
	
}

int shopping::WeiPinHui::IsWeiPinHui(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("checkout.vip.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/shan/address.php")!=-1))
	{
		return kPCWebAddAddress;
	}
	return 0;
}

int shopping::WeiPinHui::Process(const char *packet,int action)
{
	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	
	return 0;
}

int shopping::WeiPinHui::PCWebAddAddress(const char *packet)
{
	
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名	
	name_ = ms_->GetValueFromMapByKey("consignee",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("phone");
	//地址
	address_ = ms_->GetValueFromMapByKey("address",true);
		
	if ( name_.empty() || phone_.empty() || address_.empty())
	{ 
		return 0;
	}	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加新地址\r\n";
	WriteIndexFile();
	return 1;
}