
#include "StdAfx.h"
#include "QiChePiao.h"



QiChePiao::QiChePiao(HttpSession *http_session, const string &pro_name) :  Traffic(http_session, pro_name)
{
	
}

QiChePiao::~QiChePiao()
{
	
}

void QiChePiao::SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
		
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


int QiChePiao::Is(HttpSession *http_session)
{
	//pc登录
	if((http_session->m_Requestline.m_Host.Find("passport.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login")!=-1))
	{
		return kDriverLogin;
	}
	//pc查询车票
	if((http_session->m_Requestline.m_Host.Find("www.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index/searchModel")!=-1))
	{
		return kNewVoiceOrder;
	}
	//pc提交订单
	if((http_session->m_Requestline.m_Host.Find("www.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/submitOrder")!=-1))
	{
		return kPreOrder;
	}
	//pc包车
	if((http_session->m_Requestline.m_Host.Find("www.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/baoche/rentBusConfirmOrder")!=-1))
	{
		return kNewTextOrder;
	}

	//安卓登录
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/captchaLogin")!=-1))
	{
		return kPassengerLogin;
	}

	//手机提交订单
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/confirmOrder")!=-1))
	{
		return kDriverWorking;
	}

	//手机查询车票
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/schedule/getLineList.srv")!=-1))
	{
		return kDriverRegister;
	}

	//手机包车
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/confirmRentBusOrder")!=-1))
	{
		return kMobileBaoChe;
	}
	//手机约车
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/confirmZuCheOrderByPoi")!=-1))
	{
		return kMobileYueChe;
	}

	//手机快线
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/newConfirmBusOrder")!=-1))
	{
		return kMobileTrip;
	}

	return 0;
}

int QiChePiao::Process(const char *packet,int action)
{
	//pc登录
	if (action == kDriverLogin)
	{
			return DriverLogin(packet);
	}
	//pc查询车票
	if (action == kNewVoiceOrder)
	{
		return NewVoiceOrder(packet);
	}
	//pc提交订单
	if (action == kPreOrder)
	{
		return PreOrder(packet);
	}
	//pc包车
	if (action == kNewTextOrder)
	{
		return NewTextOrder(packet);
	}
	//安卓登录
	if (action == kPassengerLogin)
	{
		return PassengerLogin(packet);
	}
	//手机提交订单
	if (action == kDriverWorking)
	{
		return DriverWorking(packet);
	}
	//手机查询车票
	if (action == kDriverRegister)
	{
		return DriverRegister(packet);
	}
	//手机包车
	if (action == kMobileBaoChe)
	{
		return MobileBaoChe(packet);
	}
	//手机约车
	if (action == kMobileYueChe)
	{
		return MobileYueChe(packet);
	}
	//手机快车
	if (action == kMobileTrip)
	{
		return MobileTrip(packet);
	}
	return 0;
}


//pc登录信息
int QiChePiao::DriverLogin(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string username,password;
	//用户名
	username = ms_->GetValueFromMapByKey("username",true);
	//密码
	password = ms_->GetValueFromMapByKey("password",true);
 
	if (username.empty() || password.empty())
	{
	 	return 0;
	}
	
	output_text_ = 
	 	"user=" + username + "\r\n" +
	 	"pass=" + password + "\r\n" + 
		"platform=电脑浏览器\r\n" +
	 	"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//pc查询车票地址日期
int QiChePiao::NewVoiceOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//提取需要的数据
	src_data = src_data.substr(src_data.rfind("startCityName="),src_data.length()-src_data.rfind("startCityName"));
	ms_->SplitStringToMap(src_data,'&','=');
    //出发地
	start_ = ms_->GetValueFromMapByKey("startCityName",true);	
	//目的地
	end_ = ms_->GetValueFromMapByKey("endCityName",true);
	//出发日期
	time_ = ms_->GetValueFromMapByKey("startDate",true);
		
	if (start_.empty() || end_.empty() || time_.empty())
	{
		return 0;
	}
	output_text_ = 
			   "nick=" + start_ + "\r\n" +
			   "msg=" + end_ + "\r\n" +
			   "servername=" + time_ + "\r\n" +
			   "platform=电脑浏览器\r\n" +
			   "state=查询汽车票\r\n";
	WriteIndexFile();	

	return 1;
}

