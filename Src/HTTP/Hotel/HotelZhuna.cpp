#include "StdAfx.h"
#include "HotelZhuna.h"

hotel::HotelZhuna::HotelZhuna(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "住哪儿";
}

hotel::HotelZhuna::~HotelZhuna()
{

}

int hotel::HotelZhuna::IsHotelZhuna(HttpSession *http_session)
{
	//web注册
	if (http_session->m_Requestline.m_Host.Find("www.zhuna.cn") != -1 &&		
		http_session->m_Requestline.m_URI.Find("/account/registerInsert/") != -1)
	{
		return kWebRegister;
	}
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("www.zhuna.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/post?webpath=&style=") != -1))	
	{
		return kWebNewOrder;
	}
	//android登录
	if (http_session->m_Requestline.m_Host.Find("app.api.zhuna.cn") != -1 && 
		http_session->m_Requestline.m_URI.Find("/v30/member/userLogin.php") != -1)
	{
		return kAndroidLogin;
	}
	//android订单
	if (http_session->m_Requestline.m_Host.Find("app.api.zhuna.cn") != -1 &&
		http_session->m_Requestline.m_URI.Find("/v20/postOrder.php") != -1)
	{
		return kAndroidOrder;
	}
	return 0;
}

int hotel::HotelZhuna::Process(const char *packet,int action)
{
	//web注册
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web国内酒店预订
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	//android登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android订单
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}
//web注册
int hotel::HotelZhuna::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("phone") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("phone"));
	}
	ms_->SplitStringToMap(src_data,"&","=");
	//用户名
	string username = ms_->GetValueFromMapByKey("phone");
	//密码
	string password = ms_->GetValueFromMapByKey("pwd");
	string repassword = ms_->GetValueFromMapByKey("r_pwd");

	if (password != repassword)
	{
		return -1;
	}
	if (username.empty() || repassword.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//用户名
		"pass=" + 		password + "\r\n" +		//密码	
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=注册\r\n";	
	WriteIndexFile();
	return 1;
}
//web国内酒店预订
int hotel::HotelZhuna::WebNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("roomnum") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("roomnum"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("ordertmp[CheckInDate]");
	//退房日期
	string leavedate = ms_->GetValueFromMapByKey("ordertmp[CheckOutDate]");
	//住客姓名
	string name;
    string tempname = ms_->GetValueBetweenBeginEnd(src_data,"g_name","c_mobile");
	while (tempname.find("&") != string::npos)
	{
		string strtemp = tempname.substr(0,tempname.find("&"));
		string str1 = strtemp.substr(strtemp.find("=") +1);
		name += str1;
		name += ",";
		tempname = tempname.substr(tempname.find("&") + 1);
	}
	name += tempname.substr(tempname.find("=") +1);
	if (name.rfind(",") != string::npos)
	{
		name = name.substr(0,name.rfind(","));
	}
	//联系手机
	string mobile = ms_->GetValueFromMapByKey("c_mobile");
	//联系邮箱
	string email = ms_->GetValueFromMapByKey("c_email");
	if (email.empty())
	{
		email = "null";
	}
	//酒店名称
	string hotelname = ms_->GetValueFromMapByKey("z[hotelname]",true);
	//房间类型 
	string roomtype = ms_->GetValueFromMapByKey("ordertmp[RoomName]",true);
	//房间数量 
	string roomcount = ms_->GetValueFromMapByKey("roomnum");
    if (name.empty() || mobile.empty())
    {
		return -1;
    }
	output_text_ =
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" +	name + "\r\n" + 		//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"webname=" + 	hotelname + "\r\n" +	//酒店名称
		"clientmac=" + 	roomtype + "\r\n" +		//房间类型
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//android登录
int hotel::HotelZhuna::AndroidLogin(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("password") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("password"));
	}
    ms_->SplitStringToMap(src_data,"&","=");
	//用户名
	string username = ms_->GetValueFromMapByKey("mobile");
	//密码
	string password = ms_->GetValueFromMapByKey("password");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//用户名
		"pass=" + 		password + "\r\n" + 		//密码
		"netmark=" +	app_ + "\r\n" + 			//来源
		"clientmsg=" +  platformAndroid + "\r\n" +  //操作平台
		"state=登录\r\n";							//状态
	WriteIndexFile();
	return 1;
}
//android订单
int hotel::HotelZhuna::AndroidOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//入住日期
    string livedate = ms_->GetValueFromMapByKey("tm1");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("tm2");
	//住客姓名
	string name = ms_->GetValueFromMapByKey("guestname");
	//联系手机
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("roomNum");
	if (name.empty() || mobile.empty())
	{
		return -1;
	}
	output_text_ =
		"appname=" + 	  	livedate + "\r\n" + 		//入住日期
		"msgtype=" + 	 	leavedate + "\r\n" +		//退房日期
		"subject=" + 	  	name + "\r\n" + 			//住客姓名
		"islogin=" + 	  	mobile + "\r\n" + 			//联系手机
		"fileurl=" + 	  	roomcount + "\r\n" + 		//房间数量
		"netmark=" +		app_ + "\r\n" + 			//来源
		"clientmsg=" +		platformAndroid + "\r\n" +	//操作平台
		"state=国内酒店预订\r\n";						//状态
	WriteIndexFile();
	return 1;
}