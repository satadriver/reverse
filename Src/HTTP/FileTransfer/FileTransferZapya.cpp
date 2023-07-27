#include "StdAfx.h"
#include "FileTransferZapya.h"

filetransfer::ZapyaFileTransfer::ZapyaFileTransfer(HttpSession *http_session, const string &pro_name) : FileTransfer(http_session,pro_name)
{
	app_ = "快牙";	
}

filetransfer::ZapyaFileTransfer::~ZapyaFileTransfer()
{
	
}

int filetransfer::ZapyaFileTransfer::IsZapyaFileTransfer(HttpSession *http_session)
{
	//android注册
	if((http_session->m_Requestline.m_Host.Find("api.dewmobile.net")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/v3/users")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/v3/users/sessions")==-1)) 
	{
		return kAndroidRegister;
	}
	//android登录
	if((http_session->m_Requestline.m_Host.Find("api.dewmobile.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3/users/sessions")!=-1)) 
	{
		return kAndroidLogin;
	}
	//android定位
	if((http_session->m_Requestline.m_Host.Find("api.dewmobile.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/location/near")!=-1)) 
	{

		return kAndroidLocation;
	}
	return 0;
}

int filetransfer::ZapyaFileTransfer::Process(const char *packet,int action)
{
	//android注册
	if (action == kAndroidRegister)
	{
		return AndroidRegister(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android定位
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	return 1;
}

//android注册
int filetransfer::ZapyaFileTransfer::AndroidRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	string type,account,password,imei;
	//使用json 读取数据
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
		"user=" + 		account + "\r\n" +			//账号	
		"pass=" + 		password + "\r\n" + 		//密码	
		"imsi=" +		imei + "\r\n" + 			//imei
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=手机号注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//android登录
int filetransfer::ZapyaFileTransfer::AndroidLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	string account,password;
	//使用json 读取数据
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
		"user=" + 		account + "\r\n" +			//账号	
		"pass=" + 		password + "\r\n" + 		//密码	
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=手机号登录\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//android定位
int filetransfer::ZapyaFileTransfer::AndroidLocation(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	//使用json 读取数据
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
		//扩展信息
		Json::Value ext = value["ext"];
		imei_ = ext["dId"].asString();
		imei_ = imei_.substr(0,15);					//截取前15 位imei
		ostype_ = ext["osType"].asString();
	}
	if (longtitude_.empty() || latitude_.empty() || imei_.empty() || ostype_.empty())
	{
		return 1;
	}
	output_text_ =	
		"from=" + 	  	longtitude_ + "\r\n" +		//经度	
		"to=" + 	  	latitude_ + "\r\n" +   		//纬度	
		"imsi=" +  	    imei_ + "\r\n" +      		//imei	
		"msgtype=" + 	ostype_ + "\r\n" +			//系统类型
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=定位\r\n";				        	//状态
	WriteIndexFile();
	return 1;
}
