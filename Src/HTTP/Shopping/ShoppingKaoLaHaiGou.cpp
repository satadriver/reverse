#include "StdAfx.h"

#include "ShoppingKaoLaHaiGou.h"

shopping::KaoLaHaiGou::KaoLaHaiGou(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::KaoLaHaiGou::~KaoLaHaiGou()
{
	
}

int shopping::KaoLaHaiGou::IsKaoLaHaiGou(HttpSession *http_session)
{
	//pc������ַ
	if((http_session->m_Requestline.m_Host.Find("www.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personal/my_address/add.html")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personal/my_address/update.html")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/book.html")!=-1))
	{
		return kPCWebNewOrder;
	}
	//android������ַ //android�޸ĵ�ַ ͨ��id ���ж�
	if((http_session->m_Requestline.m_Host.Find("sp.kaola.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/address")!=-1))
	{
		return kAndroidAddAddress;
	}  
	//android�ύ����
	if((http_session->m_Requestline.m_Host.Find("sp.kaola.com") !=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/order") !=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/order/preview") ==-1))
	{
		return kAndroidNewOrder;
	}
	return 0;
}

int shopping::KaoLaHaiGou::Process(const char *packet,int action)
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
    //pc�ύ����
	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	//android������ַ //android�޸ĵ�ַ ͨ��id ���ж�
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	//android�ύ����
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	return 0;
}

//pc������ַ
int shopping::KaoLaHaiGou::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_ = ms_->GetValueFromMapByKey("name",true);	
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//�̶��绰
	tel_ =		ms_->GetValueFromMapByKey("phoneAreaNum",true) + "-" + 
				ms_->GetValueFromMapByKey("phoneNum",true) + "-" + 
 				ms_->GetValueFromMapByKey("phoneExtNum",true);
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("provinceName",true) + 
				ms_->GetValueFromMapByKey("cityName",true) +
				ms_->GetValueFromMapByKey("districtName",true) +
				ms_->GetValueFromMapByKey("address",true);	
	//����
    email_ = ms_->GetValueFromMapByKey("email",true);
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
		"state=��ӵ�ַ\r\n";
	WriteIndexFile();
	return 0;
}


//pc�޸ĵ�ַ
int shopping::KaoLaHaiGou::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_ = ms_->GetValueFromMapByKey("name",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//�̶��绰
	tel_ =	ms_->GetValueFromMapByKey("phoneAreaNum",true) + "-" + 
			ms_->GetValueFromMapByKey("phoneNum",true) + "-" + 
			ms_->GetValueFromMapByKey("phoneExtNum",true);
	//��ַ
	address_ = ms_->GetValueFromMapByKey("provinceName",true) + 
			ms_->GetValueFromMapByKey("cityName",true) +
			ms_->GetValueFromMapByKey("districtName",true) +
			ms_->GetValueFromMapByKey("address",true);
		
	//����
    email_ = ms_->GetValueFromMapByKey("email",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("id",true); //����	
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
		"state=�޸ĵ�ַ\r\n";
	WriteIndexFile();
	return 0;
}
//pc�ύ����
int shopping::KaoLaHaiGou::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
    //����
	name_ = ms_->GetValueFromMapByKey("address.name",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("address.mobile",true);
	//�̻�
	tel_ =	ms_->GetValueFromMapByKey("address.areaCode",true) + "-" + 
			ms_->GetValueFromMapByKey("address.tel",true) + "-" + 
			ms_->GetValueFromMapByKey("address.ext",true);
	//����
	email_ =  ms_->GetValueFromMapByKey("address.email",true) ;	
	//���е�ַ����
	string provinceCode,cityCode,districtCode,addressDetail;		
	provinceCode = ms_->GetValueFromMapByKey("address.provinceCode",true);
	cityCode = ms_->GetValueFromMapByKey("address.cityCode",true);
	districtCode = ms_->GetValueFromMapByKey("address.districtCode",true);
	addressDetail = ms_->GetValueFromMapByKey("address.addressDetail",true);
    //��ȡ��ַ
	addressName_ = GetAddress(districtCode);
	address_ = addressName_ + addressDetail;
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
		"state=�ύ����\r\n";
	
	WriteIndexFile();
	return 1;
}

//android������ַ //android�޸ĵ�ַ ͨ��id ���ж�
int shopping::KaoLaHaiGou::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    //ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	//�ж�����ӻ��Ǹ���
	string keyId;
	if (reader.parse(src_data,value))
	{	
		keyId = value["id"].asString();
		//���е�ַ���� ��ת�� 
		string provinceCode,cityCode,districtCode,address;
		provinceCode = value["provinceCode"].asString();
		cityCode = value["cityCode"].asString();
		districtCode = value["districtCode"].asString();
		address = value["address"].asString();
		//��ȡ��ַ
		addressName_ = GetAddress(districtCode);
		address_ = addressName_ + address;

		name_ = value["name"].asString();
		phone_ = value["mobile"].asString();		
	}

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (!keyId.empty())
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" +  
			"msg=" + address_ + "\r\n" +
			"state=�޸ĵ�ַ\r\n";
	}
	else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" +  
			"msg=" + address_ + "\r\n" +
			"state=��ӵ�ַ\r\n";
	}
	WriteIndexFile();
	return 1;
}

//android�ύ����
int shopping::KaoLaHaiGou::AndroidNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		Json::Value orderForm = value["orderForm"];
		Json::Value addressEnd = orderForm["address"];
		//���е�ַ���� ��ת�� 
		string provinceCode,cityCode,districtCode,addressDetail;		
		provinceCode = addressEnd["provinceCode"].asString();
		cityCode = addressEnd["cityCode"].asString();
		districtCode = addressEnd["districtCode"].asString();
		addressDetail = addressEnd["addressDetail"].asString();
		//��ȡ��ַ
		addressName_ = GetAddress(districtCode);
		address_ = addressName_ + addressDetail;
		//����
		name_ = addressEnd["name"].asString();
		//�绰
		phone_ = addressEnd["mobile"].asString();	
	}	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=�ύ����\r\n";	
	WriteIndexFile();
	return 1;
}