//pc车票预订提交订单
int QiChePiao::PreOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//联系人信息 (取票人)
	//姓名
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//电话
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//身份证号
	string idcard = ms_->GetValueFromMapByKey("certificateNo",true); 

    //乘客信息
	//passengerInfo=冯依波,17195279563,520322198712270862,01,0,0.0;华仁娟,17195279564,640324198704213155,01,0,0.0;
	string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);
	vector<string> vPassenger;
	//分隔符 ;
	SplitString(passengerInfo, vPassenger,";"); 
	for(vector<string>::size_type i = 0; i != vPassenger.size(); ++i)
	{
		vector<string> vInfo;
		//分隔符 ,
		SplitString(vPassenger[i], vInfo,","); 
		//乘客姓名，乘客电话，客身份证号
		string passengerName,passengerTel,passengerIdcard;
		passengerName = vInfo[0];
		passengerTel = vInfo[1];
		passengerIdcard = vInfo[2];

		if (name_.empty() || tel_.empty() || idcard.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"mid=" + name_ + "\r\n" +				//联系人姓名
			"oid=" + tel_ + "\r\n" +				//联系人电话
			"userid=" + idcard + "\r\n" +			//联系人证件号
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"clientip=" + passengerTel + "\r\n" +	//乘客电话
			"netmark=" + passengerIdcard + "\r\n" + //乘客证件号
			"platform=电脑浏览器\r\n" +
			"state=汽车票\r\n";	
		WriteIndexFile();		
	}   
	return 1;
}

//pc包车
int QiChePiao::NewTextOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//联系人信息
	//姓名
    name_ = ms_->GetValueFromMapByKey("userName",true);
	//电话
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//出发地点
	start_ = ms_->GetValueFromMapByKey("startCity",true) + ms_->GetValueFromMapByKey("startPlace",true);	
	//到达地点
	end_ = ms_->GetValueFromMapByKey("endCity",true) + ms_->GetValueFromMapByKey("arrivalPlace",true);		
	//出发时间 (用车时间)
	time_ = ms_->GetValueFromMapByKey("startTime",true);

	//是否往返
	string isBack,arrivalTime;
	isBack = ms_->GetValueFromMapByKey("isBack",true);
	//单程
	if (isBack == "1")
	{
		arrivalTime = "无";
	}
	//往返
	if (isBack == "2")
	{
		//返程时间 (返程时间)
		arrivalTime = ms_->GetValueFromMapByKey("arrivalTime",true);
	}
	//包车人数
	number_ = ms_->GetValueFromMapByKey("personCount",true);
	//包车类型 (商务车7座 2-1 ,小巴车22座 1-1 ,中巴车33座 3-1,大巴车53座 4-1)
	type_  = ms_->GetValueFromMapByKey("rentBusInfo",true);
    
	//出发地点坐标(备用) 坐标 
	string startpos = ms_->GetValueFromMapByKey("startLongLat",true);
	//到达地点坐标(备用)
	string endpos = ms_->GetValueFromMapByKey("endLongLat",true);
	//发票信息(备用) 发票标题，人，地址
    string invoice = ms_->GetValueFromMapByKey("invoice",true);
	if (invoice == "1")
	{
		string invoiceInfo = ms_->GetValueFromMapByKey("invoiceTitle",true) + ms_->GetValueFromMapByKey("recipients",true) + ms_->GetValueFromMapByKey("address",true);
	}
	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"appname=" + arrivalTime + "\r\n" + 
		"fileurl=" + number_ + "\r\n" +
		"subject=" + type_ + "\r\n" + 
		"platform=电脑浏览器\r\n" +
		"state=包车\r\n";
	WriteIndexFile();

	return 1;
}


//手机登录
int QiChePiao::PassengerLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
    
	string username,password;
	username = ms_->GetValueFromMapByKey("mobilePhone",true);
	//密码为md5 加密
	password = ms_->GetValueFromMapByKey("password",true);

	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + username + "\r\n" +
		"pass=" + password + "\r\n" + 
		"platform=手机App\r\n" +
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}


//手机查询车票 //此外还可查询详细的车次

