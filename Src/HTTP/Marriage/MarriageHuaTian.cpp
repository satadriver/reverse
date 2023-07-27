#include "StdAfx.h"
#include "MarriageHuaTian.h"

marriage::HuaTianMarriage::HuaTianMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "���׻���";	
}

marriage::HuaTianMarriage::~HuaTianMarriage()
{
	
}

int marriage::HuaTianMarriage::IsHuaTianMarriage(HttpSession *http_session)
{
	//web�޸�����
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/updateUser")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/api/user/updateUser") ==-1)) 
	{
		return kWebModify;
	}
	//android�޸�����
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/user/updateNew")!=-1)) 
	{
		return kAndroidModify;
	}
	//androidʵ����֤
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wap/user/verifyName")!=-1)) 
	{
		return kAndroidRealName;
	}	
	//android��λ 1 
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/home/allNearby")!=-1)) 
	{
		return kAndroidLocation;
	}	
	//android��λ 2	
	if((http_session->m_Requestline.m_Host.Find("love.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/location/accessLocation")!=-1)) 
	{
		return kAndroidLocation2;
	}	
	return 0;
}

int marriage::HuaTianMarriage::Process(const char *packet,int action)
{
	//web�޸�����
	if (action == kWebModify)
	{
		return WebModify(packet);
	}
	//android�޸�����
	if (action == kAndroidModify)
	{
		return AndroidModify(packet);
	}
	//androidʵ����֤
	if (action == kAndroidRealName)
	{
		return AndroidRealName(packet);
	}	
	//android��λ 1
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	//android��λ 2
	if (action == kAndroidLocation2)
	{
		return AndroidLocation2(packet);
	}
	return 1;
}

//web�޸�����
int marriage::HuaTianMarriage::WebModify(const char *packet)
{
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	ms_->SplitStringToMap(cookie,';','=');
	string P_INFO = ms_->GetValueFromMapByKey("P_INFO",true);
	string usrName;
	if (P_INFO.find("|") != string::npos)
	{
		usrName = P_INFO.substr(0,P_INFO.find("|"));
	}
    //���ݲ���
	string src_data(packet);
	if (src_data.rfind("\r\n") != -1)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickName",true);
	//��������
	string birthdate = ms_->GetValueFromMapByKey("birthday");
	
	output_text_ =
		"user=" + 		usrName + "\r\n" +			//20	[13]	//�û��� 
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //�ǳ�
		"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//�������� 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=�޸Ļ�����Ϣ\r\n";					//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//android�޸�����
int marriage::HuaTianMarriage::AndroidModify(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�ǳ�
	string nickname = ms_->GetValueFromMapByKey("nickName",true);
	//��������
	string birthdate = ms_->GetValueFromMapByKey("birthday");
	
	output_text_ =
		"msg=" + 	 	nickname + "\r\n" + 		//65	[104]   //�ǳ�
		"onick=" + 	  	birthdate + "\r\n" + 		//55	[120]	//�������� 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//����ƽ̨
		"state=�޸Ļ�����Ϣ\r\n";					//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//androidʵ����֤
int marriage::HuaTianMarriage::AndroidRealName(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	if (src_data.find("realName=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("realName="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("realName",true);
	//֤������
	string idType = "���֤";
	//֤����
	string idNumber = ms_->GetValueFromMapByKey("idCard");
	
	output_text_ =
		"mid=" +		name + "\r\n" + 				//56	[121]	//����
		"servername=" + idType + "\r\n" + 				//49	[116]   //֤������      
		"appname=" +	idNumber + "\r\n" + 			//50	[115]   //֤����
		"netmark=" + app_ + "\r\n" + 					//59	[124]	//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +		//62	[126]	//����ƽ̨
		"state=ʵ����֤\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//android��λ
int marriage::HuaTianMarriage::AndroidLocation(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');	
	//����
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	//γ��
	string latitude = ms_->GetValueFromMapByKey("latitude");
	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" + 	 longitude + "\r\n" + 			//37	[41]  	//����  
		"to=" + 	 latitude + "\r\n" + 			//38	[42]    //γ��	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//����ƽ̨
		"state=��λ\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
//android��λ 2
int marriage::HuaTianMarriage::AndroidLocation2(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	if (src_data.find("latitude=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("latitude="));
	}
	ms_->SplitStringToMap(src_data,'&','=');	
	//����
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	//γ��
	string latitude = ms_->GetValueFromMapByKey("latitude");
	if (longitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" + 	 longitude + "\r\n" + 			//37	[41]  	//����  
		"to=" + 	 latitude + "\r\n" + 			//38	[42]    //γ��	
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//����ƽ̨
		"state=��λ\r\n";							//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
