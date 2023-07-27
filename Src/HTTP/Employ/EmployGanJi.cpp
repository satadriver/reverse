#include "StdAfx.h"
#include "EmployGanJi.h"

employ::GanJi::GanJi(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "�ϼ���";	
}

employ::GanJi::~GanJi()
{
	
}

int employ::GanJi::IsGanJi(HttpSession *http_session)
{
	//ע��(����ע��)
	if((http_session->m_Requestline.m_Host.Find("www.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/register.php?method=phone_reg")!=-1) )
	{
		return kPcWebRegisterPhone;
	}
	//������Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax.php?module=pub_post_auto_save&act=add")!=-1) ) 
	{
		return kPcWebPaMain;
	}
	//�޸Ļ�����Ϣ	 
	if((http_session->m_Requestline.m_Host.Find("www.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pub/pub.php?")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("act=update&method=submit")!=-1)) 
	{
		return kPcWebResume;
	}
	//�ϴ�ͷ��	
	if((http_session->m_Requestline.m_Host.Find("upload.ganji.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upload.php")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	return 0;
}

int employ::GanJi::Process(const char *packet,int action)
{
	//����ע��
	if (action == kPcWebRegisterPhone)
	{
		return PcWebRegisterPhone(packet);
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
	return 1;
}

//ע��(�ֻ���ע��)
int employ::GanJi::PcWebRegisterPhone(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("username",true);
	//�ֻ���
    string phone = ms_->GetValueFromMapByKey("phone",true);
	//����
	string phone_password = ms_->GetValueFromMapByKey("phone_password",true);
	//ȷ������
	string phone_password2 = ms_->GetValueFromMapByKey("phone_password2",true);
	if (phone_password != phone_password2)
	{
		return 0;
	}
	if (phone.empty() || phone_password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +		    //�˺�	
		"oid=" + phone + "\r\n" + 				//�ֻ���	
		"pass=" + 	phone_password + "\r\n" + 	//����
		"netmark=" + app_ + "\r\n" + 			//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
	return 1;
}

//������Ϣ
int employ::GanJi::PcWebPaMain(const char *packet)
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
	//��Ҫ����Ϣ
	string post_info = ms_->GetValueFromMapByKey("post_info",true);
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	string name,birth,mobile,email;
	if (reader.parse(post_info,value))
	{	
		//����
		name = value["person"].asString();
		//��������
		birth = value["birthdate_year"].asString() + "-" + value["birthdate_month"].asString();
		//�ֻ�����
		mobile = value["phone"].asString();
		//����
		email = value["email"].asString();
		if (email.empty())
		{
			email = "null";
		}
	}
	output_text_ =
		"servername=" + name + "\r\n" + 	//����	              
		"msg=" + 	  	birth + "\r\n" + 	//��������	
		"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���	
		"mid=" + 	  	email + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + 		//��Դ
		"state=�½�������Ϣ\r\n";
	WriteIndexFile();    
	return 1;
}

//�޸Ļ�����Ϣ
int employ::GanJi::PcWebResume(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//boundary ֱ����ȡ
	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	 
	//����
	string name = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"person\"\r\n\r\n",boundary,true);
	//�ֻ���
	string mobile = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"phone\"\r\n\r\n",boundary,true);
	//��������
	string birth = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"birthdate_year\"\r\n\r\n",boundary,true) + "-" + 
				   ms_-> GetValueBetweenBeginEnd(src_data,"name=\"birthdate_month\"\r\n\r\n",boundary,true);
	//����
	string email = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"email\"\r\n\r\n",boundary,true);

	output_text_ =
		"servername=" + name + "\r\n" + 	//����	        
		"msg=" + 	  	birth + "\r\n" + 	//��������
		"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���	
		"mid=" + 	  	email + "\r\n" + 	//����
		"netmark=" +	app_ + "\r\n" + 	//��Դ
		"state=�޸Ļ�����Ϣ\r\n";
	WriteIndexFile();    
	return 1;
}

//�ϴ�ͷ��
int employ::GanJi::PcWebPaPhoto(const char *packet)
{	
	from_ = "�ϼ���null";
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
