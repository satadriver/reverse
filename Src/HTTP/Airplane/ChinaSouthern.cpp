
#include "StdAfx.h"

#include "ChinaSouthern.h"

plane::ChinaSouthern::ChinaSouthern(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChinaSouthern::~ChinaSouthern()
{
	
}

int plane::ChinaSouthern::IsChinaSouthern(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("b2c.csair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/B2C40/user/login.ao")!=-1))
	{
		return kpclogin;
	}
	//单程 往返 联程
	if((http_session->m_Requestline.m_Host.Find("b2c.csair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/B2C40/query/jaxb/direct/query.ao")!=-1))
	{
		return kpcticket;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("b2c.csair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/B2C40/data/order/direct/createOrder.xsql?")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ChinaSouthern::Process(const char *packet,int action)
{
	//登录
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//单程 往返 联程
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//提交订单
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//用户登录
int plane::ChinaSouthern::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//需要提取的数据
	src_data = src_data.substr(src_data.rfind("userId="),src_data.length()-src_data.rfind("userId="));
	ms_->SplitStringToMap(src_data,'&','=');

	username = ms_->GetValueFromMapByKey("userId",true);
	password = ms_->GetValueFromMapByKey("passWord",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 	username + "\r\n" + //账号
		"pass=" + 	password + "\r\n" + //密码
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//单程 往返 联程
int plane::ChinaSouthern::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("json=")+strlen("json="));

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		adtCount = value["adultnum"].asString();
		chdCount = value["childnum"].asString();
		infCount = value["infantnum"].asString();
		deptCdTxt = value["depcity"].asString();
		arrCdTxt = value["arrcity"].asString();
		deptDt = value["flightdate"].asString();
	}
	if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
	{
		return 0;
	}
	output_text_ =
		"netmark=" + 	adtCount + "\r\n" + //成年人数
		"onick=" + 	chdCount + "\r\n" +		//儿童人数
		"sex=" + 	infCount + "\r\n" +		//婴儿人数
		"nick=" + 	deptCdTxt + "\r\n" +	//出发地
		"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
		"servername=" + deptDt + "\r\n" +	//出发日期
		"state=查询\r\n";
	WriteIndexFile();
	return 1;
}

//提交订单
int plane::ChinaSouthern::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//联系人信息
	string contactInfo;
	contactInfo = ms_->GetValueBetweenBeginEnd(src_data,"<ORDERCONTACT>","</ORDERCONTACT>",true);
	//联系人姓名
	string contactName,contactMobile,contackEmail;
	contactName = contactInfo.substr(0,contactInfo.find("|"));
	//联系人电话
	contactMobile = contactInfo.substr(contactInfo.find("||") + strlen("||"),contactInfo.rfind("|") - contactInfo.find("||") - strlen("||"));
	//乘客信息
	string passengeInfo;
	passengeInfo = ms_->GetValueBetweenBeginEnd(src_data,"<PASSENGERS>","</PASSENGERS>",true);
	while (passengeInfo.find("</PASSENGER>") != -1)
	{
		string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
		string passengerInfoTemp = passengeInfo.substr(0,passengeInfo.find("</PASSENGER>") + strlen("</PASSENGER>"));
		//乘客姓名
		passengerName = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"<NAME>","</NAME>",true);
		//乘客证件类型
		passengerIDtypeTemp = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"<IDTYPE>","</IDTYPE>",true);
		if (passengerIDtypeTemp == "NI"){passengerIDtype = "身份证";};
		if (passengerIDtypeTemp == "PP"){passengerIDtype = "护照";};
		if (passengerIDtypeTemp == "ID"){passengerIDtype = "其他证件";};
		//乘客证件号
		passengerIDcard = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"<IDCARD>","</IDCARD>",true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//联系人姓名
			"oid=" + 	contactMobile + "\r\n" +	//联系人电话
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"webname=" + passengerIDtype + "\r\n" + //乘客证件类型
			"subject=" + passengerIDcard + "\r\n" + //乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	    passengeInfo = passengeInfo.substr(passengeInfo.find("</PASSENGER>") + strlen("</PASSENGER>"));
	}
	return 1;
}
