#include "StdAfx.h"
#include "ShoppingLeTao.h"

shopping::LeTao::LeTao(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::LeTao::~LeTao()
{
	
}

int shopping::LeTao::IsLeTao(HttpSession *http_session)
{
	//pc��¼
	if((http_session->m_Requestline.m_Host.Find("www.letao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("doLogin.do")!=-1))
	{
		return kPCLogin;
	}
	//pc������ַ �޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.letao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("saveAddress.do")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.letao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("submitOrder.do")!=-1))
	{
		return kPCWebOrder;
	}
	return 0;
}

int shopping::LeTao::Process(const char *packet,int action)
{
	//pc��¼
	if (action == kPCLogin)
	{
		return PCLogin(packet);
	}
	//pc������ַ �޸ĵ�ַ
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//pc�ύ����
	if (action == kPCWebOrder)
	{
		return PCWebOrder(packet);
	}
	return 0;
}
//pc��¼
int shopping::LeTao::PCLogin(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	string tookit = "userId=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	ms_->SplitStringToMap(src_data,"&","=");
	
    user_ = ms_->GetValueFromMapByKey("userId");
	password_ = ms_->GetValueFromMapByKey("pwd");
	
	if ( user_.empty() || password_.empty())
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

//pc������ַ
int shopping::LeTao::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	string tookit = "id=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
    ms_->SplitStringToMap(src_data,"&","=");
    //����
	name_ = ms_->GetValueFromMapByKey("consignee");
	//�绰
	phone_ = ms_->GetValueFromMapByKey("phone");
	//��ַ
	address_ = ms_->GetValueFromMapByKey("address") + 
		       ms_->GetValueFromMapByKey("street");
    //��ַid
	addressid_ = ms_->GetValueFromMapByKey("id");
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (addressid_.empty())
	{
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=������ַ\r\n";
		WriteIndexFile();
	}
	else
	{
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";
		WriteIndexFile();
	}
	return 1;
}
//pc�ύ����
int shopping::LeTao::PCWebOrder(const char *packet)
{
	string src_data(packet);
	string tookit = "aid=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
    ms_->SplitStringToMap(src_data,"&","=");
    //����
	name_ = ms_->GetValueFromMapByKey("consignee");
	//�绰
	phone_ = ms_->GetValueFromMapByKey("consigneePhone");
	//��ַ
	address_ = ms_->GetValueFromMapByKey("deliveryAddress");
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=�ύ����\r\n";
	WriteIndexFile();
	return 0;
}


