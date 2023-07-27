#include "StdAfx.h"
#include "ShoppingMiZhe.h"

shopping::MiZhe::MiZhe(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MiZhe::~MiZhe()
{
	
}

int shopping::MiZhe::IsMiZhe(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("d.mizhe.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/member/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find("trade.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/add_address")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find("trade.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/update_address")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc新增
	if((http_session->m_Requestline.m_Host.Find("dt.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/add_address")!=-1))
	{
		return kPCWebNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("dt.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/update_address")!=-1))
	{
		return kAndroidModifyAddress;
	}
	//android登录信息
	//android新增地址
	//android修改地址
	//android提交订单
	//android位置信息
	return 0;
}

int shopping::MiZhe::Process(const char *packet,int action)
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

	if (action ==kAndroidModifyAddress)
	{
		return AndroidModifyAddress(packet);
	}
	//android登录信息
	//android新增地址
	//android修改地址
	//android提交订单
	//android位置信息
	return 0;
}

//pc登录信息
int shopping::MiZhe::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("username",true);
	pass = ms_->GetValueFromMapByKey("passwd",true);
	
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
int shopping::MiZhe::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//地址
	string pro,city,area,address;
	pro = ms_->GetValueFromMapByKey("province_text",true);
	city = ms_->GetValueFromMapByKey("city_text",true);
	area = ms_->GetValueFromMapByKey("area_text",true);
	address = ms_->GetValueFromMapByKey("address",true);
	address_ =pro + city + area + address;
	//电话
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//邮编
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + code + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc修改地址
int shopping::MiZhe::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//地址
	string pro,city,area,address;
	pro = ms_->GetValueFromMapByKey("province_text",true);
	city = ms_->GetValueFromMapByKey("city_text",true);
	area = ms_->GetValueFromMapByKey("area_text",true);
	address = ms_->GetValueFromMapByKey("address",true);
	address_ =pro + city + area + address;
	//电话
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//邮编
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	AfxMessageBox(name_.c_str());
	AfxMessageBox(address_.c_str());
	AfxMessageBox(phone_.c_str());
	if (name_.empty() || address_.empty() || phone_.empty())
	{
		return 0;
	}

	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=修改地址\r\n";
	
	WriteIndexFile();
	return 1;
}

//pc新增
int shopping::MiZhe::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//地址
	string area,address;
	address=ms_->GetValueFromMapByKey("address",true);
	area = ms_->GetValueFromMapByKey("area",true);
		area=GetAddress(area);
	address_ =area + address;
	//电话
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//邮编
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + code + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 0;
}


//android登录信息
int shopping::MiZhe::AndroidLogin(const char *packet)
{
	return 1;
}
//android新增地址
int shopping::MiZhe::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android修改地址
int shopping::MiZhe::AndroidModifyAddress(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//地址
	string area,address;
	address=ms_->GetValueFromMapByKey("address",true);
	area = ms_->GetValueFromMapByKey("area",true);
	area=GetAddress(area);
	address_ =area + address;
	//电话
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//邮编
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + code + "\r\n" + 
		"state=修改地址\r\n";
	
	WriteIndexFile();
	return 0;
	return 1;
}
//android提交订单
int shopping::MiZhe::AndroidNewOrder(const char *packet)
{
	return 1;
}
//android位置信息
int shopping::MiZhe::AndroidLocation(const char *packet)
{
	return 1;
}

