#include "StdAfx.h"
#include "ShoppingMiZhe.h"

shopping::MiZhe::MiZhe(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MiZhe::~MiZhe()
{
	
}

int shopping::MiZhe::IsMiZhe(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("d.mizhe.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/member/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ
	if((http_session->m_Requestline.m_Host.Find("trade.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/add_address")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("trade.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/update_address")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc����
	if((http_session->m_Requestline.m_Host.Find("dt.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/add_address")!=-1))
	{
		return kPCWebNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("dt.mizhe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/update_address")!=-1))
	{
		return kAndroidModifyAddress;
	}
	//android��¼��Ϣ
	//android������ַ
	//android�޸ĵ�ַ
	//android�ύ����
	//androidλ����Ϣ
	return 0;
}

int shopping::MiZhe::Process(const char *packet,int action)
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

	if (action ==kAndroidModifyAddress)
	{
		return AndroidModifyAddress(packet);
	}
	//android��¼��Ϣ
	//android������ַ
	//android�޸ĵ�ַ
	//android�ύ����
	//androidλ����Ϣ
	return 0;
}

//pc��¼��Ϣ
int shopping::MiZhe::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("username",true);
	pass = ms_->GetValueFromMapByKey("passwd",true);
	
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
int shopping::MiZhe::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//��ַ
	string pro,city,area,address;
	pro = ms_->GetValueFromMapByKey("province_text",true);
	city = ms_->GetValueFromMapByKey("city_text",true);
	area = ms_->GetValueFromMapByKey("area_text",true);
	address = ms_->GetValueFromMapByKey("address",true);
	address_ =pro + city + area + address;
	//�绰
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//�ʱ�
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + code + "\r\n" + 
		"state=��ӵ�ַ\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc�޸ĵ�ַ
int shopping::MiZhe::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//��ַ
	string pro,city,area,address;
	pro = ms_->GetValueFromMapByKey("province_text",true);
	city = ms_->GetValueFromMapByKey("city_text",true);
	area = ms_->GetValueFromMapByKey("area_text",true);
	address = ms_->GetValueFromMapByKey("address",true);
	address_ =pro + city + area + address;
	//�绰
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//�ʱ�
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	AfxMessageBox(name_.c_str());
	AfxMessageBox(address_.c_str());
	AfxMessageBox(phone_.c_str());
	if (name_.empty() || address_.empty() || phone_.empty())
	{
		return 0;
	}

	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=�޸ĵ�ַ\r\n";
	
	WriteIndexFile();
	return 1;
}

//pc����
int shopping::MiZhe::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//��ַ
	string area,address;
	address=ms_->GetValueFromMapByKey("address",true);
	area = ms_->GetValueFromMapByKey("area",true);
		area=GetAddress(area);
	address_ =area + address;
	//�绰
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//�ʱ�
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + code + "\r\n" + 
		"state=��ӵ�ַ\r\n";
	
	WriteIndexFile();
	return 0;
}


//android��¼��Ϣ
int shopping::MiZhe::AndroidLogin(const char *packet)
{
	return 1;
}
//android������ַ
int shopping::MiZhe::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android�޸ĵ�ַ
int shopping::MiZhe::AndroidModifyAddress(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	name_=ms_->GetValueFromMapByKey("fullname",true);
	//��ַ
	string area,address;
	address=ms_->GetValueFromMapByKey("address",true);
	area = ms_->GetValueFromMapByKey("area",true);
	area=GetAddress(area);
	address_ =area + address;
	//�绰
	phone_= ms_->GetValueFromMapByKey("mobile",true);
	//�ʱ�
	string code;
	code = ms_->GetValueFromMapByKey("zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + code + "\r\n" + 
		"state=�޸ĵ�ַ\r\n";
	
	WriteIndexFile();
	return 0;
	return 1;
}
//android�ύ����
int shopping::MiZhe::AndroidNewOrder(const char *packet)
{
	return 1;
}
//androidλ����Ϣ
int shopping::MiZhe::AndroidLocation(const char *packet)
{
	return 1;
}

