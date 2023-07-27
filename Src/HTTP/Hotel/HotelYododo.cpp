#include "StdAfx.h"
#include "HotelYododo.h"

hotel::HotelYododo::HotelYododo(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "游多多";
}

hotel::HotelYododo::~HotelYododo()
{

}

hotel::HotelYododo::IsHotelYododo(HttpSession *http_session)
{
	//web 注册
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/user/registerNew.ydd") != -1))	
	{
		return kWebRegister;
	}
	//web登录
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/user/ajaxLogin.ydd") != -1))	
	{
		return kWebLogin;
	}
	//web客栈民宿搜索
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotel/ajaxCitySearchTable.ydd") != -1 ))	
	{
		return kWebSearch;
	}
	//web国内酒店搜索
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/common/ajaxLoginMessge.jsp") != -1 ))	
	{
		return kWebIntSearch;
	}
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("travel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotel/isajax/HotelFillOrder/SaveOrderNew") != -1))	
	{
		return kWebIntNewOrder;
	}
	//web客栈 登录
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/user/login.ydd") != -1))	
	{
		return kWebHotelLogin;
	}
	//web客栈 搜索
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotels/") != -1 ))	
	{
		return kWebHotelSearch;
	}
	//web客栈 预订
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/sale/addHotelOrder.ydd") != -1))	
	{
		return kWebHotelOrder;
	}
	//androdi客栈 登录
	if( ( http_session->m_Requestline.m_Host.Find("api.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/mobile/v2/login.ydd") != -1))	
	{
		return kAndroidLogin;
	}
	//android客栈 搜索
	if( ( http_session->m_Requestline.m_Host.Find("appapi.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/json/search") != -1))	
	{
		return kAndroidSearch;
	}
	//android客栈 订单
	if( ( http_session->m_Requestline.m_Host.Find("appapi.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/json/hotel/createOrder") != -1))	
	{
		return kAndroidOrder;
	}	
	return 0;
}

