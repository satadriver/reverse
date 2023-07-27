#include "StdAfx.h"
#include "MarriageTaoNan.h"

marriage::TaoNanMarriage::TaoNanMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "������";	
}

marriage::TaoNanMarriage::~TaoNanMarriage()
{
	
}

int marriage::TaoNanMarriage::IsTaoNanMarriage(HttpSession *http_session)
{
	//web������ (�޸����� ��¼ QQע��)
	if((http_session->m_Requestline.m_Host.Find("www.taonanw.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php")!=-1)) 
	{
		return kWebTaoNan;
	}
	return 0;
}

int marriage::TaoNanMarriage::Process(const char *packet,int action)
{
	//web������ (�޸����� ��¼ QQע��)
	if (action == kWebTaoNan)
	{
		return WebTaoNan(packet);
	}
	return 1;
}

//web������ (�޸����� ��¼ QQע��)
int marriage::TaoNanMarriage::WebTaoNan(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string action = ms_->GetValueFromMapByKey("action",true);
	//login_with_qq (ʹ��QQ��¼ʱ�����õĵ�¼���� ����)
	if (action == "restore" || action == "simple_reg")
	{
		//����
		string email = ms_->GetValueFromMapByKey("email");
		//���� 
		string password = ms_->GetValueFromMapByKey("password");
		string confirm_password = ms_->GetValueFromMapByKey("confirm_password");
		if (password != confirm_password)
		{
			return 0;
		}
		//�ǳ� 
		string nickName = ms_->GetValueFromMapByKey("netname",true);
		//��������
		string birthDate = ms_->GetValueFromMapByKey("birthday_year") + "-" +
						   ms_->GetValueFromMapByKey("birthday_month") + "-" +
						   ms_->GetValueFromMapByKey("birthday_day");
		output_text_ =
			"nick="	 + 	  	email + "\r\n" + 			//58	[123] 	//����
			"pass=" + 		password + "\r\n" + 		//21	[14]	//���� 	
			"msg=" + 	 	nickName + "\r\n" + 		//65	[104]   //�ǳ�
			"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
			"clientmsg=" +	platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
			"state=�½�������Ϣ\r\n";					//64	[19]	//״̬
		WriteIndexFile();
	}
	//login (��ͨ��¼)
	if (action == "do_login")
	{
		//�û���
		string username = ms_->GetValueFromMapByKey("username",true);
		//����
		string password = ms_->GetValueFromMapByKey("password");
		if (username.empty() || password.empty())
		{
			return 0;
		}
		output_text_ =
			"user=" + username + "\r\n" +				//20	[13]	//�û��� 
			"pass=" + password + "\r\n" + 				//21	[14]	//���� 	
			"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
			"state=��¼\r\n";							//64	[19]	//״̬
		WriteIndexFile();
	}
	//profile_edit (���������޸�)
	if (action == "profile_edit")
	{
		//�û�ID
		string userId = ms_->GetValueFromMapByKey("usr_id");
		//�ǳ� 
		string nickname = ms_->GetValueFromMapByKey("netname",true);
		//�������� 
		string birthdate = ms_->GetValueFromMapByKey("birthday");
		//�ֻ���
		string phonenumber = ms_->GetValueFromMapByKey("phone_number"); 
		//QQ
		string qqContact = ms_->GetValueFromMapByKey("qq");
		//WeiChat
		string WeiChat = ms_->GetValueFromMapByKey("weixin");

		output_text_ =
			"userid=" +		userId + "\r\n" + 			//28    [92]    //�û�ID
			"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //�ǳ�
			"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//�������� 
			"oid=" + 	  	phonenumber + "\r\n" + 		//54	[119]	//�ֻ��� 
			"clientip=" + 	qqContact + "\r\n" + 		//68	[147] 	//QQ
			"wifissid=" + 	WeiChat + "\r\n" + 			//69	[148] 	//΢��
			"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
			"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
			"state=�޸ĸ�����Ϣ\r\n";					//64	[19]	//״̬
		WriteIndexFile();
	}
	return 1;
}
