// PiaoWu114.cpp: implementation of the PiaoWu114 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PiaoWu114.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//定义的证件类型(国内飞机票)
string credential_type_piaowu[6] = {"身份证","护照","军官证","士兵证","台胞证","其它"};

PiaoWu114::PiaoWu114(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

PiaoWu114::~PiaoWu114()
{

}

int PiaoWu114::Is(HttpSession *http_session)
{
	//pc 静态登录
	if((http_session->m_Requestline.m_Host.Find("ucenter.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/datum/userdata/userlogin.html")!=-1))
	{
		return PC_LOGIN;
	}
	//pc 动态登录
	if((http_session->m_Requestline.m_Host.Find("ucenter.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/notVip/queryorder/loginMyOrder.html")!=-1))
	{
		return PC_LOGIN_DYNAMIC;
	}
	// PC 火车票 自取票 未核验专用通道
	if((http_session->m_Requestline.m_Host.Find("huochepiao.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/newTrainOrder")!=-1))
	{
		return PC_TRAIN;
	}
	// PC 汽车票 
	if((http_session->m_Requestline.m_Host.Find("qiche.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/indexAjax/verifyOrder.action")!=-1))
	{
		return PC_BUS;
	}
	// PC 国内飞机票 国际飞机票
	if((http_session->m_Requestline.m_Host.Find("jipiao.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/indexAjax/verifyOrder.action")!=-1))
	{
		return PC_AIRPLANE;
	}
	// SP 静态登录
	if((http_session->m_Requestline.m_Host.Find("appservice.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ucenter/userBasic_loginByPassword.services")!=-1))
	{
		return SP_LOGIN;
	}
	// SP 汽车票 1
	if((http_session->m_Requestline.m_Host.Find("m.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qiche/qicheYuDing_newOrder.action")!=-1))
	{
		return SP_BUS;
	}
	//SP 汽车票 2
	if((http_session->m_Requestline.m_Host.Find("appservice.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qiche/query/qiCheQuery_newOrder.services")!=-1))
	{
		return SP_BUSSECOND;
	}
	// SP 火车票
	if((http_session->m_Requestline.m_Host.Find("appservice.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/train/booking/singleOrder_ordinary.services")!=-1))
	{
		return SP_TRAIN;
	}
	// SP 飞机票
	if((http_session->m_Requestline.m_Host.Find("m.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jipiao/jipiaoYuDing_newOrder.action")!=-1))
	{
		return SP_AIRPLANE;
	}
	return 0;
}


int PiaoWu114::Process(const char *packet,int action)
{
	//pc 登录 
	if (action == PC_LOGIN)
	{
		string src_data(packet);
		string src_dataEnd = src_data.substr(src_data.rfind("username="));
		src_dataEnd = ms_->UrlDecode(src_dataEnd);
		ms_->SplitStringToMap(src_dataEnd,'&','=');
		
		string user,pass;
		user=ms_->GetValueFromMapByKey("username",true);
		pass=ms_->GetValueFromMapByKey("password",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=登录\r\n";	
		WriteIndexFile();
		return 0;
	}
	if (action == PC_LOGIN_DYNAMIC)
	{
		//数据部分
		string src_data(packet);
		string src_dataEnd;
		if (src_data.rfind("regPhone") != string::npos )
		{
			src_dataEnd = src_data.substr(src_data.rfind("regPhone"));
		}	
		src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
		ms_->SplitStringToMap(src_dataEnd,'&','=');

		string user,pass;
		user=ms_->GetValueFromMapByKey("regPhone",true);
		pass=ms_->GetValueFromMapByKey("randCode",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=登录\r\n";	
		WriteIndexFile();
		return 0;
	}
	//PC 国内机票 国际机票
	if (action == PC_AIRPLANE)
	{	
		//从cookie 中提取出发地 目的地
		string depCity,arrCity;
		string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
		cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
		ms_->SplitStringToMap(cookie, ';','=');	
		//出发地
		depCity = ms_->GetValueFromMapByKey("depCity");
		//目的地
		arrCity = ms_->GetValueFromMapByKey("arrCity");

		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//获取乘客的人数
		string temp = sContent.substr(sContent.rfind("passengers"),sContent.length()-sContent.rfind("passengers"));
		int pos = temp.rfind("passengers");
		string People = temp.substr(pos+ strlen("passengers") + strlen("["),1);
		int PassengerCount = atoi(People.c_str());

		string lickManName,lickManMobileNum;
		//联系人姓名
		lickManName = ms_->GetValueFromMapByKey("lickManName",true);
		//联系人电话
		lickManMobileNum = ms_->GetValueFromMapByKey("lickManMobileNum",true); 
      
		//机票类型 (国内 GN/国际 GW)
		string catype = ms_->GetValueFromMapByKey("catype",true); 
		if (catype == "GN")
		{
			//航班类型 (单程 0 /往返 1) 
			string searchType = ms_->GetValueFromMapByKey("searchType",true);
			
			if (searchType == "0")
			{
				//单程
				type_   = "国内单程机票";
			}
			if (searchType == "1")
			{
				//往返
				type_   = "国内往返机票";
			}
			//联系人姓名，联系人电话，出发地，目的地，乘客姓名，乘客类型，乘客证件类型，乘客证件号，乘客出生日期
			string PassengerName,PassengerType,PassengerCertType,PassengerCertNumber,PassengerBirthday;
			for (int j=0;j < PassengerCount + 1;j++)
			{
				//转化
				CString str;
				str.Format("%d",j);
				string strEnd = str.GetBuffer(0);
				//乘客姓名
				string strTempName = "passengers[" + strEnd + "].name";
				PassengerName = ms_->GetValueFromMapByKey(strTempName,true); 
				//乘客类型	
				string strTempType = "passengers[" + strEnd + "].type";
				string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
				if (PassengerTypeTemp == "0")
				{
					PassengerType = "成人票";
				}
				if (PassengerTypeTemp == "1")
				{
					PassengerType = "儿童票";
				}
				//乘客证件类型
				string strCertType = "passengers[" + strEnd + "].identityType";
				string PassengerCertTypeTemp = ms_->GetValueFromMapByKey(strCertType,true); 
				int temp = atoi(PassengerCertTypeTemp.c_str());
				PassengerCertType = credential_type_piaowu[temp - 1];
				//乘客证件号 
				string strCertNumber = "passengers[" + strEnd + "].identityNo";
				PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
				//乘客出生日期 (身份证号)
				if (temp == 1)
				{
					string year = PassengerCertNumber.substr(6,4);
					string month = PassengerCertNumber.substr(10,2);
					string day = PassengerCertNumber.substr(12,2);
					PassengerBirthday = year + "-" + month + "-" + day;
				}
				else if(temp > 1)
				{
					string strBirthday = "passengers[" + strEnd + "].birthday";
					PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true);
				}			
				if (searchType == "0")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//联系人姓名
						"oid=" + 		lickManMobileNum + "\r\n" +		//联系人电话
						"nick=" + 		depCity + "\r\n" +				//出发地
						"msg=" + 		arrCity + "\r\n" +				//目的地 
						"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
						"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				if (searchType == "1")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//联系人姓名
						"oid=" + 		lickManMobileNum + "\r\n" +		//联系人电话
						"nick=" + 		depCity + "\r\n" +				//出发地
						"msg=" + 		arrCity + "\r\n" +				//目的地 
						"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
						"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
		}
		else if (catype == "GW")
		{
			//航班类型 (单程 0 /往返 1) 
			string searchType = ms_->GetValueFromMapByKey("searchType",true);
			
			if (searchType == "0")
			{
				//单程
				type_   = "国际单程机票";
			}
			if (searchType == "1")
			{
				//往返
				type_   = "国际往返机票";
			}
			//联系人姓名，联系人电话，出发地，目的地，出发日期，返程日期，
			string godate,godatefy;
			//出发日期
			godate = ms_->GetValueFromMapByKey("godate");
			//返程日期
			godatefy = ms_->GetValueFromMapByKey("godatefy");
			//乘客姓名(英文名)，乘客性别，乘客类型，乘客证件类型，乘客证件号，乘客证件签发国，国籍，乘客出生日期，乘客证件有效期
			string PassengerName,PassengerGender,PassengerType,PassengerCertType,PassengerCertNumber,PassengerCardIssuePlaceName,passengerNationality,PassengerBirthday,passengerCardExpired;
			for (int j=0;j < PassengerCount + 1;j++)
			{
				//转化
				CString str;
				str.Format("%d",j);
				string strEnd = str.GetBuffer(0);
				//乘客姓名
				string strTempFirstName = "passengers[" + strEnd + "].firstName";
				string strTemplastName = "passengers[" + strEnd + "].lastName";
				PassengerName = ms_->GetValueFromMapByKey(strTempFirstName,true) + ms_->GetValueFromMapByKey(strTemplastName,true); 
				//乘客性别
				string strTempGender = "passengers[" + strEnd + "].gender";
				PassengerGender = ms_->GetValueFromMapByKey(strTempGender,true); 
				if (PassengerGender == "F")
				{
					PassengerGender = "女";
				}
				if (PassengerGender == "M")
				{
					PassengerGender = "男";
				}
				//乘客类型	
				string strTempType = "passengers[" + strEnd + "].type";
				string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
				if (PassengerTypeTemp == "0")
				{
					PassengerType = "成人票";
				}
				if (PassengerTypeTemp == "1")
				{
					PassengerType = "儿童票";
				}
				//乘客证件类型
				string strTempCardType = "passengers[" + strEnd + "].identityType";
				PassengerCertType = ms_->GetValueFromMapByKey(strTempCardType,true); 
				if (PassengerCertType == "2")
				{
					PassengerCertType = "护照";
				}
				if (PassengerCertType == "7")
				{
					PassengerCertType = "港澳通行证";
				}
				if (PassengerCertType == "8")
				{
					PassengerCertType = "回乡证";
				}
				if (PassengerCertType == "5")
				{
					PassengerCertType = "台胞证";
				}
				//乘客证件号 
				string strCertNumber = "passengers[" + strEnd + "].identityNo";
				PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
				//乘客证件签发国
				string strCertPlaceName = "passengers[" + strEnd + "].cardIssuePlaceName";
				PassengerCardIssuePlaceName = ms_->GetValueFromMapByKey(strCertPlaceName,true);  
				//国籍
				string strCertNationName = "passengers[" + strEnd + "].nationalityName";
				passengerNationality = ms_->GetValueFromMapByKey(strCertNationName,true); 
				//乘客出生日期
				string strBirthday = "passengers[" + strEnd + "].birthday";
				PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true);
				//乘客证件有效期
				string strExpired = "passengers[" + strEnd + "].cardExpired";
				passengerCardExpired = ms_->GetValueFromMapByKey(strExpired,true);

				if (searchType == "0")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//联系人姓名
						"oid=" + 		lickManMobileNum + "\r\n" +		//联系人电话
						"nick=" + 		depCity + "\r\n" +				//出发地
						"msg=" + 		arrCity + "\r\n" +				//目的地 
						"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
						"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";			
					WriteIndexFile();
				}
				if (searchType == "1")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//联系人姓名
						"oid=" + 		lickManMobileNum + "\r\n" +		//联系人电话
						"nick=" + 		depCity + "\r\n" +				//出发地
						"msg=" + 		arrCity + "\r\n" +				//目的地 
						"msgtype=" + 	PassengerName + "\r\n" +		//乘客姓名
						"webname=" + 	PassengerCertType + "\r\n" +	//乘客证件类型
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//乘客证件号
						"servername=" + godate + "\r\n" +				//出发日期
						"appname=" + 	godatefy + "\r\n" +				//返程日期
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";				
					WriteIndexFile();
				}
			}
		}
		return 0;
	}
    //火车票
	if (action == PC_TRAIN)
	{
		//联系人姓名，联系人电话，始发地，目的地，车次，出发日期，到达时间(只有时间)，
		//乘客姓名，乘客类型，乘客证件类型，乘客证件号，票类型，总价格)
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');

		//联系人姓名
		name_    = ms_->GetValueFromMapByKey("oUserName",true);
		//联系人电话
		tel_     = ms_->GetValueFromMapByKey("oUserMobile",true); 	
		//出发地
		start_   = ms_->GetValueFromMapByKey("fromStation",true);
		//目的地
		end_     = ms_->GetValueFromMapByKey("toStation",true);	
		//火车车次 
		number_ = ms_->GetValueFromMapByKey("trainCode",true);
		//出发时间
		time_ = ms_->GetValueFromMapByKey("goDate",true) + " " + ms_->GetValueFromMapByKey("startTime",true);
		//到达时间
		string arrTime = ms_->GetValueFromMapByKey("endTime",true);
		//票类型
		string ticketType = ms_->GetValueFromMapByKey("oTicketType",true);
		//总价格
		price_  = ms_->GetValueFromMapByKey("payTotal",true);
		type_   = "火车票";	

		//乘客信息
		string passengerName,passengerIsAdult,passengerCardType,passengerCardNumber,passengerBirthday;
		//乘客数量
        string headcount = ms_->GetValueFromMapByKey("ticketNum",true);
		int headNumber = atoi(headcount.c_str());

		//乘客姓名，乘客类型，乘客票类型，乘客证件类型，乘客证件号，总价格)
		for (int i = 0;i< headNumber;i++)
		{
			CString str;
			str.Format("%d",i);
			string strEnd = str.GetBuffer(0);
			//乘客姓名
			string strTempName = "passengers[" + strEnd + "].haveName";
			passengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//乘客类型	
			string strTempType = "passengers[" + strEnd + "].type";
			passengerIsAdult = ms_->GetValueFromMapByKey(strTempType,true); 
			if (passengerIsAdult == "1")
			{
				passengerIsAdult = "成人票";
			}
			if (passengerIsAdult == "2")
			{
				passengerIsAdult = "儿童票";
			}
			//乘客证件类型
			string strCertType = "passengers[" + strEnd + "].cardType";
			passengerCardType = ms_->GetValueFromMapByKey(strCertType,true); 
			//乘客证件号 
			string strCertNumber = "passengers[" + strEnd + "].haveId";
			passengerCardNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
				
			output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//联系人姓名
						"oid=" + 		tel_ + "\r\n" +					//联系人电话
						"nick=" + 		start_ + "\r\n" +				//出发地
						"msg=" + 		end_ + "\r\n" +					//目的地 
						"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
						"webname=" + 	passengerCardType + "\r\n" +	//乘客证件类型
						"netmark=" + 	passengerCardNumber + "\r\n" +	//乘客证件号
						"clientmsg=" + 	number_ + "\r\n" +				//车次 
						"servername=" + time_ + "\r\n" +				//出发日期
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;	
	}
	//汽车票
	if (action == PC_BUS)
	{
		//联系人姓名，联系人电话，出发地，目的地，出发站名，到达站名，出发时间，乘客姓名，乘客证件号，总价格(不含保险)
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
	//获取乘客的人数
		string temp = src_data.substr(src_data.rfind("passengers"),src_data.length()-src_data.rfind("passengers"));
		int pos = temp.rfind("passengers");
		string People = temp.substr(pos+ strlen("passengers") + strlen("["),1);
		int PassengerCount = atoi(People.c_str());
	//联系人信息
		//联系人姓名
		name_    = ms_->GetValueFromMapByKey("lickManName",true);
		//联系人电话
		tel_     = ms_->GetValueFromMapByKey("lickManMobileNum",true); 	   
	//车次信息
		string orderTrain = ms_->GetValueFromMapByKey("orderTrain",true); 	
		//使用json 读取数据
		string startStationName,belongPlaceName;
		Json::Reader reader;
		Json::Value value;
		//判断是添加还是更改
		if (reader.parse(orderTrain,value))
		{	
			//出发地
			start_   = value["fromCityName"].asString();
			//目的地
			end_     = value["toCityName"].asString();
			//出发站名
			startStationName = value["startStationName"].asString();
			//到达站名
			belongPlaceName  = value["belongPlaceName"].asString();
			//出发时间
			time_ = value["startDate"].asString() + " " + value["startTimestamp"].asString();
			//总价格
			int price = value["fullPrice"].asInt();
			CString strTempPrice;
			strTempPrice.Format("%d",price);
			price_  = strTempPrice.GetBuffer(0);
		}
		type_   = "汽车票";		
	//乘客信息
		string passengerName,passengerCardType,passengerCardNumber;	
		
		for (int i = 0;i<= PassengerCount;i++)
		{
			CString str;
			str.Format("%d",i);
			string strEnd = str.GetBuffer(0);
			//乘客姓名
			string strTempName = "passengers[" + strEnd + "].name";
			passengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//乘客证件类型
			passengerCardType = "身份证";
			//乘客证件号 
			string strCertNumber = "passengers[" + strEnd + "].identityNo";
			passengerCardNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
			
			output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//联系人姓名
						"oid=" + 		tel_ + "\r\n" +					//联系人电话
						"nick=" + 		start_ + "\r\n" +				//出发地
						"msg=" + 		end_ + "\r\n" +					//目的地 
						"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
						"netmark=" + 	passengerCardNumber + "\r\n" +	//乘客证件号
						"imsi=" + 		startStationName + "\r\n" +		//出发站名
						"clientmac=" + belongPlaceName + "\r\n" +		//到达站名 
						"servername=" + time_ + "\r\n" +				//出发日期
						"platform=电脑浏览器\r\n" +
						"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;	
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//app 登录 
	if (action == SP_LOGIN)
	{
		string src_data(packet);
		src_data=ms_->UrlDecode(src_data);
		ms_->SplitStringToMap(src_data,'&','=');
		
		string user,pass;
		user=ms_->GetValueFromMapByKey("userBean.userName",true);
		pass=ms_->GetValueFromMapByKey("userBean.password",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=登录\r\n";	
		WriteIndexFile();
		return 0;
	}
	if (action == SP_LOGIN_DYNAMIC)
	{
		//数据部分
		string src_data(packet);
		string src_dataEnd;
		if (src_data.rfind("regPhone") != string::npos )
		{
			src_dataEnd = src_data.substr(src_data.rfind("regPhone"));
		}	
		src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
		ms_->SplitStringToMap(src_dataEnd,'&','=');
		
		string user,pass;
		user=ms_->GetValueFromMapByKey("regPhone",true);
		pass=ms_->GetValueFromMapByKey("randCode",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=登录\r\n";	
		WriteIndexFile();
		return 0;
	}
	//飞机票
	if (action == SP_AIRPLANE)
	{	
		//单程 (联系人电话，出发地，目的地，出发机场，到达机场，出发日期，航班号) + 公共
		//往返 (联系人电话，出发地，目的地，去程出发机场，去程到达机场，去程出发日期，去程航班号，返程出发机场，返程到达机场，返程出发日期，返程航班号) + 公共
		//公共 (乘客姓名，乘客类型，乘客证件类型，乘客证件号，乘客生日，总价格)
	//联系人信息
		string telNum;
	//航班信息
		string flight,orgCityNameC,dstCityNameC,orgCityName,dstCityName,depDate,arrDate,airwaysCn,flightNo;
		string flightfy,orgCityNameCfy,dstCityNameCfy,orgCityNamefy,dstCityNamefy,depDatefy,arrDatefy,airwaysCnfy,flightNofy;
	//乘客信息
		string passengerName,passengerType,passengerCardType,passengerCard,passengerBirth,totalPrice;
	//机票类型 GN 国内 GW 国际
		string catype;
	//航班类型 1 往返 0 单程
		string searchType;
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		//乘客信息
		string passengerInfo = sContent.substr(sContent.find("passengerName"),sContent.find("telNum") - sContent.find("passengerName") - strlen("&"));
		ms_->SplitStringToMap(sContent,'&','=');
		//联系人电话
		telNum = ms_->GetValueFromMapByKey("telNum",true);    
		//航班类型 (单程 0 /往返 1) 
		searchType = ms_->GetValueFromMapByKey("searchType",true);	
		//机票类型 (国内 GN/国际 GW)
		catype = ms_->GetValueFromMapByKey("catype",true); 
		//总价格
		totalPrice = ms_->GetValueFromMapByKey("totalPrice",true);	

		if (searchType == "0")
		{
			//单程
			flight = ms_->GetValueFromMapByKey("flight",true);
			//使用json 读取数据
			Json::Reader reader;
			Json::Value value;
			//判断是添加还是更改
			if (reader.parse(flight,value))
			{	
				//出发地
				orgCityNameC = value["orgCityNameC"].asString();
				//目的地
				dstCityNameC = value["dstCityNameC"].asString();
				//去程出发机场
				orgCityName = value["orgCityName"].asString() + value["orgJetquay"].asString();
				//去程到达机场
				dstCityName = value["dstCityName"].asString() + value["dstJetquay"].asString();
				//去程出发日期
				depDate = value["depDate"].asString() + " " + value["depTime"].asString();
				//去程到达日期
				arrDate = value["arrDate"].asString() + " " + value["arriTime"].asString();
				//去程时长
				//去程航空公司
				airwaysCn = value["airwaysCn"].asString();
				//去程航班号
				flightNo = value["flightNo"].asString();
			}
		}
		if (searchType == "1")
		{
			flight = ms_->GetValueFromMapByKey("flight",true);
			//使用json 读取数据
			Json::Reader reader;
			Json::Value value;
			//判断是添加还是更改
			if (reader.parse(flight,value))
			{	
				//出发地
				orgCityNameC = value["orgCityNameC"].asString();
				//目的地
				dstCityNameC = value["dstCityNameC"].asString();
				//去程出发机场
				orgCityName = value["orgCityName"].asString() + value["orgJetquay"].asString();
				//去程到达机场
				dstCityName = value["dstCityName"].asString() + value["dstJetquay"].asString();
				//去程出发日期
				depDate = value["depDate"].asString() + " " + value["depTime"].asString();
				//去程到达日期
				arrDate = value["arrDate"].asString() + " " + value["arriTime"].asString();
				//去程时长
				//去程航空公司
				airwaysCn = value["airwaysCn"].asString();
				//去程航班号
				flightNo = value["flightNo"].asString();
			}
			flightfy = ms_->GetValueFromMapByKey("flightfy",true);
			//使用json 读取数据
			Json::Reader readerfy;
			Json::Value valuefy;
			//判断是添加还是更改
			if (readerfy.parse(flightfy,value))
			{			
				//返程出发机场
				orgCityNamefy = value["orgCityName"].asString() + value["orgJetquay"].asString();
				//返程到达机场
				dstCityNamefy = value["dstCityName"].asString() + value["dstJetquay"].asString();
				//返程出发日期
				depDatefy = value["depDate"].asString() + " " + value["depTime"].asString();
				//返程到达日期
				arrDatefy = value["arrDate"].asString() + " " + value["arriTime"].asString();
				//返程时长	
				//返程航空公司
				airwaysCnfy = value["airwaysCn"].asString();
				//返程航班号
				flightNofy = value["flightNo"].asString();
			}	
		}
		if (catype == "GN")
		{
			vector <string> passenger;
			passenger.clear();
			while(passengerInfo.length() > 0)
			{				
				int pos = passengerInfo.find("passengerName",0);
				int hos = passengerInfo.find("passTypes",pos);		
				if (pos != string::npos && hos != string::npos)
				{
					string temp = passengerInfo.substr(pos,hos + strlen("passTypes=1"));
					passenger.push_back(temp);
					passengerInfo = passengerInfo.substr(hos + strlen("passTypes=1"));
				}
			}   
			for(int i =0;i< passenger.size();i++)
			{
				ms_->SplitStringToMap(passenger[i],'&','=');
				//乘客姓名
				passengerName = ms_->GetValueFromMapByKey("passengerName",true);	
				//乘客类型	
				passengerType = ms_->GetValueFromMapByKey("passTypes",true); 
				if (passengerType == "0")
				{
					passengerType = "成人票";
				}
				if (passengerType == "1")
				{
					passengerType = "儿童票";
				}
				//乘客证件类型
				passengerCardType = ms_->GetValueFromMapByKey("chooseCard",true); 
				int temp = atoi(passengerCardType.c_str());
				if (temp == 1)
				{
					passengerCardType = "身份证";
				}
				if (temp == 2)
				{
					passengerCardType = "护照";
				}
				//乘客证件号 
				passengerCard = ms_->GetValueFromMapByKey("passengerCard",true);  
				//乘客出生日期 (身份证号)
				if (temp == 1)
				{
					string year = passengerCard.substr(6,4);
					string month = passengerCard.substr(10,2);
					string day = passengerCard.substr(12,2);
					passengerBirth = year + "-" + month + "-" + day;
				}
				if(temp == 2)
				{
					passengerBirth = ms_->GetValueFromMapByKey("passengerBirth",true);
				}		
				
				if (searchType == "0")
				{
					type_   = "国内单程机票";
					
					output_text_ = 
						"oid=" + 		telNum + "\r\n" +				//联系人电话
						"nick=" + 		orgCityNameC + "\r\n" +			//出发地
						"msg=" + 		dstCityNameC + "\r\n" +			//目的地 
						"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
						"webname=" + 	passengerCardType + "\r\n" +	//乘客证件类型
						"netmark=" + 	passengerCard + "\r\n" +		//乘客证件号
						"onick=" + 		orgCityName + "\r\n" +			//出发机场
						"mid=" + 		dstCityName + "\r\n" +			//到达机场
						"servername=" + depDate + "\r\n" +				//出发日期
						"islogin=" + 	flightNo + "\r\n" +				//航班号
						"platform=手机app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				if (searchType == "1")
				{
					type_   = "国内往返机票";

					output_text_ = 
						"oid=" + 		telNum + "\r\n" +				//联系人电话
						"nick=" + 		orgCityNameC + "\r\n" +			//出发地
						"msg=" + 		dstCityNameC + "\r\n" +			//目的地 
						"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
						"webname=" + 	passengerCardType + "\r\n" +	//乘客证件类型
						"netmark=" + 	passengerCard + "\r\n" +		//乘客证件号
						"onick=" + 		orgCityName + "\r\n" +			//出发机场
						"mid=" + 		dstCityName + "\r\n" +			//到达机场
						"servername=" + depDate + "\r\n" +				//出发日期
						"appname=" + 	depDatefy + "\r\n" +			//返程日期
						"islogin=" + 	flightNo + "\r\n" +				//航班号
						"platform=手机app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
		}
		else if (catype == "GW")
		{
	      //暂无
		}
		return 0;
	}
	//火车票 
	if (action == SP_TRAIN)
	{
		//联系人电话，始发地，目的地，车次,出发时间，运行时间，乘客姓名，乘客类型，乘客证件类型，乘客证件号，总价格
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
		//联系人电话
		tel_     = ms_->GetValueFromMapByKey("order.contactsMobile",true); 	
		//出发地
		start_   = ms_->GetValueFromMapByKey("order.startStation",true);
		//目的地
		end_     = ms_->GetValueFromMapByKey("order.endStation",true);	
		//火车车次 
		number_ = ms_->GetValueFromMapByKey("order.trainNumber",true);
		//出发时间
		time_ = ms_->GetValueFromMapByKey("order.goDate",true) + " " + ms_->GetValueFromMapByKey("order.trainGoTime",true);
		//运行时间 
		string runMinute = ms_->GetValueFromMapByKey("order.runMinute",true);
		//总价格
		price_  = ms_->GetValueFromMapByKey("order.totalPrice",true);
		type_   = "火车票";	

		//乘客信息
		string passengerName,passengerIsAdult,passengerCardType,passengerCardNumber,passengerBirthday;
		//乘客数量
        string headcount = ms_->GetValueFromMapByKey("order.ticketNum",true);
		int headNumber = atoi(headcount.c_str());
		for (int i = 0;i< headNumber;i++)
		{
			CString str;
			str.Format("%d",i);
			string strEnd = str.GetBuffer(0);
			//乘客姓名
			string strTempName = "passengers[" + strEnd + "].passengerName";
			passengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//乘客类型
			string strTemppassengerIsAdult = "passengers[" + strEnd + "].ticketType";
			passengerIsAdult = ms_->GetValueFromMapByKey(strTemppassengerIsAdult,true); 
			if (passengerIsAdult == "1")
			{
				passengerIsAdult = "成人票";
			} 
			if (passengerIsAdult == "2")
			{
				passengerIsAdult = "儿童票";
			}
			//乘客证件类型
			string strTempCardType = "passengers[" + strEnd + "].idType";
			passengerCardType = ms_->GetValueFromMapByKey(strTempCardType,true); 
			//乘客证件号 
			string strCertNumber = "passengers[" + strEnd + "].idNumber";
			passengerCardNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  

			output_text_ = 
				"oid=" + 		tel_ + "\r\n" +					//联系人电话
				"nick=" + 		start_ + "\r\n" +				//出发地
				"msg=" + 		end_ + "\r\n" +					//目的地 
				"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
				"webname=" + 	passengerCardType + "\r\n" +	//乘客证件类型
				"netmark=" + 	passengerCardNumber + "\r\n" +  //乘客证件号
				"clientmsg=" + 	number_ + "\r\n" +				//车次 
				"servername=" + time_ + "\r\n" +				//出发日期
				"platform=手机app\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;
	}
	//汽车票 1
	if (action == SP_BUS)
	{
		//联系人电话，出发地，目的地，出发站名，到达站名，出发时间，乘客姓名，乘客证件号，总价格
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
	//联系人信息
		//联系人电话
		tel_     = ms_->GetValueFromMapByKey("telNum",true); 	   
		//车次信息
		string orderTrain = ms_->GetValueFromMapByKey("carRoute",true); 	
		//使用json 读取数据
		string startStationName,belongPlaceName;
		Json::Reader reader;
		Json::Value value;
		//判断是添加还是更改
		if (reader.parse(orderTrain,value))
		{	
			//出发地
			start_   = value["fromCityName"].asString();
			//目的地
			end_     = value["toCityName"].asString();
			//出发站名
			startStationName = value["startStationName"].asString();
			//到达站名
			belongPlaceName  = value["belongPlaceName"].asString();
			//出发时间
			time_ = value["startDate"].asString() + "-" + value["startTimestamp"].asString();
		}
		//总价格
// 		int price = value["fullPrice"].asInt();
// 		CString strTempPrice;
// 		strTempPrice.Format("%d",price);
		price_  = ms_->GetValueFromMapByKey("totalPrice",true); 
		type_   = "汽车票";		
		//乘客信息
		string passengerName,passengerCardType,passengerCardNumber;
		string passengerInfo = src_data.substr(src_data.find("passengerName"),src_data.find("telNum") - src_data.find("passengerName") - strlen("&"));
		vector <string> passenger;
		passenger.clear();
		while(passengerInfo.length() > 0)
		{				
			int pos = passengerInfo.find("passengerName",0);
			int hos = passengerInfo.find("passengerBirth",pos);		
			if (pos != string::npos && hos != string::npos)
			{
				string temp = passengerInfo.substr(pos,hos + strlen("passengerBirth="));
				passenger.push_back(temp);
				passengerInfo = passengerInfo.substr(hos + strlen("passengerBirth="));
			}
		}   
		for(int i =0;i< passenger.size();i++)
		{
			ms_->SplitStringToMap(passenger[i],'&','=');
			//乘客姓名
			passengerName = ms_->GetValueFromMapByKey("passengerName",true);
			//乘客证件类型
			passengerCardType = "身份证";
			//乘客证件号 
			passengerCardNumber = ms_->GetValueFromMapByKey("passengerCard",true);  
			
			output_text_ = 
				"oid=" + 		tel_ + "\r\n" +					//联系人电话
				"nick=" + 		start_ + "\r\n" +				//出发地
				"msg=" + 		end_ + "\r\n" +					//目的地 
				"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
				"netmark=" + 	passengerCardNumber + "\r\n" +	//乘客证件号
				"imsi=" + 		startStationName + "\r\n" +		//出发站名
				"clientmac=" + 	belongPlaceName + "\r\n" +      //到达站名 
				"platform=手机app\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;
	}
	//汽车票 2
	if (action == SP_BUSSECOND)
	{
		//联系人电话，出发地，目的地，乘客姓名，乘客证件号，出发站名，到达站名，出发日期
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
		//数据
		string orderData = ms_->GetValueFromMapByKey("orderData",true);	
		//使用json 读取数据
		string startStationName,belongPlaceName;
		string passengerName,passengerCardType,passengerCardNumber;
		Json::Reader reader;
		Json::Value value;
		//判断是添加还是更改
		if (reader.parse(orderData,value))
		{	
			//联系人电话
			tel_     = value["linkPhone"].asString();   
         
			Json::Value orderTrain = value["orderTrain"];
			//出发地
			start_   = orderTrain["fromCityName"].asString();
			//目的地
			end_     = orderTrain["toCityName"].asString();
			//出发站名
			startStationName = orderTrain["startStationName"].asString();
			//到达站名
			belongPlaceName  = orderTrain["belongPlaceName"].asString();
			//出发时间
			time_ = orderTrain["startDate"].asString() + "-" + orderTrain["startTimestamp"].asString();
			type_   = "汽车票";	

			Json::Value passengerInfo = value["passengers"];
			for (int j = 0;j <passengerInfo.size();j++)
			{
				string out = passengerInfo[j]["p"].asString();
				//乘客姓名
				passengerName = passengerInfo[j]["name"].asString();	
				//乘客证件类型
				passengerCardType = "身份证";
				//乘客证件号 
				passengerCardNumber = passengerInfo[j]["identityNo"].asString();
				output_text_ = 
					"oid=" + 		tel_ + "\r\n" +					//联系人电话
					"nick=" + 		start_ + "\r\n" +				//出发地
					"msg=" + 		end_ + "\r\n" +					//目的地 
					"msgtype=" + 	passengerName + "\r\n" +		//乘客姓名
					"netmark=" + 	passengerCardNumber + "\r\n" +	//乘客证件号
					"imsi=" + 		startStationName + "\r\n" +		//出发站名
					"clientmac=" + 	belongPlaceName + "\r\n" +      //到达站名 
					"platform=手机app\r\n" +
					"state=" + type_ + "\r\n";		
				WriteIndexFile();
			}
		}
		return 0;
	}
	return 0; 
}