hotel::HotelYododo::Process(const char *packet,int action)
{
	//web登录
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web注册
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web民宿搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	//web国内酒店搜索
	if (action == kWebIntSearch)
	{
		return WebIntSearch(packet);
	}
	//web国内酒店预订
	if (action == kWebIntNewOrder)
	{
		return WebIntNewOrder(packet);
	}
	//web客栈 登录
	if (action == kWebHotelLogin)
	{
		return WebHotelLogin(packet);
	}
	//web客栈 搜索
	if (action == kWebHotelSearch)
	{
		return WebHotelSearch(packet);
	}
	//web客栈 预订
	if (action == kWebHotelOrder)
	{
		return WebHotelOrder(packet);
	}
	//android客栈 登录
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android客栈 搜索
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//android客栈 订单
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}
//web注册
hotel::HotelYododo::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("user.name",true);
	string password = ms_->GetValueFromMapByKey("user.password",true);
	string phone = ms_->GetValueFromMapByKey("user.info.mobile",true);
	if (username.empty() || password.empty() || phone.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//用户名
		"pass=" + 		password + "\r\n" + 	//密码 
		"islogin=" +    phone    +  "\r\n" +    //手机
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}
//web登录
hotel::HotelYododo::WebLogin(const char *packent)
{
	string src_data(packent);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("ajaxloginemail",true);
	string password = ms_->GetValueFromMapByKey("ajaxloginpassword",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//用户名
		"pass=" + 		password + "\r\n" + 	//密码 
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//web客栈 搜索
hotel::HotelYododo::WebSearch(const char *packet)
{
	string referer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	//目的地
	string cityname = ms_->GetValueBetweenBeginEnd(referer,"/hotels/","?");
	//入住日期
	string livedate = ms_->GetValueBetweenBeginEnd(referer,"starttime","&");
	//退房日期
	string leavedate = ms_->GetValueBetweenBeginEnd(referer,"endtime","&");
	//关键词
	string keyword = ms_->GetValueBetweenBeginEnd(referer,"name","&");
	if (keyword.empty())
	{
		keyword = "null";
	}
	if (cityname.empty() || livedate.empty() || leavedate.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//目的地
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"nick="	 + 	  	keyword + "\r\n" + 		//关键词
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=客栈民宿搜索\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//web国内酒店搜索
hotel::HotelYododo::WebIntSearch(const char *packet)
{
	string refer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	refer = ms_->UTF8ToGBK(UrlDecode(refer));
	ms_->SplitStringToMap(refer,'&','=');
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("starttime");
	//退房日期
	string leavedate = ms_->GetValueFromMapByKey("endtime");
	//关键词
	string keyword = ms_->GetValueFromMapByKey("likename",true);
	if (keyword.empty())
	{
		keyword = "null";
	}
	if ( livedate.empty() || keyword.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"nick="	 + 	  	keyword + "\r\n" + 		//关键词
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店搜索\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//web国内酒店预订
hotel::HotelYododo::WebIntNewOrder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("indate");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("outdate");
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("RoomNum");
	//住宿姓名
	string guestname;
	int num = atoi(roomcount.c_str());
	for (int i = 0;i<num; i++)
	{
		char buffer[100]={0};
		sprintf(buffer,"HotelGuestList[%d].Name",i);
		string tmpname(buffer);
		guestname +=ms_->GetValueFromMapByKey(tmpname,true) + ",";
	}
	string::size_type pos;
	pos = guestname.rfind(",");
	guestname = guestname.substr(0,pos);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("HotelContacter.phoneInfo");
	//联系邮箱 
	string email = ms_->GetValueFromMapByKey("HotelContacter.Email",true);
	//价格
	string price = ms_->GetValueFromMapByKey("TotalPrice") + ms_->GetValueFromMapByKey("Currency");
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	guestname + "\r\n" + 	//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"mid=" + 	  	price + "\r\n" + 		//价格
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//web客栈 登录
hotel::HotelYododo::WebHotelLogin(const char *packent)
{
	string src_data(packent);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("user.email",true);
	string password = ms_->GetValueFromMapByKey("user.password",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//用户名
		"pass=" + 		password + "\r\n" + 	//密码 
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=登录\r\n";						//状态
	WriteIndexFile();
	return 1;
}

//web客栈 搜索
hotel::HotelYododo::WebHotelSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//目的地
	string cityname = ms_->GetValueFromMapByKey("placeName",true);
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("starttime");
	//退房日期
	string leavedate = ms_->GetValueFromMapByKey("endtime");

	if (cityname.empty() || livedate.empty() || leavedate.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//目的地
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=客栈搜索\r\n";				    //状态
	WriteIndexFile();
	return 1;
}

//web客栈 预订
hotel::HotelYododo::WebHotelOrder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("starttime");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("endtime");
	//住客姓名
	string guestname = ms_->GetValueFromMapByKey("realname",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("number");
	//房间类型
	string roomtype = ms_->GetValueFromMapByKey("subject",true);
	if (guestname.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	guestname + "\r\n" + 	//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"clientmac=" + 	roomtype + "\r\n" + 	//房间类型
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=客栈民宿预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}
//android客栈 登录
int hotel::HotelYododo::AndroidLogin(const char *packet)
{
	string url = http_session_->m_Requestline.m_URL.GetBuffer(0);
	ms_->Replace(url,"?","&");
	url = ms_->UrlDecode(url);
    ms_->SplitStringToMap(url,"&","=");
	//用户名
	string username = ms_->GetValueFromMapByKey("loginname",true);
	//密码
	string password = ms_->GetValueFromMapByKey("loginpassword");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +				//用户名 
		"pass=" + password + "\r\n" + 				//密码 	
		"netmark=" + app_ + "\r\n" + 				//来源
		"clientmsg=" + platformAndroid + "\r\n" +	//操作平台
		"state=登录\r\n";						    //状态
	WriteIndexFile();
	return 0;
}

//android客栈 搜索
int hotel::HotelYododo::AndroidSearch(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(url,"?","&");
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');

	string livedate = ms_->GetValueFromMapByKey("checkin",true);
	string leavedate = ms_->GetValueFromMapByKey("checkout",true);
	string keyword = ms_->GetValueFromMapByKey("hotelName",true);
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	string latitude = ms_->GetValueFromMapByKey("latitude",true);
	if (keyword.empty())
	{
		keyword = "null";
	}
	if (livedate.empty() || leavedate.empty() || longitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" +	livedate + "\r\n" + 		//入住日期
		"msgtype=" + 	leavedate + "\r\n" +		//退房日期
		"from="    +    longitude +"\r\n"  +		//经度
		"to="	   +    latitude   + "\r\n" +		//纬度
		"netmark=" +	app_ + "\r\n" + 		    //来源
		"clientmsg=" +  platformAndroid + "\r\n" +  //操作平台
		"state=搜索\r\n";						    //状态
	WriteIndexFile();
	return 1;
}
//android客栈 订单
int hotel::HotelYododo::AndroidOrder(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(url,"?","&");
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("checkin");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("checkout");
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("number");
	//住宿姓名
	string guestname = ms_->GetValueFromMapByKey("realname");
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("mobile");
    if (guestname.empty() || mobile.empty())
    {
		return 0;
    }
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	guestname + "\r\n" + 	//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformAndroid + "\r\n" +  //操作平台
		"state=客栈预订\r\n";				    //状态
	WriteIndexFile();
	return 1;
}