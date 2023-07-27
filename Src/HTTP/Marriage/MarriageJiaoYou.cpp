#include "StdAfx.h"
#include "MarriageJiaoYou.h"

marriage::JiaoYouMarriage::JiaoYouMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "�й�������";	
}

marriage::JiaoYouMarriage::~JiaoYouMarriage()
{
	
}

int marriage::JiaoYouMarriage::IsJiaoYouMarriage(HttpSession *http_session)
{
	//ע��(�û��� ����)		m_Requestline.m_Action==HTTP_POST
	if((http_session->m_Requestline.m_Host.Find("www.jiaoyou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.php?step=1")!=-1)) 
	{
		return kWebRegister;
	}	
	//ע��(���� �绰 ��ַ ͨѶ��)
	if((http_session->m_Requestline.m_Host.Find("www.jiaoyou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.php?step=6")!=-1)) 
	{
		return kWebRegister;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.jiaoyou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.php?act=login")!=-1)) 
	{
		return kWebLogin;
	}	
	//web�޸�����(������Ϣ)
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/ajax_edit_contact")!=-1)) 
	{
		return kWebModify;
	}	
	return 0;
}

int marriage::JiaoYouMarriage::Process(const char *packet,int action)
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
int marriage::JiaoYouMarriage::WebRegister(const char *packet)
{
	//�ж� uri
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	//�û��� ����
	if (uri.find("/register.php?step=1") != -1)
	{
		//���ݲ���
		string src_data(packet);
		if (src_data.find("user_name=") != string::npos)
		{
			src_data = src_data.substr(src_data.find("user_name="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//�û���
		string username = ms_->GetValueFromMapByKey("user_name",true);
		//����
		string password = ms_->GetValueFromMapByKey("userpass");
		string password2 = ms_->GetValueFromMapByKey("userpass2");
		if (password != password2)
		{
			return 0;
		}
		if (username.empty() || password.empty())
		{
			return 0;
		}
		output_text_ =
			"user=" + 		username + "\r\n" +			//20	[13]	//�û��� 
			"pass=" + 		password + "\r\n" + 		//21	[14]	//����
			"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
			"state=ע��\r\n";							//64	[19]	//״̬
		WriteIndexFile();
	}
	//ע�� (���� �绰 ��ַ ͨѶ�˺�)
	if (uri.find("/register.php?step=6") != -1)
	{
		//���ݲ���
		string src_data(packet);
		if (src_data.rfind("real_name=") != string::npos )
		{
			src_data = src_data.substr(src_data.rfind("real_name="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//����
		string realName = ms_->GetValueFromMapByKey("real_name",true);
		//�绰
		string phone = ms_->GetValueFromMapByKey("p_tel");
		//��ַ
		string address = ms_->GetValueFromMapByKey("p_address",true);
		//ͨѶ����
		string contactType = "QQ/MSN/Yahooͨ";
		//ͨѶ�˺�
		string contact = ms_->GetValueFromMapByKey("p_contact",true);
		if (realName.empty())
		{
			return 0;
		}
		
		output_text_ =
			"mid=" + 	  	realName + "\r\n" + 		//56	[121]	//����	
			"oid=" + 	  	phone + "\r\n" + 			//54	[119]	//�ֻ��� 
			"msgtype=" + 	contactType + "\r\n" +		//51	[127]	//ͨѶ����
			"subject=" + 	contact + "\r\n" + 			//52	[46]    //ͨѶ��
			"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
			"state=ע��\r\n";							//64	[19]	//״̬
		WriteIndexFile();
	}
	return 1;
}
//��¼
int marriage::JiaoYouMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("user_name=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("user_name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("user_name",true);
	//����
	string password = ms_->GetValueFromMapByKey("userpass");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//20	[13]	//�û��� 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//���� 	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=��¼\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//web�޸�����
int marriage::JiaoYouMarriage::WebModify(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	//��ϵ��ʽ
	if (uri.find("/user/ajax_edit_contact") != -1)
	{
		//���ݲ���
		string src_data(packet);
		if (src_data.find("mid=") != -1)
		{
			src_data = src_data.substr(src_data.find("mid="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//�û�ID
		string userId = ms_->GetValueFromMapByKey("mid");
		//QQ
		string QQ = ms_->GetValueFromMapByKey("qq");
		//΢��
		string weixin = ms_->GetValueFromMapByKey("weixin");
		//����
		string email = ms_->GetValueFromMapByKey("email");
		
		output_text_ =
			"userid=" +		userId + "\r\n" + 		//28    [92]    //�û�ID
			"clientip=" + 	QQ + "\r\n" + 			//68	[147] 	//QQ
			"wifissid=" + 	weixin + "\r\n" + 		//69	[148] 	//΢��
			"nick="	 + 	  	email + "\r\n" + 		//58	[123] 	//����
			"netmark=" + app_ + "\r\n" + 			//59	[124]	//��Դ
			"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
			"state=�޸ĸ�����Ϣ\r\n";				//64	[19]	//״̬
		WriteIndexFile();
	}
	//������Ϣ
	if (uri.find("/user/ajax_edit_personal") != -1)
	{
		//���ݲ���
		string src_data(packet);
		if (src_data.find("mid=") != -1)
		{
			src_data = src_data.substr(src_data.find("mid="));
		}
		ms_->SplitStringToMap(src_data,'&','=');
		//�û�ID
		string userId = ms_->GetValueFromMapByKey("mid");
		//�ǳ�
		string nickname = ms_->GetValueFromMapByKey("nickname",true);
		//����
		string email = ms_->GetValueFromMapByKey("email",true);
		
		output_text_ =
			"userid=" +		userId + "\r\n" + 		//28    [92]    //�û�ID
			"msg=" + 	 	nickname + "\r\n" + 	//65	[104]   //�ǳ�
			"nick="	 + 	  	email + "\r\n" + 		//58	[123] 	//����
			"netmark=" + app_ + "\r\n" + 			//59	[124]	//��Դ
			"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
			"state=�޸ĸ�����Ϣ\r\n";				//64	[19]	//״̬
		WriteIndexFile();
	}
	return 1;
}
