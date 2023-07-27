#include "StdAfx.h"
#include "EmployRuoLin.h"

employ::RuoLin::RuoLin(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "������";
}

employ::RuoLin::~RuoLin()
{
	
}

int employ::RuoLin::IsRuoLin(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/passport/register/registerEmail")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/passport/login")!=-1) )
	{
		return kPcWebLogin;
	}
	//������Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/simple/basicSaveAjax")!=-1) ) 
	{
		return kPcWebPaMain;
	}
	//�޸Ļ�����Ϣ	 
	if((http_session->m_Requestline.m_Host.Find("www.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/basic/saveAjax")!=-1) ) 
	{
		return kPcWebResume;
	}
	//�ϴ�ͷ��	
	if((http_session->m_Requestline.m_Host.Find("upload.qiniu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	//android��¼
	if((http_session->m_Requestline.m_Host.Find("api5.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Passport/login")!=-1) )
	{
		return kAndroidLogin;
	}
	//android�޸Ļ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("m.wealink.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/resume/info/save")!=-1) ) 
	{
		return kAndroidResume;
	}
	return 0;
}

int employ::RuoLin::Process(const char *packet,int action)
{
	//����ע��
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//��¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//������Ϣ
	if (action == kPcWebPaMain)
	{
		return PcWebPaMain(packet);
	}
	//�޸Ļ�����Ϣ
	if (action == kPcWebResume)
	{
		return PcWebResume(packet);
	}
	//�ϴ�ͷ��
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	//android��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android�޸Ļ�����Ϣ
	if (action == kAndroidResume)
	{
		return AndroidResume(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::RuoLin::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
    string txtEmail = ms_->GetValueFromMapByKey("email",true);
	//����
	string txtPassword = ms_->GetValueFromMapByKey("password",true);
	if (txtEmail.empty() || txtPassword.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + txtEmail + "\r\n" + 		//����		
		"pass=" + 	txtPassword + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 		//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int employ::RuoLin::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("username",true);
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


//������Ϣ
int employ::RuoLin::PcWebPaMain(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("data[0][user_name]",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("data[0][living_city_code]",true);
	//�ֻ�����
	string mobile = ms_->GetValueFromMapByKey("data[0][mobile]",true);
	output_text_ =
		"servername=" + name + "\r\n" + 	//����	       
		"nick="	 + 	  	address + "\r\n" + 	//��ַ	
		"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
		"netmark=" +	app_ + "\r\n" + 	//��Դ
		"state=�½�������Ϣ\r\n";
	WriteIndexFile();    
	return 1;
}

//�޸Ļ�����Ϣ
int employ::RuoLin::PcWebResume(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("user_name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "M")
	{
		gender = "��";
	}
	if (gender == "F")
	{
		gender = "Ů";
	}
	//��������		//199101
	string birth = ms_->GetValueFromMapByKey("birth_year",true) + "-" + 
				ms_->GetValueFromMapByKey("birth_mon",true) + "-" + 
				ms_->GetValueFromMapByKey("birth_day",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("living_city_code",true);
	//�������ڵ�	//320802
	//address_ = ms_->GetValueFromMapByKey("birth_city_code",true);
	//�ɳ���		//3208
	//string GrowPlace = ms_->GetValueFromMapByKey("GrowPlace",true);
	output_text_ =
		"servername=" + name + "\r\n" + 	//����	       
		"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
		"msg=" + 	  	birth + "\r\n" + 	//��������	
		"nick="	 + 	  	address + "\r\n" + 	//��ַ	
		"netmark=" +	app_ + "\r\n" + 	//��Դ
		"state=�޸Ļ�����Ϣ\r\n";
	WriteIndexFile();    
	return 1;
}

//�ϴ�ͷ��
int employ::RuoLin::PcWebPaPhoto(const char *packet)
{	
	from_ = "������null";
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"file\"");          
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

//android��¼
int employ::RuoLin::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("username",true);
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

//android�޸Ļ�����Ϣ
int employ::RuoLin::AndroidResume(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("user_name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("sex",true);
	if (gender == "M")
	{
		gender = "��";
	}
	if (gender == "F")
	{
		gender = "Ů";
	}
	//��������		//199101
	string birth = ms_->GetValueFromMapByKey("birth",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("living_city_code",true);
	//�������ڵ�	//320802
	//address_ = ms_->GetValueFromMapByKey("birth_city_code",true);
	//�ɳ���		//3208
	//string GrowPlace = ms_->GetValueFromMapByKey("GrowPlace",true);
	output_text_ =
		"servername=" + name + "\r\n" + 	//����	       
		"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
		"msg=" + 	  	birth + "\r\n" + 	//��������	
		"nick="	 + 	  	address + "\r\n" + 	//��ַ	
		"netmark=" +	app_ + "\r\n" + 	//��Դ
		"state=�޸Ļ�����Ϣ\r\n";
	WriteIndexFile();    
	return 1;
}