int QiChePiao::DriverRegister(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	//提取需要的数据
	src_data = src_data.substr(src_data.rfind("sign="),src_data.length()-src_data.rfind("sign"));
	ms_->SplitStringToMap(src_data,'&','=');
    //出发地
	start_ = ms_->GetValueFromMapByKey("startCityName",true);
	//目的地
	end_ = ms_->GetValueFromMapByKey("endCityName",true);
	//出发日期
	time_ = ms_->GetValueFromMapByKey("startDate",true);
	
	if (start_.empty() || end_.empty() || time_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"platform=手机App\r\n" +
		"state=查询汽车票\r\n";
	WriteIndexFile();	
	
	return 1;
}

//手机提交订单
int QiChePiao::DriverWorking(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//联系人信息 (取票人)
	//姓名
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//电话
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//身份证号
	string idcard = ms_->GetValueFromMapByKey("certificateNo",true); 
    //乘客信息
	//passengerInfo=冯依波,17195279563,520322198712270862,01,0,0.0;华仁娟,17195279564,640324198704213155,01,0,0.0
    string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);

	vector<string> vPassenger;
	//分隔符 ;
	SplitString(passengerInfo, vPassenger,";"); 
	for(vector<string>::size_type i = 0; i != vPassenger.size(); ++i)
	{
		vector<string> vInfo;
		//分隔符 ,
		SplitString(vPassenger[i], vInfo,","); 
		//乘客姓名，乘客电话，客身份证号
		string passengerName,passengerTel,passengerIdcard;
		passengerName = vInfo[0];
		passengerTel = vInfo[1];
		passengerIdcard = vInfo[2];
		
		if (name_.empty() || tel_.empty() || idcard.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"mid=" + name_ + "\r\n" +
			"oid=" + tel_ + "\r\n" +
			"userid=" + idcard + "\r\n" +
			"msgtype=" + passengerName + "\r\n" +
			"clientip=" + passengerTel + "\r\n" +
			"webname=" + passengerIdcard + "\r\n" +
			"platform=手机App\r\n" +
			"state=汽车票\r\n";	
		WriteIndexFile();		
	}   
	return 1;
}


//手机包车
int QiChePiao::MobileBaoChe(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//联系人信息
	//姓名
    name_ = ms_->GetValueFromMapByKey("userName",true);
	//电话
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);

	//出发时间 (用车时间)
	time_ = ms_->GetValueFromMapByKey("startTime",true);
	//出发地点
	start_ = ms_->GetValueFromMapByKey("startCity",true) + ms_->GetValueFromMapByKey("startPlace",true);	
	//到达地点
	end_ = ms_->GetValueFromMapByKey("endCity",true) + ms_->GetValueFromMapByKey("arrivalPlace",true);	

	//是否往返
	string isBack,arrivalTime;
	isBack = ms_->GetValueFromMapByKey("isBack",true);
	//单程
	if (isBack == "1")
	{
		arrivalTime = "无";
	}
	//往返
	if (isBack == "2")
	{
		//返程时间 (返程时间)
		arrivalTime = ms_->GetValueFromMapByKey("arrivalTime",true);
	}

	//包车人数
	number_ = ms_->GetValueFromMapByKey("personCount",true);
	//包车类型 (商务车7座 2-1 ,小巴车22座 1-1 ,中巴车33座 3-1,大巴车53座 4-1)
	type_  = ms_->GetValueFromMapByKey("rentBusInfo",true);
    
	//出发地点坐标(备用) 坐标 
	string startpos = ms_->GetValueFromMapByKey("startLongLat",true);
	//到达地点坐标(备用)
	string endpos = ms_->GetValueFromMapByKey("endLongLat",true);
	//发票信息(备用) 发票标题，人，地址
    string invoice = ms_->GetValueFromMapByKey("invoice",true);
	if (invoice == "1")
	{
		string invoiceInfo = ms_->GetValueFromMapByKey("invoiceTitle",true) + ms_->GetValueFromMapByKey("recipients",true) + ms_->GetValueFromMapByKey("address",true);
	}
	
	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"appname=" + arrivalTime + "\r\n" + 
		"fileurl=" + number_ + "\r\n" +
		"subject=" + type_ + "\r\n" + 
		"platform=手机App\r\n" +
		"state=包车\r\n";
	WriteIndexFile();
	return 1;
}


//手机约车
int QiChePiao::MobileYueChe(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//用户id
	name_ = ms_->GetValueFromMapByKey("userId",true);
	//电话
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//出发地
	start_ = ms_->GetValueFromMapByKey("getOnPlace",true);
	//目的地
	end_ = ms_->GetValueFromMapByKey("getOffPlace",true);
    //出发时间
	time_ = ms_->GetValueFromMapByKey("startTime",true);
	//BUS id
	number_ = ms_->GetValueFromMapByKey("busId",true);

	if (name_.empty() || start_.empty() || end_.empty() || start_.empty() || time_.empty())
	{
		return 0;
	}

	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"sex=" + number_ + "\r\n" +
		"platform=手机App\r\n" +
		"state=手机约租车\r\n";
	
	WriteIndexFile();
	
	return 1;

}
//手机快线 (包含)
int QiChePiao::MobileTrip(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
 
	//取票人信息
	//用户名
	name_ = ms_->GetValueFromMapByKey("userName",true);
	//手机号
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//时间
	time_ = ms_->GetValueFromMapByKey("startTime",true);
	//开始地址
	start_ = ms_->GetValueFromMapByKey("startPlace",true);
	//到达地址
	end_ = ms_->GetValueFromMapByKey("endPlace",true);
    //人数
	number_ = ms_->GetValueFromMapByKey("count",true);

	if (name_.empty() || tel_.empty() || time_.empty() || start_.empty() || end_.empty())
	{
		return 0;
	}

	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"fileurl=" + number_ + "\r\n" +
		"platform=手机App\r\n" +
		"state=手机快线\r\n";
	WriteIndexFile();
	return 1;
}
