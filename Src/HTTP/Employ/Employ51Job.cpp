#include "StdAfx.h"
#include "Employ51Job.h"

employ::QianChengWuYou::QianChengWuYou(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "ǰ������";
}

employ::QianChengWuYou::~QianChengWuYou()
{
	
}

string employ::QianChengWuYou::GetIdType(string idType)
{
	if (idType=="0"){ idType="���֤";}
	if (idType=="1"){ idType="����";}
	if (idType=="2"){ idType="����֤";}
	if (idType=="4"){ idType="������֤";}
	if (idType=="3"){ idType="����";}
	return idType;
}

int employ::QianChengWuYou::IsQianChengWuYou(HttpSession *http_session)
{
	//ע�������Ϣ
	if((http_session->m_Requestline.m_Host.Find("i.51job.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/ajax/tri_resume.php")!=-1) )
	{
		return kPcWebInfo;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("i.51job.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/ajax/base_detail.php?action=save")!=-1) )
	{
		return kPCWebModiy;
	}
	//��¼
	if (http_session->m_Requestline.m_Host.Find("login.51job.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/ajax/login.php") != -1)
	{
		return kPcLogin;
	}
	//�ϴ�ͷ��	
	if((http_session->m_Requestline.m_Host.Find("i.51job.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/ajax/avatar.php")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	return 0;
}

int employ::QianChengWuYou::Process(const char *packet,int action)
{
	//������Ϣ
	if (action == kPcWebInfo)
	{
		return PcWebInfo(packet);
	}
	//�޸ĸ�����Ϣ
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	//��¼
	if (action == kPcLogin)
	{
		return PcLogin(packet);
	}
	//�ϴ�ͷ��
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	return 1;
}

//������Ϣ
int employ::QianChengWuYou::PcWebInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("cname",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "0")
	{
		gender = "��";
	}
	else
	{
		gender ="Ů";
	}
	//��������
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobilephone",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	if (name.empty() || birth.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + name + "\r\n" + 	//����	       
		"sex=" + 	gender + "\r\n" + 	//�Ա�	
		"msg=" + 	birth + "\r\n" + 	//��������	
		"oid=" + 	mobile + "\r\n" + 	//�ֻ���	
		"mid=" + 	email + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 	//��Դ	
		"state=�½�������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}

int employ::QianChengWuYou::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string name = ms_->GetValueFromMapByKey("cname",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "0"){gender = "��";}
	else{ gender ="Ů";}
	//��������
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	//�ֻ�
	string mobile = ms_->GetValueFromMapByKey("mobilephone",true);
	//����
	string email = ms_->GetValueFromMapByKey("email",true);
	if (name.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + name + "\r\n" + //����	       
		"sex=" + 	gender + "\r\n" + 	//�Ա�	
		"msg=" + 	birth + "\r\n" + 	//��������	
		"oid=" + 	mobile + "\r\n" + 	//�ֻ���	
		"mid=" + 	email + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 	//��Դ	
		"state=�޸Ļ�����Ϣ\r\n";    
	WriteIndexFile();
	return 1;
}

//��¼
int employ::QianChengWuYou::PcLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,"&","=");
	//�û���
	string username = ms_->GetValueFromMapByKey("loginname",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_=
		"user=" +    username + "\r\n" + //�˺�		
		"pass=" + 	 password + "\r\n" + //����
		"netmark=" + app_ + "\r\n" + 	 //��Դ	
		"state=��¼\r\n";    
	WriteIndexFile();
	return 0;
}

//�ϴ�ͷ��
int employ::QianChengWuYou::PcWebPaPhoto(const char *packet)
{	
	//cname=��������&|
	//��cookie �л������
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	from_ = ms_->GetValueBetweenBeginEnd(cookie,"cname=","&|",true);
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"avatar\"");          
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
