#include "StdAfx.h"
#include "EmployZhiLianZhaoPin.h"
#include "../BoundaryParser.h"

employ::ZhiLianZhaoPin::ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "����׿Ƹ";
}

employ::ZhiLianZhaoPin::~ZhiLianZhaoPin()
{
	
}

int employ::ZhiLianZhaoPin::IsZhiLianZhaoPin(HttpSession *http_session)
{
	//����׿Ƹ��¼
	if (http_session->m_Requestline.m_Host.Find("c.highpin.cn") != -1 &&
		http_session->m_Requestline.m_URI.Find("/Users/CLogin") != -1)
	{
        return kPcWebLogin;
	}
	//����׿Ƹ������Ϣ(�½�/�޸�)
	if((http_session->m_Requestline.m_Host.Find("c.highpin.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Resumes/SaveResumeInfo/")!=-1) )
	{
		return kPcWebInfo;
	}
	//�ϴ�ͷ��
	if((http_session->m_Requestline.m_Host.Find("c.highpin.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Handler/SeekerPhotoReceiver.ashx")!=-1) )
	{
		return kPcWebPaPhoto;
	}
	return 0;
}

int employ::ZhiLianZhaoPin::Process(const char *packet,int action)
{
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	if (action== kPcWebInfo)
	{
		return PcWebInfo(packet);
	}
	if (action == kPcWebPaPhoto)
	{
		return PcWebPaPhoto(packet);
	}
	return 1;
}

int employ::ZhiLianZhaoPin::PcWebLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,"&","=");
	//�û���
	string username = ms_->GetValueFromMapByKey("Logon_UserEmail",true);
	//����
	string pass = ms_->GetValueFromMapByKey("Logon_Password",true);
	if (username.empty() || pass.empty())
	{
		return 0;
	}
	output_text_=
		"user=" + 	username + "\r\n" +	//�˺�			 
		"pass=" + 	pass + "\r\n" + 	//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=��¼\r\n";  
		WriteIndexFile();
	return 0;
}
int employ::ZhiLianZhaoPin::PcWebInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	string ResumeID = ms_->GetValueFromMapByKey("ResumeID");
	//����׿Ƹ�޸ĸ�����Ϣ
	if (atoi(ResumeID.c_str()) > 0)
	{
		//����   
        string name = ms_->GetValueFromMapByKey("Name");
		//�Ա� 
		string gender = ms_->GetValueFromMapByKey("Gender");
		if (gender == "1")
		{
			gender = "��";
		}
		if (gender == "2")
		{
			gender = "Ů";
		}
		//�������� 
		string birthdate = ms_->GetValueFromMapByKey("Birthday");
        //�ֻ���
		string mobile = ms_->GetValueFromMapByKey("Mobile");
		//����
		string loginemail = ms_->GetValueFromMapByKey("LogonEmail");

		if (name.empty() || gender.empty())
		{
			return 0;
		}
		output_text_=	
			"servername=" + name + "\r\n" +		 //����	       
			"sex=" + 	 gender + "\r\n" + 		 //�Ա�	
			"msg=" + 	 birthdate + "\r\n" + 	 //��������	
			"oid=" + 	mobile + "\r\n" + 	     //�ֻ���		
			"mid=" + 	loginemail + "\r\n" + 	 //����	
			"netmark=" + app_ + "\r\n" + 		 //��Դ
			"state=�޸ĸ�����Ϣ\r\n";  
		WriteIndexFile();
	}
    else
	{
		//����   
        string name = ms_->GetValueFromMapByKey("Name");
		//�Ա� 
		string gender = ms_->GetValueFromMapByKey("Gender");
		if (gender == "1")
		{
			gender = "��";
		}
		if (gender == "2")
		{
			gender = "Ů";
		}
		//�������� 
		string birthdate = ms_->GetValueFromMapByKey("Birthday");
        //�ֻ���
		string mobile = ms_->GetValueFromMapByKey("Mobile");
		//����
		string loginemail = ms_->GetValueFromMapByKey("LogonEmail");
		
		if (name.empty() || gender.empty())
		{
			return 0;
		}
		output_text_=	
			"servername=" + name + "\r\n" +		 //����	       
			"sex=" + 	 gender + "\r\n" + 		 //�Ա�	
			"msg=" + 	 birthdate + "\r\n" + 	 //��������	
			"oid=" + 	mobile + "\r\n" + 	     //�ֻ���		
			"mid=" + 	loginemail + "\r\n" + 	 //����	
			"netmark=" + app_ + "\r\n" + 		 //��Դ
			"state=�½�������Ϣ\r\n";  
		WriteIndexFile();
	}
	return 1;
}

//�ϴ�ͷ��
int employ::ZhiLianZhaoPin::PcWebPaPhoto(const char *packet)
{	
	from_ = "����׿Ƹ";
	//�ļ�����
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"photofile\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	if (attachment_.empty()){ return 0;	}
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}