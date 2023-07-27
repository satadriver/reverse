#include "StdAfx.h"
#include "HotelRujia.h"

hotel::HotelRujia::HotelRujia(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "如家";
}

hotel::HotelRujia::~HotelRujia()
{

}

hotel::HotelRujia::IsHotelRujia(HttpSession *http_session)
{
	//web注册
	if( ( http_session->m_Requestline.m_Host.Find("www.homeinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/member/register") != -1 ))	
	{
		return kWebRegister;
	}
	//web国内酒店搜索
	if( http_session->m_Requestline.m_Host.Find("www.homeinns.com") != -1 && 
		(http_session->m_Requestline.m_URI.Find("/list/") != -1 || http_session->m_Requestline.m_URI.Find("/map/") != -1))	
	{

		return kWebSearch;
	}
	//web国内酒店预订 1 
	if( http_session->m_Requestline.m_Host.Find("www.homeinns.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/order/OrderCreateSubmit") != -1)	
	{
		return kWebNewOrder;
	}
	//web国内酒店预订 2
	if( http_session->m_Requestline.m_Host.Find("www.bthhotels.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/order/OrderCreateSubmit") != -1)	
	{
		return kWebNewOrder;
	}
	//web国际酒店预订 处理包问题
	if( ( http_session->m_Requestline.m_Host.Find("ihotel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/submit") != -1))	
	{
		return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelRujia::Process(const char *packet,int action)
{
	//web注册
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web国内酒店搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	//web国内酒店预订
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	//web国际酒店预订
	if (action == kWebIntNewOrder)
	{
		return WebIntNewOrder(packet);
	}
	return 0;
}
//web注册
hotel::HotelRujia::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("TrueName") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("TrueName"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	string name = ms_->GetValueFromMapByKey("TrueName",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//密码 
	string password = ms_->GetValueFromMapByKey("password");
	string password2 = ms_->GetValueFromMapByKey("password2");
	if (password != password2)
	{
		return 0;
	}
	//性别 
	string sex = ms_->GetValueFromMapByKey("sex",true);	
	//联系邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
    if (mobile.empty() || password.empty())
    {
		return 0;
    }
	output_text_ =
		"wifissid=" + 	name + "\r\n" + 		//姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"pass=" + 		password + "\r\n" +		//密码
		"sex=" +		sex + "\r\n" +			//性别
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=注册\r\n";						//状态
	WriteIndexFile();
	return 1;
}
//web国内酒店搜索
hotel::HotelRujia::WebSearch(const char *packet)
{
	//数据部分
	string src_data(packet);
	if (src_data.rfind("cityName_Chinese") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("cityName_Chinese"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//目的地
	string cityname = ms_->GetValueFromMapByKey("cityName_Chinese",true);
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("beginDate");
	//退房日期
	string leavedate = ms_->GetValueFromMapByKey("endDate");
	//关键词
	string keyword = ms_->GetValueFromMapByKey("keyDescript",true);
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
		"state=国内酒店搜索\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//web国内酒店预订
hotel::HotelRujia::WebNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//目的地
	string cityname = ms_->GetValueFromMapByKey("HotelNm",true);
	//入住日期 
	string livedate = ms_->GetValueFromMapByKey("s_ArrDate");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("s_DepDate");
	//住客姓名 
	string name = ms_->GetValueFromMapByKey("s_GustNm",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("s_Tel");
	//联系邮箱 
	string email = ms_->GetValueFromMapByKey("s_Email");
	if (email.empty())
	{
		email = "null";
	}
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("s_RmNum");
	if (cityname.empty() || livedate.empty() || leavedate.empty() || name.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" +		//目的地
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	name + "\r\n" + 		//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//web国际酒店预订
hotel::HotelRujia::WebIntNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//目的地 入住日期 退房日期 住客姓名 联系手机 联系邮箱 经度 纬度 房间数量 房间类型 价格

	string cityname,livedate,leavedate,name,mobile,email,longitude,latitude,roomcount,roomtype,price;
	//入住日期
	livedate = ms_->GetValueFromMapByKey("check_in");
	//退房日期 
	leavedate = ms_->GetValueFromMapByKey("check_out");
	//房间数量
	roomcount = ms_->GetValueFromMapByKey("room_num");

	//订单信息
	string orderData = ms_->GetValueFromMapByKey("orderData",true);
	//
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(orderData,value))
	{
		//价格
		Json::Value paymentInfo = value["paymentInfo"];
		double dbprice = paymentInfo["roomAmount"].asDouble(); 
		CString strPrice;
		strPrice.Format("%f",dbprice);
		price = strPrice.GetBuffer(0);

		//联系信息
		Json::Value contactPerson = value["contactPerson"];
		//联系手机
		mobile = contactPerson["mobileTelephone"].asString();
		//联系邮箱
		email = contactPerson["email"].asString();
		//订单信息
		Json::Value interHotelOrder = value["interHotelOrder"];
		Json::Value interHotelProduct = interHotelOrder["interHotelProducts"];
		for(int j= 0;j < interHotelProduct.size();j++)
		{
			//目的地
			cityname = interHotelProduct[j]["hotelCountry"].asString() +
							  interHotelProduct[j]["hotelCity"].asString() +
							  interHotelProduct[j]["hotelName"].asString();
			//经度 
			longitude = interHotelProduct[j]["hotelLongitude"].asString();
			//纬度
			latitude = interHotelProduct[j]["hotelLatitude"].asString();
			//房间类型
			roomtype = interHotelProduct[j]["addValues"].asString();
		}
		//住客信息
		Json::Value travelers = interHotelOrder["travelers"];
		for (int k = 0;k< travelers.size();k++)
		{
			string tempName;
			tempName = travelers[k]["firstname"].asString() + travelers[k]["surname"].asString();
			name += tempName;
			name += ",";
		}
	}
	if (cityname.empty() || livedate.empty() || leavedate.empty() || name.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//目的地
		"appname=" +	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	name + "\r\n" + 		//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"from=" + 	  	longitude + "\r\n" + 	//经度 
		"to=" + 	  	latitude + "\r\n" + 	//纬度
		"mid=" + 	  	price + "\r\n" + 		//价格
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"clientmac=" + 	roomtype + "\r\n" + 	//房间类型
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国际酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}