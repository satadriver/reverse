
#include "StdAfx.h"
#include "PlaneCaptialAirlines.h"

plane::CaptalAirLines::CaptalAirLines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::CaptalAirLines::~CaptalAirLines()
{
	
}
		
//NI 身份证 PP 有效护照 JG 军官证 ID 其他  --首都航空
string plane::CaptalAirLines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "身份证";}
	if (idType == "PP"){idTypeTrue = "有效护照";}
	if (idType == "JG"){idTypeTrue = "军官证";}
	if (idType == "ID"){idTypeTrue = "其他";}
	return idTypeTrue;
}

int plane::CaptalAirLines::IsCaptalAirLines(HttpSession *http_session)
{	
	//国内单程 往返 查询
	if((http_session->m_Requestline.m_Host.Find("www.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/b2c-flight/searchflight.action")!=-1)) 
	{										
		return kpcticketcn;
	}	
	//国际单程 往返 查询
	if((http_session->m_Requestline.m_Host.Find("new.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jdair/ticket/airList")!=-1))
	{
		return kpcticketint;
	}	
	//国内单程 往返 提交订单
	if((http_session->m_Requestline.m_Host.Find("www.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/b2c-flight/submitorder.action")!=-1) )
	{
		return kpcordercn;
	}
	//国际单程 往返 提交订单
	if((http_session->m_Requestline.m_Host.Find("new.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jdair/ticket/flightOrder")!=-1) )
	{
		return kpcorderint;
	}
	return 0;
}

