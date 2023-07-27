
#include "StdAfx.h"

#include "ChinaHainan.h"

plane::ChinaHainan::ChinaHainan(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChinaHainan::~ChinaHainan()
{
	
}

//海南航空类型
//5 身份证 6 军官证 7 警官证 3 士兵证 2 有效护照 14 其他证件
string plane::ChinaHainan::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "5"){idTypeTrue = "身份证";}
	if (idType == "6"){idTypeTrue = "军官证";}
	if (idType == "7"){idTypeTrue = "警官证";}
	if (idType == "3"){idTypeTrue = "士兵证";}
	if (idType == "2"){idTypeTrue = "有效护照";}
	if (idType == "14"){idTypeTrue = "其他证件";}
	return idTypeTrue;
}

int plane::ChinaHainan::IsChinaHainan(HttpSession *http_session)
{
	//用户登录
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/profile/processLogin.do")!=-1))
	{
		return kpclogin;
	}
	//单程 往返 (第一页)
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/deeplink/ancillary.do")!=-1))
	{
		return kpcticket;
	}
	//单程 往返 多目的地 (第二页)
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/common/processSearch.do")!=-1))
	{
		return kpcticketmulti;
	}	
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/common/processSearchForm.do")!=-1))
	{
		return kpcticketmulti;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/checkout/processPassengerDetails.do")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ChinaHainan::Process(const char *packet,int action)
{
	//登陆
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//单程 往返
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//多目的地
	if (action == kpcticketmulti)
	{
		return pcticketmulti(packet);
	}
	//提交订单
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//用户登录
int plane::ChinaHainan::pclogin(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(src_data,"?","&");
	src_data = ms_->UrlDecode(src_data);
    ms_->SplitStringToMap(src_data,"&","=");
	username = ms_->GetValueFromMapByKey("credentials/loginUsername",true);
	password = ms_->GetValueFromMapByKey("credentials/loginPassword",true);
	//登录类型
	string loginMethodType = ms_->GetValueFromMapByKey("loginMethodType",true);
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

//单程 往返
int plane::ChinaHainan::pcticket(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	//成人数量
	adtCount = ms_->GetValueFromMapByKey("TA",true);
	//儿童数量
	chdCount = ms_->GetValueFromMapByKey("TC",true);
	//出发日期
	deptDt = ms_->GetValueFromMapByKey("DD1",true);
	//返程日期
	deptDtRe = ms_->GetValueFromMapByKey("DD2",true);
	//出发地
	deptCdTxt = ms_->GetValueFromMapByKey("ORI",true);
	//目的地
	arrCdTxt = ms_->GetValueFromMapByKey("DES",true);
	
	//往返类型
	string flc = ms_->GetValueFromMapByKey("FLC",true);
	if (flc == "1") //单程
	{
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//成年人数
			"onick=" + 	chdCount + "\r\n" +		//儿童人数
			"nick=" + 	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 		
			"servername=" +deptDt + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if(flc == "2") //往返
	{
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//成年人数
			"onick=" + 	chdCount + "\r\n" +		//儿童人数
			"nick=" + 	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 		
			"servername=" + deptDt + "\r\n" +	//出发日期
			"appname=" + 	deptDtRe + "\r\n" + //返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}

//多目的地
int plane::ChinaHainan::pcticketmulti(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
    //行程类型
	string flightType = ms_->GetValueFromMapByKey("Search/flightType",true);
	if (flightType == "oneway")
	{
		//成人数量
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//儿童数量
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		//出发日期
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
		//出发地
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		//目的地
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//成年人数
			"onick=" + 	chdCount + "\r\n" +		//儿童人数
			"nick=" + 	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 		
			"servername=" +deptDt + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (flightType == "return")
	{
		//成人数量
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//儿童数量
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		//出发日期
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
		//返程日期
		deptDtRe = ms_->GetValueFromMapByKey("Search/DateInformation/returnDate",true);
		//出发地
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		//目的地
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		if (deptDt.empty() || deptDtRe.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//成年人数
			"onick=" + 	chdCount + "\r\n" +		//儿童人数
			"nick=" + 	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 		
			"servername=" +deptDt + "\r\n" +	//出发日期
			"appname=" + 	deptDtRe + "\r\n" + //返程日期
		"state=查询\r\n";
		WriteIndexFile();
	}
	if (flightType == "multi-city")
	{
		//成人数量
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//儿童数量
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		
		//Search/DateInformation$2$/departTime
		//获取城市的数量
		int pos = src_data.rfind("DateInformation$") + strlen("DateInformation$");
		int tos = src_data.rfind("$/departTime");
		string passengerCount = src_data.substr(pos,tos - pos);
		int m = atoi(passengerCount.c_str());
		
		for (int i=1;i<=m;i++)
		{		
			CString tem ;
			tem.Format("%d",i);
			string n = tem.GetBuffer(0);	
			//出发日期 //Search/DateInformation$1$/departDate
			string date1 = "Search/DateInformation$";
			string date2 = "$/departDate";
			string date = date1 + n + date2;
			deptDt = ms_->GetValueFromMapByKey(date,true);
			
			//出发地 //Search/OriginDestinationInformation/Origin$1$/location_input
			string placedep1 = "Search/OriginDestinationInformation/Origin$";
			string placedep2 = "$/location_input";
			string placedep = placedep1 + n + placedep2;
			deptCdTxt = ms_->GetValueFromMapByKey(placedep,true);
			
			//出发地 //Search/OriginDestinationInformation/Destination$2$/location_input
			string placearr1 = "Search/OriginDestinationInformation/Destination$";
			string placearr2 = "$/location_input";
			string placearr = placearr1 + n + placearr2;
			arrCdTxt = ms_->GetValueFromMapByKey(placearr,true);
			
			if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
			{
				return 0;
			}
			if (deptCdTxt == "拼音/汉字" || arrCdTxt == "拼音/汉字")
			{
				return 0;
			}
			output_text_ =
				"netmark=" + adtCount + "\r\n" +	//成年人数
				"onick=" + 	chdCount + "\r\n" +		//儿童人数
				"nick=" + 	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 		
				"servername=" + deptDt + "\r\n" +	//出发日期
			"state=查询\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}

//提交订单
int plane::ChinaHainan::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string contactName,contactMobile,contackEmail;
	//联系人姓名
	contactName = ms_->GetValueFromMapByKey("Profile/Customer/PersonName/Surname",true);
	//联系人电话
	contactMobile = ms_->GetValueFromMapByKey("Profile/Customer/Telephone$3$/@PhoneNumber",true);
	//product$1000$/passenger$ 2 $/insuranceSelected
	//获取乘客的人数
	int pos = src_data.rfind("passenger$") + strlen("passenger$");
	int tos = src_data.rfind("$/insuranceSelected");
	string passengerCount = src_data.substr(pos,tos - pos);
	int m = atoi(passengerCount.c_str());
    string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	//出发日期
	string flightDate = ms_->GetValueFromMapByKey("flightDate",true);

	for (int i=1;i<=m;i++)
	{
		string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
		
		CString tem ;
		tem.Format("%d",i);
		string n = tem.GetBuffer(0);
		string common = "product$1000$/passenger$";

		//乘客姓名
		string nameTemp = "$/Customer/PersonName/Surname";
		string name = common + n + nameTemp;
		passengerName = ms_->GetValueFromMapByKey(name,true);

		//乘客证件类型
		string idCardTypeTemp = "$/Passport/DocType";
		string idCardType = common + n + idCardTypeTemp;
		passengerIDtypeTemp = ms_->GetValueFromMapByKey(idCardType,true);
		passengerIDtype = GetIdType(passengerIDtypeTemp);

		//乘客证件号
		string idCardTemp ="$/Passport/DocID";
		string idCard = common +n +idCardTemp;
 		passengerIDcard = ms_->GetValueFromMapByKey(idCard,true); 

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//联系人姓名
			"oid=" + 	contactMobile + "\r\n" +	//联系人电话
			"from=" + 	flightDate + "\r\n" +		//出发日期
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"webname=" + passengerIDtype + "\r\n" + //乘客证件类型
			"subject=" + passengerIDcard + "\r\n" + //乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}
