
#include "StdAfx.h"

#include "PlaneSiChuanAirlines.h"

plane::SiChuanAirlines::SiChuanAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::SiChuanAirlines::~SiChuanAirlines()
{
	
}

//5 身份证 6 军官证 7 警官证 3 士兵证 15 回乡证 2 有效护照 14 其他证件
string plane::SiChuanAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "5"){idTypeTrue = "身份证";}
	if (idType == "6"){idTypeTrue = "军官证";}
	if (idType == "7"){idTypeTrue = "警官证";}
	if (idType == "3"){idTypeTrue = "士兵证";}
	if (idType == "15"){idTypeTrue = "回乡证";}
	if (idType == "2"){idTypeTrue = "有效护照";}
	if (idType == "14"){idTypeTrue = "其他证件";}
	return idTypeTrue;
}

int plane::SiChuanAirlines::IsSiChuanAirlines(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/profile/processLogin.do?successPage")!=-1) )
	{
		return kpclogin;
	}
	//行程预订-单程 往返
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/common/processSearchForm.do")!=-1))
	{
		return kpcticket;
	}
	//行程预定-多目的地
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/common/processSearch.do")!=-1))
	{
		return kpcticket;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/checkout/processPassengerDetails.do")!=-1))
	{
		return kpcorder;
	}
	return 0;
}

int plane::SiChuanAirlines::Process(const char *packet,int action)
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
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//登录
int plane::SiChuanAirlines::pclogin(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(url,"?","&");
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,"&","=");
	//用户名
	username = ms_->GetValueFromMapByKey("credentials/loginUsername");
	//密码
	password = ms_->GetValueFromMapByKey("credentials/loginPassword");

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
int plane::SiChuanAirlines::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string AirlineType = ms_->GetValueFromMapByKey("Search/flightType",true);
	//单程
	if (AirlineType == "oneway")
	{
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" +		//成年人数   		
			"onick=" + 	chdCount + "\r\n" +		//儿童人数   
			"nick=" +	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
			"servername=" + deptDt + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	//往返
	if (AirlineType == "return")
	{
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
	    deptDtRe = ms_->GetValueFromMapByKey("Search/DateInformation/returnDate",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty() || deptDtRe.empty())
		{
			return 0;
		}
		output_text_ =
		"netmark=" +adtCount + "\r\n" +		//成年人数   		
		"onick=" + 	chdCount + "\r\n" +		//儿童人数  
		"nick=" +	deptCdTxt + "\r\n" +	//出发地
		"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
		"servername=" + deptDt + "\r\n" +	//出发日期
		"appname=" + deptDtRe + "\r\n" +	//返程日期
		"state=查询\r\n";
		WriteIndexFile();
	}
	//多目的地
	if (AirlineType == "multi-city")
	{
		//成人数量
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//儿童数量
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		
		//Search/DateInformation$2$/departTime
		//获取城市的数量
		int pos = src_data.rfind("DateInformation$") + strlen("DateInformation$");
		int tos = src_data.rfind("$/departDate");
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
			
			//目的地 //Search/OriginDestinationInformation/Destination$2$/location_input
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
int plane::SiChuanAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//国内提交订单
	if (src_data.find("PersonName/GivenName") == -1)
	{
		//航班日期
		deptDt = ms_->GetValueFromMapByKey("flightDate",true);
		//联系人姓名
		contactname_ = ms_->GetValueFromMapByKey("Profile/Customer/PersonName/Surname",true);
		//联系人电话
		mobile = ms_->GetValueFromMapByKey("Profile/Customer/Telephone$3$/@PhoneNumber",true);
		//联系人邮箱
		email = ms_->GetValueFromMapByKey("Profile/Customer/Email",true);
		//product$1000$/passenger$2$/Passport/DocID=E27447195
		//获取乘客的数量
		int pos = src_data.rfind("product$1000$/passenger$") + strlen("product$1000$/passenger$");
		int tos = src_data.rfind("$/Passport/DocID");
		string passengerCount = src_data.substr(pos,tos - pos);
		int m = atoi(passengerCount.c_str());
		
		for (int i=1;i<=m;i++)
		{		
			CString tem ;
			tem.Format("%d",i);
			string n = tem.GetBuffer(0);	
			//乘客姓名 product$1000$/passenger$1$/Customer/PersonName/Surname=章明旭
			string name1 = "product$1000$/passenger$";
			string name2 = "$/Customer/PersonName/Surname";
			string name = name1 + n + name2;
			username = ms_->GetValueFromMapByKey(name,true);
			
			//乘客证件类型 product$1000$/passenger$1$/Passport/DocType=5
			string type1 = "product$1000$/passenger$";
			string type2 = "$/Passport/DocType";
			string type = type1 + n + type2;
			idtype_ = ms_->GetValueFromMapByKey(type,true);
			idtype_ = GetIdType(idtype_);
			
			//乘客证件号 product$1000$/passenger$1$/Passport/DocID=440901197709165291
			string id1 = "product$1000$/passenger$";
			string id2 = "$/Passport/DocID";
			string docid = id1 + n + id2;
			idcard = ms_->GetValueFromMapByKey(docid,true);
			
			if (username.empty() || idcard.empty())
			{
				return 0;
			}
			output_text_ =
				"mid=" + 	contactname_ + "\r\n" + //联系人姓名		 	
				"oid=" + 	mobile + "\r\n" +		//联系人电话			
				"imsi=" + 	email + "\r\n" + 		//联系人邮箱
				"msgtype=" + username + "\r\n" +	//乘客姓名				
				"webname=" + idtype_ + "\r\n" +		//乘客证件类型 	
				"subject=" + idcard + "\r\n" +		//乘客证件号	
				"state=提交订单\r\n";
			WriteIndexFile();
		}
	}
    //国际提交订单
	if (src_data.find("PersonName/GivenName") != -1)
	{
		//航班日期
		deptDt = ms_->GetValueFromMapByKey("flightDate",true);
		//联系人姓名
		contactname_ = ms_->GetValueFromMapByKey("Profile/Customer/PersonName/Surname",true);
		//联系人电话
		mobile = ms_->GetValueFromMapByKey("Profile/Customer/Telephone$3$/@PhoneNumber",true);
		//联系人邮箱
		email = ms_->GetValueFromMapByKey("Profile/Customer/Email",true);
		//product$1000$/passenger$2$/CustLoyalty/@ProgramID=FFP
		//获取乘客的数量
		int pos = src_data.rfind("product$1000$/passenger$") + strlen("product$1000$/passenger$");
		int tos = src_data.rfind("$/CustLoyalty/@ProgramID");
		string passengerCount = src_data.substr(pos,tos - pos);
		int m = atoi(passengerCount.c_str());
		
		for (int i=1;i<=m;i++)
		{		
			CString tem ;
			tem.Format("%d",i);
			string n = tem.GetBuffer(0);	
			//乘客姓名 product$1000$/passenger$2$/Customer/PersonName/Surname=chen
			//		   product$1000$/passenger$2$/Customer/PersonName/GivenName=zaiming
			string name1 = "product$1000$/passenger$";
			string name2 = "$/Customer/PersonName/Surname";
			string name3 = "$/Customer/PersonName/GivenName";
			string name = name1 + n + name2;
			string givename = name1 + n + name3;
			username = ms_->GetValueFromMapByKey(name,true) + 
					   ms_->GetValueFromMapByKey(givename);	
			//乘客证件类型 
			idtype_ = GetIdType("2");
			//乘客证件号 product$1000$/passenger$2$/Passport/DocID=E27447195
			string id1 = "product$1000$/passenger$";
			string id2 = "$/Passport/DocID";
			string docid = id1 + n + id2;
			idcard = ms_->GetValueFromMapByKey(docid,true);
			
			if (username.empty() || idcard.empty())
			{
				return 0;
			}
			output_text_ =
				"mid=" + 	contactname_ + "\r\n" + //联系人姓名		 	
				"oid=" + 	mobile + "\r\n" +		//联系人电话			
				"imsi=" + 	email + "\r\n" + 		//联系人邮箱
				"msgtype=" + username + "\r\n" +	//乘客姓名				
				"webname=" + idtype_ + "\r\n" +		//乘客证件类型 	
				"subject=" + idcard + "\r\n" +		//乘客证件号	
				"state=提交订单\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}
