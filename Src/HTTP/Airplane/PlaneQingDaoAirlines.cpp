
#include "StdAfx.h"

#include "PlaneQingDaoAirlines.h"

plane::QingDaoAirlines::QingDaoAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::QingDaoAirlines::~QingDaoAirlines()
{
	
}

// 0 身份证，1 护照，2 军官证 3 回乡证 4 台胞证 5港澳通行证 6 警官证 7 士兵证
string plane::QingDaoAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "0"){idTypeTrue = "身份证";}
	if (idType == "1"){idTypeTrue = "护照";}
	if (idType == "2"){idTypeTrue = "军官证";}
	if (idType == "3"){idTypeTrue = "回乡证";}
	if (idType == "4"){idTypeTrue = "台胞证";}
	if (idType == "5"){idTypeTrue = "港澳通行证";}
	if (idType == "6"){idTypeTrue = "警官证";}
	if (idType == "7"){idTypeTrue = "士兵证";}
	return idTypeTrue;
}

int plane::QingDaoAirlines::IsQingDaoAirlines(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.qdairlines.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ffpService.do")!=-1) )
	{
		return kpclogin;
	}
	//行程预订
	if((http_session->m_Requestline.m_Host.Find("www.qdairlines.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/FlightSearch.do")!=-1))
	{
		return kpcticket;
	}
	//提交订单
	if((http_session->m_Requestline.m_Host.Find("www.qdairlines.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/PaxInfoInput.do")!=-1))
	{
		return kpcorder;
	}
	return 0;
}

int plane::QingDaoAirlines::Process(const char *packet,int action)
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
int plane::QingDaoAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	username = ms_->GetValueFromMapByKey("user",true);
	password = ms_->GetValueFromMapByKey("pwd",true);
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
int plane::QingDaoAirlines::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	adtCount = ms_->GetValueFromMapByKey("adt",true);
	chdCount = ms_->GetValueFromMapByKey("chd",true);
	string journeyType = ms_->GetValueFromMapByKey("journeyType",true);
	if (journeyType == "OW")
	{
		//单程
		deptDt = ms_->GetValueFromMapByKey("depDate",true);
		deptCdTxt = ms_->GetValueFromMapByKey("orgCityInput",true);
		arrCdTxt = ms_->GetValueFromMapByKey("destCityInput",true);
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" + //成年人数
			"onick=" +	chdCount + "\r\n" + //儿童人数
			"nick=" +	deptCdTxt + "\r\n" + //出发地
			"msg=" + 	arrCdTxt + "\r\n" + //目的地 
			"servername=" + deptDt + "\r\n" + //出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (journeyType == "RT")
	{
		deptDt = ms_->GetValueFromMapByKey("depDate",true);
		deptCdTxt = ms_->GetValueFromMapByKey("orgCityInput",true);
		arrCdTxt = ms_->GetValueFromMapByKey("destCityInput",true);
		string retDate = ms_->GetValueFromMapByKey("retDate",true);
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty() || retDate.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" + //成年人数
			"sex=" +	chdCount + "\r\n" + //儿童人数
			"nick=" +	deptCdTxt + "\r\n" + //出发地
			"msg=" + 	arrCdTxt + "\r\n" + //目的地 
			"servername=" + deptDt + "\r\n" + //出发日期
			"appname=" +  retDate + "\r\n" + //返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}
//提交订单
int plane::QingDaoAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//成人数量
	string adult_num = ms_->GetValueFromMapByKey("adult_num",true);
	//儿童数量
	string child_num = ms_->GetValueFromMapByKey("child_num",true);
	int adnum = atoi(adult_num.c_str());
	int chidnum = atoi(child_num.c_str());
	//获取联系人信息
	string con_ming = ms_->GetValueFromMapByKey("con_ming",true);
	string con_mobile = ms_->GetValueFromMapByKey("con_mobile",true);
	string con_email = ms_->GetValueFromMapByKey("con_email",true);
	if (con_ming.empty() || con_mobile.empty() || con_email.empty())
	{
		return 0;
	}
	//成年人乘客
	for (int i = 0;i<adnum;i++)
	{
		//姓名
		string adult_ming;
		//证件号
		string adult_pspt;
		//证件类型
		string adultIdType;
		stringstream ss;
		stringstream sss;
		stringstream ssss;

		ss<<"adult_ming"<<i;
		sss<<"adult_pspt"<<i;
		ssss<<"adultIdType"<<i;
		adult_ming = ms_->GetValueFromMapByKey(ss.str(),true);
		adult_pspt = ms_->GetValueFromMapByKey(sss.str(),true);
		adultIdType = ms_->GetValueFromMapByKey(ssss.str(),true);
		adultIdType = this->GetIdType(adultIdType);
		output_text_ =
			"mid=" + 	con_ming + "\r\n" +		//联系人姓名
			"oid=" + 	con_mobile + "\r\n" +	//联系人电话
			"imsi=" +  con_email + "\r\n" +		//联系人邮箱
			"msgtype=" + adult_ming + "\r\n" +	//乘客姓名
			"webname=" + adult_pspt + "\r\n" +	//乘客证件类型
			"subject=" + adultIdType + "\r\n" + //乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();

	}
	//婴儿乘客
	for(int j=0;j<chidnum;j++)
	{
		stringstream ss;
		stringstream sss;
		stringstream ssss;

		string child_ming;
		string child_pspt;
		string chileType;

		ss<<"child_ming"<<j;
		sss<<"child_pspt"<<j;
		ssss<<"childIdType"<<j;
		child_ming = ms_->GetValueFromMapByKey(ss.str(),true);
		child_pspt = ms_->GetValueFromMapByKey(sss.str(),true);
		chileType = ms_->GetValueFromMapByKey(ssss.str(),true);
		chileType = this->GetIdType(chileType);
		if (child_ming.empty() || child_pspt.empty())
		{
			return 0;
		}
		output_text_ =
			"mid=" + 	con_ming + "\r\n" +		//联系人姓名
			"oid=" + 	con_mobile + "\r\n" +	//联系人电话
			"imsi=" +  con_email + "\r\n" +		//联系人邮箱
			"msgtype=" + child_ming + "\r\n" +	//乘客姓名
			"webname=" + child_pspt + "\r\n" +	//乘客证件类型
			"subject=" + chileType + "\r\n" +	//乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}
