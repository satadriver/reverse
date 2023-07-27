
#include "StdAfx.h"

#include "ShoppingYouGou.h"


shopping::YouGou::YouGou(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::YouGou::~YouGou()
{
	
}

int shopping::YouGou::IsYouGou(HttpSession *http_session)
{
	//pc���� �޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.yougou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/my/updateAddressBookAjax.jhtml")!=-1))
	{
		return kPcAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.yougou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/createOrder")!=-1))
	{
		return kPCcreateOrder;
	}
	//android��¼
	if((http_session->m_Requestline.m_Host.Find("mobile.yougou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login")!=-1))
	{
		return kMobileLogin;
	}
	//android������ַ
	if((http_session->m_Requestline.m_Host.Find("mobile.yougou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("addaddress")!=-1))
	{
		return KMobileAddAddress;
	}
	//android�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("mobile.yougou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("editaddress")!=-1))
	{
		return kMobileEditAddress;
	}
	return 0;
}

int shopping::YouGou::Process(const char *packet,int action)
{
	if (action == kPcAddress)
	{
		return PcAddress(packet);
	}
	if (action==kPCcreateOrder)
	{
		return PCcreateOrder(packet);
	}
	if (action==kMobileLogin)
	{
		return MobileLogin(packet);
	}
	if (action==kMobileEditAddress)
	{
		return MobileEditAddress(packet);
	}
	if (action==KMobileAddAddress)
	{
		return MobileAddAddress(packet);
	}
	return 0;
}
//pc���� �޸ĵ�ַ
int shopping::YouGou::PcAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	if (src_data.rfind("receivingName=") != -1)
	{
		src_data = src_data.substr(src_data.rfind("receivingName="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_=ms_->GetValueFromMapByKey("receivingName",true);
	//�绰
	phone_=ms_->GetValueFromMapByKey("receivingMobilePhone",true);
    //��ַ
	address_= ms_->GetValueFromMapByKey("receivingProvince",true) + "," +
				ms_->GetValueFromMapByKey("receivingCity",true) + "," +
				ms_->GetValueFromMapByKey("receivingDistrict",true) + "," +
				ms_->GetValueFromMapByKey("receivingAddress",true);
	//�ʱ�
	postcode_=ms_->GetValueFromMapByKey("receivingZipCode",true);
	//��ַid
	addressid_=ms_->GetValueFromMapByKey("addressId",true);
	if (name_.empty() || address_.empty())
	{
		return 0;
	}
	if (addressid_.length() >0 )
	{
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" 
			"state=�޸ĵ�ַ\r\n";
		WriteIndexFile();
	}
	else
	{
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" 
			"state=������ַ\r\n";
		WriteIndexFile();
	}
	return 1;
}
//�ύ����
int shopping::YouGou::PCcreateOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
    //����
	name_=ms_->GetValueFromMapByKey("receivingName",true);
	//�绰
	phone_=ms_->GetValueFromMapByKey("receivingMobilePhone",true);
	//��ַ
	string pro,city,country;
	pro=ms_->GetValueFromMapByKey("receivingProvinceName",true);
	city=ms_->GetValueFromMapByKey("receivingCityName",true);
	country=ms_->GetValueFromMapByKey("receivingDistrictName",true);
	address_= pro + city + country;
	//�ʱ�
	postcode_=ms_->GetValueFromMapByKey("receivingZipCode",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" +
		"state=�ύ����\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int shopping::YouGou::MobileLogin(const char *packet)
{
	string password;
	string src_data(packet);
	if (src_data.rfind("password=") != -1)
	{
		src_data = src_data.substr(src_data.rfind("password="));
	}
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	user_= ms_->GetValueFromMapByKey("username");
	//���ܷ�ʽ(md532λСд)
	password= ms_->GetValueFromMapByKey("password");

	if (user_.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" +		user_ + "\r\n" +
		"password=" +	password + "\r\n" + 
		"state=�û���¼\r\n";
	WriteIndexFile();
	return 1;
}
//�޸ĵ�ַ
int shopping::YouGou::MobileEditAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	if (src_data.rfind("area=") != -1)
	{
		src_data = src_data.substr(src_data.rfind("area="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_=ms_->GetValueFromMapByKey("name",true);
	//�绰
	phone_=ms_->GetValueFromMapByKey("mobilephone",true);
    //��ַ
	address_= ms_->GetValueFromMapByKey("area",true) + "," +
				ms_->GetValueFromMapByKey("province",true) + "," +
				ms_->GetValueFromMapByKey("city",true) + "," +
				ms_->GetValueFromMapByKey("detail",true);
	//�ʱ�
	postcode_=ms_->GetValueFromMapByKey("zipcode",true);
	//��ַid
	addressid_=ms_->GetValueFromMapByKey("id",true);
	if (name_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" 
		"state=�޸ĵ�ַ\r\n";
	WriteIndexFile();
	return 1;
}
//��ӵ�ַ
int shopping::YouGou::MobileAddAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	if (src_data.rfind("area=") != -1)
	{
		src_data = src_data.substr(src_data.rfind("area="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_=ms_->GetValueFromMapByKey("name",true);
	//�绰
	phone_=ms_->GetValueBetweenBeginEnd(src_data,"mobilephone=","&");	
    //��ַ
	address_= ms_->GetValueFromMapByKey("area",true) + "," +
				ms_->GetValueFromMapByKey("province",true) + "," +
				ms_->GetValueFromMapByKey("city",true) + "," +
				ms_->GetValueFromMapByKey("detail",true);
	//�ʱ�
	postcode_=ms_->GetValueFromMapByKey("zipcode",true);	
	if (name_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" 
		"state=��ӵ�ַ\r\n";
	WriteIndexFile();
	return 1;
}