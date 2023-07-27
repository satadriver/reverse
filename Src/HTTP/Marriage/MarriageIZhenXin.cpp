#include "StdAfx.h"
#include "MarriageIZhenXin.h"

marriage::IZhenXInMarriage::IZhenXInMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "爱真心";	
}

marriage::IZhenXInMarriage::~IZhenXInMarriage()
{
	
}

int marriage::IZhenXInMarriage::IsIZhenXInMarriage(HttpSession *http_session)
{
	//手机登录
	if((http_session->m_Requestline.m_Host.Find("c.izhenxin.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cmiajax/?mod=login&func=doLogin")!=-1)) 
	{
		return kAndroidLogin;
	}
	//手机修改
	if((http_session->m_Requestline.m_Host.Find("c.izhenxin.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cmiajax/?mod=profile&func=basicinfo_save")!=-1)) 
	{
		return kAndroidModiy;
	}
	return 0;
}

int marriage::IZhenXInMarriage::Process(const char *packet,int action)
{
	//手机登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//手机修改
	if (action == kAndroidModiy)
	{
		return AndroidModiy(packet);
	}
	return 1;
}

//手机登录
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
		"user=" +   login_name + "\r\n" +       //用户名 
		"pass=" +   password  + "\r\n" +        //密码  
		"from=" +     l + "\r\n" +				//经度  
        "to=" +     la + "\r\n" +				//纬度
		"netmark=" + app_ + "\r\n" + 	        //来源
		"clientmsg=" + platformAndroid + "\r\n" +//操作平台
		"state=登录\r\n";
	WriteIndexFile();
	return 1;

}
//手机修改
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
	//用户id
	string userid = value["uid"].asString();
	root = value["basic"];
	//昵称
	string nick = root["nick"].asString();
	//手机号
	string phone = ms_->GetValueFromMapByKey("device",true);
	output_text_=
		"userid=" +		userid + "\r\n" +			//用户ID
		"msg=" +		nick + "\r\n" +             //昵称
		"oid=" +		phone + "\r\n" +			//手机号
		"netmark=" +	app_ + "\r\n" + 	        //来源
		"clientmsg=" +	platformAndroid + "\r\n" +	//操作平台
		"state=修改个人信息\r\n";
	WriteIndexFile();
	return 1;
}
