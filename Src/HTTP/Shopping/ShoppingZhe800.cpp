#include "StdAfx.h"
#include "ShoppingZhe800.h"

shopping::Zhe800::Zhe800(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::Zhe800::~Zhe800()
{
	
}

int shopping::Zhe800::IsZhe800(HttpSession *http_session)
{
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find("shop.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/nnc/users/addresses/add")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find("shop.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/nnc/users/addresses/update_address")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//android新增地址
	if((http_session->m_Requestline.m_Host.Find("tbuy.m.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/address/add")!=-1))
	{
		return kAndroidAddAddress;
	}
	//android修改地址
	if((http_session->m_Requestline.m_Host.Find("tbuy.m.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/address/edit")!=-1))
	{
		return kAndroidModifyAddress;
	}
	//android提交订单
	if((http_session->m_Requestline.m_Host.Find("th5.m.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/orders/h5/create")!=-1))
	{
		return kAndroidNewOrder;
	}
	return 0;
}

int shopping::Zhe800::Process(const char *packet,int action)
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
	//android新增地址
	if (action ==kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	//android修改地址
	if (action == kAndroidModifyAddress)
	{
		return AndroidModifyAddress(packet);
	}
	//android提交订单
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	return 0;
}

//pc新增地址
int shopping::Zhe800::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("receiverName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	email_ = ms_->GetValueFromMapByKey("email");
	//固话
	tel_ = ms_->GetValueFromMapByKey("telCode") + "-" +
		   ms_->GetValueFromMapByKey("tel") + "-" +
		   ms_->GetValueFromMapByKey("telExtNumber");		  
	//地址
	address_ =  ms_->GetValueFromMapByKey("provinceName",true) + 
				ms_->GetValueFromMapByKey("cityName",true) +
				ms_->GetValueFromMapByKey("countyName",true) +
				ms_->GetValueFromMapByKey("address",true);
	//地址code
	string addressCode_ = ms_->GetValueFromMapByKey("provinceId",true) + 
				  ms_->GetValueFromMapByKey("cityId",true) + 
			      ms_->GetValueFromMapByKey("countyId",true);

	//邮编
    postcode_ = ms_->GetValueFromMapByKey("postCode",true);

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
		"state=添加地址\r\n";	
	WriteIndexFile();
	return 0;
}

//pc修改地址
int shopping::Zhe800::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("receiverName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	email_ = ms_->GetValueFromMapByKey("email");
	//固话
	tel_ = ms_->GetValueFromMapByKey("telCode") + "-" +
			ms_->GetValueFromMapByKey("tel") + "-" +
			ms_->GetValueFromMapByKey("telExtNumber");
		  
	//地址
	address_ =  ms_->GetValueFromMapByKey("provinceName",true) + 
				ms_->GetValueFromMapByKey("cityName",true) +
				ms_->GetValueFromMapByKey("countyName",true) +
				ms_->GetValueFromMapByKey("address",true);
	//地址code
	string addressCode_ = ms_->GetValueFromMapByKey("provinceId",true) + 
				  ms_->GetValueFromMapByKey("cityId",true) + 
			      ms_->GetValueFromMapByKey("countyId",true);
	//邮编
    postcode_ = ms_->GetValueFromMapByKey("postCode",true);
	//地址id
	addressid_ = ms_->GetValueFromMapByKey("id",true);

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
		"state=修改地址\r\n";	
	WriteIndexFile();
	return 0;
}

//android新增地址
int shopping::Zhe800::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	//固话
	//地址
	address_ =  ms_->GetValueFromMapByKey("province_name",true) + 
		ms_->GetValueFromMapByKey("city_name",true) +
		ms_->GetValueFromMapByKey("county_name",true) +
		ms_->GetValueFromMapByKey("address",true);
	//地址code
	string addressCode_ = ms_->GetValueFromMapByKey("province_id",true) + 
		ms_->GetValueFromMapByKey("city_id",true) + 
		ms_->GetValueFromMapByKey("county_id",true);
	
	//邮编
    postcode_ = ms_->GetValueFromMapByKey("post_code",true);
	
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
//android修改地址
int shopping::Zhe800::AndroidModifyAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户名
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	//固话
	//地址
	address_ =  ms_->GetValueFromMapByKey("province_name",true) + 
		ms_->GetValueFromMapByKey("city_name",true) +
		ms_->GetValueFromMapByKey("county_name",true) +
		ms_->GetValueFromMapByKey("address",true);
	//地址code
	string addressCode_ = ms_->GetValueFromMapByKey("province_id",true) + 
		ms_->GetValueFromMapByKey("city_id",true) + 
		ms_->GetValueFromMapByKey("county_id",true);
	
	//邮编
    postcode_ = ms_->GetValueFromMapByKey("post_code",true);
	//地址id
	addressid_ = ms_->GetValueFromMapByKey("id",true);
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
int shopping::Zhe800::AndroidNewOrder(const char *packet)
{		
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	//相关变量
	string product_name,cur_price,amount,count,imagesUrl;
	//物品名称
	product_name = ms_->GetValueFromMapByKey("product_name",true);
	//总价格 (单位是 分)
	amount = ms_->GetValueFromMapByKey("amount",true);
	//数量
	count = ms_->GetValueFromMapByKey("count",true);
	//前面 的 地址 http://z2.tuanimg.com/imagev2/trade/
	//图片地址
	imagesUrl = ms_->GetValueFromMapByKey("imagesUrl",true);
	//用户名
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("receiver_mobile");
	//具体地址
	address_ = ms_->GetValueFromMapByKey("receiver_address",true);
	//地址id
	addressid_ = ms_->GetValueFromMapByKey("address_id",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + product_name + "\r\n" +
		"clientmsg=" + count + "\r\n" +
		"msgtype=" + amount + "\r\n" +
		"mid=" + imagesUrl + "\r\n" + 
		"state=提交订单\r\n";	
	WriteIndexFile();
	return 1;
}
