#include "StdAfx.h"
#include "ShoppingMoonBsSa.h"

shopping::MoonBaSa::MoonBaSa(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MoonBaSa::~MoonBaSa()
{
	
}

int shopping::MoonBaSa::IsMoonBaSa(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("login.moonbasa.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/home/userlogin")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址 修改地址
	if((http_session->m_Requestline.m_Host.Find("shopping.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/address/OrderAddressSubmit")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find("shopping.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/OrderSubmit")!=-1))
	{
		return kPCWebNewOrder;
	}
	//android登录
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPMemberApi/Login")!=-1))
	{
		return kAndroidLogin;
	}
	//android新增地址
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPMemberApi/AddCustAddr")!=-1))
	{
		return kAndroidAddAddress;
	}
	//android修改地址
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPMemberApi/UpdateCustAddr")!=-1))
	{
		return kAndroidEditAddress;
	}
	//android提交订单
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPSPOrderApi/OrderSubmit")!=-1))
	{
		return kAndroidOrder;
	}
	return 0;
}

int shopping::MoonBaSa::Process(const char *packet,int action)
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
	//pc提交订单
	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android新增地址
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	//android修改地址
	if (action == kAndroidEditAddress)
	{
		return AndroidEditAddress(packet);
	}
	//android提交订单
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}

//pc登录信息 
int shopping::MoonBaSa::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//用户名
	string username = ms_->GetValueFromMapByKey("txtLoginID");
	//密码
	string password = ms_->GetValueFromMapByKey("txtLoginPwd");

    if (username.empty() || password.empty())
    {
		return 0;
    }
	WriteVirtualIdentity(username,"上线");
	
	output_text_ = 
		"user=" + username + "\r\n" + 
		"pass=" + password + "\r\n" + 
		"state=登录\r\n";	
	WriteIndexFile();
	return 1;
}

//pc新增地址
int shopping::MoonBaSa::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//收货人
	name_=ms_->GetValueFromMapByKey("AccepterName",true);
	//地址
	address_ = ms_->GetValueFromMapByKey("Province",true) + 
		       ms_->GetValueFromMapByKey("City",true) +
			   ms_->GetValueFromMapByKey("District",true) +
			   ms_->GetValueFromMapByKey("Address",true);

	postcode_ =ms_->GetValueFromMapByKey("PostCode",true);
	//联系
	string phone,mobile;
	//手机号
	phone_ =ms_->GetValueFromMapByKey("Mobile",true);
	//邮箱(备用)
	string email=ms_->GetValueFromMapByKey("Email",true);
	//邮编
	postcode_ = ms_->GetValueFromMapByKey("PostCode",true);
	//判断ID
	string id;
	id=ms_->GetValueFromMapByKey("ID",true);
	int n=atoi(id.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" +
			"state=新增地址\r\n";	
		WriteIndexFile();
	}
	else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" +
			"state=修改地址\r\n";		
	   WriteIndexFile();
	}
	return 0;
}

//pc提交订单
int shopping::MoonBaSa::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	user_=ms_->GetValueFromMapByKey("Title",true);
	//地址
	address_ = ms_->GetValueFromMapByKey("invprovince",true) + 
			   ms_->GetValueFromMapByKey("invcity",true) + 
			   ms_->GetValueFromMapByKey("invdistrict",true);
    string ss = src_data;
	if (user_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + user_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=提交订单\r\n";
	WriteIndexFile();
    return 1;
}


//android登录
int shopping::MoonBaSa::AndroidLogin(const char *packet)
{
	string src_data(packet);
	string tookit = "{\"logEntity\"";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		//用户名
		user_ = value["loginName"].asString();
		//密码
		password_ = value["pwd"].asString();
	}
    if (user_.empty() || password_.empty())
    {
		return 0;
    }
	WriteVirtualIdentity(user_,"上线");	
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + password_ + "\r\n" + 
		"state=登录\r\n";	
	WriteIndexFile();
	return 1;
}

//android新增地址
int shopping::MoonBaSa::AndroidAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//json提取数据
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value custAddrEntity = value["custAddrEntity"];
		//姓名
		name_ = custAddrEntity["accepterName"].asString();
		//电话
		phone_ = custAddrEntity["mobile"].asString();
		//地址
		address_ =  custAddrEntity["country"].asString() +
					custAddrEntity["province"].asString() +
					custAddrEntity["city"].asString() +
					custAddrEntity["address"].asString();
		//邮编
		postcode_ = custAddrEntity["postCode"].asString();
	}
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" +
		"state=新增地址\r\n";	
	WriteIndexFile();
	return 0;
}

//android修改地址
int shopping::MoonBaSa::AndroidEditAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//json提取数据
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value custAddrEntity = value["custAddrEntity"];
		//姓名
		name_ = custAddrEntity["accepterName"].asString();
		//电话
		phone_ = custAddrEntity["mobile"].asString();
		//地址
		address_ =  custAddrEntity["country"].asString() +
			custAddrEntity["province"].asString() +
			custAddrEntity["city"].asString() +
			custAddrEntity["address"].asString();
		//邮编
		postcode_ = custAddrEntity["postCode"].asString();
	}
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

//android提交订单
int shopping::MoonBaSa::AndroidOrder(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));

	//json提取数据
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value orderform = value["orderform"];
		//姓名
		name_ = orderform["cusName"].asString();
		//电话
		phone_ = orderform["mobilePhone"].asString();
		Json::Value address = value["address"];
		//地址
		address_ = address["Country"].asString() +
			       address["Province"].asString() +
				   address["City"].asString() + 
				   address["District"].asString();
		//邮编
		postcode_ = address["PostCode"].asString();

	}
	if (name_.empty() || phone_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" +
		"state=提交订单\r\n";
	WriteIndexFile();
    return 1;
}
