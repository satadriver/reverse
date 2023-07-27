#include "StdAfx.h"
#include "EmployZhiTong.h"

employ::ZhiTong::ZhiTong(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "��ͨ�˲���";
}

employ::ZhiTong::~ZhiTong()
{
	
}

int employ::ZhiTong::IsZhiTong(HttpSession *http_session)
{
	//ע�������Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.job5156.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("per/new/step1/save")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("my.job5156.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/per/resume")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}
int employ::ZhiTong::Process(const char *packet,int action)
{
	//ע�������Ϣ
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//�޸Ļ�����Ϣ
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//ע�������Ϣ
int employ::ZhiTong::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("userName",true);
	string sex = ms_->GetValueFromMapByKey("gender",true);
	if (sex == "1"){sex ="��";}
	else{sex = "Ů";}
	string birth = ms_->GetValueFromMapByKey("birthday",true);
	string location = ms_->GetValueFromMapByKey("name_location",true);
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	string idtype = ms_->GetValueFromMapByKey("cardType",true);
	if (idtype == "1")
	{
		idtype = "���֤";
	}
	string cardNum = ms_->GetValueFromMapByKey("cardNum",true);
	output_text_=
		"servername=" + username + "\r\n" + 	//����	        
		"sex=" + sex + "\r\n" + 		//�Ա�	
		"msg=" + birth	+ "\r\n" + 		//��������	
		"oid=" + mobile + "\r\n" + 		//�ֻ���
		"imsi=" + idtype + "\r\n" + 	//֤������	
		"appname=" + cardNum + "\r\n" + //֤����	
        "nick="	 + location + "\r\n" + 	//��ַ		
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=�½�������Ϣ\r\n";  
	WriteIndexFile();
	return 1;
}


//�޸Ļ�����Ϣ
int employ::ZhiTong::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string name = ms_->GetValueFromMapByKey("userName",true);
	string gender = ms_->GetValueFromMapByKey("gender",true);
	if (gender == "1")
	{
		gender = "��";
	}else
	{
		gender = "Ů";
	}
	string birthday = ms_->GetValueFromMapByKey("birthday",true);
	string location = ms_->GetValueFromMapByKey("name_location",true);
	string email = ms_->GetValueFromMapByKey("email",true);
	string cardType = ms_->GetValueFromMapByKey("cardType",true);
	if (cardType == "1")
	{
		cardType = "���֤";
	}
	string cardid = ms_->GetValueFromMapByKey("cardNum",true);
	output_text_=
		"servername=" + name + "\r\n" + //����	        
		"sex=" + gender + "\r\n" + 		//�Ա�	
		"msg=" + birthday + "\r\n" + 	//��������
		"imsi=" + cardType + "\r\n" + 	//֤������	
		"appname=" + cardid + "\r\n" + 	//֤����	
        "nick="	 + location + "\r\n" + 	//��ַ
		"mid=" + email + "\r\n" + 		//����	
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=�޸Ļ�����Ϣ\r\n";  
	WriteIndexFile();
	return 1;
}