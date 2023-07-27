
#include "StdAfx.h"

#include "ShoppingZhenPin.h"

shopping::ZhenPin::ZhenPin(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::ZhenPin::~ZhenPin()
{
	
}

int shopping::ZhenPin::IsZhenPin(HttpSession *http_session)
{
	//�ֻ�������ַ
	if((http_session->m_Requestline.m_Host.Find("uc.zhen.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/uc/myzhenpin/addAddress")!=-1))
	{
		return kMobileAddAddress;
	}

	if ((http_session->m_Requestline.m_Host.Find("uc.zhen.com")!=-1)&&
		 (http_session->m_Requestline.m_URI.Find("/uc/myzhenpin/updateAddress")!=-1))
	{
		return kMobileModiyAddress;
	}
    //�ֻ���¼
	if((http_session->m_Requestline.m_Host.Find("ppt.zhen.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ppt/access/onLogin")!=-1))
	{
		return kMoblieLogin;
	}

	return 0;
}

int shopping::ZhenPin::Process(const char *packet,int action)
{
	
	if (action == kMobileAddAddress)
	{
		return MobileAddAddress(packet);
	}
	if (action == kMoblieLogin)
	{
		return MoblieLogin(packet);
	}
	if (action==kMobileModiyAddress)
	{
		return MobileModiyAddress(packet);
	}
	return 0;
}
//�ֻ���������ַ
int shopping::ZhenPin::MobileAddAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//������
	name_=ms_->GetValueFromMapByKey("receiver",true);
	//�ֻ��� 
	string mobile,phone;
	phone=ms_->GetValueFromMapByKey("phone",true);
	mobile=ms_->GetValueFromMapByKey("mobile",true);
	//��ַ
	string pro,city,town,postcode,address;
	pro=ms_->GetValueFromMapByKey("provinceRegionId",true);
	city=ms_->GetValueFromMapByKey("cityRegionId",true);
	town=ms_->GetValueFromMapByKey("townRegionId",true);
	postcode=ms_->GetValueFromMapByKey("postcode",true);
	address=ms_->GetValueFromMapByKey("address",true);
	address_=pro+","+city+","+town+","+address;

	if (address_.empty() || name_.empty() || phone.empty())
	{
		return 0;
	}
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + mobile + "\r\n" + 
			"clientmsg=" + phone + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode + "\r\n" +
			"state=��ӵ�ַ\r\n";
    }
	WriteIndexFile();
	return 1;
}
//�ֻ����޸ĵ�ַ
int shopping::ZhenPin::MobileModiyAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	//������
	name_=ms_->GetValueFromMapByKey("receiver",true);
	//�ֻ���
	string mobile,phone;
	phone=ms_->GetValueFromMapByKey("phone",true);
	mobile=ms_->GetValueFromMapByKey("mobile",true);
	//��ַ
	string pro,city,town,postcode,address;
	pro=ms_->GetValueFromMapByKey("provinceRegionId",true);
	city=ms_->GetValueFromMapByKey("cityRegionId",true);
	town=ms_->GetValueFromMapByKey("townRegionId",true);
	postcode=ms_->GetValueFromMapByKey("postcode",true);
	address=ms_->GetValueFromMapByKey("address",true);
	address_=pro+","+city+","+town+","+address;
	
	if (address_.empty() || name_.empty() || phone.empty())
	{
		return 0;
	}
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone + "\r\n" +
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode + "\r\n" +
			"state=�޸ĵ�ַ\r\n";
    }
	WriteIndexFile();
	return 1;

}
int shopping::ZhenPin::MoblieLogin(const char *packet)
{

	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string user,pass;
	user=ms_->GetValueFromMapByKey("username",true);
	pass=ms_->GetValueFromMapByKey("password",true);
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


