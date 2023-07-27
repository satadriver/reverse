#include "StdAfx.h"
#include "MarriageZhiJiWang.h"

marriage::ZhiJiMarriage::ZhiJiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "֪����";	
}

marriage::ZhiJiMarriage::~ZhiJiMarriage()
{
	
}

int marriage::ZhiJiMarriage::IsZhiJiMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("www.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg2008/reg_newg_submit.asp")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("zhanghu.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login_submit.asp")!=-1)) 
	{
		return kPcWebLogin;
	}
	//��ҳ�޸�����
	if((http_session->m_Requestline.m_Host.Find("zhanghu.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/members/templete/ajax_register_change_submit.asp")!=-1)) 
	{
		return kPcWebModiy;
	}
	//�ֻ���¼
	if((http_session->m_Requestline.m_Host.Find("app.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/zhiji-mobile-web/login/loginios")!=-1)) 
	{
		return kAndroidLogin;
	}
	//�ֻ��޸�
	if((http_session->m_Requestline.m_Host.Find("app.zhiji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/zhiji-mobile-web/memberdata/xiugai")!=-1)) 
	{
		return kAndroidModiy;
	}
	return 0;
}

int marriage::ZhiJiMarriage::Process(const char *packet,int action)
{
	//��ҳע��
	if (action == kPcWebRegister)
	{
		return PcWebRegister(packet);
	}
	//��ҳ��¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//��ҳ�޸ĸ�������
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//�ֻ���¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	if (action == kAndroidModiy)
	{
		return AndroidModiy(packet);
	}
	return 1;
}
//pcע��
int marriage::ZhiJiMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	nickname = ms_->UnicodeToGBKPrefix_u(nickname);
	//ͨѶ����
	string contactType = "QQ";
	//QQ
	string qq = ms_->GetValueFromMapByKey("QQ",true);
	//�ֻ�
	string phone = ms_->GetValueFromMapByKey("phone",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true);
	if(sex == "1"){sex = "��";}
	else{sex = "Ů";}
	//��������
	string year = ms_->GetValueFromMapByKey("ldy_age",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("province_name",true)+
		             ms_->GetValueFromMapByKey("city_name",true);
	address = ms_->UnicodeToGBKPrefix_u(address);
	output_text_ =
		"msg=" + 	 nickname + "\r\n" + 		//�ǳ� 
		"msgtype=" + contactType + "\r\n" +		//ͨѶ����
		"subject=" + 	  	qq + "\r\n" + 		//ͨѶ��
		"sex=" +sex + "\r\n" +					//�Ա�
		"onick=" + year + "\r\n" +				//��������
		"oid=" + phone + "\r\n" +				//�ֻ���
		"islogin=" + address + "\r\n" +			//��ַ
		"netmark=" + app_ + "\r\n" + 			//��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//����ƽ̨
		"state=ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//web��¼
int marriage::ZhiJiMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("login_name",true);
	string password = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +          //�û���
		"pass=" + password + "\r\n" +          //����
		"netmark=" + app_ + "\r\n" + 		   //��Դ
		"clientmsg=" + platformWeb + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";			           //״̬
	WriteIndexFile();
	return 1;
}

//��ϸ���ϸ�����Ϣ
int marriage::ZhiJiMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	email = ms_->UnicodeToGBKPrefix_u(email);
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("mobileno",true);
	phone = ms_->UnicodeToGBKPrefix_u(phone);
	//QQ
	string oicq = ms_->GetValueFromMapByKey("oicq",true);
    oicq = ms_->UnicodeToGBKPrefix_u(oicq);
	//΢��
	string msn = ms_->GetValueFromMapByKey("msn",true);
	msn = ms_->UnicodeToGBKPrefix_u(msn);
	string oicqid = "qq/΢��";
	oicq = oicq + "/" +msn;
	//��������
	string birth = ms_->GetValueFromMapByKey("birth_year",true)+"��"+
		           ms_->GetValueFromMapByKey("birth_mon",true)+"��"+
				   ms_->GetValueFromMapByKey("birth_day",true)+"��";
	birth = ms_->UnicodeToGBKPrefix_u(birth);
	//��ַ
	string province = ms_->GetValueFromMapByKey("register_province_name",true);
	string city	 =   ms_->GetValueFromMapByKey("register_city_name",true);
	province = ms_->UnicodeToGBKPrefix_u(province);
	city = ms_->UnicodeToGBKPrefix_u(city);
	string address = province + city;
	ms_->Replace(address,"%u","");
	address = ms_->UnicodeToGBK(address);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true);
	sex = ms_->UnicodeToGBKPrefix_u(sex);
	if (sex == "1"){sex = "��";}
	else{sex = "Ů";}
	output_text_ =
		"msgtype=" + oicqid + "\r\n" +		//ͨѶ����
		"subject=" +  oicq + "\r\n" +       //ͨѶ��
		"nick="  +     email + "\r\n" +		//����
		"sex=" +sex + "\r\n" +              //�Ա�
		"onick=" + birth + "\r\n" +         //��������
		"oid=" + phone + "\r\n" +           //�ֻ���
		"islogin=" + address + "\r\n" +     //��ַ
		"netmark=" + app_ + "\r\n" + 	    //��Դ
		"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";
 	WriteIndexFile();
	return 1;
}
//�ֻ���¼
int marriage::ZhiJiMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string lon = ms_->GetValueFromMapByKey("lon",true);
	string lat = ms_->GetValueFromMapByKey("lat",true);
	string login_name = ms_->GetValueFromMapByKey("login_name",true);
	login_name = ms_->UnicodeToGBKPrefix_u(login_name);
	string password = ms_->GetValueFromMapByKey("password",true);
	output_text_ =
		"user=" +   login_name + "\r\n" +			//�û��� 
		"pass=" +   password  + "\r\n" +			//����  
		"from=" +     lon + "\r\n" +				//����  
        "to=" +     lat + "\r\n" +					//γ��
		"netmark=" + app_ + "\r\n" + 				//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//����ƽ̨
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;

}
//�ֻ��޸�
int marriage::ZhiJiMarriage::AndroidModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��¼��
	string login_name = ms_->GetValueFromMapByKey("login_name",true);
	login_name = ms_->UnicodeToGBKPrefix_u(login_name);
	//����
	string province = ms_->GetValueFromMapByKey("province_name",true);
	string city = ms_->GetValueFromMapByKey("city_name",true);
	province = ms_->UnicodeToGBKPrefix_u(province);
	city = ms_->UnicodeToGBKPrefix_u(city);
	string address = province+city;
	//��������
	string birth = ms_->GetValueFromMapByKey("birth_year",true)+"-"+
		           ms_->GetValueFromMapByKey("birth_mon",true)+"-"+
				   ms_->GetValueFromMapByKey("birth_day",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex_data",true);
	if (sex=="0")
	{
		sex = "��";
	}
	else
	{
		sex = "Ů";
	}
	output_text_=
		"msg=" + 	 login_name + "\r\n" + 		//�ǳ� 
		"islogin=" + address + "\r\n" +			//��ַ
		"sex=" +sex + "\r\n" +					//�Ա�
		"onick=" + birth + "\r\n" +				//��������
		"netmark=" + app_ + "\r\n" + 	        //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";
	WriteIndexFile();
	return 1;
}


