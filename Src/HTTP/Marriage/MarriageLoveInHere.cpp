#include "StdAfx.h"
#include "MarriageLoveInHere.h"

marriage::LoveInHereMarriage::LoveInHereMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "�������";	
}

marriage::LoveInHereMarriage::~LoveInHereMarriage()
{
	
}

int marriage::LoveInHereMarriage::IsLoveInHereMarriage(HttpSession *http_session)
{
	//ע��	
	if((http_session->m_Requestline.m_Host.Find("www.loveinhere.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/memberData_appe.aspx")!=-1)) 
	{
		return kWebRegister;
	}	
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.loveinhere.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/login_check.aspx")!=-1)) 
	{
		return kWebLogin;
	}	
	//web�޸�����
	if((http_session->m_Requestline.m_Host.Find("www.loveinhere.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/memberData_adm_modi.aspx")!=-1)) 
	{
		return kWebModify;
	}	
	return 0;
}

int marriage::LoveInHereMarriage::Process(const char *packet,int action)
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
int marriage::LoveInHereMarriage::WebRegister(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("memberid");
	//����
	string password = ms_->GetValueFromMapByKey("pass");
	string passconfirm = ms_->GetValueFromMapByKey("pass_confirm");
	if (password != passconfirm)
	{
		return 0;
	}
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//����
	string name = ms_->GetValueFromMapByKey("name",true); 
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex")=="1"?"��":"Ů";
	//��������
	string birthday = ms_->GetValueFromMapByKey("y12") +
					  ms_->GetValueFromMapByKey("y3") +
					  ms_->GetValueFromMapByKey("y4") + "-" +
					  ms_->GetValueFromMapByKey("m") + "-" +
					  ms_->GetValueFromMapByKey("d");
	//֤������
	string idtype = "���֤";
	//���֤��
	string personID = ms_->GetValueFromMapByKey("personID");
	if (personID.empty())
	{
		personID = "δ��д";
	}
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("tel_mobile");
	//ͨѶ����
	string IMkind = "QQ/Skype/΢��";
	//ͨѶ��
	string IMnumber = ms_->GetValueFromMapByKey("IM_number");
	if (userid.empty() || name.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" +		userid + "\r\n" + 		//28    [92]    //�û�ID
		"pass=" + 		password + "\r\n" + 	//21	[14]	//���� 
		"msg=" + 	  	nickname + "\r\n" + 	//63	[128] 	//�ǳ� 
		"mid=" + 	  	name + "\r\n" + 		//56	[121]	//����
		"sex=" + 	  	sex + "\r\n" + 			//60	[125]	//�Ա�
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//��������
		"islogin=" + 	address + "\r\n" + 		//53	[118]   //��ַ
		"servername=" + idtype + "\r\n" + 		//49	[116]   //֤������      
		"appname=" + 	personID + "\r\n" + 	//50	[115]   //֤���� 
		"oid=" + 	  	mobile + "\r\n" + 		//54	[119]	//�ֻ��� 
		"msgtype=" + 	IMkind + "\r\n" +		//51	[127]	//ͨѶ����
		"subject=" + 	IMnumber + "\r\n" + 	//52	[46]    //ͨѶ��
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//��Դ
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
		"state=ע��\r\n";						//64	[19]	//״̬
		WriteIndexFile();
	return 1;
}
//��¼
int marriage::LoveInHereMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("LoginKind=") != -1)
	{
		src_data = src_data.substr(src_data.rfind("LoginKind="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�û�ID
	string memberid = ms_->GetValueFromMapByKey("memberid");
	if (memberid.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" + 	memberid + "\r\n" + 		//28    [92]    //�û�ID
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" +	platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=��¼\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//web�޸�����
int marriage::LoveInHereMarriage::WebModify(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("memberid");
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//����
	string name = ms_->GetValueFromMapByKey("name",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex")=1?"��":"Ů";
	//��������
	string birthday = ms_->GetValueFromMapByKey("y12") + 
		              ms_->GetValueFromMapByKey("y3") +
					  ms_->GetValueFromMapByKey("y4") + "-" +
					  ms_->GetValueFromMapByKey("m") + "-" +
					  ms_->GetValueFromMapByKey("d");
	//֤������
	string idtype = "���֤";
 	//���֤��
	string personID = ms_->GetValueFromMapByKey("personID");
	if (personID.empty())
	{
		personID = "δ��д";
	}
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("tel_mobile");
	//ͨѶ����
	string IMkind = "QQ/Skype/΢��";
	//ͨѶ��
	string IMnumber = ms_->GetValueFromMapByKey("IM_number");

	output_text_ =
		"userid=" +		userid + "\r\n" + 		//28    [92]    //�û�ID
		"msg=" + 	  	nickname + "\r\n" + 	//63	[128] 	//�ǳ� 
		"mid=" + 	  	name + "\r\n" + 		//56	[121]	//����
		"sex=" + 	  	sex + "\r\n" + 			//60	[125]	//�Ա�
		"onick=" + 	  	birthday + "\r\n" + 	//55	[120]	//��������
		"servername=" + idtype + "\r\n" + 		//49	[116]   //֤������      
		"appname=" + 	personID + "\r\n" + 	//50	[115]   //֤���� 
		"islogin=" + 	address + "\r\n" + 		//53	[118]   //��ַ
		"oid=" + 	  	mobile + "\r\n" + 		//54	[119]	//�ֻ��� 
		"msgtype=" + 	IMkind + "\r\n" +		//51	[127]	//ͨѶ����
		"subject=" + 	IMnumber + "\r\n" + 	//52	[46]    //ͨѶ��
		"netmark=" +	app_ + "\r\n" + 		//59	[124]	//��Դ
		"clientmsg=" +	platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";				//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
