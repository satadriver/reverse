#include "StdAfx.h"
#include "ShoppingMoGuJie.h"

shopping::MoGuJie::MoGuJie(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MoGuJie::~MoGuJie()
{
	
}

int shopping::MoGuJie::IsMoGuJie(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("portal.mogujie.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/api/login/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ
	if((http_session->m_Requestline.m_Host.Find(".mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/addReceiveAddressActionlet/1")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find(".mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/modifyReceiveAddressActionlet/1")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find(".mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/createOrderActionlet/1")!=-1))
	{
		return kPCWebNewOrder;
	}
	//pcλ����Ϣ
	if((http_session->m_Requestline.m_Host.Find("portal.mogujie.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/validate/loginconfig")!=-1))
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

int shopping::MoGuJie::Process(const char *packet,int action)
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
	//pcλ����Ϣ
	if (action == kPCWebLocation)
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
int shopping::MoGuJie::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("uname",true);
	//�������
	pass = ms_->GetValueFromMapByKey("password");
	
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
int shopping::MoGuJie::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("province",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("address",true);
	//�ʱ�
    postcode_ = ms_->GetValueFromMapByKey("zip",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" + 
		"state=��ӵ�ַ\r\n";	
	WriteIndexFile();
	return 0;
}

//pc�޸ĵ�ַ
int shopping::MoGuJie::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//��ַ
	address_ = ms_->GetValueFromMapByKey("province",true) + 
			   ms_->GetValueFromMapByKey("city",true) +
			   ms_->GetValueFromMapByKey("area",true) +
		       ms_->GetValueFromMapByKey("address",true);	
	//�ʱ�
    postcode_ = ms_->GetValueFromMapByKey("zip",true);
    //��ַid
	addressid_ = ms_->GetValueFromMapByKey("addressId",true);	//��ʱû��
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

//pc�ύ����
int shopping::MoGuJie::PCWebNewOrder(const char *packet)
{
	//��Ϣ��ȫ
    return 0;
}

//pcλ����Ϣ
int shopping::MoGuJie::PCWebLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string uname,areaCode;
	uname = ms_->GetValueFromMapByKey("uname");
	areaCode = ms_->GetValueFromMapByKey("areaCode");

	if ( uname.empty() || areaCode.empty())
	{
		return 0;
	}
	
	output_text_ = "uname=" + uname + "\r\n" + 
		"areaCode=" + areaCode + "\r\n" + 
		"state=��λ\r\n";
	
	//	WriteIndexFile();
	return 1;
}


//android��¼��Ϣ
int shopping::MoGuJie::AndroidLogin(const char *packet)
{
	return 1;
}
//android������ַ
int shopping::MoGuJie::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android�޸ĵ�ַ
int shopping::MoGuJie::AndroidModifyAddress(const char *packet)
{
	return 1;
}
//android�ύ����
int shopping::MoGuJie::AndroidNewOrder(const char *packet)
{
	return 1;
}
//androidλ����Ϣ
int shopping::MoGuJie::AndroidLocation(const char *packet)
{
	return 1;
}

