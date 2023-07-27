#include "StdAfx.h"
#include "MarriageIZhenXin.h"

marriage::IZhenXInMarriage::IZhenXInMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "������";	
}

marriage::IZhenXInMarriage::~IZhenXInMarriage()
{
	
}

int marriage::IZhenXInMarriage::IsIZhenXInMarriage(HttpSession *http_session)
{
	//�ֻ���¼
	if((http_session->m_Requestline.m_Host.Find("c.izhenxin.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cmiajax/?mod=login&func=doLogin")!=-1)) 
	{
		return kAndroidLogin;
	}
	//�ֻ��޸�
	if((http_session->m_Requestline.m_Host.Find("c.izhenxin.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cmiajax/?mod=profile&func=basicinfo_save")!=-1)) 
	{
		return kAndroidModiy;
	}
	return 0;
}

int marriage::IZhenXInMarriage::Process(const char *packet,int action)
{
	//�ֻ���¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//�ֻ��޸�
	if (action == kAndroidModiy)
	{
		return AndroidModiy(packet);
	}
	return 1;
}

//�ֻ���¼
int marriage::IZhenXInMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string form = ms_->GetValueFromMapByKey("form",true);
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(form,value))
	{
		return 0;
	}
	string login_name = value["username"].asString();
	string password = value["password"].asString();
	double lon = value["lng"].asDouble();
	double lat = value["lat"].asDouble();
	CString lng,lut;
	lng.Format("%f",lon);
	lut.Format("%f",lat);
	string l = lng.GetBuffer(0);
	string la = lut.GetBuffer(0);
	output_text_ =
		"user=" +   login_name + "\r\n" +       //�û��� 
		"pass=" +   password  + "\r\n" +        //����  
		"from=" +     l + "\r\n" +				//����  
        "to=" +     la + "\r\n" +				//γ��
		"netmark=" + app_ + "\r\n" + 	        //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +//����ƽ̨
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;

}
//�ֻ��޸�
int marriage::IZhenXInMarriage::AndroidModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string form = ms_->GetValueFromMapByKey("form",true);
	Json::Value value;
	Json::Value root;
	Json::Reader reader;
	if (!reader.parse(form,value))
	{
		return 0;
	}
	//�û�id
	string userid = value["uid"].asString();
	root = value["basic"];
	//�ǳ�
	string nick = root["nick"].asString();
	//�ֻ���
	string phone = ms_->GetValueFromMapByKey("device",true);
	output_text_=
		"userid=" +		userid + "\r\n" +			//�û�ID
		"msg=" +		nick + "\r\n" +             //�ǳ�
		"oid=" +		phone + "\r\n" +			//�ֻ���
		"netmark=" +	app_ + "\r\n" + 	        //��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";
	WriteIndexFile();
	return 1;
}
