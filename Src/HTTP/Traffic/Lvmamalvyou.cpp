// Lvmamalvyou.cpp: implementation of the Lvmamalvyou class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lvmamalvyou.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Lvmamalvyou::Lvmamalvyou(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

Lvmamalvyou::~Lvmamalvyou()
{

}

//定义的证件类型
char* credential_type[][2] = 
{{"ID","身份证"},
{"ID_CARD","身份证"},
{"PASSPORT","护照"},
{"GANGAO","港澳通行证"},
{"HUIXIANG","回乡证"},
{"TAIBAO","台湾通行证"},
{"OFFICER","军官证"},
{"SOLDIER","士兵证"},
{"TAIBAOZHENG","台胞证"},
{"OTHER","其他"}};

//获取证件类型
string Lvmamalvyou::GetCredentialType(string passengerIDCardTypeTemp)
{
	for (int iCode = 0; iCode < sizeof(credential_type)/sizeof(credential_type[0]); iCode ++)
	{
		if (passengerIDCardTypeTemp.compare(credential_type[iCode][0]) == 0)
		{
			return credential_type[iCode][1];;
		}
	}
	return passengerIDCardTypeTemp;
}

//实现
void Lvmamalvyou::SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
		
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


int Lvmamalvyou::Is(HttpSession *http_session)
{
	//pc 登录
	if((http_session->m_Requestline.m_Host.Find("login.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/nsso/login")!=-1))
	{
		return PC_LOGIN;
	}
	// PC 火车票
	if((http_session->m_Requestline.m_Host.Find("train.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/book/booking")!=-1))
	{
		return PC_TRAIN;
	}
	// PC 飞机票
	if((http_session->m_Requestline.m_Host.Find("flight.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/submitOrder")!=-1))
	{
		return PC_AIRPLANE;
	}
	// SP 火车票
	if((http_session->m_Requestline.m_Host.Find("m.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/new/api/router/rest.do?method=api.com.train.order.submitorder")!=-1))
	{
		return SP_TRAIN;
	}
	// SP 飞机票
	if((http_session->m_Requestline.m_Host.Find("m.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/client-service/router/rest.do?method=api.com.flight.order.booking")!=-1))
	{
		return SP_AIRPLANE;
	}

	return 0;
}


