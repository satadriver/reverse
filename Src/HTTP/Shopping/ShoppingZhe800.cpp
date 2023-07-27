#include "StdAfx.h"
#include "ShoppingZhe800.h"

shopping::Zhe800::Zhe800(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::Zhe800::~Zhe800()
{
	
}

int shopping::Zhe800::IsZhe800(HttpSession *http_session)
{
	//pc������ַ
	if((http_session->m_Requestline.m_Host.Find("shop.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/nnc/users/addresses/add")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("shop.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/nnc/users/addresses/update_address")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//android������ַ
	if((http_session->m_Requestline.m_Host.Find("tbuy.m.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/address/add")!=-1))
	{
		return kAndroidAddAddress;
	}
	//android�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("tbuy.m.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/address/edit")!=-1))
	{
		return kAndroidModifyAddress;
	}
	//android�ύ����
	if((http_session->m_Requestline.m_Host.Find("th5.m.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/orders/h5/create")!=-1))
	{
		return kAndroidNewOrder;
	}
	return 0;
}

int shopping::Zhe800::Process(const char *packet,int action)
{
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
	//android������ַ
	if (action ==kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	//android�޸ĵ�ַ
	if (action == kAndroidModifyAddress)
	{
		return AndroidModifyAddress(packet);
	}
	//android�ύ����
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	return 0;
}

//pc������ַ
int shopping::Zhe800::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("receiverName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	email_ = ms_->GetValueFromMapByKey("email");
	//�̻�
	tel_ = ms_->GetValueFromMapByKey("telCode") + "-" +
		   ms_->GetValueFromMapByKey("tel") + "-" +
		   ms_->GetValueFromMapByKey("telExtNumber");		  
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("provinceName",true) + 
				ms_->GetValueFromMapByKey("cityName",true) +
				ms_->GetValueFromMapByKey("countyName",true) +
				ms_->GetValueFromMapByKey("address",true);
	//��ַcode
	string addressCode_ = ms_->GetValueFromMapByKey("provinceId",true) + 
				  ms_->GetValueFromMapByKey("cityId",true) + 
			      ms_->GetValueFromMapByKey("countyId",true);

	//�ʱ�
    postcode_ = ms_->GetValueFromMapByKey("postCode",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + email_ + "\r\n" +
		"msgtype=" + postcode_ + "\r\n" +
		"state=��ӵ�ַ\r\n";	
	WriteIndexFile();
	return 0;
}

//pc�޸ĵ�ַ
int shopping::Zhe800::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("receiverName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	email_ = ms_->GetValueFromMapByKey("email");
	//�̻�
	tel_ = ms_->GetValueFromMapByKey("telCode") + "-" +
			ms_->GetValueFromMapByKey("tel") + "-" +
			ms_->GetValueFromMapByKey("telExtNumber");
		  
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("provinceName",true) + 
				ms_->GetValueFromMapByKey("cityName",true) +
				ms_->GetValueFromMapByKey("countyName",true) +
				ms_->GetValueFromMapByKey("address",true);
	//��ַcode
	string addressCode_ = ms_->GetValueFromMapByKey("provinceId",true) + 
				  ms_->GetValueFromMapByKey("cityId",true) + 
			      ms_->GetValueFromMapByKey("countyId",true);
	//�ʱ�
    postcode_ = ms_->GetValueFromMapByKey("postCode",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("id",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + email_ + "\r\n" +
		"msgtype=" + postcode_ + "\r\n" +
		"state=�޸ĵ�ַ\r\n";	
	WriteIndexFile();
	return 0;
}

//android������ַ
int shopping::Zhe800::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	//�̻�
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("province_name",true) + 
		ms_->GetValueFromMapByKey("city_name",true) +
		ms_->GetValueFromMapByKey("county_name",true) +
		ms_->GetValueFromMapByKey("address",true);
	//��ַcode
	string addressCode_ = ms_->GetValueFromMapByKey("province_id",true) + 
		ms_->GetValueFromMapByKey("city_id",true) + 
		ms_->GetValueFromMapByKey("county_id",true);
	
	//�ʱ�
    postcode_ = ms_->GetValueFromMapByKey("post_code",true);
	
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
//android�޸ĵ�ַ
int shopping::Zhe800::AndroidModifyAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//email
	//�̻�
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("province_name",true) + 
		ms_->GetValueFromMapByKey("city_name",true) +
		ms_->GetValueFromMapByKey("county_name",true) +
		ms_->GetValueFromMapByKey("address",true);
	//��ַcode
	string addressCode_ = ms_->GetValueFromMapByKey("province_id",true) + 
		ms_->GetValueFromMapByKey("city_id",true) + 
		ms_->GetValueFromMapByKey("county_id",true);
	
	//�ʱ�
    postcode_ = ms_->GetValueFromMapByKey("post_code",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("id",true);
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
int shopping::Zhe800::AndroidNewOrder(const char *packet)
{		
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	//��ر���
	string product_name,cur_price,amount,count,imagesUrl;
	//��Ʒ����
	product_name = ms_->GetValueFromMapByKey("product_name",true);
	//�ܼ۸� (��λ�� ��)
	amount = ms_->GetValueFromMapByKey("amount",true);
	//����
	count = ms_->GetValueFromMapByKey("count",true);
	//ǰ�� �� ��ַ http://z2.tuanimg.com/imagev2/trade/
	//ͼƬ��ַ
	imagesUrl = ms_->GetValueFromMapByKey("imagesUrl",true);
	//�û���
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("receiver_mobile");
	//�����ַ
	address_ = ms_->GetValueFromMapByKey("receiver_address",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("address_id",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + product_name + "\r\n" +
		"clientmsg=" + count + "\r\n" +
		"msgtype=" + amount + "\r\n" +
		"mid=" + imagesUrl + "\r\n" + 
		"state=�ύ����\r\n";	
	WriteIndexFile();
	return 1;
}
