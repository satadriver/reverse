
#include "StdAfx.h"

#include "PlaneShenZhenAirlines.h"

plane::ShenZhenAirlines::ShenZhenAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ShenZhenAirlines::~ShenZhenAirlines()
{
	
}

// 0 身份证，1 护照，9 其他证件
string plane::ShenZhenAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "0"){idTypeTrue = "身份证";}
	if (idType == "1"){idTypeTrue = "护照";}
	if (idType == "9"){idTypeTrue = "其他证件";}
	return idTypeTrue;
}

int plane::ShenZhenAirlines::IsShenZhenAirlines(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.shenzhenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/szair_B2C/login/login.action")!=-1) )
	{
		return kpclogin;
	}
	//行程预订
	if((http_session->m_Requestline.m_Host.Find("www.shenzhenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/szair_B2C/flightSearch.action")!=-1))
	{
		return kpcticket;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("www.shenzhenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/szair_B2C/saveOrder.action")!=-1))
	{
		return kpcorder;
	}
	return 0;
}

int plane::ShenZhenAirlines::Process(const char *packet,int action)
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
	//登录
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//登录
int plane::ShenZhenAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	ms_->SplitStringToMap(src_data,'&','=');
	username = ms_->GetValueFromMapByKey("condition.user_id",true);
	password = ms_->GetValueFromMapByKey("condition.password",true);
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
int plane::ShenZhenAirlines::pcticket(const char *packet)
{
	string referer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	referer = ms_->UrlDecode(referer);
	ms_->Replace(referer,"?","&");
	ms_->SplitStringToMap(referer,"&","=");

	deptCdTxt = ms_->GetValueFromMapByKey("orgCity",true);
	arrCdTxt = ms_->GetValueFromMapByKey("dstCity",true);
	deptDt = ms_->GetValueFromMapByKey("orgDate",true);
	deptDtRe = ms_->GetValueFromMapByKey("dstDate",true);
	//判断单程 往返程
	string hcType = ms_->GetValueFromMapByKey("hcType",true);

	if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
	{
		return 0;
	}
	if (hcType == "DC")
	{
		//单程
		output_text_ =
			"nick=" +	deptCdTxt + "\r\n" +  //出发地
			"msg=" + 	arrCdTxt + "\r\n" +   //目的地 
			"servername=" + deptDt + "\r\n" + //出发日期
			"state=国内单程查询\r\n";
		WriteIndexFile();
	}
	if (hcType == "WF")
	{
		//往返
		string desdata = ms_->GetValueFromMapByKey("condition.dstDate",true);
		//单程
		output_text_ =
			"nick=" +	deptCdTxt + "\r\n" +   //出发地
			"msg=" + 	arrCdTxt + "\r\n" +    //目的地 
			"servername=" + deptDt + "\r\n" +  //出发日期
			"appname=" +  desdata + "\r\n" +   //返程日期
			"state=国内往返查询\r\n";
		WriteIndexFile();
	}
	return 1;
}
//提交订单
int plane::ShenZhenAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("passengerJson=")+strlen("passengerJson="));
	//提起json
	Json::Value value;
	Json::Reader reader;
	Json::Value passengerList;
	if (reader.parse(src_data,value))
	{
		passengerList = value["passengerList"];
		username = value["contactName"].asString();
		mobile = value["contactMobile"].asString();
		email = value["contactEmail"].asString();
	}
	if (username .empty() || mobile.empty())
	{
		return 0;
	}
	//乘客信息
	string psgrName,certNo,certType;
	for (int i = 0; i<passengerList.size();i++)
	{
		psgrName = passengerList[i]["psgrName"].asString();
		certNo = passengerList[i]["certNo"].asString();
		certType = passengerList[i]["certType"].asString();
		if (psgrName .empty() || certNo.empty())
		{
			return 0;
		}
		certType = this->GetIdType(certType);
		output_text_ =
			"mid=" + 	username + "\r\n" +		//联系人姓名
			"oid=" + 	mobile + "\r\n" +		//联系人电话
			"imsi=" + 	email + "\r\n" +		//联系人邮箱
			"msgtype=" + psgrName + "\r\n" +	//乘客姓名
			"webname=" + certType + "\r\n" +	//乘客证件类型
			"subject=" + certNo + "\r\n" +		//乘客证件号
			"state=提交订单\r\n";
			WriteIndexFile();
	}
	return 1;
}
