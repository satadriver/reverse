
#include "StdAfx.h"

#include "ShoppingDangDang.h"

shopping::DangDang::DangDang(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::DangDang::~DangDang()
{
	
}

int shopping::DangDang::IsDangDang(HttpSession *http_session)
{
	//pc��������� ������ַ �޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("checkoutb.dangdang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/web/consignee/submit")!=-1))
	{
		return kPcWebAddAddress;
	}
	//pc��ַ������ ������ַ �޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("myaddress.dangdang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myaddress_handle.php")!=-1))
	{
		return kPcWebModifyAddress;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("user.mapi.dangdang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("action=login_user")))
	{
		return kAndroidLogin;
	}
	//��ַ����(������ַ �޸ĵ�ַ)
	if((http_session->m_Requestline.m_Host.Find("mcheckout.dangdang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?trace_id")!=-1))
	{
		return kAndroidAddAddress;
	}
	return 0;
}

int shopping::DangDang::Process(const char *packet,int action)
{
	if (action == kPcWebAddAddress)
	{
		return PcWebAddAddress(packet);
	}
	if (action == kPcWebModifyAddress)
	{
		return PcWebModifyAddress(packet);
	}
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	return 0;
}

int shopping::DangDang::PcWebAddAddress(const char *packet)
{
    string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,"&","=");
    //����
	name_ = ms_->GetValueFromMapByKey("ship_name",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("ship_mb",true);
	//��ַ
	address_ = ms_->GetValueFromMapByKey("country_id",true) + "," +
			   ms_->GetValueFromMapByKey("province_id",true) + "," +
			   ms_->GetValueFromMapByKey("city_id",true) + "," +
			   ms_->GetValueFromMapByKey("town_id",true) + "," +
			   ms_->GetValueFromMapByKey("ship_address",true);
	//�ʱ�
	postcode_ = ms_->GetValueFromMapByKey("ship_zip",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("addr_id",true);
	if (name_.empty() || address_.empty())
	{
		return 0;
	}
	if ("0" == addressid_)
	{
		output_text_ = 
			"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
			"oid=" + 		phone_ + "\r\n" + 	//�绰
			"msg=" + 		address_ + "\r\n" + //�ջ���ַ
			"msgtype=" + 	postcode_ + "\r\n" +//�ʱ�
			"state=������ַ\r\n";		
		WriteIndexFile();
	}
	else
	{
		output_text_ = 
			"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
			"oid=" + 		phone_ + "\r\n" + 	//�绰
			"msg=" + 		address_ + "\r\n" + //�ջ���ַ
			"msgtype=" + 	postcode_ + "\r\n" +//�ʱ�
			"state=�޸ĵ�ַ\r\n";		
		WriteIndexFile();
	}
	return 0;
}


int shopping::DangDang::PcWebModifyAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,"&","=");
    //����
	name_ = ms_->GetValueFromMapByKey("ship_man",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("ship_mb",true);
	//��ַ
	address_ = ms_->GetValueFromMapByKey("country_id",true) + "," +
		ms_->GetValueFromMapByKey("province_id",true) + "," +
		ms_->GetValueFromMapByKey("city_id",true) + "," +
		ms_->GetValueFromMapByKey("town_id",true) + "," +
		ms_->GetValueFromMapByKey("addr_detail",true);
	//�ʱ�
	postcode_ = ms_->GetValueFromMapByKey("ship_zip",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("cust_address_id",true);
	if (name_.empty() || address_.empty())
	{
		return 0;
	}
	if ("0" == addressid_)
	{
		output_text_ = 
			"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
			"oid=" + 		phone_ + "\r\n" + 	//�绰
			"msg=" + 		address_ + "\r\n" + //�ջ���ַ
			"msgtype=" + 	postcode_ + "\r\n" +//�ʱ�
			"state=������ַ\r\n";		
		WriteIndexFile();
	}
	else
	{
		output_text_ = 
			"nick="	 + 		name_ + "\r\n" + 	//�ǳ�
			"oid=" + 		phone_ + "\r\n" + 	//�绰
			"msg=" + 		address_ + "\r\n" + //�ջ���ַ
			"msgtype=" + 	postcode_ + "\r\n" +//�ʱ�
			"state=�޸ĵ�ַ\r\n";		
		WriteIndexFile();
	}
	return 0;
}

int shopping::DangDang::AndroidLogin(const char *packet)
{
	string src_data(packet);
	//������
    string tookit = "sword=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	password_= ms_->GetValueFromMapByKey("sword");
	//����(����)
	longitude_ = ms_->GetValueFromMapByKey("x");
	//γ��(����)
	latitude_ = ms_->GetValueFromMapByKey("y");
	//IMEI(����)
	imei_ = ms_->GetValueFromMapByKey("imei");
	//mac(����)
	mac_ = ms_->GetValueFromMapByKey("mac");
	if ( user_.empty() || password_.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user_,"����");	
	output_text_ =  
		"user=" +		user_ + "\r\n" + 
		"pass=" +		password_ + "\r\n" + 
		"from=" + 	  	longitude_ + "\r\n" + 			
		"to=" + 	  	latitude_ + "\r\n" + 			
		"fileurl=" +	imei_ + "\r\n" + 		
		"clientmac=" +	mac_ + "\r\n" + 		
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

int shopping::DangDang::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
    src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//��������
	string action = ms_->GetValueFromMapByKey("action");
	if (action == "add_address")
	{
		//�ռ���
		name_ = ms_->GetValueFromMapByKey("receiver_name");
		//�ֻ�
		phone_ = ms_->GetValueFromMapByKey("mobile");
		//�̻�
		tel_ = ms_->GetValueFromMapByKey("tel");
		//��ַ(��ַ��+��ϸ��Ϣ)
		address_ = ms_->GetValueFromMapByKey("province_id") + "," +
			       ms_->GetValueFromMapByKey("city_id") + "," +
				   ms_->GetValueFromMapByKey("region_id") + "," +
				   ms_->GetValueFromMapByKey("quarter_id") + "," +
				   ms_->GetValueFromMapByKey("address_detail");

		//�ʱ�
		postcode_ = ms_->GetValueFromMapByKey("postcode");
		//IMEI
		imei_ = ms_->GetValueFromMapByKey("imei");
		//IMSI
		imsi_ = ms_->GetValueFromMapByKey("imsi");
		//MAC
		mac_ = ms_->GetValueFromMapByKey("macaddress");

		if ( name_.empty() || address_.empty())
		{
			return 0;
		}
		output_text_ =  
			"nick=" +		name_ + "\r\n" + 
			"oid=" +		phone_ + "\r\n" + 
			"onick=" + 		tel_ + "\r\n" + 		//�̻�
			"msg=" +		address_ + "\r\n" + 
			"msgtype=" +	postcode_ + "\r\n" +
			"imsi=" + 		imsi_ + "\r\n" + 		//imsi
			"fileurl=" +	imei_ + "\r\n" + 		//imei
			"clientmac=" + 	mac_ + "\r\n" + 		//mac
			"state=������ַ\r\n";
		WriteIndexFile();	
	}
	else if (action == "modify_address") 
	{
		//�ռ���
		name_ = ms_->GetValueFromMapByKey("receiver_name");
		//�ֻ�
		phone_ = ms_->GetValueFromMapByKey("mobile");
		//�̻�
		tel_ = ms_->GetValueFromMapByKey("tel");
		//��ַ(��ַ��+��ϸ��Ϣ)
		address_ = ms_->GetValueFromMapByKey("province_id") + "," +
			       ms_->GetValueFromMapByKey("city_id") + "," +
				   ms_->GetValueFromMapByKey("region_id") + "," +
				   ms_->GetValueFromMapByKey("quarter_id") + "," +
				   ms_->GetValueFromMapByKey("address_detail");

		//�ʱ�
		postcode_ = ms_->GetValueFromMapByKey("postcode");
		//IMEI
		imei_ = ms_->GetValueFromMapByKey("imei");
		//IMSI
		imsi_ = ms_->GetValueFromMapByKey("imsi");
		//MAC
		mac_ = ms_->GetValueFromMapByKey("macaddress");

		if ( name_.empty() || address_.empty())
		{
			return 0;
		}
		output_text_ =  
			"nick=" +		name_ + "\r\n" + 
			"oid=" +		phone_ + "\r\n" + 
			"onick=" + 		tel_ + "\r\n" + 		
			"msg=" +		address_ + "\r\n" + 
			"msgtype=" +	postcode_ + "\r\n" +
			"imsi=" + 		imsi_ + "\r\n" + 		//imsi
			"fileurl=" +	imei_ + "\r\n" + 		//imei
			"clientmac=" + 	mac_ + "\r\n" + 		//mac
			"state=�޸ĵ�ַ\r\n";
		WriteIndexFile();		
	}
    else if (action ==  "list_invoice_type")
    {
		name_ = ms_->GetValueFromMapByKey("visual_gift_order[ship_name]");
		phone_ = ms_->GetValueFromMapByKey("visual_gift_order[register_mb]");
		tel_ = ms_->GetValueFromMapByKey("visual_gift_order[ship_tel_plain]");
		address_ = ms_->GetValueFromMapByKey("visual_gift_order[country_name]") + 
				   ms_->GetValueFromMapByKey("visual_gift_order[province_name]") +
				   ms_->GetValueFromMapByKey("visual_gift_order[city_name]") + 
				   ms_->GetValueFromMapByKey("visual_gift_order[town_name]") + 
				   ms_->GetValueFromMapByKey("visual_gift_order[quarter_name]") + 
				   ms_->GetValueFromMapByKey("visual_gift_order[ship_address_plain]");
		postcode_ = ms_->GetValueFromMapByKey("visual_gift_order[ship_zip]");
		if (name_.empty() || phone_.empty() || address_.empty())
		{
			return 0;
		}
		output_text_ =  
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" +
			"clientmsg=" + tel_ + "\r\n" + 
			"state=���߷�Ʊ\r\n";
		WriteIndexFile();
    }
	return 1;
}


