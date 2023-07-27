#include "StdAfx.h"
#include "HotelElong.h"

hotel::HotelElong::HotelElong(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "艺龙";
}

hotel::HotelElong::~HotelElong()
{

}

hotel::HotelElong::IsHotelElong(HttpSession *http_session)
{
	//web国内酒店搜索
	if( ( http_session->m_Requestline.m_Host.Find("hotel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/ajax/list/getpageonedata") != -1 ))	
	{
		//return kWebSearch;
	}
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("hotel.elong.com") != -1))	
	{
		//return kWebNewOrder;
	}
	//web国际酒店预订 处理包问题
	if( ( http_session->m_Requestline.m_Host.Find("ihotel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/submit") != -1))	
	{
		return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelElong::Process(const char *packet,int action)
{
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

//web国内酒店搜索
hotel::HotelElong::WebSearch(const char *packet)
{
	//从cookie 中获得数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,'&','=');
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
//搜索 1 
    if (referer.find("hotel") == string::npos)
    {
		//目的地
		string cityname = ms_->GetValueFromMapByKey("CityName",true);
		//入住日期
		string livedate = ms_->GetValueFromMapByKey("checkIn");
		//退房日期
		string leavedate = ms_->GetValueFromMapByKey("checkOut");
		//关键词
		string keyword = ms_->GetValueFromMapByKey("keywordNew",true);
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
			"clientip=" + 	placelogin + "\r\n" + 	//操作地点
			"netmark=" +	app_ + "\r\n" + 		//来源
			"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
			"state=国内酒店搜索\r\n";				//状态
		WriteIndexFile();
    }
//搜索 2
	if (referer.find("hotel") != string::npos)
	{
		//目的地
		string cityname = ms_->GetValueFromMapByKey("cityName",true);
		//入住日期
		string livedate = ms_->GetValueFromMapByKey("StartTime");
		//退房日期
		string leavedate = ms_->GetValueFromMapByKey("DepTime");
		//关键词
		string keyword = ms_->GetValueFromMapByKey("txtkeyword",true);
		//经度
		string longitude = ms_->GetValueFromMapByKey("cityLng");
		//纬度
		string latitude = ms_->GetValueFromMapByKey("cityLat");
		if (keyword.empty())
		{
			keyword = "null";
		}
		if (cityname.empty() || livedate.empty() || leavedate.empty() || longitude.empty() || latitude.empty())
		{
			return 0;
		}
		output_text_ =
			"servername=" + cityname + "\r\n" + 	//目的地
			"appname=" + 	livedate + "\r\n" + 	//入住日期
			"msgtype=" + 	leavedate + "\r\n" +	//退房日期
			"nick="	 + 	  	keyword + "\r\n" + 		//关键词
			"from=" + 	  	longitude + "\r\n" + 	//经度 
			"to=" + 	  	latitude + "\r\n" + 	//纬度
			"clientip=" + 	placelogin + "\r\n" + 	//操作地点
			"netmark=" +	app_ + "\r\n" + 		//来源
			"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
			"state=国内酒店搜索\r\n";				//状态
		WriteIndexFile();
	}
	return 1;
}

//web国内酒店预订
hotel::HotelElong::WebNewOrder(const char *packet)
{
	//从cookie 中获得数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//操作地点
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//入住日期
	string livedate = ms_->GetValueFromMapByKey("Arrival");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("Departure");
	//住客姓名
	string guestname = ms_->GetValueFromMapByKey("GuestName",true);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("MobilePhone");
	//联系邮箱 
	string email = ms_->GetValueFromMapByKey("ContactEmail");
	//联系固话 
	string tel = ms_->GetValueFromMapByKey("ContactTelArea") + "-" + 
				 ms_->GetValueFromMapByKey("ContactTelNum");
	if (tel.empty())
	{
		tel = "null";
	}
	//价格
	string price = ms_->GetValueFromMapByKey("Price") + ms_->GetValueFromMapByKey("Currency");
	//房间数量
	string roomcount = ms_->GetValueFromMapByKey("selectedRoomCount");
	//房间类型
	string roomtype = ms_->GetValueFromMapByKey("RoomName",true);
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	guestname + "\r\n" + 	//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"onick=" + 	  	tel + "\r\n" + 			//联系固话
		"mid=" + 	  	price + "\r\n" + 		//价格
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"clientmac=" + 	roomtype + "\r\n" + 	//房间类型
		"clientip=" + 	placelogin + "\r\n" + 	//操作地点
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国内酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}

//web国际酒店预订
hotel::HotelElong::WebIntNewOrder(const char *packet)
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