#include "StdAfx.h"
#include "ShoppingMaiHaoHao.h"

shopping::MaiHaoHao::MaiHaoHao(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MaiHaoHao::~MaiHaoHao()
{
	
}

int shopping::MaiHaoHao::IsMaiHaoHao(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/login.html")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/useraddress/create")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/useraddress/update")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("myhaohao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/create")!=-1))
	{
		return kPCWebNewOrder;
	}	
	//�ֻ���¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/app/checkuser")!=-1))
	{
		return kAndroidLogin;
	}
	//�ֻ��ύ����
	if((http_session->m_Requestline.m_Host.Find("www.myhaohao.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/app/ordercreate")!=-1))
	{
		return kAndroidNewOrder;
	}
	return 0;
}

int shopping::MaiHaoHao::Process(const char *packet,int action)
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
	//�ֻ���¼��Ϣ
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}	
	//�ֻ��ύ����
	if (action == kAndroidNewOrder)
	{
		return AndroidNewOrder(packet);
	}
	return 0;
}

//pc��¼��Ϣ
int shopping::MaiHaoHao::PCWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueBetweenBeginEnd(src_data,"LoginForm[Email]","&");
	pass = ms_->GetValueFromMapByKey("LoginForm[Password]",true);
	
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
int shopping::MaiHaoHao::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0; 

	item_end_pos = src_data.find("UserAddress[UserName]",item_beg_pos);
	if (item_end_pos==string::npos)
	{
		//�ռ���
		name_ = ms_->GetValueFromMapByKey("Consignee",true);
		//��ַ
		string pro,city,town,address;
		pro = ms_->GetValueFromMapByKey("Province",true);
		city = ms_->GetValueFromMapByKey("City",true);
		town = ms_->GetValueFromMapByKey("District",true);
		address = ms_->GetValueFromMapByKey("Address",true);
		address_ = pro + city + town + address;
		//�ʱ�
		string code;
		code = ms_->GetValueFromMapByKey("Code",true);
		//�ֻ���
		string mobile,phone;
		mobile = ms_->GetValueFromMapByKey("Mobile",true);
		phone = ms_->GetValueFromMapByKey("Tel",true);
		
		
		if ( name_.empty() || phone.empty() || address_.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone + "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + code + "\r\n" +
			"state=��ӵ�ַ\r\n";
		
	WriteIndexFile();

	}else
	{
		//�û���
		name_ = ms_->GetValueFromMapByKey("UserAddress[UserName]",true);
		//�ֻ�
		string phone,mobile;
		phone= ms_->GetValueFromMapByKey("UserAddress[Mobile]",true);
		mobile =ms_->GetValueFromMapByKey("UserAddress[Tel]",true);
		//��ַ
		string pro,city,town,address;
		pro=ms_->GetValueFromMapByKey("UserAddress[Province]",true);
		city=ms_->GetValueFromMapByKey("UserAddress[City]",true);
		town=ms_->GetValueFromMapByKey("UserAddress[District]",true);
		address=ms_->GetValueFromMapByKey("UserAddress[Address]",true);
		address_ =pro + city + town + address;
		//�ʱ�
		string code;
		code = ms_->GetValueFromMapByKey("UserAddress[Code]",true);
		
		if ( name_.empty() || phone.empty() || address_.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone+ "\r\n" + 
			"clientmsg=" + mobile + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msg=" + code + "\r\n" + 
			"state=��ӵ�ַ\r\n";
		
	WriteIndexFile();

	}



	return 0;
}

//pc�޸ĵ�ַ
int shopping::MaiHaoHao::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	name_ = ms_->GetValueFromMapByKey("UserAddress[UserName]",true);
	//�ֻ�
	string phone,mobile;
	phone= ms_->GetValueFromMapByKey("UserAddress[Mobile]",true);
	mobile =ms_->GetValueFromMapByKey("UserAddress[Tel]",true);
	//��ַ
	string pro,city,town,address;
	pro=ms_->GetValueFromMapByKey("UserAddress[Province]",true);
	city=ms_->GetValueFromMapByKey("UserAddress[City]",true);
	town=ms_->GetValueFromMapByKey("UserAddress[District]",true);
	address=ms_->GetValueFromMapByKey("UserAddress[Address]",true);
	address_ =pro + city + town + address;
	//�ʱ�
	string code;
	code = ms_->GetValueFromMapByKey("UserAddress[Code]",true);

	if ( name_.empty() || phone.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone+ "\r\n" + 
		"clientmsg=" + mobile + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msg=" + code + "\r\n" + 
		"state=�޸ĵ�ַ\r\n";
	
	WriteIndexFile();
	return 0;
}

//pc�ύ����
int shopping::MaiHaoHao::PCWebNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	name_ = ms_->GetValueFromMapByKey("Consignee",true);
	//��ַ
	string pro,city,town,address;
	pro = ms_->GetValueFromMapByKey("Province",true);
	city = ms_->GetValueFromMapByKey("City",true);
	town = ms_->GetValueFromMapByKey("District",true);
	address = ms_->GetValueFromMapByKey("Address",true);
	address_ = pro + city + town + address;
	//�ֻ���
	string phone,mobile;
	phone = ms_->GetValueFromMapByKey("Mobile",true);
	mobile = ms_->GetValueFromMapByKey("Tel",true);
	phone_ =mobile +","+ phone;
	//�ʱ�
	string zipcode;
	zipcode = ms_->GetValueFromMapByKey("Zipcode",true);
	if (name_.empty() || address_.empty() || phone_.empty())
	{
		return 0;
	}

	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + zipcode + "\r\n" + 
		"state=�ύ����\r\n";
	
	WriteIndexFile();
    return 1;
}



//android��¼��Ϣ
int shopping::MaiHaoHao::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string user,pass;
	user = ms_->GetValueBetweenBeginEnd(src_data,"name=","&pass",true); 
	pass = ms_->GetValueFromMapByKey("pass",true);
	
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
//android������ַ
int shopping::MaiHaoHao::AndroidAddAddress(const char *packet)
{
	return 1;
}
//android�޸ĵ�ַ
int shopping::MaiHaoHao::AndroidModifyAddress(const char *packet)
{
	return 1;
}
//android�ύ����
int shopping::MaiHaoHao::AndroidNewOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ�������
	name_ = ms_->GetValueFromMapByKey("name",true);
	address_ = ms_->GetValueFromMapByKey("address",true);
	phone_ = ms_->GetValueFromMapByKey("telephone",true);
	if (name_.empty() || address_.empty() || phone_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + name_ + "\r\n" + 
		"oid=" +phone_ + "\r\n" +
		"msg=" + address_ + "\r\n" +
		"state=�ύ����\r\n";	
	WriteIndexFile();
	return 1;
}
//androidλ����Ϣ
int shopping::MaiHaoHao::AndroidLocation(const char *packet)
{
	return 1;
}

