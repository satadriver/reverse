// Yilong.cpp: implementation of the Yilong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Yilong.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Yilong::Yilong(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

Yilong::~Yilong()
{

}

//定义的证件类型
string credential_type[6] = {"身份证","护照","军人证","回乡证","台胞证","港澳通行证"};


int Yilong::Is(HttpSession *http_session)
{
	// PC 火车票
	if((http_session->m_Requestline.m_Host.Find("train.elong.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/isajax/Order/CreateOrder")!=-1))
	{
		return PC_TRAIN;
	}
	// PC 飞机票 国内机票
	if((http_session->m_Requestline.m_Host.Find("flight.elong.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/isajax/FillOrder/SaveOrder")!=-1))
	{
		return PC_AIRPLANE;
	}
    // PC  飞机票 国际及港澳台机票
	if((http_session->m_Requestline.m_Host.Find("iflight.elong.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/saveOrder")!=-1))
	{
		return PC_AIRPLANE_INTER;
	}
	return 0;
}

int Yilong::Process(const char *packet,int action)
{
	//国内机票
	if (action == PC_AIRPLANE)
	{
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//联系人姓名，联系人电话，始发地(中文)，目的地(中文)，出发航班号，出发日期，返程航班号，返程日期，乘客姓名，乘客票类型，乘客证件类型，乘客证件号，乘客生日，订单地址)	
		string Name,MobilePhone,FlightNumber,BackflightNumber,DepartDate,BackDate,DepartCityName,ArriveCityName;
		string PassengerName,PassengerType,PassengerCertType,PassengerCertNumber,PassengerBirthday,OrderUrl,FlightType;
		//获取乘客的人数
		string temp = sContent.substr(sContent.rfind("PassengerList"),sContent.length()-sContent.rfind("PassengerList"));
		int pos = temp.rfind("PassengerList");
		string People = temp.substr(pos+ strlen("PassengerList") + strlen("["),1);
	    int PassengerCount = atoi(People.c_str());
		//联系人姓名
		Name = ms_->GetValueFromMapByKey("Contact.Name",true);
		//联系人电话
		MobilePhone = ms_->GetValueFromMapByKey("Contact.MobilePhone",true); 

		//始发地(中文)
		DepartCityName = ms_->GetValueFromMapByKey("RequestInfo.DepartCityName",true); 
		//目的地(中文))
		ArriveCityName = ms_->GetValueFromMapByKey("RequestInfo.ArriveCityName",true); 

		//航班类型
		FlightType = ms_->GetValueFromMapByKey("RequestInfo.FlightType",true);

		if (strcmp(FlightType.c_str(),"OneWay") == 0)
		{
			//单程
			//出发航班号
			FlightNumber = ms_->GetValueFromMapByKey("flightnum",true); 
			//出发日期
			DepartDate = ms_->GetValueFromMapByKey("RequestInfo.DepartDate",true); 

			type_   = "国内单程机票";
		}
		if (strcmp(FlightType.c_str(),"RoundTrip") == 0)
		{
			//往返
			//出发航班号
			FlightNumber = ms_->GetValueFromMapByKey("flightnumber",true); 
			//出发日期
			DepartDate = ms_->GetValueFromMapByKey("RequestInfo.DepartDate",true); 
			//返程航班号
			BackflightNumber = ms_->GetValueFromMapByKey("backflightnumber",true);
			//返程日期
			BackDate = ms_->GetValueFromMapByKey("RequestInfo.BackDate",true); 

			type_   = "国内往返机票";
		}
		
		//订单地址
		OrderUrl = ms_->GetValueFromMapByKey("Url",true); 

        for (int j=0;j < PassengerCount + 1;j++)
        {
			//转化
			CString str;
			str.Format("%d",j);
			string strEnd = str.GetBuffer(0);
			//乘客姓名
			string strTempName = "PassengerList[" + strEnd + "].Name";
			PassengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//乘客票类型	
			string strTempType = "PassengerList[" + strEnd + "].PassengerType";
			string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
			if (PassengerTypeTemp == "1")
			{
				PassengerType = "成人票";
			}
			if (PassengerTypeTemp == "0")
			{
				PassengerType = "儿童票";
			}
			//乘客证件类型
			string strCertType = "PassengerList[" + strEnd + "].CertType";
			string PassengerCertTypeTemp = ms_->GetValueFromMapByKey(strCertType,true); 
			int temp = atoi(PassengerCertTypeTemp.c_str());
			PassengerCertType = credential_type[temp];
			//乘客证件号
			string strCertNumber = "PassengerList[" + strEnd + "].CertNumber";
			PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
			//乘客生日
			string strBirthday = "PassengerList[" + strEnd + "].Birthday";
			PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true);
			
			if (strcmp(FlightType.c_str(),"OneWay") == 0)
			{
				output_text_ = 
					"mid=" + 		Name + "\r\n" +					//联系人姓名
					"oid=" + 		MobilePhone + "\r\n" +			//联系人电话
					"nick=" + 		DepartCityName + "\r\n" +		//出发地
					"msg=" + 		ArriveCityName + "\r\n" +		//目的地 
					"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
					"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
					"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
					"servername=" + DepartDate + "\r\n" +			//出发日期
					"islogin=" + 	FlightNumber + "\r\n" +			//航班号
					"platform=电脑浏览器\r\n" +
					"state=" + type_ + "\r\n";		
				WriteIndexFile();
			}
			if (strcmp(FlightType.c_str(),"RoundTrip") == 0)
			{
				output_text_ = 
					"mid=" + 		Name + "\r\n" +					//联系人姓名
					"oid=" + 		MobilePhone + "\r\n" +			//联系人电话
					"nick=" + 		DepartCityName + "\r\n" +		//出发地
					"msg=" + 		ArriveCityName + "\r\n" +		//目的地 
					"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
					"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
					"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
					"servername=" + DepartDate + "\r\n" +			//出发日期
					"appname=" + 	BackDate + "\r\n" +				//返程日期
					"islogin=" + 	FlightNumber + "\r\n" +			//航班号
					"platform=电脑浏览器\r\n" +
					"state=" + type_ + "\r\n";	
				WriteIndexFile();
			}
        }		
		return 0;
	}
	//国际及港澳台机票  
	if (action == PC_AIRPLANE_INTER)
	{
		string sContent = ms_->UrlDecode(packet);   
		ms_->SplitStringToMap(sContent,'&','=');

		//单程 (联系人姓名，联系人电话，联系人邮箱，出发地(英文名)，目的地(英文名)，出发日期) + 公共
		//往返 (联系人姓名，联系人电话，联系人邮箱，出发地(英文名)，目的地(英文名)，出发日期，返程航班号，返程日期) + 公共
        //公共 (乘客姓名(英文)，乘客类型，乘客国籍(代码)，乘客性别，乘客生日，乘客证件类型，乘客证件号，乘客证件有效期)	
	
		//获取订单信息
		string InfoNessary;
		InfoNessary = ms_->GetValueFromMapByKey("orderDataStr",true);

		//使用json 读取数据
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(InfoNessary,value))
		{          
			//联系人信息
			string name,mobilePhone,email;
            Json::Value contact = value["contact"];
			//联系人姓名
			name = contact["name"].asString();
			//联系人电话
			mobilePhone = contact["mobilePhone"].asString();
			//联系人邮箱
			email = contact["email"].asString();

			//航班信息
			int flyType;
			string departCityNameEn,arriveCityNameEn,departDate,goFlightNo,backDate;
            Json::Value requestInfo = value["requestInfo"];
			//航班类型
			flyType = requestInfo["flyType"].asInt();

			if (flyType == 0)
			{
				//单程
				//出发地
				departCityNameEn = requestInfo["departCityNameEn"].asString();
				//目的地
				arriveCityNameEn = requestInfo["arriveCityNameEn"].asString();
				//出发日期
				departDate = requestInfo["departDate"].asString();

				type_   = "国际单程机票";
			}
			if (flyType == 1)
			{
				//往返
				//出发地
				departCityNameEn = requestInfo["departCityNameEn"].asString();
				//目的地
				arriveCityNameEn = requestInfo["arriveCityNameEn"].asString();
				//出发日期
				departDate = requestInfo["departDate"].asString();
				//返程航班号
				goFlightNo = requestInfo["goFlightNo"].asString();
				//返程日期
				backDate = requestInfo["backDate"].asString();

				type_   = "国际往返机票";
			}

			//乘客信息
			string PassengerTypeTemp,SexTemp,certTypeTemp;
			string Name,PassengerType,Country,Birthday,Sex,certType,CertNumber,CertPeriod;
			Json::Value passengerList = value["passengerList"];

			for (int k = 0;k <passengerList.size();k++)
			{
				Json::Value flightOrderPassengersPlit = passengerList[k];

				//乘客姓名(英文)
				Name  = flightOrderPassengersPlit["Name"].asString();
				//乘客类型
				PassengerTypeTemp  = flightOrderPassengersPlit["PassengerType"].asString();
				if (PassengerTypeTemp == "0")
				{
					PassengerType = "成人票";
				}
				if (PassengerTypeTemp == "1")
				{
					PassengerType = "儿童票";
				}
				//乘客国籍(代码)
				Country  = flightOrderPassengersPlit["Country"].asString();
				//乘客生日
				Birthday  = flightOrderPassengersPlit["Birthday"].asString();
				//乘客性别
				SexTemp  = flightOrderPassengersPlit["Sex"].asString();
				if (SexTemp == "0")
				{
					Sex = "男";
				}
				if (SexTemp == "1")
				{
					Sex = "女";
				}
				//乘客证件类型
				certTypeTemp  = flightOrderPassengersPlit["certType"].asString();
				if (certTypeTemp == "2")
				{
					certType = "护照";
				}
				if (certTypeTemp == "7")
				{
					certType = "回乡证";
				}
				if (certTypeTemp == "8")
				{
					certType = "台胞证";
				}
				if (certTypeTemp == "10")
				{
					certType = "港澳通行证";
				}
				if (certTypeTemp == "22")
				{
					certType = "台湾通行证";
				}

				//乘客证件号
				CertNumber  = flightOrderPassengersPlit["CertNumber"].asString();
				//乘客证件有效期
				CertPeriod  = flightOrderPassengersPlit["CertPeriod"].asString();
				
				//单程
				if(flyType == 0)
				{
					output_text_ = 
						"mid=" + 		name + "\r\n" +				//联系人姓名
						"oid=" + 		mobilePhone + "\r\n" +		//联系人电话
						"nick=" + 		departCityNameEn + "\r\n" + //出发地
						"msg=" + 		arriveCityNameEn + "\r\n" + //目的地 
						"msgtype=" + 	Name + "\r\n" +				//乘客姓名
						"webname=" + 	certType + "\r\n" +			//乘客证件类型
						"netmark=" + 	CertNumber + "\r\n" +		//乘客证件号
						"servername=" + departDate + "\r\n" +		//出发日期
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";			
					WriteIndexFile();
				}
				//往返
				if(flyType == 1)
				{
					output_text_ = 
						"mid=" + 		name + "\r\n" +				//联系人姓名
						"oid=" + 		mobilePhone + "\r\n" +		//联系人电话
						"nick=" + 		departCityNameEn + "\r\n" + //出发地
						"msg=" + 		arriveCityNameEn + "\r\n" + //目的地 
						"msgtype=" + 	Name + "\r\n" +				//乘客姓名
						"webname=" + 	certType + "\r\n" +			//乘客证件类型
						"netmark=" + 	CertNumber + "\r\n" +		//乘客证件号
						"servername=" + departDate + "\r\n" +		//出发日期
						"appname=" + 	backDate + "\r\n" +			//返程日期
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
        }		
		return 0;
	}
	if (action == PC_TRAIN)
	{
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//(联系人电话，车次，出发日期，始发地(中文)，目的地(中文)，乘客姓名，乘客票类型，乘客证件类型，乘客证件号，乘客生日)
		string contactMobile,trainNumber,startDate,fromStationName,toStationName;
		string PassengerName,PassengerType,PassengerCertType,PassengerCertNumber,PassengerBirthday;	
		//获取乘客的人数
		string temp = sContent.substr(sContent.rfind("passengers"),sContent.length()-sContent.rfind("passengers"));
		int pos = temp.rfind("passengers");
		string People = temp.substr(pos+ strlen("passengers") + strlen("["),1);
		int PassengerCount = atoi(People.c_str());
		//联系人电话
		contactMobile = ms_->GetValueFromMapByKey("contactMobile",true); 		
		//车次
		trainNumber = ms_->GetValueFromMapByKey("trainNumber",true);	
		//出发日期
		startDate = ms_->GetValueFromMapByKey("startDate",true); 
		//始发地(中文)
		fromStationName = ms_->GetValueFromMapByKey("fromStationName",true); 
		//目的地(中文))
		toStationName = ms_->GetValueFromMapByKey("toStationName",true); 
		
		type_   = "火车票";
        for (int j=0;j < PassengerCount + 1;j++)
        {
			//转化
			CString str;
			str.Format("%d",j);
			string strEnd = str.GetBuffer(0);
			//乘客姓名
			string strTempName = "passengers[" + strEnd + "].name";
			PassengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//乘客票类型
			string strTempType = "passengers[" + strEnd + "].passengerType";
			string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
			if (PassengerTypeTemp == "1")
			{
				PassengerType = "成人票";
			}
			if (PassengerTypeTemp == "0")
			{
				PassengerType = "儿童票";
			}
			//乘客证件类型
			string strCertType = "passengers[" + strEnd + "].certType";
			string PassengerCertTypeTemp = ms_->GetValueFromMapByKey(strCertType,true); 
			if (PassengerCertTypeTemp == "1")
			{
				PassengerCertType = "二代身份证";
			}
			if (PassengerCertTypeTemp == "B")
			{
				PassengerCertType = "护照";
			}
			if (PassengerCertTypeTemp == "C")
			{
				PassengerCertType = "港澳通行证";
			}
			//乘客证件号
			string strCertNumber = "passengers[" + strEnd + "].certNumber";
			PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
			//乘客生日
			string strBirthday = "passengers[" + strEnd + "].birthday";
			PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true); 
			
			output_text_ = 
				"oid=" + 		contactMobile + "\r\n" +		//联系人电话
				"nick=" + 		fromStationName + "\r\n" +		//出发地
				"msg=" + 		toStationName + "\r\n" +		//目的地 
				"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
				"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
				"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
				"clientmsg=" + 	trainNumber + "\r\n" +			//车次 
				"servername=" + startDate + "\r\n" +			//出发日期
				"platform=电脑浏览器\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
        }
		return 0;
	}
	return 0; 
}
