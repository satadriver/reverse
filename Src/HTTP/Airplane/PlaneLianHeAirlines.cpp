
#include "StdAfx.h"

#include "PlaneLianHeAirlines.h"

plane::LianHeAirlines::LianHeAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::LianHeAirlines::~LianHeAirlines()
{
	
}

// ID_CARD 身份证，PASSPORT 护照，OTHER 其他证件
string plane::LianHeAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "ID_CARD"){idTypeTrue = "身份证";}
	if (idType == "PASSPORT"){idTypeTrue = "护照";}
	if (idType == "OTHER"){idTypeTrue = "其他证件";}
	return idTypeTrue;
}

int plane::LianHeAirlines::IsLianHeAirlines(HttpSession *http_session)
{
	//行程预订
	if((http_session->m_Requestline.m_Host.Find("www.flycua.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/otabooking/flight-search!doFlightSearch.shtml")!=-1))
	{
		return kpcticket;
	}
	return 0;
}

int plane::LianHeAirlines::Process(const char *packet,int action)
{
	//行程预订
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	return 1;
}

// 行程预订
int plane::LianHeAirlines::pcticket(const char *packet)
{
	string src_data(packet);
	string tookit = "earchCond=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit) + tookit.length());
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));	
	//提取json
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		//类型
		triptype_ = value["tripType"].asString();
		CString str;
		//成年人数
		int adt = value["adtCount"].asInt();
		str.Format("%d",adt);
		adtCount = str.GetBuffer(0);
		//儿童人数
		int chd = value["chdCount"].asInt();
		str.Format("%d",chd);
		chdCount = str.GetBuffer(0);
		//婴儿人数
		int inf = value["infCount"].asInt();
		str.Format("%d",inf);
		infCount = str.GetBuffer(0);
		//
		Json::Value segmentList = value["segmentList"];
		int one = 0,two = 1;
		//出发地
		deptCdTxt = segmentList[one]["deptCd"].asString();
		//目的地
		arrCdTxt = segmentList[one]["arrCd"].asString();
		//出发日期
		deptDt = segmentList[one]["deptDt"].asString();
		//目的日期 
		deptDtRe = segmentList[two]["deptDt"].asString();
		if ("OW" == triptype_)
		{
			output_text_ =
				"netmark=" + adtCount + "\r\n" +   //成年人数    
				"onick=" + chdCount + "\r\n" +	   //儿童人数    
				"sex=" + infCount + "\r\n" +	   //婴儿人数 
				"nick=" + deptCdTxt + "\r\n" +	   //出发地
				"msg=" + arrCdTxt + "\r\n" +	   //目的地 
				"servername=" + deptDt + "\r\n" +  //出发日期
				"state=国内单程查询\r\n";
			WriteIndexFile();
		}
		else if ("RT" == triptype_)
		{
			output_text_ =
				"netmark=" + adtCount + "\r\n" +   //成年人数    
				"onick=" + chdCount + "\r\n" +	   //儿童人数    
				"sex=" + infCount + "\r\n" +	   //婴儿人数 
				"nick=" + deptCdTxt + "\r\n" +	   //出发地
				"msg=" + arrCdTxt + "\r\n" +	   //目的地 
				"servername=" + deptDt + "\r\n" +  //出发日期
				"appname=" + deptDtRe + "\r\n" +   //返程日期 
				"state=国内往返查询\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}
