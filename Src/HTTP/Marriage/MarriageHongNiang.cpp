#include "StdAfx.h"
#include "MarriageHongNiang.h"

marriage::HongNiangMarriage::HongNiangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "�й�������";	
}

marriage::HongNiangMarriage::~HongNiangMarriage()
{
	
}

int marriage::HongNiangMarriage::IsHongNiangMarriage(HttpSession *http_session)
{
	//ע��
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/do_register_new1")!=-1)) 
	{
		return kWebRegister;
	}	
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/account/do_login")!=-1)) 
	{
		return kWebLogin;
	}	
	//web�޸�����(ע����Ϣ)
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/save_percenter")!=-1)) 
	{
		return kWebModify;
	}
	//web�޸�����(��ϸ��Ϣ)
	if((http_session->m_Requestline.m_Host.Find("www.hongniang.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/save_detail")!=-1)) 
	{
		return kWebModifyM;
	}
	return 0;
}

int marriage::HongNiangMarriage::Process(const char *packet,int action)
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
	//web�޸�����ע����Ϣ
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	//web�޸�������ϸ����
	if (action == kWebModifyM)
	{
		return WebModifyM(packet);
	}
	return 1;
}
//ע��
int marriage::HongNiangMarriage::WebRegister(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	if (src_data.find("sex=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("sex="));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//����
	string password = ms_->GetValueFromMapByKey("password");
	string re_password = ms_->GetValueFromMapByKey("re_password");
	if (password != re_password)
	{
		return 0;
	}
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//��������
	string birthdate = ms_->GetValueFromMapByKey("year") + "-" +
					   ms_->GetValueFromMapByKey("month") + "-" +
					   ms_->GetValueFromMapByKey("day");
	
	output_text_ =
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//����
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //�ǳ�
		"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//�������� 
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=ע��\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//��¼
int marriage::HongNiangMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("username=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("username="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("username",true);
	//����
	string password = ms_->GetValueFromMapByKey("password");
	
	output_text_ =
		"user=" + 		username + "\r\n" +			//20	[13]	//�û��� 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//���� 	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=��¼\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//web�޸�������ϸ��Ϣ
int marriage::HongNiangMarriage::WebModifyM(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("realname", true);
	if (name.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" +		name + "\r\n" + 		 //����
		"netmark=" + app_ + "\r\n" + 			//��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//����ƽ̨
		"state=�޸���ϸ��Ϣ\r\n";				//״̬
	WriteIndexFile();
	return 1;
}
//web�޸�����ע����Ϣ
int marriage::HongNiangMarriage::WebModify(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//�ֻ���
	string email = ms_->GetValueFromMapByKey("mobile",true);
	if (nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 	//65	[104]   //�ǳ�
		"mobile="	 + 	  	email + "\r\n" + 	//�ֻ���
		"netmark=" + app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
		"state=�޸�ע����Ϣ\r\n";				//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
