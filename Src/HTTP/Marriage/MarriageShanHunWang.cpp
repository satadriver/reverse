#include "StdAfx.h"
#include "MarriageShanHunWang.h"

marriage::ShanHunMarriage::ShanHunMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "������";	
}

marriage::ShanHunMarriage::~ShanHunMarriage()
{
	
}

int marriage::ShanHunMarriage::IsShanHunMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("www.91shanhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.asp")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("www.91shanhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.asp?action=chk")!=-1)) 
	{
		return kPcWebLogin;
	}
	//��ҳ�޸�����
	if((http_session->m_Requestline.m_Host.Find("www.91shanhun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/u_mod.asp")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::ShanHunMarriage::Process(const char *packet,int action)
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

	return 1;
}
//pcע��
int marriage::ShanHunMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�û���
	string username = ms_->GetValueFromMapByKey("username",true);
	//����
	string password = ms_->GetValueFromMapByKey("password1",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//QQ 
	string oicq = ms_->GetValueFromMapByKey("oicq",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true);
	//�ֻ�
	string tel = ms_->GetValueFromMapByKey("tel",true);
	//΢��
	string weixin = ms_->GetValueFromMapByKey("phone",true);

	output_text_ =
		"user=" + username + "\r\n" +			//�û���
		"pass=" +  password  + "\r\n" +			//���� 
        "islogin=" +  address+ "\r\n" +			//��ַ
		"nick=" +email + "\r\n" +               //����
		"sex=" + sex+ "\r\n" +					//�Ա�
		"oid=" + tel + "\r\n" +					//�ֻ�
		"clientip=" + 	  	oicq + "\r\n" + 	//QQ
		"wifissid=" + 	 	weixin + "\r\n" + 	//΢��
		"netmark=" + app_ + "\r\n" + 			//��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//����ƽ̨
		"state=ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//web��¼
int marriage::ShanHunMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("username",true);
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
int marriage::ShanHunMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//QQ 
	string oicq = ms_->GetValueFromMapByKey("oicq",true);
	//�ֻ�
	string tel = ms_->GetValueFromMapByKey("tel",true);
	//΢��
	string weixin = ms_->GetValueFromMapByKey("phone",true);
	
	output_text_ =
        "islogin=" +  address+ "\r\n" +         //��ַ
		"nick=" +email + "\r\n" +               //����
		"oid=" + tel + "\r\n" +                 //�ֻ�
		"clientip=" + 	  	oicq + "\r\n" + 	//QQ
		"wifissid=" + 	 	weixin + "\r\n" + 	//΢��
		"netmark=" + app_ + "\r\n" + 	        //��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //����ƽ̨
		"state=�޸�\r\n";			            //״̬
	WriteIndexFile();
	return 1;
}