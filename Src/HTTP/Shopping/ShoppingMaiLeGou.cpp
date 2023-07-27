
#include "StdAfx.h"
#include "ShoppingMaiLeGou.h"

shopping::MaiLeGOu::MaiLeGOu(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MaiLeGOu::~MaiLeGOu()
{
	
}

int shopping::MaiLeGOu::IsMaiLeGOu(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/user/doLogin.do")!=-1))
	{
		return kPCWebLogin;
	}
	//IE pc新增地址 //pc修改地址
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/SaveAddressById_NP.do")!=-1))
	{
		return kPCWebAddAddress;
	}
	//Google pc新增地址 //pc修改地址
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/my/doEditAddress.do")!=-1))
	{
		return kPCWebGoogleAddAddress;
	}
	//pc触屏版 新增地址
	if((http_session->m_Requestline.m_Host.Find("m.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/My/DoEditAddress.html")!=-1))
	{
		return kPCWebChuPingAddAddress;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/SubmitOrder_NP.do")!=-1))
	{
		return kPCWebNewOrder;
	}
	return 0;
}

int shopping::MaiLeGOu::Process(const char *packet,int action)
{
	//pc登录
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	//IE pc新增地址 修改地址
	if (action==kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//Google pc新增地址 修改地址
	if (action==kPCWebGoogleAddAddress)
	{
		return PCWebGoogleAddAddress(packet);
	}
	//pc触屏版新增地址
	if (action==kPCWebChuPingAddAddress)
	{
		return PCWebChuPingAddAddress(packet);
	}
	//pc提交订单
	if (action==kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	return 0;
}

//pc登录
int shopping::MaiLeGOu::PCWebLogin(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	//密码加密(加密方式未知)
	string password = ms_->GetValueFromMapByKey("pwd",true);
	
	if ( user_.empty() || password.empty() )
	{
		return 0;
	}
	output_text_ =  
		"user=" + user_ + "\r\n" + 
		"pass=" + password +  "\r\n" + 
		"state=登录\r\n";

	WriteIndexFile();
	return 1;
}

//pc触屏版新增地址
int shopping::MaiLeGOu::PCWebChuPingAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	name_=ms_->GetValueFromMapByKey("address.TrueName",true);
    //电话
	phone_=ms_->GetValueFromMapByKey("address.HandPhone",true);
	//地址
	string province,city,zone,detailAddress,isEdit;
	province=ms_->GetValueFromMapByKey("pro",true);
	city=ms_->GetValueFromMapByKey("city",true);
	zone=ms_->GetValueFromMapByKey("zone",true);
	detailAddress=ms_->GetValueFromMapByKey("address.DetailAddress",true);
	//从地址编码提取地址名
	addressName_ = GetAddress(zone);
	address_= addressName_ + detailAddress ;
	//用于判断的 条件
	addressid_ = ms_->GetValueFromMapByKey("addressId",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + email_ + "\r\n" +
		"msgtype=" + postcode_ + "\r\n" +
		"state=添加地址\r\n";

	WriteIndexFile();
	return 1;
}

//IE pc新增地址 修改地址
int shopping::MaiLeGOu::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//地址
	string province,city,zone,detailAddress,isEdit;
	province=ms_->GetValueFromMapByKey("pro",true);
	city=ms_->GetValueFromMapByKey("city",true);
	zone=ms_->GetValueFromMapByKey("zone",true);
	detailAddress=ms_->GetValueFromMapByKey("detailAddress",true);
	//从地址编码提取地址名
	addressName_ = GetAddress(zone);
	address_= addressName_ + detailAddress ;
    //电话
	phone_=ms_->GetValueFromMapByKey("handPhone",true);
	//姓名
	name_=ms_->GetValueFromMapByKey("trueName",true);
	//邮编
	postcode_ =ms_->GetValueFromMapByKey("zip",true);
	//用于判断的 条件
	addressid_ = ms_->GetValueFromMapByKey("addressId",true);
	isEdit =ms_->GetValueFromMapByKey("isEdit",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (!addressid_.empty() && isEdit == "true")
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=修改地址\r\n";
    } 
    else if (addressid_.empty() && isEdit == "true")
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=添加地址\r\n";
    }
	WriteIndexFile();
	return 1;
}



//Google pc新增地址 修改地址
int shopping::MaiLeGOu::PCWebGoogleAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//地址
	string province,city,zone,detailAddress;
	province=ms_->GetValueFromMapByKey("pro",true);
	city=ms_->GetValueFromMapByKey("city",true);
	zone=ms_->GetValueFromMapByKey("zone",true);
	detailAddress=ms_->GetValueFromMapByKey("MAddressInfo.DetailAddress",true);
	//从地址编码提取地址名
	addressName_ = GetAddress(zone);
	address_= addressName_ + detailAddress ;
    //电话
	phone_=ms_->GetValueFromMapByKey("MAddressInfo.HandPhone",true);
	//固话
	tel_ = ms_->GetValueFromMapByKey("MAddressInfo.LinkPhone",true);
	//邮箱
    email_ = ms_->GetValueFromMapByKey("MAddressInfo.Email",true);
	//姓名
	name_=ms_->GetValueFromMapByKey("MAddressInfo.TrueName",true);
	//邮编
	postcode_ =ms_->GetValueFromMapByKey("MAddressInfo.Zip",true);
	//用于判断的 条件
	addressid_ = ms_->GetValueFromMapByKey("MAddressInfo.AddressId",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (!addressid_.empty())
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" +
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=修改地址\r\n";
    } 
    else if (addressid_.empty())
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=添加地址\r\n";
    }
	WriteIndexFile();
	return 1;
}

//pc提交订单
int shopping::MaiLeGOu::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
    //姓名
	name_ = ms_->GetValueFromMapByKey("MAddressInfo.TrueName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("MAddressInfo.HandPhone",true);
	//固话
	tel_ = ms_->GetValueFromMapByKey("MAddressInfo.LinkPhone",true);
	//邮箱
	email_ = ms_->GetValueFromMapByKey("MAddressInfo.Email",true);
	//邮编
	postcode_ = ms_->GetValueFromMapByKey("MAddressInfo.Zip",true);
	//城市地址编码
	string provinceCode,cityCode,districtCode,addressDetail;		
	provinceCode = ms_->GetValueFromMapByKey("MAddressInfo.Province",true);
	cityCode = ms_->GetValueFromMapByKey("MAddressInfo.City",true);
	districtCode = ms_->GetValueFromMapByKey("MAddressInfo.Zone",true);
	addressDetail = ms_->GetValueFromMapByKey("MAddressInfo.DetailAddress",true);
	addressid_ = ms_->GetValueFromMapByKey("MAddressInfo.AddressId",true); //备用
    //获取地址
	addressName_ = GetAddress(districtCode);
	address_ = addressName_ + addressDetail;
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + email_ + "\r\n" +
		"msgtype=" + postcode_ + "\r\n" +
		"state=提交订单\r\n";	
	WriteIndexFile();
	return 1;
}