int plane::CaptalAirLines::Process(const char *packet,int action)
{
	//国内单程 往返 查询
	if (action == kpcticketcn)
	{
		return pcticketcn(packet);
	}
	//国际单程 往返 查询
	if(action == kpcticketint)
	{
		return pcticketint(packet);
	}
	//国内单程 往返 提交订单
	if (action == kpcordercn)
	{
		return pcordercn(packet);
	}
	//国际单程 往返 提交订单
	if (action == kpcorderint)
	{
		return pcorderint(packet);
	}
	return 1;
}
//国内单程 往返 查询
int plane::CaptalAirLines::pcticketcn(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//单程 往返 
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (tripType == "ONEWAY")
	{
		//出发地
		string orgCity = ms_->GetValueFromMapByKey("orgCity_m",true);
        //目的地
		string dstCity = ms_->GetValueFromMapByKey("dstCity_m",true);
		//出发日期
		string flightDate = ms_->GetValueFromMapByKey("flightDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //出发地
			"msg=" + 	dstCity + "\r\n" +    //目的地 
			"servername=" +flightDate + "\r\n" + //出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}

	if (tripType == "ROUNDTRIP")
	{
		//出发地
		string orgCity = ms_->GetValueFromMapByKey("orgCity_m",true);
        //目的地
		string dstCity = ms_->GetValueFromMapByKey("dstCity_m",true);
		//出发日期
		string flightDate = ms_->GetValueFromMapByKey("flightDate",true);
		//返程日期
		string returnDate = ms_->GetValueFromMapByKey("returnDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //出发地
			"msg=" + 	dstCity + "\r\n" +    //目的地 
			"servername=" + flightDate + "\r\n" + //出发日期
			"appname=" + 	returnDate + "\r\n" + //返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}

//国际单程 往返 查询
int plane::CaptalAirLines::pcticketint(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	//判断是添加还是更改
	if (reader.parse(src_data,value))
	{	
		string tripType = value["tripType"].asString();
		if (tripType == "OW")
		{
			//成年人数
			string adtNum = value["adtNum"].asCString();
			//儿童人数
			string chdNum = value["chdNum"].asCString();
			//出发地(航空码)
			string originCode = value["originCode"].asCString();
			//目的地(航空码)
			string destCode = value["destCode"].asCString();
			//出发日期
			string departureDate = value["departureDate"].asCString();
			output_text_ =
				"netmark=" + adtNum + "\r\n" +		//成年人数    
				"onick=" + 	chdNum + "\r\n" +		//儿童人数 
				"nick=" + 	originCode + "\r\n" +	//出发地
				"msg=" + 	destCode + "\r\n" +		//目的地 
				"servername=" + departureDate + "\r\n" + //出发日期
				"state=查询\r\n";
			WriteIndexFile();
		}
		if (tripType == "RT")
		{
			//成年人数
			string adtNum = value["adtNum"].asCString();
			//儿童人数
			string chdNum = value["chdNum"].asCString();
			//出发地(航空码)
			string originCode = value["originCode"].asCString();
			//目的地(航空码)
			string destCode = value["destCode"].asCString();
			//出发日期
			string departureDate = value["departureDate"].asCString();
			//返程日期
			string returnDate = value["returnDate"].asCString();
			output_text_ =
				"netmark=" + adtNum + "\r\n" +		//成年人数    
				"onick=" + 	chdNum + "\r\n" +		//儿童人数 
				"nick=" + 	originCode + "\r\n" +	//出发地
				"msg=" + 	destCode + "\r\n" +		//目的地 
				"servername=" + departureDate + "\r\n" + //出发日期
				"appname=" + returnDate + "\r\n" +	//返程日期
				"state=查询\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}


//国内单程 往返 提交订单
int plane::CaptalAirLines::pcordercn(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//获取乘客的人数
	string temp = src_data.substr(src_data.rfind("passengerList"),src_data.find("&contactName") - src_data.rfind("passengerList"));
	string People = ms_->GetValueBetweenBeginEnd(temp,"[","]");
	int PassengerCount = atoi(People.c_str());
    string depCodes,arrCodes;
	//单程 往返
	string segmentSize = ms_->GetValueFromMapByKey("segmentSize",true);
	if (segmentSize == "1") //单程
	{ 
		//出发地(航空码)
		depCodes = ms_->GetValueFromMapByKey("depCodeS",true);
		//目的地(航空码)
		arrCodes = ms_->GetValueFromMapByKey("arrCodeS",true);
	}
	if (segmentSize == "2") //往返 -- 待优化
	{ 
		string tempSegment = src_data.substr(src_data.find("cabinClass"),src_data.find("&passengerList") - src_data.find("cabinClass"));

		//出发地(航空码)
		depCodes = ms_->GetValueFromMapByKey("depCodeS",true);
		//目的地(航空码)
		arrCodes = ms_->GetValueFromMapByKey("arrCodeS",true);
	}
    //联系人姓名
	string contactName = ms_->GetValueFromMapByKey("contactName",true);
	//联系人电话
	string contactMobile = ms_->GetValueFromMapByKey("contactMobile",true);

	for(int i =1;i<= PassengerCount;i++)
	{
		CString str;
		str.Format("%d",i);
		string strTemp = str.GetBuffer(0);
		//乘客姓名
		string publicTemp = "passengerList";
		string passengerNameTemp = publicTemp + "[" + strTemp + "]" + ".name";
		string passengerName = ms_->GetValueFromMapByKey(passengerNameTemp,true);
		//乘客类型
		string remarkTemp = publicTemp + "[" + strTemp + "]" + ".remark";
		string remark = ms_->GetValueFromMapByKey(remarkTemp,true);

		string cateType,cateNo;
		if (remark == "ADULT" || remark == "CHILD")
		{
			//乘客证件类型
			string cateTypeTemp = publicTemp + "[" + strTemp + "]" + ".certificateType";
			cateType = ms_->GetValueFromMapByKey(cateTypeTemp,true);
			cateType = GetIdType(cateType);
			//乘客证件号
			string cateNoTemp = publicTemp + "[" + strTemp + "]" + ".certificateNo";
			cateNo = ms_->GetValueFromMapByKey(cateNoTemp,true);
		}
		if (remark == "INFANT")
		{
			//乘客证件类型
			cateType = "无";
			//乘客证件号
			string cateNoTemp = publicTemp + "[" + strTemp + "]" + ".birthday";
			cateNo = ms_->GetValueFromMapByKey(cateNoTemp,true);
		}
		//乘客电话
		string mobilePhoneTemp = publicTemp + "[" + strTemp + "]" + ".mobilePhone";
		string mobilePhone = ms_->GetValueFromMapByKey(mobilePhoneTemp,true);
		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//联系人姓名
			"oid=" + 	contactMobile + "\r\n" +	//联系人电话
			"nick=" + 	depCodes + "\r\n" +			//出发地
			"msg=" + 	arrCodes + "\r\n" +			//目的地 
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"clientmsg=" + mobilePhone + "\r\n" +	//乘客电话
			"webname=" + cateType + "\r\n" +		//乘客证件类型
			"subject=" + cateNo + "\r\n" +			//乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}

//国际单程 往返 提交订单
int plane::CaptalAirLines::pcorderint(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
	//单程 OW 往返 RT
		string tripType = value["request.tripType"].asCString();
		if (tripType == "OW")
		{
			//联系人姓名
			string contactName = value["contactName"].asCString();
			//联系人电话
			string contactTel = value["contactTel"].asCString();
			//联系人邮箱
			string contactEmail = value["contactEmail"].asCString();
			//出发地(航空码)
			string originCode = value["request.originCode"].asCString();
			//目的地(航空码)
			string destCode = value["request.destCode"].asCString();
			//出发日期
			string departureDate = value["request.departureDate"].asCString();    
			
// 			//航班信息
// 			Json::Value flightSegments = value["flightSegments"];
// 			//出发机场(航空码 + 站台)
// 			int k = 0;
// 			string departureAir = flightSegments[k]["departureAirport"].asString() + " " + flightSegments[k]["departureTerminal"].asString();
// 			//到达机场(航空码 + 站台)
// 			string arrvalAir = flightSegments[k]["arrivalAirport"].asString() + " " + flightSegments[k]["arrivalTerminal"].asString();
// 			//航班号
// 			string flightNumber = flightSegments[k]["flightNumber"].asCString();
			//乘客信息
			Json::Value passengers = value["passengers"];
			for (int i=0;i< passengers.size();i++)
			{
				//乘客姓名
				string lastName = passengers[i]["lastName"].asCString();
				string firstName = passengers[i]["firstName"].asCString();
				string passengerName =  lastName + firstName;
				//乘客证件类型
				string certTypeTemp = passengers[i]["certType"].asCString();
				string certType = GetIdType(certTypeTemp);
				//乘客证件号
				string certNo = passengers[i]["certNo"].asCString();
				output_text_ =
					"mid=" + 	  contactName + "\r\n" +	//联系人姓名
					"oid=" + 	  contactTel + "\r\n" +		//联系人电话
					"imsi=" + 	  contactEmail + "\r\n" +	//联系人邮箱
					"nick=" + 	  originCode + "\r\n" +		//出发地
					"msg=" + 	  destCode + "\r\n" +		//目的地 
					"msgtype=" +  passengerName + "\r\n" +	//乘客姓名
					"webname=" +  certType + "\r\n" +		//乘客证件类型
					"netmark=" +  certNo + "\r\n" +			//乘客证件号
					"servername=" + departureDate + "\r\n" +//出发日期
					"state=提交订单\r\n";
				WriteIndexFile();
			}
		}
		if (tripType == "RT")
		{
			//联系人姓名
			string contactName = value["contactName"].asCString();
			//联系人电话
			string contactTel = value["contactTel"].asCString();
			//联系人邮箱
			string contactEmail = value["contactEmail"].asCString();
			//出发地(航空码)
			string originCode = value["request.originCode"].asCString();
			//目的地(航空码)
			string destCode = value["request.destCode"].asCString();
			//出发日期
			string departureDate = value["request.departureDate"].asCString();   
			//返程日期
			string departureDateRt = value["request.returnDate"].asCString(); 
			
			//航班信息
// 			Json::Value flightSegments = value["flightSegments"];
// 			int k = 0;
// 			//出发机场(航空码 + 站台)
// 			string departureAir = flightSegments[k]["departureAirport"].asString() + " " + flightSegments[k]["departureTerminal"].asString();
// 			//到达机场(航空码 + 站台)
// 			string arrvalAir = flightSegments[k]["arrivalAirport"].asString() + " " + flightSegments[k]["arrivalTerminal"].asString();
// 			//航班号
// 			string flightNumber = flightSegments[k]["flightNumber"].asCString();
			//乘客信息
			Json::Value passengers = value["passengers"];
			for (int i=0;i<passengers.size();i++)
			{
				//乘客姓名
				string lastName = passengers[i]["lastName"].asCString();
				string firstName = passengers[i]["firstName"].asCString();
				string passengerName =  lastName + firstName;
				//乘客证件类型
				string certTypeTemp = passengers[i]["certType"].asCString();
				string certType = GetIdType(certTypeTemp);
				//乘客证件号
				string certNo = passengers[i]["certNo"].asCString();
				output_text_ =
					"mid=" + 	  contactName + "\r\n" +	//联系人姓名
					"oid=" + 	  contactTel + "\r\n" +		//联系人电话
					"imsi=" + 	  contactEmail + "\r\n" +	//联系人邮箱
					"nick=" + 	  originCode + "\r\n" +		//出发地
					"msg=" + 	  destCode + "\r\n" +		//目的地 
					"msgtype=" +  passengerName + "\r\n" +	//乘客姓名
					"webname=" +  certType + "\r\n" +		//乘客证件类型
					"netmark=" +  certNo + "\r\n" +			//乘客证件号
					"servername=" + departureDate + "\r\n" +//出发日期
					"appname=" + departureDateRt + "\r\n" + //返程日期
					"state=提交订单\r\n";
				WriteIndexFile();
			}
		}
	}
	return 1;
}
