//MangGuo.cpp: implementation of the MangGuo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MangGuo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MangGuo::MangGuo(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

MangGuo::~MangGuo()
{

}

// 定义的证件类型
extern char* credential_type_Manguo[][2] = 
{{"IDC","身份证"},
{"PSP","护照"},
{"MOC","军官证"},
{"TW1","回乡证"},
{"TW2","台胞证"},
{"HKM","港澳通行证"},
{"HKP","户口簿"},
{"CSZ","出生证明"},
{"SEA","国际海员证"},
{"FOR","外国人永久居留证"},
{"OTHER","其他"},
};

//获取证件类型
string MangGuo::GetCredentialType(string passengerIDCardTypeTemp)
{
	for (int iCode = 0; iCode < sizeof(credential_type_Manguo)/sizeof(credential_type_Manguo[0]); iCode ++)
	{
		if (passengerIDCardTypeTemp.compare(credential_type_Manguo[iCode][0]) == 0)
		{
			return credential_type_Manguo[iCode][1];;
		}
	}
	return passengerIDCardTypeTemp;
}

int MangGuo::Is(HttpSession *http_session)
{
	//pc 登录
	if((http_session->m_Requestline.m_Host.Find("www.mangocity.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mbrWebCenter/login/doLogin.action")!=-1))
	{
		return PC_LOGIN;
	}
	// PC 飞机票 国内机票
	if((http_session->m_Requestline.m_Host.Find("flight1.mangocity.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reserve.shtml")!=-1))
	{
		return PC_AIRPLANE;
	}
	return 0;
}

