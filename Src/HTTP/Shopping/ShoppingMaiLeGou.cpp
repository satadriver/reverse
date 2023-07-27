
#include "StdAfx.h"
#include "ShoppingMaiLeGou.h"

shopping::MaiLeGOu::MaiLeGOu(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MaiLeGOu::~MaiLeGOu()
{
	
}

int shopping::MaiLeGOu::IsMaiLeGOu(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/user/doLogin.do")!=-1))
	{
		return kPCWebLogin;
	}
	//IE pc������ַ //pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/SaveAddressById_NP.do")!=-1))
	{
		return kPCWebAddAddress;
	}
	//Google pc������ַ //pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/my/doEditAddress.do")!=-1))
	{
		return kPCWebGoogleAddAddress;
	}
	//pc������ ������ַ
	if((http_session->m_Requestline.m_Host.Find("m.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/My/DoEditAddress.html")!=-1))
	{
		return kPCWebChuPingAddAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.gou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/SubmitOrder_NP.do")!=-1))
	{
		return kPCWebNewOrder;
	}
	return 0;
}

int shopping::MaiLeGOu::Process(const char *packet,int action)
{
	//pc��¼
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	//IE pc������ַ �޸ĵ�ַ
	if (action==kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//Google pc������ַ �޸ĵ�ַ
	if (action==kPCWebGoogleAddAddress)
	{
		return PCWebGoogleAddAddress(packet);
	}
	//pc������������ַ
	if (action==kPCWebChuPingAddAddress)
	{
		return PCWebChuPingAddAddress(packet);
	}
	//pc�ύ����
	if (action==kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}
	return 0;
}

//pc��¼
int shopping::MaiLeGOu::PCWebLogin(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	//�������(���ܷ�ʽδ֪)
	string password = ms_->GetValueFromMapByKey("pwd",true);
	
	if ( user_.empty() || password.empty() )
	{
		return 0;
	}
	output_text_ =  
		"user=" + user_ + "\r\n" + 
		"pass=" + password +  "\r\n" + 
		"state=��¼\r\n";

	WriteIndexFile();
	return 1;
}

//pc������������ַ
int shopping::MaiLeGOu::PCWebChuPingAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	name_=ms_->GetValueFromMapByKey("address.TrueName",true);
    //�绰
	phone_=ms_->GetValueFromMapByKey("address.HandPhone",true);
	//��ַ
	string province,city,zone,detailAddress,isEdit;
	province=ms_->GetValueFromMapByKey("pro",true);
	city=ms_->GetValueFromMapByKey("city",true);
	zone=ms_->GetValueFromMapByKey("zone",true);
	detailAddress=ms_->GetValueFromMapByKey("address.DetailAddress",true);
	//�ӵ�ַ������ȡ��ַ��
	addressName_ = GetAddress(zone);
	address_= addressName_ + detailAddress ;
	//�����жϵ� ����
	addressid_ = ms_->GetValueFromMapByKey("addressId",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"netmark=" + email_ + "\r\n" +
		"msgtype=" + postcode_ + "\r\n" +
		"state=��ӵ�ַ\r\n";

	WriteIndexFile();
	return 1;
}

//IE pc������ַ �޸ĵ�ַ
int shopping::MaiLeGOu::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ַ
	string province,city,zone,detailAddress,isEdit;
	province=ms_->GetValueFromMapByKey("pro",true);
	city=ms_->GetValueFromMapByKey("city",true);
	zone=ms_->GetValueFromMapByKey("zone",true);
	detailAddress=ms_->GetValueFromMapByKey("detailAddress",true);
	//�ӵ�ַ������ȡ��ַ��
	addressName_ = GetAddress(zone);
	address_= addressName_ + detailAddress ;
    //�绰
	phone_=ms_->GetValueFromMapByKey("handPhone",true);
	//����
	name_=ms_->GetValueFromMapByKey("trueName",true);
	//�ʱ�
	postcode_ =ms_->GetValueFromMapByKey("zip",true);
	//�����жϵ� ����
	addressid_ = ms_->GetValueFromMapByKey("addressId",true);
	isEdit =ms_->GetValueFromMapByKey("isEdit",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (!addressid_.empty() && isEdit == "true")
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=�޸ĵ�ַ\r\n";
    } 
    else if (addressid_.empty() && isEdit == "true")
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=��ӵ�ַ\r\n";
    }
	WriteIndexFile();
	return 1;
}



//Google pc������ַ �޸ĵ�ַ
int shopping::MaiLeGOu::PCWebGoogleAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ַ
	string province,city,zone,detailAddress;
	province=ms_->GetValueFromMapByKey("pro",true);
	city=ms_->GetValueFromMapByKey("city",true);
	zone=ms_->GetValueFromMapByKey("zone",true);
	detailAddress=ms_->GetValueFromMapByKey("MAddressInfo.DetailAddress",true);
	//�ӵ�ַ������ȡ��ַ��
	addressName_ = GetAddress(zone);
	address_= addressName_ + detailAddress ;
    //�绰
	phone_=ms_->GetValueFromMapByKey("MAddressInfo.HandPhone",true);
	//�̻�
	tel_ = ms_->GetValueFromMapByKey("MAddressInfo.LinkPhone",true);
	//����
    email_ = ms_->GetValueFromMapByKey("MAddressInfo.Email",true);
	//����
	name_=ms_->GetValueFromMapByKey("MAddressInfo.TrueName",true);
	//�ʱ�
	postcode_ =ms_->GetValueFromMapByKey("MAddressInfo.Zip",true);
	//�����жϵ� ����
	addressid_ = ms_->GetValueFromMapByKey("MAddressInfo.AddressId",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (!addressid_.empty())
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" +
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=�޸ĵ�ַ\r\n";
    } 
    else if (addressid_.empty())
    {
		output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"netmark=" + email_ + "\r\n" +
			"msgtype=" + postcode_ + "\r\n" +
			"state=��ӵ�ַ\r\n";
    }
	WriteIndexFile();
	return 1;
}

//pc�ύ����
int shopping::MaiLeGOu::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
    //����
	name_ = ms_->GetValueFromMapByKey("MAddressInfo.TrueName",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("MAddressInfo.HandPhone",true);
	//�̻�
	tel_ = ms_->GetValueFromMapByKey("MAddressInfo.LinkPhone",true);
	//����
	email_ = ms_->GetValueFromMapByKey("MAddressInfo.Email",true);
	//�ʱ�
	postcode_ = ms_->GetValueFromMapByKey("MAddressInfo.Zip",true);
	//���е�ַ����
	string provinceCode,cityCode,districtCode,addressDetail;		
	provinceCode = ms_->GetValueFromMapByKey("MAddressInfo.Province",true);
	cityCode = ms_->GetValueFromMapByKey("MAddressInfo.City",true);
	districtCode = ms_->GetValueFromMapByKey("MAddressInfo.Zone",true);
	addressDetail = ms_->GetValueFromMapByKey("MAddressInfo.DetailAddress",true);
	addressid_ = ms_->GetValueFromMapByKey("MAddressInfo.AddressId",true); //����
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
		"msgtype=" + postcode_ + "\r\n" +
		"state=�ύ����\r\n";	
	WriteIndexFile();
	return 1;
}
