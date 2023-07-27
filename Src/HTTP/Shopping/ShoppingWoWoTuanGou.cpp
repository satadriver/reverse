#include "StdAfx.h"

#include "ShoppingWoWoTuanGou.h"

shopping::WoWoTuanGou::WoWoTuanGou(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::WoWoTuanGou::~WoWoTuanGou()
{
	
}

int shopping::WoWoTuanGou::IsWoWoTuanGou(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("www.55tuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/buy/editAddr.do")!=-1))
	{
		return kPCWebAddAddress;
	}
	if((http_session->m_Requestline.m_Host.Find("login.55.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/loginByAjax")!=-1))
	{
		return kPCWebLogin;
	}
	if((http_session->m_Requestline.m_Host.Find("jinan.55tuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/shenbian/upser.do")!=-1))
	{
		return kPCWebShenBian;
	}
	return 0;
}

int shopping::WoWoTuanGou::Process(const char *packet,int action)
{
	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	if (action == kPCWebShenBian)
	{
	//	return PCWebShenBian(packet);
	}
	return 0;
}

int shopping::WoWoTuanGou::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');	
	name_ = ms_->GetValueFromMapByKey("consignee",true);
	phone_ = ms_->GetValueFromMapByKey("mobile");
	address_ = ms_->GetValueFromMapByKey("address",true);
	string addressname;
	addressname = ms_->GetValueFromMapByKey("addressName",true);
	address_ = addressname + address_;
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
	return 0;
}

int shopping::WoWoTuanGou::PCWebLogin(const char *packet)
{
	string password;
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	
	user_ = ms_->GetValueFromMapByKey("email",true);
	password = ms_->GetValueFromMapByKey("password",true);
	
	if (user_.empty() || password.empty())
	{
		return 0;
	}
	

	WriteVirtualIdentity(user_,"上线");
	output_text_ = "user=" + user_ + "\r\n" +  
		"password=" + password + "\r\n" + 
		"state=登录\r\n";
	
	WriteIndexFile();
	return 0;
}

int shopping::WoWoTuanGou::PCWebShenBian(const char *packet)
{
	string localname,localpoint;
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	localname = ms_->GetValueFromMapByKey("localName",true);
	localpoint = ms_->GetValueFromMapByKey("localPoint",true);
	
	if (localname.empty() || localpoint.empty())
	{
		return 0;
	}
	
	output_text_ = "localname=" + localname + "\r\n" +  
		"localpoint" + localpoint + "\r\n" + 
		"state=登录\r\n";
	
	WriteIndexFile();
	return 1;
}