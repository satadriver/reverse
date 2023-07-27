
#include "StdAfx.h"
#include "MarriageSuPeiWang.h"

marriage::SuPeiWangMarriage::SuPeiWangMarriage(HttpSession *http_session, const string &pro_name) : Marriage(http_session,pro_name)
{
	app_ = "速配网";	
}

marriage::SuPeiWangMarriage::~SuPeiWangMarriage()
{
	
}

int marriage::SuPeiWangMarriage::IsSuPeiWangMarriage(HttpSession *http_session)
{
	//网页注册
	if((http_session->m_Requestline.m_Host.Find("www.supei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/regstep0.jsp")!=-1)) 
	{
		return kPcWebRegister;
	}
	//网页登录
	if((http_session->m_Requestline.m_Host.Find("www.supei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/login.jsp")!=-1)) 
	{
		return kPcWebLogin;

	}
	//网页修改资料

	if((http_session->m_Requestline.m_Host.Find("www.supei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/regstep1.jsp")!=-1)) 
	{
		return kPcWebModiy;
	}
	return 0;
}

int marriage::SuPeiWangMarriage::Process(const char *packet,int action)
{
	//网页注册
	if (action == kPcWebRegister)
	{
		return PcWebRegister(packet);
	}
	//网页登录
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	//网页修改个人资料
	if (action == kPcWebModiy)
	{
		return PcWebModiy(packet);
	}
	return 1;
}
//pc注册
int marriage::SuPeiWangMarriage::PcWebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//用户名
	string username = ms_->GetValueFromMapByKey("loginid",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	//昵称
	string name = ms_->GetValueFromMapByKey("nickname",true);
	//性别
	string sex = ms_->GetValueFromMapByKey("gender",true);
	if (sex == "M"){sex ="男";}
	else{sex = "女";}
	//出生日期
	string data = ms_->GetValueFromMapByKey("year",true) + "年" +
		ms_->GetValueFromMapByKey("month",true) + "月" +
		ms_->GetValueFromMapByKey("day",true)+"日";
	//真实姓名
	string truename = ms_->GetValueFromMapByKey("truename",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//证件类型
	string idtype = ms_->GetValueFromMapByKey("idcardtype",true);
	if (idtype=="A"){idtype = "身份证";}
	if (idtype=="B"){idtype = "护照";}
	if (idtype=="C"){idtype = "军人证";}
	//证件号
	string idno = ms_->GetValueFromMapByKey("idcardno",true);
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//手机号
	string mobile = ms_->GetValueFromMapByKey("mobile",true);
	//即时通讯
	string icqtype = ms_->GetValueFromMapByKey("icqtype",true);
	if (icqtype=="A"){icqtype = "QQ";}
	if (icqtype=="C"){icqtype = "Yahoo";}
	if (icqtype=="D"){icqtype = "微信";}
	string icqid = ms_->GetValueFromMapByKey("icqid",true);
	output_text_ = 
		"user=" +		username + "\r\n" +        //用户名 
		"pass=" +		password + "\r\n" +        //密码 
		"msg=" + 		name + "\r\n" + 		   //昵称 
		"mid=" +		truename + "\r\n" +        //姓名  
		"sex=" +		sex + "\r\n" +             //性别 
		"onick=" +		data + "\r\n" +				//出生日期 
		"servername=" +	idtype + "\r\n" + 		   //证件类型      
		"appname=" + 	idno + "\r\n" + 		   //证件号
		"nick="	 + 	  	email + "\r\n" + 			//邮箱
		"msgtype=" + 	icqtype + "\r\n" +			//通讯类型
		"subject=" + 	icqid + "\r\n" +			//通讯号
		"islogin=" +    address + "\r\n" +			//地址
		"oid=" +		mobile + "\r\n" +			//手机号 
		"netmark=" +    app_ + "\r\n" +				//来源 
		"clientmsg=" +  platformWeb + "\r\n" +		//操作平台
		"state=注册" + "\r\n" ;						//状态 
		WriteIndexFile();
	return 1;
}

//web登录
int marriage::SuPeiWangMarriage::PcWebLogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string username = ms_->GetValueFromMapByKey("loginid",true);
	string pwd = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || pwd.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +          //用户名
		"pass=" + pwd + "\r\n" +               //密码
		"netmark=" + app_ + "\r\n" + 		   //来源
		"clientmsg=" + platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";			           //状态
	WriteIndexFile();
	return 1;
}

//详细资料个人信息修改
int marriage::SuPeiWangMarriage::PcWebModiy(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	//姓名
	string name = ms_->GetValueFromMapByKey("truename",true);
	//证件类型
	string idtype = ms_->GetValueFromMapByKey("idcardtype",true);
	if (idtype=="A"){idtype = "身份证";}
	if (idtype=="B"){idtype = "护照";}
	if (idtype=="C"){idtype = "军人证";}
	//证件号
	string idno = ms_->GetValueFromMapByKey("idcardno",true);
	//地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//即时通讯
	string icqtype = ms_->GetValueFromMapByKey("icqtype",true);
	if (icqtype=="A"){icqtype = "QQ";}
	if (icqtype=="C"){icqtype = "Yahoo";}
	if (icqtype=="D"){icqtype = "微信";}
	string icqid = ms_->GetValueFromMapByKey("icqid",true);

	output_text_ = 
		"mid=" +		name+ "\r\n" +						//姓名  
		"servername=" + idtype + "\r\n" + 				   //证件类型      
		"appname=" + 	idno + "\r\n" +					   //证件号
		"nick="	 + 	  	email + "\r\n" +				 	//邮箱
		"msgtype=" + 	icqtype + "\r\n" +					//通讯类型
		"subject=" + 	icqid + "\r\n" +				    //通讯号
		"islogin=" +    address + "\r\n" +					//地址
		"netmark=" +    app_ + "\r\n" +						//来源 
		"clientmsg=" +  platformWeb + "\r\n" +				//操作平台
		"state=修改个人信息\r\n" ;							//状态 
		WriteIndexFile();
	return 1;
}

