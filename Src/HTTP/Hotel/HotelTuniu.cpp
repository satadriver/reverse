#include "StdAfx.h"
#include "HotelTuniu.h"

hotel::HotelTuniu::HotelTuniu(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "途牛";
}

hotel::HotelTuniu::~HotelTuniu()
{

}

hotel::HotelTuniu::IsHotelTuniu(HttpSession *http_session)
{
	//web国内酒店预订
	if( ( http_session->m_Requestline.m_Host.Find("hotel.tuniu.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/subscribe") != -1))	
	{
		return kWebNewOrder;
	}
	//web国际酒店预订 
	if( ( http_session->m_Requestline.m_Host.Find("globalhotel.tuniu.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/yii.php?r=/globalHotel/ajax/addOrder") != -1))	
	{
		return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelTuniu::Process(const char *packet,int action)
{
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

//web国内酒店预订
hotel::HotelTuniu::WebNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//酒店名称
	string hotelname = ms_->GetValueFromMapByKey("hotelName",true);
	//房间类型 
	string roomtype = ms_->GetValueFromMapByKey("roomName",true);
	//房间数量 
	string roomcount = ms_->GetValueFromMapByKey("roomNum");
	//入住日期 
	string livedate = ms_->GetValueFromMapByKey("checkInDate");
	//退房日期 
	string leavedate = ms_->GetValueFromMapByKey("checkOutDate");
	//住客姓名 
	string name;
	int pos1 = src_data.find("contactName");
	int pos2 = src_data.find("&tel");
	string tempname = src_data.substr(pos1,pos2 - pos1);
	tempname = ms_->UTF8ToGBK(UrlDecode(tempname));
	while (tempname.find("&") != string::npos)
	{
		string strtemp = tempname.substr(0,tempname.find("&"));
		string str1 = strtemp.substr(strtemp.find("=") +1);
		name += str1;
		name += ",";
		tempname = tempname.substr(tempname.find("&") + 1);
	}
	name += tempname.substr(tempname.find("=") +1);
	//联系手机 
	string mobile = ms_->GetValueFromMapByKey("intlCode") + ms_->GetValueFromMapByKey("tel");
	//联系邮箱
	string email = ms_->GetValueFromMapByKey("email",true);
	if (name.empty() || mobile.empty())
	{
		return 0;
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

//web国际酒店预订
hotel::HotelTuniu::WebIntNewOrder(const char *packet)
{
	//数据部分
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//数据
	string d = ms_->GetValueFromMapByKey("d",true);
	//入住日期 退房日期 住客姓名 联系手机 联系邮箱 房间数量
	string livedate,leavedate,name,mobile,email,roomcount,totalprice;
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(d,value))
	{
		//入住日期
		livedate = value["checkInDate"].asString();
		//退房日期
		leavedate = value["checkOutDate"].asString();
		//联系手机
		mobile = value["contactPhoneNum"].asString();
		//联系邮箱
		email = value["contactEmail"].asString();
		//价格
		double price = value["totalPrice"].asDouble();
		CString strprice;
		strprice.Format("%f",price);
		totalprice = strprice.GetBuffer(strprice.GetLength());
		//房间数量
		int room = value["numberOfRooms"].asInt();
		CString strroom;
		strroom.Format("%d",room);
		roomcount = strroom.GetBuffer(strroom.GetLength());
		//住客姓名 
		const Json::Value customerList = value["customerList"];
		for (int i=0;i<customerList.size();i++)
		{
			Json::Value customerInfo = customerList[i]["customerInfo"];
			for(int j=0;j<customerInfo.size();j++)
			{
				
				string tempname;
				tempname = customerInfo[j]["firstName"].asString() + customerInfo[j]["lastName"].asString();
				name += tempname;
				name += ",";
			}
		}
		if (name.rfind(",") != string::npos)
		{
			name = name.substr(0,name.rfind(","));
		}
	}
	if (name.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"subject=" + 	name + "\r\n" + 		//住客姓名
		"islogin=" + 	mobile + "\r\n" + 		//联系手机
		"oid=" + 	  	email + "\r\n" + 		//联系邮箱
		"mid=" + 	  	totalprice + "\r\n" + 	//价格
		"fileurl=" + 	roomcount + "\r\n" + 	//房间数量
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=国际酒店预订\r\n";				//状态
	WriteIndexFile();
	return 1;
}