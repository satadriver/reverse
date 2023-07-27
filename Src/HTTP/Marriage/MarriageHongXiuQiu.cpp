#include "StdAfx.h"
#include "MarriageHongXiuQiu.h"

marriage::HongXiuQiuMarriage::HongXiuQiuMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "������";	
}

marriage::HongXiuQiuMarriage::~HongXiuQiuMarriage()
{
	
}

int marriage::HongXiuQiuMarriage::IsHongXiuQiuMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("www.xiuqiu99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register.aspx")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("www.xiuqiu99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login.aspx")!=-1)) 
	{
		return kPcWebLogin;
	}
	//��ҳ�޸�����
	if((http_session->m_Requestline.m_Host.Find("www.xiuqiu99.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/setProfile.aspx")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::HongXiuQiuMarriage::Process(const char *packet,int action)
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
int marriage::HongXiuQiuMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ֻ�
	string tbphone = ms_->GetValueFromMapByKey("tbPhone",true);
	//��¼��
	string username = ms_->GetValueFromMapByKey("tbEmail",true);
	//�ǳ�
	string nick = ms_->GetValueFromMapByKey("tbNicheng",true);
	//����
	string password = ms_->GetValueFromMapByKey("tbPwd",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("rblSex",true);

	output_text_ =
		"user=" +   username + "\r\n" +           //�û��� 
        "pass=" +   password + "\r\n" +           //����  
		"msg=" +    nick + "\r\n" +               //�ǳ�
		"oid=" +     tbphone + "\r\n" +           //�ֻ��� 
		"sex=" +     sex + "\r\n" +               //�Ա� 
		"netmark=" + app_ + "\r\n" + 	          //��Դ
		"clientmsg=" + platformWeb + "\r\n" +     //����ƽ̨
		"state=ע��\r\n";			              //״̬
	WriteIndexFile();
	return 1;
}

//web��¼
int marriage::HongXiuQiuMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}	
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("tbEmail",true);
	string pwd = ms_->GetValueFromMapByKey("tbPwd");
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
int marriage::HongXiuQiuMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ʵ����
	string truename = ms_->GetValueFromMapByKey("tbTrueName",true);
	//���֤
	string idcard = ms_->GetValueFromMapByKey("tbSFZ",true);
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("tbTel",true);
	//QQ
	string QQ = ms_->GetValueFromMapByKey("tbQQ",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("tbAddr",true);

	output_text_ =
		"mid=" + truename + "\r\n" +                //����	
        "servername=" + "���֤"  + "\r\n" +       //֤������     
		"appname=" +     idcard + "\r\n" +         //֤����
		"oid=" +     phone + "\r\n" +              //�ֻ��� 
		"msgtype=" +    "QQ" + "\r\n" +            //ͨѶ����
        "subject=" +     QQ + "\r\n" +             //ͨѶ��
		"islogin=" +     address + "\r\n" +        //��ַ
		"netmark=" + app_ + "\r\n" + 		       //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	   //����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			       //״̬
	WriteIndexFile();
	return 1;
}

