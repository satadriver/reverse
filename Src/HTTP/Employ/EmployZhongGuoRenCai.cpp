#include "StdAfx.h"
#include "EmployZhongGuoRenCai.h"

employ::ZhongGuoRenCai::ZhongGuoRenCai(HttpSession *http_session, const string &pro_name) : Employ(http_session,pro_name)
{
	app_ = "�й��˲�����";
}

employ::ZhongGuoRenCai::~ZhongGuoRenCai()
{
	
}

int employ::ZhongGuoRenCai::IsZhongGuoRenCai(HttpSession *http_session)
{
	//ע��
	if((http_session->m_Requestline.m_Host.Find("www.cjol.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Jobseekers/register.aspx")!=-1) )
	{
		return kPcWebRegisterEmail;
	}
	//�޸ĸ�����Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.cjol.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jobseekers/Service/ResumeHandler.ashx")!=-1) )
	{
		return kPcWebModiy;
	}
	return 0;
}

int employ::ZhongGuoRenCai::Process(const char *packet,int action)
{
	//ע��(����ע��)
	if (action == kPcWebRegisterEmail)
	{
		return PcWebRegisterEmail(packet);
	}
	//�޸Ļ�����Ϣ
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}

//ע��(����ע��)
int employ::ZhongGuoRenCai::PcWebRegisterEmail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("tbxEmailAddress",true);
	string pwd = ms_->GetValueFromMapByKey("tbxPassword",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=="  + username + "\r\n" + 	//����	        
		"pass=" + pwd + "\r\n" +		//����
		"netmark=" + app_ + "\r\n" + 	//��Դ
		"state=ע��\r\n";
	WriteIndexFile();
 	return 1;
}

//�޸Ļ�����Ϣ
int employ::ZhongGuoRenCai::PcWebModiy(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("base64",true);
	int tmp_i = src_data.length();
	src_data = ms_->Base64Decode(src_data,tmp_i);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		string username = value["Name"].asString();
		string sex = value["Gender"].asString();
		if (sex == "1")
		{
			sex = "��";
		}else
		{
			sex = "Ů";
		}
		string tel = value["MobileNum"].asString();
		string email = value["Email"].asString();
		string birth = value["DateOfBirth"].asString();
		output_text_=
			"servername=" + username + "\r\n" + //����	      
			"sex=" + sex + "\r\n" + 	//�Ա�	
			"msg=" + birth + "\r\n" + 	//��������	
			"oid=" + tel + "\r\n" + 	//�ֻ���		
			"mid=" + email + "\r\n" + 	//����
			"netmark=" + app_ + "\r\n" + //��Դ
			"state=�޸Ļ�����Ϣ\r\n";
		WriteIndexFile();
	}
	return 1;
}