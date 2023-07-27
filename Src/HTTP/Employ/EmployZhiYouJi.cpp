#include "StdAfx.h"
#include "EmployZhiYouJi.h"

employ::ZhiYouJi::ZhiYouJi(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "ְ�Ѽ�";	
}

employ::ZhiYouJi::~ZhiYouJi()
{
	
}

int employ::ZhiYouJi::IsZhiYouJi(HttpSession *http_session)
{
	//������Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.ZhiYouJi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax.php?module=pub_post_auto_save&act=add")!=-1)) 
	{
		return kPcWebPaMain;
	}
	//�޸Ļ�����Ϣ 
	if((http_session->m_Requestline.m_Host.Find("www.jobui.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/process/")!=-1)) 
	{
		return kPcWebResume;
	}
	return 0;
}

int employ::ZhiYouJi::Process(const char *packet,int action)
{
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
	return 1;
}

//������Ϣ
int employ::ZhiYouJi::PcWebPaMain(const char *packet)
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
int employ::ZhiYouJi::PcWebResume(const char *packet)
{
	//�ж� �ϴ�ͼƬ ���� ������Ϣ	
	//��referer ����ȡ����
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	if (referer.find("photo") == string::npos)
	{
		//��ȷ��ȡ����  
		string src_data(packet);
		if (src_data.rfind("\r\n") != 0)
		{
			src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
		}
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string formAction = ms_->GetValueFromMapByKey("formAction",true);
		//������Ϣ ���� ��ַ
		if (!strcmp("user_profile_editInfo",formAction.c_str()))
		{
			string name = ms_->GetValueFromMapByKey("userName",true);
			//���ڵ��� (���б���)
			string address = ms_->GetValueFromMapByKey("cityCode",true);
			output_text_ =
				"servername=" + name + "\r\n" + 	//����	        
				"nick="	 + 	 address + "\r\n" +     //��ַ		
				"netmark=" + app_ + "\r\n" + 	    //��Դ
				"state=�޸Ļ�����Ϣ\r\n";
			WriteIndexFile();    
		}
		//��ϵ��Ϣ 
		if (!strcmp("user_profile_editContact",formAction.c_str()))
		{
			//�ֻ���
			string mobile = ms_->GetValueFromMapByKey("phone",true);
			//����
			string email = ms_->GetValueFromMapByKey("email",true);
			output_text_ =
				"oid=" + 	 mobile + "\r\n" + 	//�ֻ���
				"mid=" + 	 email + "\r\n" + 	//����	
				"netmark=" + app_ + "\r\n" + 	//��Դ
				"state=�޸Ļ�����Ϣ\r\n";
			WriteIndexFile();    
		}  
	}
	//�ϴ�ͷ��
	if (referer.find("photo") != string::npos)
	{
		from_ = "ְ�Ѽ�null";
		//�ļ�����
		string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
		BoundaryParser bp;
		bp.Parse(packet, http_session_->m_datalen, boundary); 
		BoundaryValue *bv = NULL;
		string key;
		key = bp.GetBoundaryKey("name=\"userPhoto\"");          
		bv = bp.GetBoundaryValue(key);	
		if (bv == NULL || key.empty())
		{
			return 0;
		}
		attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
		string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
		ConstructUploadAttachmentOuptputString();
		WriteIndexFile(real_file_name);
	}
	return 1;
}


