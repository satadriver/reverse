#include "StdAfx.h"
#include "EmployZhongHuaYingCai.h"
#include "../BoundaryParser.h"

employ::ZhongHuanYingCai::ZhongHuanYingCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "�л�Ӣ����";	
}

employ::ZhongHuanYingCai::~ZhongHuanYingCai()
{
	
}
//��ȡ֤������
string employ::ZhongHuanYingCai::GetIdType(string idType)
{
	if (idType=="1"){	idType = "���֤";}
	if (idType=="2"){	idType = "����";}
	if (idType=="3"){	idType = "����֤";}
	if (idType=="7"){	idType = "������֤";}
	if (idType=="8"){	idType = "�������֤";}
	if (idType=="5"){	idType = "�۰�ͨ��֤";}
	if (idType=="6"){	idType = "̨��֤";}
	if (idType=="4"){	idType = "����";}
	return idType;
}
int employ::ZhongHuanYingCai::IsZhongHuanYingCai(HttpSession *http_session)
{
	//������Ϣ
	if((http_session->m_Requestline.m_Host.Find("my.chinahr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cv/ajax/createCvFirst")!=-1) )
	{
		return kPcWebInfo;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("my.chinahr.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/saveBasic")!=-1) )
	{
		return kPCWebModiy;
	}
	return 0;
}

int employ::ZhongHuanYingCai::Process(const char *packet,int action)
{
	//������Ϣ
	if (action == kPcWebInfo)
	{
		return PcWebInfo(packet);
	}
	if (action==kPCWebModiy)
	{
		return PCWebModiy(packet);
	}
	return 1;
}

//������Ϣ
int employ::ZhongHuanYingCai::PcWebInfo(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("basicInfo.name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("basicInfo.gender",true);
	if (gender == "1"){gender = "��";}
	if (gender == "2"){gender = "Ů";}
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("basicInfo.mobile",true);
	//����
	string birth = ms_->GetValueFromMapByKey("birth",true);
	//�־�ס��
	string address = ms_->GetValueFromMapByKey("basicInfo.living.fullpath",true);
	//�μ�����ʱ��(����)
	string workTime = ms_->GetValueFromMapByKey("workTime",true);
	output_text_=
		"servername=" + name + "\r\n" + 	//����	       
		"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
		"msg=" + 	  	birth + "\r\n" + 	//��������	
		"nick="	 + 	  	address + "\r\n" + 	//��ַ	
		"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
		"netmark=" +	app_ + "\r\n" + 	//��Դ
		"state=�½�������Ϣ\r\n";
	WriteIndexFile();
	return 1;
}
//�޸ĸ�����Ϣ
int employ::ZhongHuanYingCai::PCWebModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("basicInfo.name",true);
	//�Ա�
	string gender = ms_->GetValueFromMapByKey("basicInfo.gender",true);
	if (gender == "1")
	{
		gender = "��";
	}
	if (gender == "2")
	{
		gender = "Ů";
	}
	//����
	string birth = ms_->GetValueFromMapByKey("birth",true);
	//�־�ס��
	string address = ms_->GetValueFromMapByKey("basicInfo.living.fullpath",true);
	//�ֻ�
	string mobile = ms_->GetValueFromMapByKey("basicInfo.mobile",true);
	//����
	string email = ms_->GetValueFromMapByKey("basicInfo.email",true);
	if (name.empty() || gender.empty() || birth.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_=
		"servername=" + name + "\r\n" + 	//����	       
		"sex=" + 	  	gender + "\r\n" + 	//�Ա�		
		"msg=" + 	  	birth + "\r\n" + 	//��������	
		"nick="	 + 	  	address + "\r\n" + 	//��ַ	
		"oid=" + 	  	mobile + "\r\n" + 	//�ֻ���
		"mid=" + 	  	email + "\r\n" + 	//����	
		"netmark=" +	app_ + "\r\n" + 	//��Դ
		"state=�޸Ļ�����Ϣ\r\n";
	WriteIndexFile();
	return 1;
}