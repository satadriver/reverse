#include "StdAfx.h"
#include "HotelPodinns.h"

hotel::HotelPodinns::HotelPodinns(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "�����Ƶ�";
}

hotel::HotelPodinns::~HotelPodinns()
{

}

hotel::HotelPodinns::IsHotelPodinns(HttpSession *http_session)
{
	//webע��
	if( ( http_session->m_Requestline.m_Host.Find("www.podinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Account/RegPost") != -1 ))	
	{
		return kWebRegister;
	}
	//web��¼
	if( ( http_session->m_Requestline.m_Host.Find("www.podinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Account/LogOn") != -1 ))	
	{
		return kWebLogin;
	}
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("www.podinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/HotelBook/EnterBookInfo") != -1))	
	{
		return kWebNewOrder;
	}
	//��������
	if (http_session->m_Requestline.m_Host.Find("www.podinns.com") != 1 &&
		http_session->m_Requestline.m_URI.Find("/Member/MyCommonUserEdit") != -1)
	{
		return kWebCommonUser;
	}
	//android��¼
	if (http_session->m_Requestline.m_Host.Find("smart.zhotels.cn:8081") != 1 &&
		http_session->m_Requestline.m_URI.Find("/app/login") != -1)
	{
		return kAndroidLogin;
	}
	//android��λ
	if (http_session->m_Requestline.m_Host.Find("115.236.19.4:9008") != 1 &&
		http_session->m_Requestline.m_URI.Find("/login/main") != -1)
	{
		return kAndroidLocation;
	}
	return 0;
}

hotel::HotelPodinns::Process(const char *packet,int action)
{
	//webע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web��¼
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web���ھƵ�Ԥ��
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	//web��������
	if (action == kWebCommonUser)
	{
		return WebCommonUser(packet);
	}
	//android��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android��λ
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 0;
}
//webע��
hotel::HotelPodinns::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("Name") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("Name"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("Name",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("Mobile");
	//���� 
	string password = ms_->GetValueFromMapByKey("Password2");	
	string confirmpassword = ms_->GetValueFromMapByKey("ConfirmPassword");
    if (password != confirmpassword)
    {
		return -1;
    }
	//���֤��
	string idcard = ms_->GetValueFromMapByKey("Sfz");
	if (idcard.empty())
	{
		idcard = "δ��д";
	}
	//����
	string email = ms_->GetValueFromMapByKey("Email",true);
	if (email.empty())
	{
		email = "δ��д";
	}
	if (name.empty() || mobile.empty() || password.empty())
	{
		return -1;
	}
	output_text_ =
		"wifissid=" + 	name + "\r\n" + 		//����
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"pass=" + 		password + "\r\n" +		//����
		"msg=" + 	  	idcard + "\r\n" + 		//���֤��
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}
//web��¼
hotel::HotelPodinns::WebLogin(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("isTravel") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("isTravel"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    string username = ms_->GetValueFromMapByKey("UserName",true);
	string password = ms_->GetValueFromMapByKey("Password");

	if (username.empty() || password.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +	    //�û���
		"pass=" + 		password + "\r\n" +		//����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//web���ھƵ�Ԥ��
hotel::HotelPodinns::WebNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("InName") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("InName"));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//ס������ 
	string name = ms_->GetValueFromMapByKey("InName",true);
    //���֤��
	string idcard = ms_->GetValueFromMapByKey("Sfzh");
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("LinkMobile");
	//��ϵ���� 
	string email = ms_->GetValueFromMapByKey("LinkEmail",true);

	if (name.empty() || idcard.empty() || mobile.empty() || email.empty())
	{
		return -1;
	}
	output_text_ =
		"subject=" + 	name + "\r\n" + 		//ס������
		"msg=" + 	  	idcard + "\r\n" + 		//���֤��
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}
//web��������
hotel::HotelPodinns::WebCommonUser(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("MCU_ID") != -1)
	{
		src_data = src_data.substr(src_data.rfind("MCU_ID"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ж� ���� �޸�
	string mcu_id = ms_->GetValueFromMapByKey("MCU_ID");
	//����
	string username = ms_->GetValueFromMapByKey("MCU_USERNAME",true);
	//���֤��
	string idcard = ms_->GetValueFromMapByKey("MCU_LINKUSER");
	//��ϵ�ֻ�
	string mobile = ms_->GetValueFromMapByKey("MCU_MOBILE");
	//��ϵ����
	string email = ms_->GetValueFromMapByKey("MCU_EMAIL",true);
	if ("0" == mcu_id)
	{
		//����
		output_text_ =
			"wifissid=" + 	username + "\r\n" + 	//����
			"msg=" + 	  	idcard + "\r\n" + 		//���֤��
			"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
			"oid=" + 	  	email + "\r\n" + 		//��ϵ����
			"netmark=" +	app_ + "\r\n" + 		//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=������������\r\n";					//״̬
	}
	else if ("0" != mcu_id)
	{
		//�޸�
		output_text_ =
			"wifissid=" + 	username + "\r\n" + 	//����
			"msg=" + 	  	idcard + "\r\n" + 		//���֤��
			"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
			"oid=" + 	  	email + "\r\n" + 		//��ϵ����
			"netmark=" +	app_ + "\r\n" + 		//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=�޸ĳ�������\r\n";	
	}
	WriteIndexFile();
   return 0;
}

//android��¼
hotel::HotelPodinns::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("phone") != -1)
	{
		src_data = src_data.substr(src_data.rfind("phone"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//��ϵ�ֻ�
	string mobile = ms_->GetValueFromMapByKey("phone");
	if (mobile.empty())
	{
		return -1;
	}
	//�޸�
	output_text_ =
		"islogin=" + 	mobile + "\r\n" + 			//��ϵ�ֻ�
		"netmark=" +	app_ + "\r\n" + 			//��Դ
		"clientmsg=" +  platformAndroid + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";	
	WriteIndexFile();
	return 0;
}

//android��λ
hotel::HotelPodinns::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("{\"memberId") != -1)
	{
		src_data = src_data.substr(src_data.rfind("{\"memberId"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	AfxMessageBox(src_data.c_str());

	string username,nickname,longitude,latitude;
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		username = value["username"].asString();
		nickname = value["nickname"].asString();
		longitude = value["longitude"].asString();
		latitude = value["latitude"].asString();
	}
	if (username.empty() || longitude.empty() || latitude.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//�û���
		"from=" + 	  	longitude + "\r\n" + 		//���� 
		"to=" + 	  	latitude + "\r\n" + 		//γ��
		"netmark=" +	app_ + "\r\n" + 			//��Դ
		"clientmsg=" +  platformAndroid + "\r\n" +  //����ƽ̨
		"state=��λ-��̬Ȧ\r\n";	
	WriteIndexFile();
	return 0;
}