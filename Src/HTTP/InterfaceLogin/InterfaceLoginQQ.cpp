#include "StdAfx.h"
#include "InterfaceLoginQQ.h"

interfacelogin::InterfaceLoginQQ::InterfaceLoginQQ(HttpSession *http_session, const string &pro_name): InterfaceLogin(http_session,pro_name)
{

}

interfacelogin::InterfaceLoginQQ::~InterfaceLoginQQ()
{

}

interfacelogin::InterfaceLoginQQ::IsInterfaceLoginQQ(HttpSession *http_session)
{
	//获得哪个应用 使用 QQ登录 (ofo小黄车)
	if (http_session->m_Requestline.m_Host.Find("c.isdspeed.qq.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/code.cgi?domain=mobile.opensdk.com&cgi=opensdk") != -1)
	{
		return kAndroidInterfaceQQCheck;
	}
	//获得那个应用 使用 QQ登录 (sina微博)
	if (http_session->m_Requestline.m_Host.Find("appsupport.qq.com") != -1 &&
		http_session->m_Requestline.m_URI.Find("/cgi-bin/appstage/mstats_report?timezone") != -1)
	{
		return kAndroidInterfaceQQCheckSec;
	}
	//获得对应 QQ号 与 IMEI IMSI
	if( ( http_session->m_Requestline.m_Host.Find("appsupport.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/cgi-bin/appstage/mstats_batch_report?mType=yingyongbao") != -1 ))	
	{
		return kAndroidInterfaceQQ;
	}
	return 0;
}

interfacelogin::InterfaceLoginQQ::Process(const char *packet,int action)
{
	if (action == kAndroidInterfaceQQCheck)
	{
		return AndroidInterFaceQQCheck(packet);
	}
	if (action == kAndroidInterfaceQQCheckSec)
	{
		return AndroidInterfaceQQCheckSec(packet);
	}
	if (action == kAndroidInterfaceQQ)
	{
		return AndroidInterfaceQQ(packet);
	}
	return 0;
}


interfacelogin::InterfaceLoginQQ::AndroidInterFaceQQCheck(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,"&","=");
    //
	string cmd = ms_->GetValueFromMapByKey("cmd");
	if (cmd != "LOGIN_CHECK_SDK")
	{
		return 0;
	}
    string packagename = ms_->GetValueFromMapByKey("packagename");
    string network = ms_->GetValueFromMapByKey("network");
	string model_name = ms_->GetValueFromMapByKey("model_name");
	string platform = ms_->GetValueFromMapByKey("platform");
	if (platform == "1")
	{
		platform = "Android";
	}
	else
	{
		platform = "other";
	}
	if (packagename.empty() || network.empty() || model_name.empty())
	{
		return 0;
	}
	output_text_ =
		"islogin=" + 	packagename + "\r\n" +  //应用名称
		"mid=" + 	  	model_name + "\r\n" + 	//手机型号			
		"sex=" + 	  	network + "\r\n" + 		//网络类型
		"clientmsg=" + 	platform + "\r\n" + 	//操作平台	
		"state=QQ第三方授权登录\r\n";		    //状态
	WriteIndexFile();
	return 1;
}

interfacelogin::InterfaceLoginQQ::AndroidInterfaceQQCheckSec(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,"&","=");
    //
	string imei = ms_->GetValueFromMapByKey("imei");
    string packagename = ms_->GetValueFromMapByKey("packagename");
    string network = ms_->GetValueFromMapByKey("network");
	string model_name = ms_->GetValueFromMapByKey("model_name");
	string platform = ms_->GetValueFromMapByKey("platform");
	if (platform == "1")
	{
		platform = "Android";
	}
	else
	{
		platform = "other";
	}
	if (imei.empty() || packagename.empty() || network.empty() || model_name.empty())
	{
		return 0;
	}
	output_text_ =
		"islogin=" + 	packagename + "\r\n" +  //应用名称
		"mid=" + 	  	model_name + "\r\n" + 	//手机型号	
        "fileurl=" + 	imei + "\r\n" + 		//IMEI
		"sex=" + 	  	network + "\r\n" + 		//网络类型
		"clientmsg=" + 	platform + "\r\n" + 	//操作平台	
		"state=QQ第三方授权登录\r\n";		    //状态
	WriteIndexFile();
	return 1;
}

interfacelogin::InterfaceLoginQQ::AndroidInterfaceQQ(const char *packet)
{

	string src_data(packet);
	if (src_data.find("data=") != -1)
	{
		src_data = src_data.substr(src_data.find("data=") + strlen("data="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    //json 提取数据
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	Json::Value data = value["data"];
	for(int j=0 ; j<data.size() ; j++)
	{
		//QQ号 
		string uin = data[j]["uin"].asString();
		//QQ版本号 
		string qq_ver = data[j]["qq_ver"].asString();
		//手机型号 
		string model_name = data[j]["model_name"].asString();
		//系统版本 
		string os_ver = data[j]["os_ver"].asString();
		//分辨率 
		string resolution = data[j]["resolution"].asString();
		//网络类型 
		string network = data[j]["network"].asString();
		//IMEI 
		string imei = data[j]["imei"].asString();
		//IMSI 
		string imsi = data[j]["imsi"].asString();
		//mac地址
		string mac = data[j]["mac"].asString();
		//操作平台
		string plaform = data[j]["platform"].asString();
		if (plaform == "4")
		{
			plaform = "Android";
		}
		if (uin.empty() || imei.empty() || imsi.empty())
		{
			return 0;
		}
		output_text_ =
			"oid=" + 	  	uin + "\r\n" + 			//QQ号
			"onick=" + 	  	qq_ver + "\r\n" + 		//QQ版本号
			"mid=" + 	  	model_name + "\r\n" + 	//手机型号		
			"nick="	 + 	  	resolution + "\r\n" + 	//分辨率 	
			"netmark=" + 	os_ver + "\r\n" + 		//系统版本	
			"sex=" + 	  	network + "\r\n" + 		//网络类型
			"clientmsg=" + 	plaform + "\r\n" + 		//操作平台
			"imsi=" + 	    imsi + "\r\n" + 		//IMSI
			"fileurl=" + 	imei + "\r\n" + 		//IMEI
			"clientmac=" + 	mac +  "\r\n" + 		//MAC地址	
			"state=QQ第三方授权登录\r\n";		    //状态
		WriteIndexFile();
	}
	return 1;
}
