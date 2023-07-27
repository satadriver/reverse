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
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/account/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/address/add/")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("lashou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/address/modify/")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc�ύ����
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

	//android��¼��Ϣ
	//android������ַ
	//android�޸ĵ�ַ
	//android�ύ����
	//androidλ����Ϣ
	return 0;
}

int shopping::LaShouWang::Process(const char *packet,int action)
{
	//pc��¼��Ϣ
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc������ַ
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//pc�޸ĵ�ַ
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	//pc�ύ����
	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	if (action==kPCWebLocation)
	{
		return PCWebLocation(packet);
	}
	//android��¼��Ϣ
	//android������ַ
	//android�޸ĵ�ַ
	//android�ύ����
	//androidλ����Ϣ
	return 0;
}

//pc��¼��Ϣ
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
	WriteVirtualIdentity(user,"����");
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

//pc������ַ
int shopping::LaShouWang::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//��ȡ��ַ
	string areacode,address;
	areacode=ms_->GetValueFromMapByKey("town",true);
	address=ms_->GetValueFromMapByKey("address",true);
	areacode=GetAddress(areacode);
	address_=areacode + address;
	//�ֻ���
	phone_=ms_->GetValueFromMapByKey("mobile",true);
	//�ʱ�
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("zipcode",true);
	//�ռ���
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
		"state=��ӵ�ַ\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc�޸ĵ�ַ
int shopping::LaShouWang::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("username",true);
	//��ַ
	string areacode,address;
	areacode = ms_->GetValueFromMapByKey("areacode",true);
	address = ms_->GetValueFromMapByKey("address",true);
	areacode = GetAddress(areacode);
	address_ = areacode + address;
	//�ʱ�
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("zipcode",true);
	//�ֻ���
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
		"state=�޸ĵ�ַ\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc�ύ����
int shopping::LaShouWang::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_ = ms_->GetValueFromMapByKey("sendUser",true);
	//��ַ
	address_ = ms_->GetValueFromMapByKey("street",true);
	//�ֻ���
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//�ʱ�
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
		"state=�ύ����\r\n";
	
	WriteIndexFile();
    return 1;
}
int shopping::LaShouWang::PCWebLocation(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueBetweenBeginEnd(src_data,"username","&");
	//��ַ
	string areacode,address;
	areacode = ms_->GetValueFromMapByKey("town",true);
	address = ms_->GetValueFromMapByKey("address",true);
	areacode = GetAddress(areacode);
	address_ = areacode + address;
	//�ʱ�
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("code",true);
	//�ֻ���
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
		"state=������ַ\r\n";
	}else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + zipcode + "\r\n" +
		"state=�޸ĵ�ַ\r\n";
	}

	
	WriteIndexFile();
	return 0;
}
//android��¼��Ϣ
int shopping::LaShouWang::AndroidLogin(const char *packet)
{
	return 1;
}
//android������ַ
int shopping::LaShouWang::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android�޸ĵ�ַ
int shopping::LaShouWang::AndroidModifyAddress(const char *packet)
{
	return 1;
}
//android�ύ����
int shopping::LaShouWang::AndroidNewOrder(const char *packet)
{
	return 1;
}
//androidλ����Ϣ
int shopping::LaShouWang::AndroidLocation(const char *packet)
{
	return 1;
}

