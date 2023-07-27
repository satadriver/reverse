#include "StdAfx.h"
#include "MarriageTongChengYueHui.h"

marriage::TongChengMarriage::TongChengMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "ͬ��Լ��";	
}

marriage::TongChengMarriage::~TongChengMarriage()
{
	
}

int marriage::TongChengMarriage::IsTongChengMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register3.do")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ�޸�����
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myinfobasic.do")!=-1)) 
	{
		return kPcWebModiy;
	}
	//Androidע��
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/app/appregister2.do")!=-1)) 
	{
		return kAndroidRegister;
	}
	//Android�޸�
	if((http_session->m_Requestline.m_Host.Find("yuehui.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/app/updateuserbase.do")!=-1)) 
	{
		return kAndroidModiy;
	}
	return 0;
}

int marriage::TongChengMarriage::Process(const char *packet,int action)
{
	//��ҳע��
	if (action == kPcWebRegister)
	{
		return PcWebRegister(packet);
	}
	//��ҳ�޸ĸ�������
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//Androidע��
	if (action == kAndroidRegister)
	{
		return AndroidRegister(packet);
	}
	//Android�޸�
	if (action == kAndroidModiy)
	{
		return AndroidModiy(packet);
	}
	return 1;
}
//pcע��
int marriage::TongChengMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�˺�
	string account = ms_->GetValueFromMapByKey("account",true);
	//�ǳ�
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true);
	if (sex == "1")
	{
		sex = "��";
	}
	else
	{
		sex = "Ů";
	}
	//��������
	string birthday = ms_->GetValueFromMapByKey("byear",true)+"-"+
					  ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true)+"-";
	output_text_ =
	    "user=" + account + "\r\n" +         //�û���
	    "msg=" + nick + "\r\n" +             //�ǳ�
		"sex=" +sex + "\r\n" +               //�Ա�
		"onick=" + birthday + "\r\n" +       //��������
		"netmark=" + app_ + "\r\n" + 	     //��Դ
		"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
		"state=ע��\r\n";			         //״̬
	WriteIndexFile();
	return 1;
}
//��ϸ���ϸ�����Ϣ
int marriage::TongChengMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�û�
	string account = ms_->GetValueFromMapByKey("account",true);
	//�ǳ�
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true)=="1"?"��":"Ů";
	//��������
	string birthday = ms_->GetValueFromMapByKey("byear",true)+"-"+
		              ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true)+"-";
	output_text_ =
	    "user=" + account + "\r\n" +          //�û���
	    "msg=" + nick + "\r\n" +             //�ǳ�
		"sex=" +sex + "\r\n" +               //�Ա�
		"onick=" + birthday + "\r\n" +       //��������
		"netmark=" + app_ + "\r\n" + 	     //��Դ
		"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			 //״̬
	WriteIndexFile();
	return 1;
}
//Androidע��
int marriage::TongChengMarriage::AndroidRegister(const char* packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ�
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true)=="1"?"��":"Ů";
	//��������
	string birthday = ms_->GetValueFromMapByKey("byear",true)+"-"+
		              ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true);
	//�û���
	string account = ms_->GetValueFromMapByKey("account",true);
	output_text_ =
		"user=" + account + "\r\n" +          //�û���
		"msg=" + nick + "\r\n" +             //�ǳ�
		"sex=" +sex + "\r\n" +               //�Ա�
		"onick=" + birthday + "\r\n" +       //��������
		"netmark=" + app_ + "\r\n" + 	     //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +//����ƽ̨
		"state=ע��\r\n";			         //״̬
	WriteIndexFile();
	return 1;
}
//Android�޸�
int marriage::TongChengMarriage::AndroidModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�ǳ�
	string nick = ms_->GetValueFromMapByKey("nick",true);
	//�˻�
	string account = ms_->GetValueFromMapByKey("account",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex",true)=="1"?"��":"Ů";
	//��������
	string birhtday = ms_->GetValueFromMapByKey("byear",true)+"-"+
		              ms_->GetValueFromMapByKey("bmonth",true)+"-"+
					  ms_->GetValueFromMapByKey("bday",true);
	output_text_ =
		"user=" + account + "\r\n" +          //�û���
		"msg=" + nick + "\r\n" +             //�ǳ�
		"sex=" +sex + "\r\n" +               //�Ա�
		"onick=" + birhtday + "\r\n" +       //����
		"netmark=" + app_ + "\r\n" + 	     //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			         //״̬
	WriteIndexFile();
	return 1;
}

