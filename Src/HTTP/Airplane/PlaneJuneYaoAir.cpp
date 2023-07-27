
#include "StdAfx.h"
#include "PlaneJuneYaoAir.h"

plane::PlaneJuneYaoAir::PlaneJuneYaoAir(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::PlaneJuneYaoAir::~PlaneJuneYaoAir()
{
	
}
		
// NI 身份证，PP 护照，CC 其他
string plane::PlaneJuneYaoAir::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "身份证";}
	if (idType == "PP"){idTypeTrue = "护照";}
	if (idType == "CC"){idTypeTrue = "其他";}
	return idTypeTrue;
}

int plane::PlaneJuneYaoAir::IsPlaneJuneYaoAir(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/ReservationLogin.do")!=-1)) 
	{										
		return kpclogin;
	}	
	//查询 --处理包问题
	if((http_session->m_Requestline.m_Host.Find("beacon.tingyun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xhr?")!=-1)) 
	{										
		return kpcticket;
	}		
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("www.juneyaoair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pages/Flight/flight_confirm.aspx")!=-1) )   
	{
		return kpcorder;
	}
	//提交订单 国际
	if((http_session->m_Requestline.m_Host.Find("www.juneyaoair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pages/Flight/flight_confirm_Int.aspx")!=-1) )  
	{
		return kpcorderint;
	}
	return 0;
}

int plane::PlaneJuneYaoAir::Process(const char *packet,int action)
{
	//登录
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//查询 
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//提交订单
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	//提交订单
	if (action == kpcorderint)
	{
		return pcorderint(packet);
	}
	return 1;
}

//登录
int plane::PlaneJuneYaoAir::pclogin(const char *packet)
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

//查询 国内 国际
int plane::PlaneJuneYaoAir::pcticket(const char *packet)
{	
	//从uri
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//单程 往返 OW RT
	string flightType = ms_->GetValueFromMapByKey("flightType",true);
	//国内 国际 N I
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (flightType == "OW")
	{
		//出发地
		string originLocation = ms_->GetValueFromMapByKey("sendCity",true);
		//目的地
		string destinationLocation = ms_->GetValueFromMapByKey("arrCity",true);
		//出发日期
		string departureDate = ms_->GetValueFromMapByKey("departureDate",true);
		output_text_ =
			"nick=" + 	originLocation + "\r\n" +		//出发地
			"msg=" + 	destinationLocation + "\r\n" +	//目的地 
			"servername=" +departureDate + "\r\n" +		//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (flightType == "RT")
	{
		//出发地
		string originLocation = ms_->GetValueFromMapByKey("sendCity",true);
		//目的地
		string destinationLocation = ms_->GetValueFromMapByKey("arrCity",true);
		//出发日期
		string departureDate = ms_->GetValueFromMapByKey("departureDate",true);
		//返程日期 
		string departureDateRt = ms_->GetValueFromMapByKey("returnDate",true);
		
		output_text_ =
			"nick=" + 	originLocation + "\r\n" +		//出发地
			"msg=" + 	destinationLocation + "\r\n" +	//目的地 
			"servername=" + departureDate + "\r\n" +	//出发日期
			"appname=" + departureDateRt + "\r\n" +		//返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}

//提交订单
int plane::PlaneJuneYaoAir::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//联系人信息
	string contactName,contactMobile,contackEmail;
	string linkuser = ms_->GetValueFromMapByKey("linkuser",true);
	//使用json 读取数据
	Json::Reader readerLink;
	Json::Value valueLink;
	if (readerLink.parse(linkuser,valueLink))
	{	
		contactName = valueLink["Linker"].asString();
		contactMobile = valueLink["LinkerHandphone"].asString();
		contackEmail = valueLink["LinkerEMail"].asString();
	}	
	//乘客信息 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);
	while(passengerInfo.find("IsBuyInsurance") != string::npos)
	{
		string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("IsBuyInsurance") + strlen("IsBuyInsurance"));		
		//乘客姓名
		passengerName = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"PassengerName\":\"","\",\"IsSaveCommon");
		//乘客电话
		passengerTel = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"HandphoneNo\":\"","\",\"AdtNameToInf");
		//乘客证件类型
	 	passengerIDtype = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertType\":\"","\",\"CertNo");
		passengerIDtype = GetIdType(passengerIDtype);
		//乘客证件号
		passengerIDcard = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertNo\":\"","\",\"Birthdate");
		
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
		passengerInfo = passengerInfo.substr(passengerInfo.find("IsBuyInsurance") + strlen("IsBuyInsurance"));
	}
	return 1;
}


//提交订单 国际
int plane::PlaneJuneYaoAir::pcorderint(const char *packet)
{
	// 	//从cookie 中获得user_
	// 	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	// 	cookie = ms_->UrlDecode(cookie);
	// //AfxMessageBox(cookie.c_str());
	// 	ms_->SplitStringToMap(cookie, ';','=');
	// 	//账号
	// 	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	//     //出发地
	// 	//目的地
	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	
	//联系人信息
	string contactName,contactMobile,contackEmail;
	string linkuser = ms_->GetValueFromMapByKey("linkuser",true);
	//使用json 读取数据
	Json::Reader readerLink;
	Json::Value valueLink;
	if (readerLink.parse(linkuser,valueLink))
	{	
		contactName = valueLink["Linker"].asString();
		contactMobile = valueLink["LinkerHandphone"].asString();
		contackEmail = valueLink["LinkerEMail"].asString();
	}	
	//乘客信息 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);
	while(passengerInfo.find("CNTax") != string::npos)
	{
		string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("CNTax") + strlen("CNTax"));		
		//乘客姓名
		passengerName = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"PassengerName\":\"","\",\"PassengerType");
		//乘客电话
		passengerTel = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"HandphoneNo\":\"","\",\"AdtNameToInf");
		//乘客证件类型
		passengerIDtype = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertType\":\"","\",\"CertNo");
		passengerIDtype = GetIdType(passengerIDtype);
		//乘客证件号
		passengerIDcard = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertNo\":\"","\",\"Birthdate");
		
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
		passengerInfo = passengerInfo.substr(passengerInfo.find("CNTax") + strlen("CNTax"));
	}
	return 1;
}