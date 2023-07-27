#include "StdAfx.h"
#include "ShoppingMaiHaoHao.h"

shopping::MaiHaoHao::MaiHaoHao(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MaiHaoHao::~MaiHaoHao()
{
	
}

int shopping::MaiHaoHao::IsMaiHaoHao(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/login.html")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/useraddress/create")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/useraddress/update")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find("myhaohao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/create")!=-1))
	{
		return kPCWebNewOrder;
	}	
	//手机登录信息
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/app/checkuser")!=-1))
	{
		return kAndroidLogin;
	}
	//手机提交订单
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/app/ordercreate")!=-1))
	{
		return kAndroidNewOrder;
	}
	return 0;
}

int shopping::MaiHaoHao::Process(const char *packet,int action)
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
	//手机登录信息
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}	
	//手机提交订单
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	return 0;
}

//pc登录信息
int shopping::MaiHaoHao::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueBetweenBeginEnd(src_data,"LoginForm[Email]","&");
	pass = ms_->GetValueFromMapByKey("LoginForm[Password]",true);
	
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
int shopping::MaiHaoHao::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 

	item_end_pos = src_data.find("UserAddress[UserName]",item_beg_pos);
	if (item_end_pos==string::npos)
	{
		//收件人
		name_ = ms_->GetValueFromMapByKey("Consignee",true);
		//地址
		string pro,city,town,address;
		pro = ms_->GetValueFromMapByKey("Province",true);
		city = ms_->GetValueFromMapByKey("City",true);
		town = ms_->GetValueFromMapByKey("District",true);
		address = ms_->GetValueFromMapByKey("Address",true);
		address_ = pro + city + town + address;
		//邮编
		string code;
		code = ms_->GetValueFromMapByKey("Code",true);
		//手机号
		string mobile,phone;
		mobile = ms_->GetValueFromMapByKey("Mobile",true);
		phone = ms_->GetValueFromMapByKey("Tel",true);
		
		
		if ( name_.empty() || phone.empty() || address_.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone + "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + code + "\r\n" +
			"state=添加地址\r\n";
		
	WriteIndexFile();

	}else
	{
		//用户名
		name_ = ms_->GetValueFromMapByKey("UserAddress[UserName]",true);
		//手机
		string phone,mobile;
		phone= ms_->GetValueFromMapByKey("UserAddress[Mobile]",true);
		mobile =ms_->GetValueFromMapByKey("UserAddress[Tel]",true);
		//地址
		string pro,city,town,address;
		pro=ms_->GetValueFromMapByKey("UserAddress[Province]",true);
		city=ms_->GetValueFromMapByKey("UserAddress[City]",true);
		town=ms_->GetValueFromMapByKey("UserAddress[District]",true);
		address=ms_->GetValueFromMapByKey("UserAddress[Address]",true);
		address_ =pro + city + town + address;
		//邮编
		string code;
		code = ms_->GetValueFromMapByKey("UserAddress[Code]",true);
		
		if ( name_.empty() || phone.empty() || address_.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone+ "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msg=" + code + "\r\n" + 
			"state=添加地址\r\n";
		
	WriteIndexFile();

	}



	return 0;
}

//pc修改地址
int shopping::MaiHaoHao::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("UserAddress[UserName]",true);
	//手机
	string phone,mobile;
	phone= ms_->GetValueFromMapByKey("UserAddress[Mobile]",true);
	mobile =ms_->GetValueFromMapByKey("UserAddress[Tel]",true);
	//地址
	string pro,city,town,address;
	pro=ms_->GetValueFromMapByKey("UserAddress[Province]",true);
	city=ms_->GetValueFromMapByKey("UserAddress[City]",true);
	town=ms_->GetValueFromMapByKey("UserAddress[District]",true);
	address=ms_->GetValueFromMapByKey("UserAddress[Address]",true);
	address_ =pro + city + town + address;
	//邮编
	string code;
	code = ms_->GetValueFromMapByKey("UserAddress[Code]",true);

	if ( name_.empty() || phone.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone+ "\r\n" + 
		"clientmsg=" + mobile + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msg=" + code + "\r\n" + 
		"state=修改地址\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc提交订单
int shopping::MaiHaoHao::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	name_ = ms_->GetValueFromMapByKey("Consignee",true);
	//地址
	string pro,city,town,address;
	pro = ms_->GetValueFromMapByKey("Province",true);
	city = ms_->GetValueFromMapByKey("City",true);
	town = ms_->GetValueFromMapByKey("District",true);
	address = ms_->GetValueFromMapByKey("Address",true);
	address_ = pro + city + town + address;
	//手机号
	string phone,mobile;
	phone = ms_->GetValueFromMapByKey("Mobile",true);
	mobile = ms_->GetValueFromMapByKey("Tel",true);
	phone_ =mobile +","+ phone;
	//邮编
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("Zipcode",true);
	if (name_.empty() || address_.empty() || phone_.empty())
	{
		return 0;
	}

	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + zipcode + "\r\n" + 
		"state=提交订单\r\n";
	
	WriteIndexFile();
    return 1;
}



//android登录信息
int shopping::MaiHaoHao::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string user,pass;
	user = ms_->GetValueBetweenBeginEnd(src_data,"name=","&pass",true); 
	pass = ms_->GetValueFromMapByKey("pass",true);
	
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
//android新增地址
int shopping::MaiHaoHao::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android修改地址
int shopping::MaiHaoHao::AndroidModifyAddress(const char *packet)
{
	return 1;
}
//android提交订单
int shopping::MaiHaoHao::AndroidNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人姓名
	name_ = ms_->GetValueFromMapByKey("name",true);
	address_ = ms_->GetValueFromMapByKey("address",true);
	phone_ = ms_->GetValueFromMapByKey("telephone",true);
	if (name_.empty() || address_.empty() || phone_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + name_ + "\r\n" + 
		"oid=" +phone_ + "\r\n" +
		"msg=" + address_ + "\r\n" +
		"state=提交订单\r\n";	
	WriteIndexFile();
	return 1;
}
//android位置信息
int shopping::MaiHaoHao::AndroidLocation(const char *packet)
{
	return 1;
}

