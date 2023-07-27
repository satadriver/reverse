#include "StdAfx.h"

#include "ShoppingKaoLaHaiGou.h"

shopping::KaoLaHaiGou::KaoLaHaiGou(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::KaoLaHaiGou::~KaoLaHaiGou()
{
	
}

int shopping::KaoLaHaiGou::IsKaoLaHaiGou(HttpSession *http_session)
{
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find("www.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personal/my_address/add.html")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find("www.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personal/my_address/update.html")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find("www.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/book.html")!=-1))
	{
		return kPCWebNewOrder;
	}
	//android新增地址 //android修改地址 通过id 来判断
	if((http_session->m_Requestline.m_Host.Find("sp.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/address")!=-1))
	{
		return kAndroidAddAddress;
	}  
	//android提交订单
	if((http_session->m_Requestline.m_Host.Find("sp.kaola.com") !=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/order") !=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/order/preview") ==-1))
	{
		return kAndroidNewOrder;
	}
	return 0;
}

int shopping::KaoLaHaiGou::Process(const char *packet,int action)
{
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
	//android新增地址 //android修改地址 通过id 来判断
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	//android提交订单
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	return 0;
}

//pc新增地址
int shopping::KaoLaHaiGou::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	name_ = ms_->GetValueFromMapByKey("name",true);	
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//固定电话
	tel_ =		ms_->GetValueFromMapByKey("phoneAreaNum",true) + "-" + 
				ms_->GetValueFromMapByKey("phoneNum",true) + "-" + 
 				ms_->GetValueFromMapByKey("phoneExtNum",true);
	//地址
	address_ =  ms_->GetValueFromMapByKey("provinceName",true) + 
				ms_->GetValueFromMapByKey("cityName",true) +
				ms_->GetValueFromMapByKey("districtName",true) +
				ms_->GetValueFromMapByKey("address",true);	
	//邮箱
    email_ = ms_->GetValueFromMapByKey("email",true);
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
		"state=添加地址\r\n";
	WriteIndexFile();
	return 0;
}


//pc修改地址
int shopping::KaoLaHaiGou::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	name_ = ms_->GetValueFromMapByKey("name",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//固定电话
	tel_ =	ms_->GetValueFromMapByKey("phoneAreaNum",true) + "-" + 
			ms_->GetValueFromMapByKey("phoneNum",true) + "-" + 
			ms_->GetValueFromMapByKey("phoneExtNum",true);
	//地址
	address_ = ms_->GetValueFromMapByKey("provinceName",true) + 
			ms_->GetValueFromMapByKey("cityName",true) +
			ms_->GetValueFromMapByKey("districtName",true) +
			ms_->GetValueFromMapByKey("address",true);
		
	//邮箱
    email_ = ms_->GetValueFromMapByKey("email",true);
	//地址id
	addressid_ = ms_->GetValueFromMapByKey("id",true); //备用	
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
		"state=修改地址\r\n";
	WriteIndexFile();
	return 0;
}
//pc提交订单
int shopping::KaoLaHaiGou::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
    //姓名
	name_ = ms_->GetValueFromMapByKey("address.name",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("address.mobile",true);
	//固话
	tel_ =	ms_->GetValueFromMapByKey("address.areaCode",true) + "-" + 
			ms_->GetValueFromMapByKey("address.tel",true) + "-" + 
			ms_->GetValueFromMapByKey("address.ext",true);
	//邮箱
	email_ =  ms_->GetValueFromMapByKey("address.email",true) ;	
	//城市地址编码
	string provinceCode,cityCode,districtCode,addressDetail;		
	provinceCode = ms_->GetValueFromMapByKey("address.provinceCode",true);
	cityCode = ms_->GetValueFromMapByKey("address.cityCode",true);
	districtCode = ms_->GetValueFromMapByKey("address.districtCode",true);
	addressDetail = ms_->GetValueFromMapByKey("address.addressDetail",true);
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
		"state=提交订单\r\n";
	
	WriteIndexFile();
	return 1;
}

//android新增地址 //android修改地址 通过id 来判断
int shopping::KaoLaHaiGou::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    //使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	//判断是添加还是更改
	string keyId;
	if (reader.parse(src_data,value))
	{	
		keyId = value["id"].asString();
		//城市地址编码 待转换 
		string provinceCode,cityCode,districtCode,address;
		provinceCode = value["provinceCode"].asString();
		cityCode = value["cityCode"].asString();
		districtCode = value["districtCode"].asString();
		address = value["address"].asString();
		//获取地址
		addressName_ = GetAddress(districtCode);
		address_ = addressName_ + address;

		name_ = value["name"].asString();
		phone_ = value["mobile"].asString();		
	}

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (!keyId.empty())
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" +  
			"msg=" + address_ + "\r\n" +
			"state=修改地址\r\n";
	}
	else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" +  
			"msg=" + address_ + "\r\n" +
			"state=添加地址\r\n";
	}
	WriteIndexFile();
	return 1;
}

//android提交订单
int shopping::KaoLaHaiGou::AndroidNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		Json::Value orderForm = value["orderForm"];
		Json::Value addressEnd = orderForm["address"];
		//城市地址编码 待转换 
		string provinceCode,cityCode,districtCode,addressDetail;		
		provinceCode = addressEnd["provinceCode"].asString();
		cityCode = addressEnd["cityCode"].asString();
		districtCode = addressEnd["districtCode"].asString();
		addressDetail = addressEnd["addressDetail"].asString();
		//获取地址
		addressName_ = GetAddress(districtCode);
		address_ = addressName_ + addressDetail;
		//姓名
		name_ = addressEnd["name"].asString();
		//电话
		phone_ = addressEnd["mobile"].asString();	
	}	
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
