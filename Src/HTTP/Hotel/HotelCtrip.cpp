#include "StdAfx.h"
#include "HotelCtrip.h"

hotel::HotelCtrip::HotelCtrip(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "携程";
}

hotel::HotelCtrip::~HotelCtrip()
{

}

hotel::HotelCtrip::IsHotelCtrip(HttpSession *http_session)
{
	//web国内酒店搜索
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotel") != -1 ))	
	{
		return kWebSearch;
	}
	//web国际酒店搜索
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/international") != -1 ))	
	{
		return kWebIntSearch;
	}
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/DomesticBook/DomeInputNewOrderCS.aspx") != -1))	
	{
		return kWebNewOrder;
	}
	//web国际酒店预订 处理包问题
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/internationalbook/inputneworder.aspx") != -1))	
	{
		//return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelCtrip::Process(const char *packet,int action)
{
	//web国内酒店搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	//web国际酒店搜索
	if (action == kWebIntSearch)
	{
		return WebIntSearch(packet);
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
hotel::HotelCtrip::WebSearch(const char *packet)
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

//web国际酒店搜索
hotel::HotelCtrip::WebIntSearch(const char *packet)
{
	//从cookie 中获得数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	//搜索 1 
	if (referer == "http://hotels.ctrip.com/international/")
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
			"state=国际酒店搜索\r\n";				//状态
		WriteIndexFile();
    }
	//搜索 2
	if (referer != "http://hotels.ctrip.com/international/")
	{
		//目的地
		string cityname = ms_->GetValueFromMapByKey("cityName",true);
		//入住日期
		string livedate = ms_->GetValueFromMapByKey("StartTime");
		//退房日期
		string leavedate = ms_->GetValueFromMapByKey("DepTime");
		//关键词
		string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
		string keyword = uri.substr(uri.rfind("/") + strlen("/"));
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
			"state=国际酒店搜索\r\n";				//状态
		WriteIndexFile();
	}
	return 1;
}
//web国内酒店预订
hotel::HotelCtrip::WebNewOrder(const char *packet)
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
hotel::HotelCtrip::WebIntNewOrder(const char *packet)
{
	//从cookie 中获得数据
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//操作地点
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//国际酒店
	//IntHotelCityID=73split%E6%96%B0%E5%8A%A0%E5%9D%A1%EF%BC%8C%E6%96%B0%E5%8A%A0%E5%9D%A1splitsingaporesplit2017-03-15split2017-03-16split2017-03-14%2014%3A49%3A51split0split2split2
	string strTemp = ms_->GetValueFromMapByKey("IntHotelCityID",true);
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
		"state=国际酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}