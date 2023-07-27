#include "StdAfx.h"
#include "ShoppingHaiLanZhiJia.h"

shopping::HaiLanZhiJia::HaiLanZhiJia(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::HaiLanZhiJia::~HaiLanZhiJia()
{
	
}

int shopping::HaiLanZhiJia::IsHaiLanZhiJia(HttpSession *http_session)
{
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.hlamall.cn")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/logincenter/ajax/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ �޸ĵ�ַ	(������Ʒʱ)
	if((http_session->m_Requestline.m_Host.Find("www.hlamall.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/Index/ajaxOrderAddress")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc������ַ �޸ĵ�ַ2	(��������-��ַ����)
	if((http_session->m_Requestline.m_Host.Find("www.hlamall.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/order/UpdateOrderAddr")!=-1))
	{
		return kPCWebModiyAdd;
	}
	//�ֻ��˵�¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("m.heilanhome.cn")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/logincenter/appindex/actLogin")!=-1))
	{
		return kMobileLogin;
	}
	//�ֻ���������ַ�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("m.heilanhome.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/appindex/UpdateOrderAddress")!=-1))
	{
		return KMobileAddress;
	}
	if((http_session->m_Requestline.m_Host.Find("m.heilanhome.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/apporder/hfUpdateOrderAddr")!=-1))
	{
		return kMobileModiy;
	}
	return 0;
}

int shopping::HaiLanZhiJia::Process(const char *packet,int action)
{
	//pc��¼��Ϣ
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc������ַ �޸ĵ�ַ 1
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//pc������ַ �޸ĵ�ַ 2
	if (action ==kPCWebModiyAdd)
	{
		return PCWebModiyAdd(packet);
	}
	//�ֻ��˵�¼��Ϣ
	if (action == kMobileLogin)
	{
		return MobileLogin(packet);
	}
	//pc�������޸���Ϣ
	if (action == KMobileAddress)
	{
		return MobileAddress(packet);
	}
	if (action==kMobileModiy)
	{
		return MobileModiy(packet);
	}
	return 0;
}

//pc��¼��Ϣ
int shopping::HaiLanZhiJia::PCWebLogin(const char *packet)
{
	string src_data(packet);
	string tookit = "name=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	string pass;
	user_=ms_->GetValueFromMapByKey("name",true);
	pass=ms_->GetValueFromMapByKey("password",true);

	if (user_.empty() || pass.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user_,"����");
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

//pc������ַ�޸ĵ�ַ
int shopping::HaiLanZhiJia::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//�ջ���
	name_=ms_->GetValueFromMapByKey("receiver",true);
	//��ַ
    address_ = ms_->GetValueFromMapByKey("province",true) + "," +
			   ms_->GetValueFromMapByKey("city",true) + "," +
			   ms_->GetValueFromMapByKey("district",true) + "," +
			   ms_->GetValueFromMapByKey("address",true);
	//�ֻ���
	phone_=ms_->GetValueFromMapByKey("mobile",true);

	//�ж�ID
	string aid;
	aid=ms_->GetValueFromMapByKey("aid",true);
	int n=atoi(aid.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=������ַ\r\n";		
		WriteIndexFile();
	}
	else
	{

		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";	
		WriteIndexFile();
	}
	return 0;
}

int shopping::HaiLanZhiJia::PCWebModiyAdd(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�ջ���
	name_=ms_->GetValueFromMapByKey("r",true);
	//��ַ
	address_ = ms_->GetValueFromMapByKey("p",true) + "," +
			   ms_->GetValueFromMapByKey("c",true) + "," +
			   ms_->GetValueFromMapByKey("d",true) + "," +
			   ms_->GetValueFromMapByKey("addr",true);
	//�ֻ���
	phone_=ms_->GetValueFromMapByKey("m",true);
	
	//�ж�ID
	string aid;
	aid=ms_->GetValueFromMapByKey("id",true);
	int n=atoi(aid.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=������ַ\r\n";
		
		WriteIndexFile();
		
	}else{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";
		
		WriteIndexFile();
		
	}
	return 0;
}

//�ֻ���¼
int shopping::HaiLanZhiJia::MobileLogin(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string pass;
	user_=ms_->GetValueFromMapByKey("name",true);
	pass=ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || pass.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}
//�ֻ�������ַ�޸ĵ�ַ
int shopping::HaiLanZhiJia::MobileAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ռ���
	name_=ms_->GetValueFromMapByKey("receiver",true);
	//�ֻ���
	phone_=ms_->GetValueFromMapByKey("mobile",true);
	//��ַ
	address_= ms_->GetValueFromMapByKey("inp_province_ident",true) +
		      ms_->GetValueFromMapByKey("inp_city_ident",true) +
			  ms_->GetValueFromMapByKey("inp_district_ident",true) +
			  ms_->GetValueFromMapByKey("address",true);
	//�ж�ID ������ַ�����޸ĵ�ַ
	string id;
	id=ms_->GetValueFromMapByKey("id",true);
	int n=atoi(id.c_str());
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=��ӵ�ַ\r\n";
		
	WriteIndexFile();
	}
	else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";
		
	WriteIndexFile();
	}
	return 0;
}

int shopping::HaiLanZhiJia::MobileModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	src_data = src_data.substr(src_data.rfind("id="),src_data.length() - src_data.rfind("id="));
	
	ms_->SplitStringToMap(src_data,'&','=');
	//�ջ���
	name_=ms_->GetValueFromMapByKey("r",true);
	//��ַ
	address_=  ms_->GetValueFromMapByKey("p",true) + 
		       ms_->GetValueFromMapByKey("c",true) +
			   ms_->GetValueFromMapByKey("d",true) +
			   ms_->GetValueFromMapByKey("addr",true);

	//�ֻ���
	phone_=ms_->GetValueFromMapByKey("m",true);
	
	//�ж�ID
	string aid;
	aid=ms_->GetValueFromMapByKey("id",true);
	int n=atoi(aid.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=��ӵ�ַ\r\n";
		
		WriteIndexFile();
		
	}else{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";
		
		WriteIndexFile();
		
	}
	return 0;
}