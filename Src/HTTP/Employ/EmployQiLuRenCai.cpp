#include "StdAfx.h"
#include "EmployQiLuRenCai.h"

employ::QiLuRenCai::QiLuRenCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "��³�˲���";	
}

employ::QiLuRenCai::~QiLuRenCai()
{
	
}

int employ::QiLuRenCai::IsQiLuRenCai(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Personal/sys/Register")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//ע��(�ֻ���ע��)
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Personal/sys/MobileRegister")!=-1) )
	{
		return kPcWebRegisterPhone;
	}
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/personal/sys/login")!=-1) )
	{
		return kPcWebLogin;
	}
	//������Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.qlrc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Personal/Account/SavePaMain")!=-1) )
	{
		return kPcWebPaMain;
	}
	//�ϴ�ͷ��	
	if((http_session->m_Requestline.m_Host.Find("down.51rc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ImagesManager/PaPhotoProc2.asp")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	//�ϴ����� 
	if((http_session->m_Requestline.m_Host.Find("down.51rc.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ImagesManager/AttachmentQuickProcNew.asp")!=-1) )
	{
		return kPcWebPaResume;
	}
	return 0;
}

int employ::QiLuRenCai::Process(const char *packet,int action)
{
	//ע��(����ע��)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//ע��(�ֻ���ע��)
	if (action == kPcWebRegisterPhone)
	{
		return PcWebRegisterPhone(packet);
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
	//�ϴ�ͷ��
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	//�ϴ�����
	if (action == kPcWebPaResume)
	{
		return PcWebPaResume(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::QiLuRenCai::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
    string txtEmail = ms_->GetValueFromMapByKey("txtEmail",true);
	//����
	string txtPassword = ms_->GetValueFromMapByKey("txtPassword",true);
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

//ע��(�ֻ���ע��)
int employ::QiLuRenCai::PcWebRegisterPhone(const char *packet)
{
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ֻ���
    string txtMobile = ms_->GetValueFromMapByKey("txtMobile",true);
	//����
	string txtMobilePassword = ms_->GetValueFromMapByKey("txtMobilePassword",true);
	if (txtMobile.empty() || txtMobilePassword.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + txtMobile + "\r\n" + 			//�ֻ���		
		"pass=" + txtMobilePassword + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 			//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//��¼
int employ::QiLuRenCai::PcWebLogin(const char *packet)
{
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
    string UserName = ms_->GetValueFromMapByKey("UserName",true);
	//����
	string Password = ms_->GetValueFromMapByKey("Password",true);
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
int employ::QiLuRenCai::PcWebPaMain(const char *packet)
{
	//��cookie �л������
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');	
	string PaLoginID = ms_->GetValueFromMapByKey("PaLoginID",true);
	//http://www.qlrc.com/personal/cv/stepOne/26718570
	//http://www.qlrc.com/Personal/cv/Resume/26718465
	//��referer �л������
	BOOL falg;
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
    if (referer.find("stepOne") != string::npos)
    {
		falg = TRUE;
    }
	if (referer.find("Resume") != string::npos)
	{
		falg = FALSE;
	}
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("Name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("Gender",true);
	if (gender == "0")
	{
		gender = "��";
	}
	if (gender == "1")
	{
		gender = "Ů";
	}
	//��������		//199101
	string birth = ms_->GetValueFromMapByKey("BirthDay",true);
	//�־�ס��		//3208
	string address = ms_->GetValueFromMapByKey("LivePlace",true);
	//�������ڵ�	//320802
	//address_ = ms_->GetValueFromMapByKey("AccountPlace",true);
	//�ɳ���		//3208
	//string GrowPlace = ms_->GetValueFromMapByKey("GrowPlace",true);
	//�ֻ�����
	string mobile = ms_->GetValueFromMapByKey("Mobile",true);
	//��������
	string email = ms_->GetValueFromMapByKey("Email",true);
	if (email.empty())
	{
		email = "null";
	}
	if (falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"mid=" + 	  	email + "\r\n" + 	//����		
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=�޸Ļ�����Ϣ\r\n";
	}
	if (!falg)
	{
		output_text_ =
			"servername=" + name + "\r\n" + 	//����	       
			"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
			"msg=" + 	  	birth + "\r\n" + 	//��������	
			"nick="	 + 	  	address + "\r\n" + 	//��ַ		
			"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
			"mid=" + 	  	email + "\r\n" + 	//����	
			"netmark=" +	app_ + "\r\n" + 	//��Դ
			"state=�޸Ļ�����Ϣ\r\n";
	}
	WriteIndexFile();    
	return 1;
}

//�ϴ�ͷ��
int employ::QiLuRenCai::PcWebPaPhoto(const char *packet)
{	
	//��uri����ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string userTemp = ms_->GetValueFromMapByKey("un", true);
	from_ = userTemp.substr(0,userTemp.find("_"));

	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upfile\"");          
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

//�ϴ�����
int employ::QiLuRenCai::PcWebPaResume(const char *packet)
{	
	//��uri����ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	from_ = ms_->GetValueFromMapByKey("paMainID", true);
	
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upfile\"");          
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
