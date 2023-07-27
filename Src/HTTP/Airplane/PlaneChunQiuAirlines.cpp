
#include "StdAfx.h"

#include "PlaneChunQiuAirlines.h"

plane::ChunQiuAirlines::ChunQiuAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChunQiuAirlines::~ChunQiuAirlines()
{
	
}

// NI 身份证，PP 护照，ORI 回乡证，MIL 军官证，OTHER 其他，HTPP 港澳通行证，BR 出生日期
string plane::ChunQiuAirlines::GetIdType(string idType)
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

int plane::ChunQiuAirlines::IsChunQiuAirlines(HttpSession *http_session)
{
	//行程预订
	if((http_session->m_Requestline.m_Host.Find("flights.ch.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/default/SearchByTime")!=-1))
	{
		return kpcticket;
	}
	return 0;
}

int plane::ChunQiuAirlines::Process(const char *packet,int action)
{
	//行程预订
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	return 1;
}
// 行程预订
int plane::ChunQiuAirlines::pcticket(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	string IfRet = ms_->GetValueFromMapByKey("IfRet",true);
	//单程
	if (IfRet=="false")
	{
		deptCdTxt = ms_->GetValueFromMapByKey("OriCity",true);
		arrCdTxt = ms_->GetValueFromMapByKey("DestCity",true);
		deptDt =ms_->GetValueFromMapByKey("FDate",true);
		adtCount = ms_->GetValueFromMapByKey("ANum",true);
		chdCount = ms_->GetValueFromMapByKey("CNum",true);
		infCount = ms_->GetValueFromMapByKey("INum",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" +		//成年人数
			"onick=" +	chdCount + "\r\n" +		//儿童人数
			"sex=" +	infCount + "\r\n" +		//婴儿人数
			"nick=" +	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
			"servername=" + deptDt + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	else if(IfRet == "true")
	{
		deptCdTxt = ms_->GetValueFromMapByKey("OriCity",true);
		arrCdTxt = ms_->GetValueFromMapByKey("DestCity",true);
		deptDt =ms_->GetValueFromMapByKey("FDate",true);
		string arrdt = ms_->GetValueFromMapByKey("RetFDate",true);
		adtCount = ms_->GetValueFromMapByKey("ANum",true);
		chdCount = ms_->GetValueFromMapByKey("CNum",true);
		infCount = ms_->GetValueFromMapByKey("INum",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty() || arrdt.empty())
		{
			return 0;
		}

		output_text_ =
			"netmark=" +adtCount + "\r\n" +		//成年人数
			"onick=" +	chdCount + "\r\n" +		//儿童人数
			"sex=" +	infCount + "\r\n" +		//婴儿人数
			"nick=" +	deptCdTxt + "\r\n" +	//出发地
			"msg=" + 	arrCdTxt + "\r\n" +		//目的地 
			"servername=" + deptDt + "\r\n" +	//出发日期
			"appname=" +arrdt +"\r\n" +			//往返日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}