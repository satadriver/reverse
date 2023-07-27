#include "StdAfx.h"
#include "ShoppingMoGuJie.h"

shopping::MoGuJie::MoGuJie(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MoGuJie::~MoGuJie()
{
	
}

int shopping::MoGuJie::IsMoGuJie(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("portal.mogujie.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/api/login/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find(".mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/addReceiveAddressActionlet/1")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find(".mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/modifyReceiveAddressActionlet/1")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find(".mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/createOrderActionlet/1")!=-1))
	{
		return kPCWebNewOrder;
	}
	//pc位置信息
	if((http_session->m_Requestline.m_Host.Find("portal.mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/validate/loginconfig")!=-1))
	{
		return kPCWebLocation;
	}
	//android登录信息
	//android新增地址
	//android修改地址
	//android提交订单
	//android位置信息
	return 0;
}

int shopping::MoGuJie::Process(const char *packet,int action)
{
	//pc登录信息
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc新增地址
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//pc修改地址
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	//pc提交订单
	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	//pc位置信息
	if (action == kPCWebLocation)
	{
		return PCWebLocation(packet);
	}	
	//android登录信息
	//android新增地址
	//android修改地址
	//android提交订单
	//android位置信息
	return 0;
}

//pc登录信息
int shopping::MoGuJie::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("uname",true);
	//密码加密
	pass = ms_->GetValueFromMapByKey("password");
	
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

//pc新增地址
int shopping::MoGuJie::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//地址
	address_ =  ms_->GetValueFromMapByKey("province",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("address",true);
	//邮编
    postcode_ = ms_->GetValueFromMapByKey("zip",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" + 
		"state=添加地址\r\n";	
	WriteIndexFile();
	return 0;
}

//pc修改地址
int shopping::MoGuJie::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//地址
	address_ = ms_->GetValueFromMapByKey("province",true) + 
			   ms_->GetValueFromMapByKey("city",true) +
			   ms_->GetValueFromMapByKey("area",true) +
		       ms_->GetValueFromMapByKey("address",true);	
	//邮编
    postcode_ = ms_->GetValueFromMapByKey("zip",true);
    //地址id
	addressid_ = ms_->GetValueFromMapByKey("addressId",true);	//暂时没用
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" + 
		"state=修改地址\r\n";	
	WriteIndexFile();
	return 0;
}

//pc提交订单
int shopping::MoGuJie::PCWebNewOrder(const char *packet)
{
	//信息不全
    return 0;
}

//pc位置信息
int shopping::MoGuJie::PCWebLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string uname,areaCode;
	uname = ms_->GetValueFromMapByKey("uname");
	areaCode = ms_->GetValueFromMapByKey("areaCode");

	if ( uname.empty() || areaCode.empty())
	{
		return 0;
	}
	
	output_text_ = "uname=" + uname + "\r\n" + 
		"areaCode=" + areaCode + "\r\n" + 
		"state=定位\r\n";
	
	//	WriteIndexFile();
	return 1;
}


//android登录信息
int shopping::MoGuJie::AndroidLogin(const char *packet)
{
	return 1;
}
//android新增地址
int shopping::MoGuJie::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android修改地址
int shopping::MoGuJie::AndroidModifyAddress(const char *packet)
{
	return 1;
}
//android提交订单
int shopping::MoGuJie::AndroidNewOrder(const char *packet)
{
	return 1;
}
//android位置信息
int shopping::MoGuJie::AndroidLocation(const char *packet)
{
	return 1;
}

