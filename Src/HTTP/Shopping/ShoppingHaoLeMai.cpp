#include "StdAfx.h"
#include "ShoppingHaoLeMai.h"

shopping::HaoLeMai::HaoLeMai(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::HaoLeMai::~HaoLeMai()
{
	
}

int shopping::HaoLeMai::IsHaoLeMai(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.okbuy.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/members/login/ajax_login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ �޸ĵ�ַ (���������)
	if((http_session->m_Requestline.m_Host.Find("www.okbuy.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/order/saveDstAddress/")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc������ַ �޸ĵ�ַ (��ַ����)
	if((http_session->m_Requestline.m_Host.Find("www.okbuy.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/memberaddress/save")!=-1))
	{
		return kPCWebModifyAddress;
	}
	return 0;
}

int shopping::HaoLeMai::Process(const char *packet,int action)
{
	//pc��¼��Ϣ
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	return 0;
}

//pc��¼��Ϣ
int shopping::HaoLeMai::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string user,pass;
	user = ms_->GetValueFromMapByKey("username",true);
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

//pc������ַ �޸ĵ�ַ(���������)
int shopping::HaoLeMai::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	string tookit = "addressid=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_ = ms_->GetValueFromMapByKey("truename",true);
	//��ַ(��ַ��)
	address_ = ms_->GetValueFromMapByKey("prov",true) + "," +
				ms_->GetValueFromMapByKey("city",true) + "," +
				ms_->GetValueFromMapByKey("area",true) + "," +
				ms_->GetValueFromMapByKey("address",true);
	//�ʱ�
	postcode_ = ms_->GetValueFromMapByKey("zipcode",true);
	//�ֻ�
	string pretel,tel,phone;
	phone_ = ms_->GetValueFromMapByKey("cell",true);

	addressid_ = ms_->GetValueFromMapByKey("addressid",true);
    
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (atoi(addressid_.c_str()) == 0)
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

int shopping::HaoLeMai::PCWebModifyAddress(const char *packet)
{
	string src_data(packet);
	string tookit = "addressid=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_ = ms_->GetValueFromMapByKey("truename",true);
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("prov",true) + "," +
				ms_->GetValueFromMapByKey("city",true) + "," +
				ms_->GetValueFromMapByKey("area",true) + "," +
				ms_->GetValueFromMapByKey("address",true);
	//�ʱ�
	postcode_ = ms_->GetValueFromMapByKey("zipcode",true);
	//�ֻ�
	phone_ = ms_->GetValueFromMapByKey("cell",true);

	addressid_ = ms_->GetValueFromMapByKey("addressid",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (addressid_.empty())
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