int Lvmamalvyou::Process(const char *packet,int action)
{
	//pc 登录
	if (action == PC_LOGIN)
	{
		string src_data(packet);
		src_data=ms_->UrlDecode(src_data);
		ms_->SplitStringToMap(src_data,'&','=');
		string user,pass;
		user=ms_->GetValueFromMapByKey("username",true);
		pass=ms_->GetValueFromMapByKey("password",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=登录\r\n";	
		WriteIndexFile();
	}
	//飞机票
	if (action == PC_AIRPLANE)
	{	
		//单程-联系人电话，联系人姓名，出发地，目的地， 出发机场，达到机场，航班号，出发日期，
		//往返-联系人电话，联系人姓名，出发地，目的地， 去程出发机场，去程达到机场，去程航班号，去程日期，返程出发机场，返程到达机场，返程航班号，返程日期，
        //公共-乘客姓名，乘客类型，乘客证件类型，乘客姓名，乘客证件类型，乘客证件号码，乘客生日，总价格
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		//使用json 读取数据
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(sContent,value))
		{          
			//联系人信息
            Json::Value flightOrderContacter = value["flightOrderContacter"];
			//联系人姓名
			name_    = flightOrderContacter["name"].asString();
			//联系人电话
			tel_     = flightOrderContacter["cellphone"].asString();

			//OW 单程机票 RT 往返机票 
			string routeTypeTemp,routeType;
			routeTypeTemp  = value["routeType"].asString();
			if (routeTypeTemp == "OW")
			{
				routeType == "单程机票";
			}
			if (routeTypeTemp == "RT")
			{
				routeType == "往返机票";
			}
			//出发地
			start_ = value["deptCityName"].asString();
			//目的地
			end_  = value["arrvCityName"].asString();

            //航班信息
			string flightTripType,seatClassCode;
			string departureAirOw,arrivalAirOw,flightNoOw,departureDateOw;
			string departureAirRt,arrivalAirRt,flightNoRt,departureDateRt;

			//去程
			if (routeTypeTemp == "OW")
			{
				Json::Value flightOrderBookingDetailRequests = value["flightOrderBookingDetailRequests"];
				//注意 这个地方的json 目前只有一个 0 去程 1 返程
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = flightOrderBookingDetailRequests[f];
				//出发机场
				departureAirOw = value["owdepartureAirName"].asString() + value["owdepartureAirBulding"].asString();
				//达到机场
				arrivalAirOw  = value["owarrivalAirName"].asString() + value["owarrivalAirBulding"].asString();
				//航班号
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//出发日期
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();

				type_   = "国内单程机票";
			} 
			//返程
			if(routeTypeTemp == "RT")
			{
				Json::Value flightOrderBookingDetailRequests = value["flightOrderBookingDetailRequests"];
				//注意 这个地方的json 目前只有一个 0 去程 1 返程
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = flightOrderBookingDetailRequests[f];
				//去程出发机场
				departureAirOw = value["owdepartureAirName"].asString() + value["owdepartureAirBulding"].asString();
				//去程达到机场
				arrivalAirOw  = value["owarrivalAirName"].asString() + value["owarrivalAirBulding"].asString();
				//去程航班号
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//去程日期
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();
				
				int g = 1;
				Json::Value flightOrderBookingDetailRequestsPlitRT = flightOrderBookingDetailRequests[g];
				//返程出发机场
				departureAirRt = value["rtdepartureAirName"].asString() + value["rtdepartureAirBulding"].asString();	
				//返程达到机场
				arrivalAirRt  = value["rtarrivalAirName"].asString() + value["rtarrivalAirBulding"].asString();
				//返程航班号
				flightNoRt = flightOrderBookingDetailRequestsPlitRT["flightNo"].asString();
				//返程日期
				departureDateRt  = flightOrderBookingDetailRequestsPlitRT["departureDate"].asString();

				type_   = "国内往返机票";
			}

			//总价格
			price_  = value["orderTotalSalesAmount"].asString();

			//乘客信息
			string strTemp,passengerName,passengerIDCardType,passengerIDCardTypeTemp,passengerIDCardNo,gender,passengerType,passengerBirthday;
			Json::Value flightOrderPassengers = value["flightOrderPassengers"];
			for (int k = 0;k <flightOrderPassengers.size();k++)
			{
				Json::Value flightOrderPassengersPlit = flightOrderPassengers[k];
				//乘客姓名
				passengerName  = flightOrderPassengersPlit["passengerName"].asString();
				//乘客证件类型
				passengerIDCardTypeTemp = flightOrderPassengersPlit["passengerIDCardType"].asString();
				passengerIDCardType = GetCredentialType(passengerIDCardTypeTemp);
				//乘客证件号码
				passengerIDCardNo = flightOrderPassengersPlit["passengerIDCardNo"].asString();
				//乘客性别
				gender = flightOrderPassengersPlit["gender"].asString();
				//乘客类型
				passengerType = flightOrderPassengersPlit["passengerType"].asString();
				//乘客生日
				passengerBirthday = flightOrderPassengersPlit["passengerBirthday"].asString();
		
				//单程
				if(routeTypeTemp == "OW")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//联系人姓名
						"oid=" + 		tel_ + "\r\n" +					//联系人电话
						"nick=" + 		start_ + "\r\n" +				//出发地
						"msg=" + 		end_ + "\r\n" +					//目的地 
						"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
						"webname=" + 	passengerIDCardType + "\r\n" +	//乘客证件类型
						"netmark=" + 	passengerIDCardNo + "\r\n" +	//乘客证件号
						"onick=" + 		departureAirOw + "\r\n" +		//出发机场
						"lang=" + 		arrivalAirOw + "\r\n" +			//到达机场
						"servername=" + departureDateOw + "\r\n" +		//出发日期
						"islogin=" + 	flightNoOw + "\r\n" +			//航班号
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				//往返
				if(routeTypeTemp == "RT")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//联系人姓名
						"oid=" + 		tel_ + "\r\n" +					//联系人电话
						"nick=" + 		start_ + "\r\n" +				//出发地
						"msg=" + 		end_ + "\r\n" +					//目的地 
						"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
						"webname=" + 	passengerIDCardType + "\r\n" +	//乘客证件类型
						"netmark=" + 	passengerIDCardNo + "\r\n" +	//乘客证件号
						"onick=" + 		departureAirOw + "\r\n" +		//出发机场
						"mid=" + 		arrivalAirOw + "\r\n" +			//到达机场
						"servername=" + departureDateOw + "\r\n" +		//出发日期
						"appname=" + 	departureDateRt + "\r\n" +		//返程日期
						"islogin=" + 	flightNoOw + "\r\n" +			//航班号
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";	
					WriteIndexFile();
				}
			}
		}
		return 0;
	}
    //火车票
	if (action == PC_TRAIN)
	{
		//联系人电话，联系人姓名，车次，车号，始发地(中文)，目的地(中文)，出发时间，到站时间，乘客姓名，乘客类型，乘客证件类型，乘客生日，总价格)
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		//使用json 读取数据
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(sContent,value))
		{          
			//联系人姓名
			name_    = value["contact"].asString();
			//联系人电话
			tel_     = value["contactphone"].asString();
			//车次信息
			string trainNo;
			//车次
			number_ = value["traincode"].asString();
			//车号
			trainNo = value["trainNo"].asString();
			//始发地(中文)
			start_   = value["fromstationname"].asString();
			//目的地(中文)
			end_     = value["tostationname"].asString();
			//出发时间  
			time_ = value["startdate"].asString() + " " + value["fromtime"].asString();
			//到站时间
			string arriveTime;  
			arriveTime = value["arrivedate"].asString() + " " + value["totime"].asString();
			//总价格
			price_  = value["totalprice"].asString();
			type_   = "火车票";

			//乘客信息
			string strTemp,passengerName,passengerType,passengerIDCardType,passengerIDCardNo,passengerBirthday;   
			Json::Value passengers = value["passengers"];
			for (int k = 0;k <passengers.size();k++)
			{
				Json::Value passengersPlit = passengers[k];
				//乘客姓名
				passengerName  = passengersPlit["passengerName"].asString();
				//乘客类型
				passengerType = passengersPlit["passengerType"].asString();
				//乘客证件类型
				passengerIDCardType = passengersPlit["cardType"].asString();
				if (passengerIDCardType == "ID_CARD")
				{
					passengerIDCardType = "身份证";
				}
				if (passengerIDCardType == "GANGAO")
				{
					passengerIDCardType = "港澳通行证";
				}
				if (passengerIDCardType == "TAIBAO")
				{
					passengerIDCardType = "台湾通行证";
				}
				if (passengerIDCardType == "HUZHAO")
				{
					passengerIDCardType = "护照";
				}
				//乘客证件号码
				passengerIDCardNo = passengersPlit["cardNumbers"].asString();
				//乘客生日
				passengerBirthday = passengersPlit["passengerBirthday"].asString();

				output_text_ = 
					"mid=" + 		name_ + "\r\n" +				//联系人姓名
					"oid=" + 		tel_ + "\r\n" +					//联系人电话
					"nick=" + 		start_ + "\r\n" +				//出发地
					"msg=" + 		end_ + "\r\n" +					//目的地 
					"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
					"webname=" + 	passengerIDCardType + "\r\n" +	//乘客证件类型
					"netmark=" + 	passengerIDCardNo + "\r\n" +	//乘客证件号
					"servername=" + time_ + "\r\n" +				//出发日期
					"platform=电脑浏览器\r\n" +
					"state=" + type_ + "\r\n";		
				WriteIndexFile();
			}
		}		
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//飞机票
	if (action == SP_AIRPLANE)
	{

		//单程 (联系人电话，联系人姓名，出发机场(代码)，目的机场(代码)，航班号，出发日期) + 公共
		//往返 (联系人电话，联系人姓名，去程出发机场(代码)，去程目的机场(代码)，去程航班号，去程日期，返程出发机场，返程到达机场，返程航班号，返程日期) + 公共
		//公共 (乘客姓名，乘客类型，乘客证件类型，乘客生日，总价格)
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
		//提取所需数据
		string sContent;
		sContent = ms_->GetValueFromMapByKey("param",true); 
		//使用json 读取数据
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(sContent,value))
		{          
			  
			//联系人信息
            Json::Value orderContacter = value["orderContacter"];
			//联系人姓名
			name_    = orderContacter["name"].asString();
			//联系人电话
			tel_     = orderContacter["cellphone"].asString();
			
			//OW 单程机票 RT 往返机票 
			string routeTypeTemp,routeType;
			routeTypeTemp  = value["routeType"].asString();
			if (routeTypeTemp == "OW")
			{
				routeType == "单程机票";
			}
			if (routeTypeTemp == "RT")
			{
				routeType == "往返机票";
			}

			//航班信息
			string flightTripType,seatClassCode;
			string departureAirOw,arrivalAirOw,flightNoOw,departureDateOw;
			string departureAirRt,arrivalAirRt,flightNoRt,departureDateRt;
			
			//去程
			if (routeTypeTemp == "OW")
			{
				Json::Value orderBookingDetailRequests = value["orderBookingDetailRequests"];
				//注意 这个地方的json 目前只有一个 0 去程 1 返程
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = orderBookingDetailRequests[f];
				//去程出发机场
				departureAirOw = flightOrderBookingDetailRequestsPlit["departureAirportCode"].asString();
				//去程达到机场
				arrivalAirOw  = flightOrderBookingDetailRequestsPlit["arrivalAirportCode"].asString();
				//去程航班号
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//去程日期
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();
				
				type_   = "国内单程机票";
			} 
			//返程
			if(routeTypeTemp == "RT")
			{
				Json::Value orderBookingDetailRequests = value["orderBookingDetailRequests"];
				//注意 这个地方的json 目前只有一个 0 去程 1 返程
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = orderBookingDetailRequests[f];
				//去程出发机场
				departureAirOw = flightOrderBookingDetailRequestsPlit["departureAirportCode"].asString();
				//去程达到机场
				arrivalAirOw  = flightOrderBookingDetailRequestsPlit["arrivalAirportCode"].asString();
				//去程航班号
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//去程日期
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();
				
				int g = 1;
				Json::Value flightOrderBookingDetailRequestsPlitRT = orderBookingDetailRequests[g];
				//返程出发机场
				departureAirRt = flightOrderBookingDetailRequestsPlitRT["departureAirportCode"].asString();	
				//返程达到机场
				arrivalAirRt  = flightOrderBookingDetailRequestsPlitRT["arrivalAirportCode"].asString();
				//返程航班号
				flightNoRt = flightOrderBookingDetailRequestsPlitRT["flightNo"].asString();
				//返程日期
				departureDateRt  = flightOrderBookingDetailRequestsPlitRT["departureDate"].asString();
				
				type_   = "国内往返机票";
			}		
			//总价格
			price_  = value["totalSalesAmount"].asString();

			//乘客信息
			string strTemp,passengerName,passengerIDCardType,passengerIDCardTypeTemp,passengerIDCardNo,gender,passengerType,passengerBirthday;
			Json::Value flightOrderPassengers = value["orderPassengers"];
			for (int k = 0;k <flightOrderPassengers.size();k++)
			{
				Json::Value flightOrderPassengersPlit = flightOrderPassengers[k];
				//乘客姓名
				passengerName  = flightOrderPassengersPlit["passengerName"].asString();
				//乘客类型
				passengerType = flightOrderPassengersPlit["passengerType"].asString();
				//乘客证件类型
				passengerIDCardTypeTemp = flightOrderPassengersPlit["passengerIDCardType"].asString();
				passengerIDCardType = GetCredentialType(passengerIDCardTypeTemp);
				//乘客证件号码
				passengerIDCardNo = flightOrderPassengersPlit["passengerIDCardNo"].asString();
				//乘客生日
				passengerBirthday = flightOrderPassengersPlit["passengerBirthday"].asString();
			
				//单程
				if(routeTypeTemp == "OW")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +					//联系人姓名
						"oid=" + 		tel_ + "\r\n" +						//联系人电话
						"msgtype=" + 	passengerName + "\r\n" +			//乘客姓名
						"webname=" + 	passengerIDCardType + "\r\n" +		//乘客证件类型
						"netmark=" + 	passengerIDCardNo + "\r\n" +		//乘客证件号
						"onick=" + 		departureAirOw + "\r\n" +			//出发机场
						"mid=" + 		arrivalAirOw + "\r\n" +				//到达机场
						"servername=" + departureDateOw + "\r\n" +			//出发日期
						"islogin=" + 	flightNoOw + "\r\n" +				//航班号
						"platform=手机app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				//往返
				if(routeTypeTemp == "RT")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +					//联系人姓名
						"oid=" + 		tel_ + "\r\n" +						//联系人电话
						"msgtype=" + 	passengerName + "\r\n" +			//乘客姓名
						"webname=" + 	passengerIDCardType + "\r\n" +		//乘客证件类型
						"netmark=" + 	passengerIDCardNo + "\r\n" +		//乘客证件号
						"onick=" + 		departureAirOw + "\r\n" +			//出发机场
						"mid=" + 		arrivalAirOw + "\r\n" +				//到达机场
						"servername=" + departureDateOw + "\r\n" +			//出发日期
						"appname=" + 	departureDateRt + "\r\n" +			//返程日期
						"islogin=" + 	flightNoOw + "\r\n" +				//航班号
						"platform=手机app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
		}	
		return 0;
	}
	//火车票
	if (action == SP_TRAIN)
	{

		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
	
		//联系人姓名
		name_    = ms_->GetValueFromMapByKey("contact",true);
		//联系人电话
		tel_     = ms_->GetValueFromMapByKey("contactphone",true); 	
		//火车车次 
		number_ = ms_->GetValueFromMapByKey("traincode",true);
		//火车编号
		string trainNo = ms_->GetValueFromMapByKey("trainNo",true);
		//出发地
		start_   = ms_->GetValueFromMapByKey("fromstationname",true);
		//目的地
		end_     = ms_->GetValueFromMapByKey("tostationname",true);	
		//出发时间
		time_ = ms_->GetValueFromMapByKey("startdate",true) + " " + ms_->GetValueFromMapByKey("fromtime",true);
		//到达时间
		string arrTime = ms_->GetValueFromMapByKey("arrivedate",true) + " " + ms_->GetValueFromMapByKey("totime",true);

		//总价格
		price_  = ms_->GetValueFromMapByKey("totalprice",true);
		type_   = "火车票";	

		//乘客信息
		//names=郎朗&names=韦觅丹&types=CONTACT&types=CONTACT&cardNumbers=G19782981&cardNumbers=320684198109198745&cardTypes=HUZHAO&cardTypes=ID_CARD
		string passengerName,passengerIsAdult,passengerCardType,passengerCardNumber,passengerBirthday;
		vector<string> vPassengerName,vPassengerIsadult,vPassengerCardType,vPassengerCardNum,vPassengerBirthday;
		//乘客数量
        string headcount = ms_->GetValueFromMapByKey("headcounts",true);
		int headNumber = atoi(headcount.c_str());

		//乘客姓名
		string strNameTemp = src_data.substr(src_data.find("names"),(src_data.find("types") - src_data.find("names")) - 1);
		ms_->Replace(strNameTemp,"names=","");
		//分隔符 &
		SplitString(strNameTemp, vPassengerName,"&"); 

		//乘客类型
		string strIsAdult = src_data.substr(src_data.find("isAdults"),(src_data.find("insuranceProId") - src_data.find("isAdults")) - 1);
		ms_->Replace(strIsAdult,"isAdults=","");
		//分隔符 &
		SplitString(strIsAdult, vPassengerIsadult,"&"); 

		//乘客证件类型
		string strCardTypeTemp = src_data.substr(src_data.find("cardTypes"),(src_data.find("passengerSexs") - src_data.find("cardTypes")) - 1);
		ms_->Replace(strCardTypeTemp,"cardTypes=","");
		//分隔符 &
		SplitString(strCardTypeTemp, vPassengerCardType,"&"); 

		//乘客证件号
		string strCardNumTemp = src_data.substr(src_data.find("cardNumbers"),(src_data.find("cardTypes") - src_data.find("cardNumbers")) - 1);
		ms_->Replace(strCardNumTemp,"cardNumbers=","");
		//分隔符 &
		SplitString(strCardNumTemp, vPassengerCardNum,"&"); 
       
		//乘客生日
		string strbirthdaysTemp = src_data.substr(src_data.find("birthdays"),(src_data.find("isInsurance") - src_data.find("birthdays")) - 1);
		ms_->Replace(strbirthdaysTemp,"birthdays=","");
		//分隔符 &
		SplitString(strbirthdaysTemp, vPassengerBirthday,"&"); 

		for (int i = 0;i< headNumber;i++)
		{
			passengerName = vPassengerName[i];
			passengerIsAdult = vPassengerIsadult[i];
			if (passengerIsAdult == "0")
			{
				passengerIsAdult = "儿童票";
			}
			if (passengerIsAdult == "1")
			{
				passengerIsAdult = "成人票";
			}
			passengerCardType = vPassengerCardType[i];
			if (passengerCardType == "ID_CARD")
			{
				passengerCardType = "身份证";
			}
			if (passengerCardType == "GANGAO")
			{
				passengerCardType = "港澳通行证";
			}
			if (passengerCardType == "TAIBAO")
			{
				passengerCardType = "台湾通行证";
			}
			if (passengerCardType == "HUZHAO")
			{
				passengerCardType = "护照";
			}
			passengerCardNumber = vPassengerCardNum[i];
			passengerBirthday = vPassengerBirthday[i];

			output_text_ = 
				"mid=" + 		name_ + "\r\n" +				//联系人姓名
				"oid=" + 		tel_ + "\r\n" +					//联系人电话
				"nick=" + 		start_ + "\r\n" +				//出发地
				"msg=" + 		end_ + "\r\n" +					//目的地 
				"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
				"webname=" + 	passengerCardType + "\r\n" +	//乘客证件类型
				"netmark=" + 	passengerCardNumber + "\r\n" +	//乘客证件号
				"clientmsg=" + 	number_ + "\r\n" +				//车次 
				"servername=" + time_ + "\r\n" +				//出发日期
				"platform=手机app\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;
	}
	return 0; 
}
