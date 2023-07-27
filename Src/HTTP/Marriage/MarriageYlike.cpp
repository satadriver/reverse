#include "StdAfx.h"
#include "MarriageYlike.h"

marriage::YlikeMarriage::YlikeMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "Ե����";	
}

marriage::YlikeMarriage::~YlikeMarriage()
{
	
}

int marriage::YlikeMarriage::IsYlikeMarriage(HttpSession *http_session)
{
	//ע��(�û��� ����)	
	if((http_session->m_Requestline.m_Host.Find("www.ylike.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ReglSave.do")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.ylike.com/Reg_Base.do") != -1)) 
	{
		return kWebRegister;
	}	
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.ylike.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ReglSave.do")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.ylike.com/UserLogin.do") != -1)) 
	{
		return kWebLogin;
	}	
	//web�޸�����
	if((http_session->m_Requestline.m_Host.Find("www.ylike.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/save.do")!=-1)) 
	{
		return kWebModify;
	}	
	return 0;
}

int marriage::YlikeMarriage::Process(const char *packet,int action)
{
	//ע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//��¼
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web�޸�����
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	return 1;
}
//ע��
int marriage::YlikeMarriage::WebRegister(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.find("choseType=") != -1)
	{
		src_data = src_data.substr(src_data.find("choseType="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    //�û�����
	string action = ms_->GetValueFromMapByKey("action");
	//�û���
	string username = ms_->GetValueFromMapByKey("User_Name",true);
	//����
	string password = ms_->GetValueFromMapByKey("User_Password");
	string passwordconfirm = ms_->GetValueFromMapByKey("User_Password2");
	if (password != passwordconfirm)
	{
		return 0;
	}
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("Name",true); 
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("Sex",true);
	//��������
	string birthday = ms_->GetValueFromMapByKey("Birthday");
	//QQ
	string QQ = ms_->GetValueFromMapByKey("QQ");
	//����
	string email = ms_->GetValueFromMapByKey("Mail",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("Canton",true) +
					 ms_->GetValueFromMapByKey("City",true) +
					 ms_->GetValueFromMapByKey("Town",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	
	output_text_ =
		"user=" + 		username + "\r\n" +		//20	[13]	//�û��� 
		"pass=" + 		password + "\r\n" + 	//21	[14]	//���� 
		"msg=" + 	  	nickname + "\r\n" + 	//63	[128] 	//�ǳ� 
		"sex=" + 	  	sex + "\r\n" + 			//60	[125]	//�Ա�
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//��������
		"clientip=" + 	QQ + "\r\n" + 			//68	[147] 	//QQ
		"nick="	 + 	  	email + "\r\n" + 		//58	[123] 	//����
		"islogin=" + 	address + "\r\n" + 		//53	[118]   //��ַ
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//��Դ
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
		"state=ע��\r\n";						//64	[19]	//״̬
		WriteIndexFile();
	return 1;
}
//��¼
int marriage::YlikeMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.find("User_Name=") != -1)
	{
		src_data = src_data.substr(src_data.find("User_Name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    //�û�����
	string action = ms_->GetValueFromMapByKey("action");
	//�û���
	string username = ms_->GetValueFromMapByKey("User_Name",true);
	//����
	string password = ms_->GetValueFromMapByKey("User_Password");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//20	[13]	//�û��� 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//���� 	
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" +	platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=��¼\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//web�޸�����
int marriage::YlikeMarriage::WebModify(const char *packet)
{
	//��cookie�л�ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	if (cookie.find("Sky=") != string::npos)
	{
		cookie = cookie.substr(cookie.find("Sky=") + strlen("Sky="));
	}
	ms_->SplitStringToMap(cookie,'&','=');
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("User_ID");
	//�Ա� 
	string sex = ms_->GetValueFromMapByKey("Sex");
	//���ݲ���
	string src_data(packet);
	if (src_data.find("Canton=") != -1)
	{
		src_data = src_data.substr(src_data.find("Canton="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//��������
	string birthday = ms_->GetValueFromMapByKey("Birthday");
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("tel");
	//QQ
	string qq = ms_->GetValueFromMapByKey("QQ");
	//΢��
	string weixin = ms_->GetValueFromMapByKey("MSN");
	//����
	string email = ms_->GetValueFromMapByKey("Mail",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("Canton",true) +
					 ms_->GetValueFromMapByKey("City",true) +
					 ms_->GetValueFromMapByKey("Town",true);
	output_text_ =
		"userid=" +		userid + "\r\n" + 		//28    [92]    //�û�ID
		"sex=" + 	  	sex + "\r\n" +			//60	[125]	//�Ա�
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//��������
		"oid=" + 	  	mobile + "\r\n" + 		//54	[119]	//�ֻ��� 
		"clientip=" + 	qq + "\r\n" + 			//68	[147] 	//QQ
		"wifissid=" + 	weixin + "\r\n" + 		//69	[148] 	//΢��
		"nick="	 + 		email + "\r\n" + 		//58	[123] 	//����
		"islogin=" +	address + "\r\n" + 		//53	[118]   //��ַ
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//��Դ
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";				//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
