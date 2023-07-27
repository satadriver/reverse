#include "StdAfx.h"

#include "ShoppingGuoMei.h"

shopping::GuoMei::GuoMei(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::GuoMei::~GuoMei()
{
	
}

int shopping::GuoMei::IsGuoMei(HttpSession *http_session)
{
	//��������� ������ַ
	if((http_session->m_Requestline.m_Host.Find("cart.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/home/api/consignee/addAddress")!=-1))
	{
		return kPCWebAddAddress;
	}
	//��������� �޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("cart.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/home/api/consignee/updateAddress")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//��ַ������ ������ַ (reverse���� Get����ȡ����)
	if((http_session->m_Requestline.m_Host.Find("member.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myaccount/address/insertAddress")!=-1))
	{
		return kPCWebAccountInsertAddress;
	}
	//��ַ������ �޸ĵ�ַ (reverse���� Get����ȡ����)
	if((http_session->m_Requestline.m_Host.Find("member.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myaccount/address/updateAddress")!=-1))
	{
		return kPCWebAccountUpdateAddress;
	}
	return 0;
}

int shopping::GuoMei::Process(const char *packet,int action)
{
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	if (action == kPCWebAccountInsertAddress)
	{
		return PCWebAccountInsertAddress(packet);
	}
	if (action == kPCWebAccountUpdateAddress)
	{
		return PCWebAccountUpdateAddress(packet);
	}
	return 0;
}

int shopping::GuoMei::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//����
	name_ = ms_->GetValueFromMapByKey("name");
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//��ַ
	address_ = ms_->GetValueFromMapByKey("province") + "," +
			   ms_->GetValueFromMapByKey("city") + "," +
			   ms_->GetValueFromMapByKey("country") + "," +
			   ms_->GetValueFromMapByKey("town") + "," +
			   ms_->GetValueFromMapByKey("address");
	//����
	email_ = ms_->GetValueFromMapByKey("email");
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
		"oid=" + 		phone_ + "\r\n" + 	//�绰
		"msg=" + 		address_ + "\r\n" + //�ջ���ַ
		"appname=" +	email_ + "\r\n" + 	//����
		"state=������ַ\r\n";		
	WriteIndexFile();
	return 1;
}

int shopping::GuoMei::PCWebModifyAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//����
	name_ = ms_->GetValueFromMapByKey("name");
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//��ַ
	address_ = ms_->GetValueFromMapByKey("province") + "," +
		ms_->GetValueFromMapByKey("city") + "," +
		ms_->GetValueFromMapByKey("country") + "," +
		ms_->GetValueFromMapByKey("town") + "," +
		ms_->GetValueFromMapByKey("address");
	//����
	email_ = ms_->GetValueFromMapByKey("email");
	
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
		"oid=" + 		phone_ + "\r\n" + 	//�绰
		"msg=" + 		address_ + "\r\n" + //�ջ���ַ
		"appname=" +	email_ + "\r\n" + 	//����
		"state=�޸ĵ�ַ\r\n";	
	WriteIndexFile();
	return 1;
}

int shopping::GuoMei::PCWebAccountInsertAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,"&","=");
	//����
	name_ = ms_->GetValueFromMapByKey("firstName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
    //��ַ
	address_ = ms_->GetValueFromMapByKey("state",true) + "," +
			   ms_->GetValueFromMapByKey("city",true) + "," +
			   ms_->GetValueFromMapByKey("county",true) + "," +
			   ms_->GetValueFromMapByKey("town",true) + "," +
			   ms_->GetValueFromMapByKey("address",true);
    //����
	email_ = ms_->GetValueFromMapByKey("email",true);

	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
		"oid=" + 		phone_ + "\r\n" + 	//�绰
		"msg=" + 		address_ + "\r\n" + //�ջ���ַ
		"appname=" +	email_ + "\r\n" + 	//����
	"state=������ַ\r\n";
	WriteIndexFile();
	return 1;
}

int shopping::GuoMei::PCWebAccountUpdateAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,"&","=");
	//����
	name_ = ms_->GetValueFromMapByKey("firstName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
    //��ַ
	address_ = ms_->GetValueFromMapByKey("state",true) + "," +
		ms_->GetValueFromMapByKey("city",true) + "," +
		ms_->GetValueFromMapByKey("county",true) + "," +
		ms_->GetValueFromMapByKey("town",true) + "," +
		ms_->GetValueFromMapByKey("address",true);
    //����
	email_ = ms_->GetValueFromMapByKey("email",true);
	
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
		"oid=" + 		phone_ + "\r\n" + 	//�绰
		"msg=" + 		address_ + "\r\n" + //�ջ���ַ
		"appname=" +	email_ + "\r\n" + 	//����
		"state=�޸ĵ�ַ\r\n";
	WriteIndexFile();
	return 1;
}