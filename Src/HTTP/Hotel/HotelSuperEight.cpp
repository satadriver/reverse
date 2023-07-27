#include "StdAfx.h"
#include "HotelSuperEight.h"

hotel::HotelSuperEight::HotelSuperEight(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "速八酒店";
}

hotel::HotelSuperEight::~HotelSuperEight()
{

}

hotel::HotelSuperEight::IsHotelSuperEight(HttpSession *http_session)
{
	//web注册
	if( ( http_session->m_Requestline.m_Host.Find("www.super8.com.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Register/RegisterSm") != -1 ))	
	{
		return kWebRegister;
	}
	//web登录
	if( ( http_session->m_Requestline.m_Host.Find("www.super8.com.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/MemInfo/Index") != -1 ))	
	{
		return kWebLogin;
	}
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("www.super8.com.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Book/ToPay") != -1))	
	{
		return kWebNewOrder;
	}
	return 0;
}

hotel::HotelSuperEight::Process(const char *packet,int action)
{
	//web注册
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web登录
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web国内酒店预订
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	return 0;
}
//web注册
hotel::HotelSuperEight::WebRegister(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("CustomeName",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("PhoneNum");
	//性别 
	string sex = ms_->GetValueFromMapByKey("Gender",true);	
	if (sex == "1")
	{
		sex = "男";
	}
	else if (sex == "2")
	{
		sex = "女";
	}
	if (name.empty() || mobile.empty() || sex.empty())
	{
		return -1;
	}
	output_text_ =
		"wifissid=" + 	name + "\r\n" + 		//姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"sex=" +		sex + "\r\n" +			//性别
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}
//web登录
hotel::HotelSuperEight::WebLogin(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("LoginName") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("LoginName"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    string username = ms_->GetValueFromMapByKey("LoginName",true);
	string password = ms_->GetValueFromMapByKey("PassWd");

	if (username.empty() || password.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +	    //用户名
		"pass=" + 		password + "\r\n" +		//密码
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//web国内酒店预订
hotel::HotelSuperEight::WebNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//入住日期 
	string livedate = ms_->GetValueFromMapByKey("sdate");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("edate");
	//住客姓名 
	string name = ms_->GetValueFromMapByKey("gstname",true);
	//联系姓名
	string contactname = ms_->GetValueFromMapByKey("conname",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("conmoblie");
	//联系邮箱 
	string email = ms_->GetValueFromMapByKey("conemail",true);
	if (email.empty())
	{
		email = "未填写";
	}
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("roomnum");

	if (name.empty() || contactname.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	name + "\r\n" + 		//住客姓名
		"clientpcname=" + contactname + "\r\n" +//联系姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}