int MangGuo::Process(const char *packet,int action)
{
	//pc 登录
	if (action == PC_LOGIN)
	{
		string src_data(packet);
		src_data=ms_->UrlDecode(src_data);
		ms_->SplitStringToMap(src_data,'&','=');
		string user,pass;
		user=ms_->GetValueFromMapByKey("loginid1",true);
		pass=ms_->GetValueFromMapByKey("loginpwd1",true);
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

	if (action == PC_AIRPLANE)
	{
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//单程 (联系人姓名，联系人电话，联系人邮箱，出发机场(航空码)，目的机场(航空码)，航班号，出发日期，到达日期) + 公共
		//返程 (联系人姓名，联系人电话，联系人邮箱，去程出发机场(航空码)，去程目的机场(航空码)，去程航班号，去程出发日期，去程到达日期，返程出发机场(航空码)，返程到达机场(航空码)，返程航班	
		//		号，返程出发日期，返程到达日期)
		//公共 (乘客姓名，乘客类型，乘客证件类型，乘客证件号，乘客出生日期)	

		string linkmanName,linkmanMobile,linkmanEmail;
		string FlightType;
		string depCityOw,arrcityOw,airlineOw,depDateOw,depTimeOw,depTerminalOw,arrDateOw,arrTiemOw,arrTerminalOw;
		string depCityRt,arrcityRt,airlineRt,depDateRt,depTimeRt,depTerminalRt,arrDateRt,arrTiemRt,arrTerminalRt;
		string psgName,psgType,certType,certId,psgBirthdate;

		//获取乘客的人数
		string temp = sContent.substr(sContent.rfind("bookPassengers"),sContent.length()-sContent.rfind("bookPassengers"));
		int pos = temp.rfind("bookPassengers");
		string People = temp.substr(pos+ strlen("bookPassengers") + strlen("["),1);
	    int PassengerCount = atoi(People.c_str());

		//联系人姓名
		linkmanName = ms_->GetValueFromMapByKey("linkmanInfo.linkmanName",true);
		//联系人电话
		linkmanMobile = ms_->GetValueFromMapByKey("linkmanInfo.linkmanMobile",true); 
		//联系人邮箱
		//linkmanEmail = ms_->GetValueFromMapByKey("linkmanInfo.linkmanEmail",true); 

		//航班类型
		FlightType = ms_->GetValueFromMapByKey("bookTrips[1].tripType",true);
		if (strcmp(FlightType.c_str(),"RT") != 0)
		{
			//单程
			//去程出发机场(航空码)
			depCityOw = ms_->GetValueFromMapByKey("bookTrips[0].depCity",true); 
			//去程目的机场(航空码)
			arrcityOw = ms_->GetValueFromMapByKey("bookTrips[0].arrCity",true); 
			//去程航班号
			airlineOw = ms_->GetValueFromMapByKey("bookTrips[0].airline",true); 
			//去程出发日期 depTimeOw(具体时间) depTerminalOw(站台)
			depDateOw = ms_->GetValueFromMapByKey("bookTrips[0].depDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].depTime",true);  
			//去程到达日期 arrTimeOw(具体时间) arrTerminalOw(站台)
			arrDateOw = ms_->GetValueFromMapByKey("bookTrips[0].arrDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].arrTime",true);  
				
			type_   = "国内单程机票";
		}
		if (strcmp(FlightType.c_str(),"RT") == 0)
		{
			//往返
			//去程出发机场(航空码)
			depCityOw = ms_->GetValueFromMapByKey("bookTrips[0].depCity",true); 
			//去程目的机场(航空码)
			arrcityOw = ms_->GetValueFromMapByKey("bookTrips[0].arrCity",true); 
			//去程航班号
			airlineOw = ms_->GetValueFromMapByKey("bookTrips[0].airline",true); 
			//去程出发日期 depTimeOw(具体时间) depTerminalOw(站台)
			depDateOw = ms_->GetValueFromMapByKey("bookTrips[0].depDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].depTime",true);  
			//去程到达日期 arrTimeOw(具体时间) arrTerminalOw(站台)
			arrDateOw = ms_->GetValueFromMapByKey("bookTrips[0].arrDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].arrTime",true); 

			//返程出发机场(航空码)
			depCityRt = ms_->GetValueFromMapByKey("bookTrips[1].depCity",true);
			//返程到达机场(航空码)
			arrcityRt = ms_->GetValueFromMapByKey("bookTrips[1].arrCity",true);
			//返程航班号
			airlineRt = ms_->GetValueFromMapByKey("bookTrips[1].airline",true); 
			//返程出发日期 depTimeRt(具体时间) depTerminalRt(站台)
			depDateRt = ms_->GetValueFromMapByKey("bookTrips[1].depDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[1].depTime",true); 
			//返程到达日期 arrTiemRt(具体时间) arrTerminalRt(站台)
			arrDateRt = ms_->GetValueFromMapByKey("bookTrips[1].arrDate",true) + " " +ms_->GetValueFromMapByKey("bookTrips[1].arrTime",true); 

			type_   = "国内往返机票";
		}
		
        for (int j=0;j < PassengerCount + 1;j++)
        {
			//转化
			CString str;
			str.Format("%d",j);
			string strEnd = str.GetBuffer(0);
			//乘客姓名
			string strTempName = "bookPassengers[" + strEnd + "].psgName";
			psgName = ms_->GetValueFromMapByKey(strTempName,true); 
			//乘客类型	
			string strTempType = "bookPassengers[" + strEnd + "].psgType";
			psgType = ms_->GetValueFromMapByKey(strTempType,true); 
			if (psgType == "ADT")
			{
				psgType = "成人票";
			}
			if (psgType == "CHD")
			{
				psgType = "儿童票";
			}
			if (psgType == "INF")
			{
				psgType = "婴儿票";
			}
			//乘客证件类型
			string strCertType = "bookPassengers[" + strEnd + "].certType";
			certType = ms_->GetValueFromMapByKey(strCertType,true); 
			certType = GetCredentialType(certType);
			//乘客证件号
			string strCertNumber = "bookPassengers[" + strEnd + "].certId";
			certId = ms_->GetValueFromMapByKey(strCertNumber,true);  
			//乘客生日 (暂时没有字段保存)
			string strBirthday = "bookPassengers[" + strEnd + "].psgBirthdate";
			psgBirthdate = ms_->GetValueFromMapByKey(strBirthday,true);			
			if (strcmp(FlightType.c_str(),"RT") != 0)
			{
				output_text_ = 
					"mid=" + 		linkmanName + "\r\n" +		//联系人姓名
					"oid=" + 		linkmanMobile + "\r\n" +	//联系人电话
					"msgtype=" + 	psgName + "\r\n" +			//乘客姓名
					"webname=" + 	certType + "\r\n" +			//乘客证件类型
					"netmark=" + 	certId + "\r\n" +			//乘客证件号
					"onick=" + 		depCityOw + "\r\n" +		//出发机场
					"mid=" + 		arrcityOw + "\r\n" +		//到达机场
					"servername=" + depDateOw + "\r\n" +		//出发日期
					"islogin=" + 	airlineOw + "\r\n" +		//航班号
					"platform=电脑浏览器\r\n" +
					"state=" + type_ + "\r\n";	
				WriteIndexFile();
			}
			if (strcmp(FlightType.c_str(),"RT") == 0)
			{
				output_text_ = 
					"mid=" + 		linkmanName + "\r\n" +		//联系人姓名
					"oid=" + 		linkmanMobile + "\r\n" +	//联系人电话
					"msgtype=" + 	psgName + "\r\n" +			//乘客姓名
					"webname=" + 	certType + "\r\n" +			//乘客证件类型
					"netmark=" + 	certId + "\r\n" +			//乘客证件号
					"onick=" + 		depCityOw + "\r\n" +		//出发机场
					"mid=" + 		arrcityOw + "\r\n" +		//到达机场
					"servername=" + depDateOw + "\r\n" +		//出发日期
					"appname=" + 	depDateRt + "\r\n" +		//返程日期
					"islogin=" + 	airlineOw + "\r\n" +		//航班号
					"platform=电脑浏览器\r\n" +
					"state=" + type_ + "\r\n";			
				WriteIndexFile();
			}
        }		
		return 0;
	}
	return 0; 
}
