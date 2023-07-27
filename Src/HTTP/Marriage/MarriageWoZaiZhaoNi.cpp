#include "StdAfx.h"
#include "MarriageWoZaiZhaoNi.h"

marriage::WoZaiZhaoNiMarriage::WoZaiZhaoNiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "����������";	
}

marriage::WoZaiZhaoNiMarriage::~WoZaiZhaoNiMarriage()
{
	
}

int marriage::WoZaiZhaoNiMarriage::IsWoZaiZhaoNiMarriage(HttpSession *http_session)
{
	//ע��	
	if((http_session->m_Requestline.m_Host.Find("www.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/user/register/")!=-1)) 
	{
		return kWebRegister;
	}	
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/user/login/")!=-1)) 
	{
		return kWebLogin;
	}	
	//web�޸�����
	if((http_session->m_Requestline.m_Host.Find("www.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ajax/user/upMember")!=-1)) 
	{
		return kWebModify;
	}	
	//androidע��
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v150/index/register/")!=-1)) 
	{
		return kAndroidRegister;
	}
	//android��¼
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v160/index/mlogin/")!=-1)) 
	{
		return kAndroidLogin;
	}	
	//android�޸�����
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v150/user/update/")!=-1)) 
	{
		return kAndroidModify;
	}	
	//android��λ
	if((http_session->m_Requestline.m_Host.Find("mi.95195.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/updatecoor/")!=-1)) 
	{
		return kAndroidLocation;
	}
	return 0;
}

int marriage::WoZaiZhaoNiMarriage::Process(const char *packet,int action)
{
	//ע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//��¼
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web�޸�����
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	//androidע��
	if (action == kAndroidRegister)
	{
		return AndroidRegister(packet);
	}
	//android��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android�޸�����
	if (action == kAndroidModify)
	{
		return AndroidModify(packet);
	}
	//android��λ
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 1;
}
//ע��
int marriage::WoZaiZhaoNiMarriage::WebRegister(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("nickname=") != string::npos )
	{
		src_data = src_data.substr(src_data.rfind("nickname="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//����
	string passwd = ms_->GetValueFromMapByKey("passwd");
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex")=="1"?"��":"Ů";
	if (nickname.empty() || mobile.empty() || passwd.empty() )
	{
		return 0;
	}	
	output_text_ =
		"msg=" + 	 nickname + "\r\n" + 		//65	[104]   //�ǳ�
		"oid=" + 	 mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"pass=" + 	 passwd + "\r\n" + 			//21	[14]	//���� 	
		"sex=" + 	 sex + "\r\n" + 			//60	[125]	//�Ա� 
		"netmark=" + app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
	"state=ע��\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//��¼
int marriage::WoZaiZhaoNiMarriage::WebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("mobile=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("mobile="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//����
	string password = ms_->GetValueFromMapByKey("passwd");
	
	output_text_ =
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//���� 	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=��¼\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//web�޸�����
int marriage::WoZaiZhaoNiMarriage::WebModify(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.find("nickname=") != -1)
	{
		src_data = src_data.substr(src_data.find("nickname="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//����
	string input = ms_->GetValueFromMapByKey("input",true);
	if (input != "nickname" || nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 	//65	[104]   //�ǳ�
		"netmark=" + app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126]	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";				//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//androidע��
int marriage::WoZaiZhaoNiMarriage::AndroidRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("vocation=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("vocation="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickname",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//����
	string passwd = ms_->GetValueFromMapByKey("passwd");
	//�Ա�
	string sex = ms_->GetValueFromMapByKey("sex")=="1" ? "��":"Ů";
	//����
	string longitude = ms_->GetValueFromMapByKey("lng");
	//γ��
	string latitude = ms_->GetValueFromMapByKey("lat");
	if (nickname.empty() || mobile.empty() || passwd.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //�ǳ�
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"pass=" + 		passwd + "\r\n" + 			//21	[14]	//���� 	
		"sex=" + 	  	sex + "\r\n" + 				//60	[125]	//�Ա� 
		"from=" + 	  	longitude + "\r\n" + 		//37	[41]  	//����  
		"to=" + 	  	latitude + "\r\n" + 		//38	[42]    //γ��
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//����ƽ̨
		"state=ע��\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//android��¼
int marriage::WoZaiZhaoNiMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("sign=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("sign="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//����
	string password = ms_->GetValueFromMapByKey("passwd");
	if (mobile.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"pass=" + 		password + "\r\n" + 		//21	[14]	//���� 	
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//����ƽ̨
		"state=��¼\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//android�޸�����
int marriage::WoZaiZhaoNiMarriage::AndroidModify(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.find("value=") != -1)
	{
		src_data = src_data.substr(src_data.find("value="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("value",true);
	//����
	string input = ms_->GetValueFromMapByKey("field",true);
	if (input != "nickname" || nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //�ǳ�
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";					//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//android��λ
int marriage::WoZaiZhaoNiMarriage::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("lat=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("lat="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string longitude = ms_->GetValueFromMapByKey("lng");
	//γ��
	string latitude = ms_->GetValueFromMapByKey("lat");
    if (longitude.empty() || latitude.empty())
    {
		return 0;
    }	
	output_text_ =
		"from=" + 		longitude + "\r\n" + 		//37	[41]  	//����  
		"to=" + 		latitude + "\r\n" + 		//38	[42]    //γ��
		"netmark=" +	app_ + "\r\n" + 			//59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//����ƽ̨
		"state=��λ\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}


