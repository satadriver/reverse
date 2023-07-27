#include "StdAfx.h"
#include "MarriageZhenQingZaiXian.h"

marriage::ZhenQingZaiXianMarriage::ZhenQingZaiXianMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "��������";	
}

marriage::ZhenQingZaiXianMarriage::~ZhenQingZaiXianMarriage()
{
	
}

int marriage::ZhenQingZaiXianMarriage::IsZhenQingZaiXianMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("www.lol99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg/index.php")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("www.lol99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/login.php")!=-1)) 
	{
		return kPcWebLogin;
	}
	//��ҳ�޸�����
	if((http_session->m_Requestline.m_Host.Find("www.lol99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/edit.php")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::ZhenQingZaiXianMarriage::Process(const char *packet,int action)
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

	//��ҳ�����޸�
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}
//��ҳע��
int marriage::ZhenQingZaiXianMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ�
	string nick = ms_->GetValueFromMapByKey("name",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("gender",true)=="0"?"��":"Ů";
	//��������
	string birthday = ms_->GetValueFromMapByKey("birth_year",true) + "-" +
		              ms_->GetValueFromMapByKey("birth_month",true) + "-" + 
					  ms_->GetValueFromMapByKey("birth_day",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//��ʵ����
	string truename = ms_->GetValueFromMapByKey("truename",true);
	//���֤
	string idcard = ms_->GetValueFromMapByKey("sfcard",true);
	output_text_ =
		"msg=" +nick + "\r\n" +              //�ǳ�
		"sex=" +sex + "\r\n" +               //�Ա�
		"onick=" + birthday + "\r\n" +        //��������
		"oid=" +mobile + "\r\n" +            //�ֻ���
		"nick=" +email + "\r\n" +            //����
		"mid=" +truename + "\r\n" +          //��ʵ����
		"servername=" +"���֤" + "\r\n" +   //֤������
		"appname=" +idcard + "\r\n" +        //���֤��
 		"netmark=" + app_ + "\r\n" + 	     //��Դ
		"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
		"state=ע��\r\n";			         //״̬
	WriteIndexFile();
	return 1;
}

//web��¼
int marriage::ZhenQingZaiXianMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string username = ms_->GetValueFromMapByKey("mobile",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +          //�û���
		"pass=" + pwd + "\r\n" +               //����
		"netmark=" + app_ + "\r\n" + 		   //��Դ
		"clientmsg=" + platformWeb + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";			           //״̬
	WriteIndexFile();
	return 1;
}

//��ϸ���ϸ�����Ϣ
int marriage::ZhenQingZaiXianMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	string name = ms_->GetValueFromMapByKey("truename",true);
	//email
	string email = ms_->GetValueFromMapByKey("email",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	output_text_ =
		"mid=" +name + "\r\n" +                 //����
		"nick=" + email + "\r\n" +              //����
		"islogin=" +address + "\r\n" +          //��ַ
		"netmark=" + app_ + "\r\n" + 		    //��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			    //״̬
	WriteIndexFile();
	return 1;
}

