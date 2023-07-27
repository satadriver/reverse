#include "StdAfx.h"
#include "MarriageJiaYuan.h"

marriage::JiaYuanMarriage::JiaYuanMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "���ͼ�Ե��";	
}

marriage::JiaYuanMarriage::~JiaYuanMarriage()
{
	
}

int marriage::JiaYuanMarriage::IsJiaYuanMarriage(HttpSession *http_session)
{
	//��ҳ�޸�����	
	if((http_session->m_Requestline.m_Host.Find("www.jiayuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/usercp/profile_postdo_new.php")!=-1)) 
	{
		return kPcWebModiy;
	}
	//��׿�ֻ�ע��
	if((http_session->m_Requestline.m_Host.Find("api.jiayuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg/register_basic.php")!=-1)) 
	{
		return kAndroidRegiste;
	}
	return 0;
}

int marriage::JiaYuanMarriage::Process(const char *packet,int action)
{
	//��ҳ�޸Ļ�����Ϣ
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//��׿�ֻ�ע��
	if (action == kAndroidRegiste)
	{
		return AndroidRegiste(packet);
	}
	return 1;
}

//��ҳ�޸Ļ�����Ϣ
int marriage::JiaYuanMarriage::PcWebModiy(const char *packet)
{
	//��cookie ����ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�û���
	string userName = ms_->GetValueFromMapByKey("save_jy_login_name",true);
	//������Ϣ 
	string proFile = ms_->GetValueFromMapByKey("PROFILE",true);
	//�û�ID
	int pos1 = proFile.find(":");
	int pos2 = proFile.find(":",pos1 + 1);
	string userId = proFile.substr(0,pos1);
	//�ǳ�
    string nickName = proFile.substr(pos1,pos2 - pos1); 
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("true_name",true);
	//֤������
	string cardType = "���֤";
	//���֤��
	string idCard = ms_->GetValueFromMapByKey("id_card");
	//ͨѶ����
	string connectType = "QQ";
	//qq��
	string qq = ms_->GetValueFromMapByKey("qq");
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//�ʱ� (����)
    string postcode = ms_->GetValueFromMapByKey("postcode");

	output_text_ =
		"user=" + userName + "\r\n" +			//20	[13]	//�û��� 
		"userid=" + userId + "\r\n" + 			//28    [92]    //�û�ID
		"msg=" + 	nickName + "\r\n" + 		//65	[104]   //�ǳ�
		"mid=" + 	name + "\r\n" + 			//56	[121]	//����
		"servername=" + cardType + "\r\n" + 	//49	[116]   //֤������      
		"appname=" + idCard + "\r\n" + 			//50	[115]   //֤����
		"msgtype=" + connectType + "\r\n" +		//51	[127]	//ͨѶ����
		"subject=" + qq + "\r\n" + 				//52	[46]    //ͨѶ��
		"islogin=" + address + "\r\n" + 		//53	[118]   //��ַ
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			    //64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��׿�ֻ�ע��
int marriage::JiaYuanMarriage::AndroidRegiste(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//���� 
	string password = ms_->GetValueFromMapByKey("password");
	string chkpass = ms_->GetValueFromMapByKey("chkpass");
	if (strcmp(password.c_str(),chkpass.c_str()) != 0)
	{
		return 0;
	}
	//�ǳ� 
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//�Ա� 
	string sex = ms_->GetValueFromMapByKey("sex");
	if (sex == "m")
	{
		sex = "��";
	}
	else 
	{
		sex = "Ů";
	}
	//��������
	string birthdate = ms_->GetValueFromMapByKey("year") + "-" +
					   ms_->GetValueFromMapByKey("month") + "-" +
					   ms_->GetValueFromMapByKey("day");
	if (mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" +	mobile + "\r\n" + 				//54	[119]	//�ֻ��� 
		"pass=" +	password + "\r\n" + 			//21	[14]	//���� 
		"msg=" +	nickname + "\r\n" + 			//65	[104]   //�ǳ�
		"sex=" +	sex + "\r\n" + 					//60	[125]	//�Ա� 
		"onick=" +	birthdate + "\r\n" + 			//55	[120]	//�������� 
		"netmark=" +	app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +  //62	[126]	//����ƽ̨
		"state=ע��\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

