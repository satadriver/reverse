#include "StdAfx.h"
#include "FileTransferZapya.h"

filetransfer::ZapyaFileTransfer::ZapyaFileTransfer(HttpSession *http_session, const string &pro_name) : FileTransfer(http_session,pro_name)
{
	app_ = "����";	
}

filetransfer::ZapyaFileTransfer::~ZapyaFileTransfer()
{
	
}

int filetransfer::ZapyaFileTransfer::IsZapyaFileTransfer(HttpSession *http_session)
{
	//androidע��
	if((http_session->m_Requestline.m_Host.Find("api.dewmobile.net")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/v3/users")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/v3/users/sessions")==-1)) 
	{
		return kAndroidRegister;
	}
	//android��¼
	if((http_session->m_Requestline.m_Host.Find("api.dewmobile.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/users/sessions")!=-1)) 
	{
		return kAndroidLogin;
	}
	//android��λ
	if((http_session->m_Requestline.m_Host.Find("api.dewmobile.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/near")!=-1)) 
	{

		return kAndroidLocation;
	}
	return 0;
}

int filetransfer::ZapyaFileTransfer::Process(const char *packet,int action)
{
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
	//android��λ
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 1;
}

//androidע��
int filetransfer::ZapyaFileTransfer::AndroidRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	string type,account,password,imei;
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (!reader.parse(src_data,value))
	{	
       return 0;
	}
	account = value["code"].asString();
	password = value["pwd"].asString();	
	imei = value["imei"].asString();
	if (account.empty() || password.empty() || imei.empty())
	{
		return 1;
	}
	output_text_ =	
		"user=" + 		account + "\r\n" +			//�˺�	
		"pass=" + 		password + "\r\n" + 		//����	
		"imsi=" +		imei + "\r\n" + 			//imei
		"clientmsg=" + platformAndroid + "\r\n" +	//����ƽ̨
		"state=�ֻ���ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//android��¼
int filetransfer::ZapyaFileTransfer::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	string account,password;
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{	
		account = value["id"].asString();
		password = value["pwd"].asString();	
	}
	if (account.empty() || password.empty())
	{
		return 1;
	}
	output_text_ =	
		"user=" + 		account + "\r\n" +			//�˺�	
		"pass=" + 		password + "\r\n" + 		//����	
		"clientmsg=" + platformAndroid + "\r\n" +	//����ƽ̨
		"state=�ֻ��ŵ�¼\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//android��λ
int filetransfer::ZapyaFileTransfer::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{	
		CString str;
		double lon = value["lnt"].asDouble();
		str.Format("%f",lon);
		longtitude_ = str.GetBuffer(0);
		double lat = value["lat"].asDouble();
		str.Format("%f",lat);
		latitude_ = str.GetBuffer(0);
		//��չ��Ϣ
		Json::Value ext = value["ext"];
		imei_ = ext["dId"].asString();
		imei_ = imei_.substr(0,15);					//��ȡǰ15 λimei
		ostype_ = ext["osType"].asString();
	}
	if (longtitude_.empty() || latitude_.empty() || imei_.empty() || ostype_.empty())
	{
		return 1;
	}
	output_text_ =	
		"from=" + 	  	longtitude_ + "\r\n" +		//����	
		"to=" + 	  	latitude_ + "\r\n" +   		//γ��	
		"imsi=" +  	    imei_ + "\r\n" +      		//imei	
		"msgtype=" + 	ostype_ + "\r\n" +			//ϵͳ����
		"clientmsg=" + platformAndroid + "\r\n" +	//����ƽ̨
		"state=��λ\r\n";				        	//״̬
	WriteIndexFile();
	return 1;
}
