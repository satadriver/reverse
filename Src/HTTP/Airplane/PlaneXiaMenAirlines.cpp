
#include "StdAfx.h"
#include "PlaneXiaMenAirlines.h"

plane::XiaMenAirLines::XiaMenAirLines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::XiaMenAirLines::~XiaMenAirLines()
{
	
}
		
int plane::XiaMenAirLines::IsXiaMenAirLines(HttpSession *http_session)
{	
	//国内单程 往返 查询
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/book/findFlights.json")!=-1)) 
	{										
		return kpcticketcn;
	}	
	//国际单程 往返 查询
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/intlbook/intlFindFlights.json")!=-1))
	{
		return kpcticketint;
	}	
	//国内单程 往返 提交订单
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/book/view.action")!=-1) )
	{
		return kpcordercn;
	}
	//国际单程 往返 提交订单
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/intlbook/intlConfirmOrder.action")!=-1) )
	{
		return kpcorderint;
	}
	return 0;
}

int plane::XiaMenAirLines::Process(const char *packet,int action)
{
	//国内单程 往返 查询
	if (action == kpcticketcn)
	{
		return pcticketcn(packet);
	}
	//国际单程 往返 查询
	if(action == kpcticketint)
	{
		return pcticketint(packet);
	}
	//国内单程 往返 提交订单
	if (action == kpcordercn)
	{
		return pcordercn(packet);
	}
	//国际单程 往返 提交订单
	if (action == kpcorderint)
	{
		return pcorderint(packet);
	}
	return 1;
}

//国内单程 往返 查询
int plane::XiaMenAirLines::pcticketcn(const char *packet)
{	
	//从url中提取数据
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');
	//单程 往返 
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (tripType == "0")
	{
		//出发地(航空码)
		string orgCity = ms_->GetValueFromMapByKey("orgCity",true);
        //目的地(航空码)
		string dstCity = ms_->GetValueFromMapByKey("dstCity",true);
		//出发日期
		string takeoffDate = ms_->GetValueFromMapByKey("takeoffDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //出发地
			"msg=" + 	dstCity + "\r\n" +    //目的地 
			"servername=" + takeoffDate + "\r\n" + //出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	if (tripType == "1")
	{
		//出发地(航空码)
		string orgCity = ms_->GetValueFromMapByKey("orgCity",true);
        //目的地(航空码)
		string dstCity = ms_->GetValueFromMapByKey("dstCity",true);
		//出发日期
		string takeoffDate = ms_->GetValueFromMapByKey("takeoffDate",true);
		//返程日期
		string returnDate = ms_->GetValueFromMapByKey("returnDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //出发地
			"msg=" + 	dstCity + "\r\n" +    //目的地 
			"servername=" + takeoffDate + "\r\n" + //出发日期
			"appname=" + 	returnDate + "\r\n" + //返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}

//国际单程 往返 查询
int plane::XiaMenAirLines::pcticketint(const char *packet)
{	
	//从refer中提取数据
	string refer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	ms_->Replace(refer,"?","&");
	refer = ms_->UTF8ToGBK(UrlDecode(refer));
	ms_->SplitStringToMap(refer,'&','=');
	//单程 往返 
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (tripType == "0")
	{
	//乘客信息 0,1;1,1
		string psgrInfo = ms_->GetValueFromMapByKey("psgrInfo",true);
        //成人人数
		string adultTemp = psgrInfo.substr(0,psgrInfo.find(";"));
		string adult = adultTemp.substr(adultTemp.find(",") + strlen(","));
        //儿童人数
		string childTemp = psgrInfo.substr(psgrInfo.find(";"));
		string child = childTemp.substr(childTemp.find(",") + strlen(","));

	//航班信息 XMN,HKG,2016-12-09
		string queryFlightInfo = ms_->GetValueFromMapByKey("queryFlightInfo",true);
		//出发地(航空码)
		string orgCity = queryFlightInfo.substr(0,queryFlightInfo.find(","));	
        //目的地(航空码)
		string dstCity =  queryFlightInfo.substr(queryFlightInfo.find(",") + strlen(","),queryFlightInfo.rfind(",") - queryFlightInfo.find(",") - strlen(","));
		//出发日期
		string takeoffDate = queryFlightInfo.substr(queryFlightInfo.rfind(",") + strlen(","));

		output_text_ =
			"netmark=" + 	adult + "\r\n" +	//成年人数    
			"onick=" + 	child + "\r\n" +		//儿童人数 
			"nick=" + 	orgCity + "\r\n" +		//出发地
			"msg=" + 	dstCity + "\r\n" +		//目的地 
			"servername=" + takeoffDate + "\r\n" +	//出发日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	
	if (tripType == "1")
	{
	//乘客信息 0,1;1,1
		string psgrInfo = ms_->GetValueFromMapByKey("psgrInfo",true);
        //成人人数
		string adultTemp = psgrInfo.substr(0,psgrInfo.find(";"));
		string adult = adultTemp.substr(adultTemp.find(",") + strlen(","));
        //儿童人数
		string childTemp = psgrInfo.substr(psgrInfo.find(";"));
		string child = childTemp.substr(childTemp.find(",") + strlen(","));
		
		//航班信息 AMS,HGH,2016-12-09;HGH,AMS,2016-12-10
		string queryFlightInfo = ms_->GetValueFromMapByKey("queryFlightInfo",true);
		string queryFlightInfoOw = queryFlightInfo.substr(0,queryFlightInfo.find(";"));
		string queryFlightInfoRe = queryFlightInfo.substr(queryFlightInfo.rfind(";"));
		//出发地(航空码)
		string orgCity = queryFlightInfoOw.substr(0,queryFlightInfoOw.find(","));	
        //目的地(航空码)
		string dstCity =  queryFlightInfoOw.substr(queryFlightInfoOw.find(",") + strlen(","),queryFlightInfoOw.rfind(",") - queryFlightInfoOw.find(",") - strlen(","));
		//出发日期
		string takeoffDate = queryFlightInfoOw.substr(queryFlightInfoOw.rfind(",") + strlen(","));
		//返程日期
		string returnDate = queryFlightInfoRe.substr(queryFlightInfoRe.rfind(",") + strlen(","));	
		output_text_ =
			"netmark=" + adult + "\r\n" +		//成年人数    
			"onick=" + 	child + "\r\n" +		//儿童人数 
			"nick=" + 	orgCity + "\r\n" +		//出发地
			"msg=" + 	dstCity + "\r\n" +		//目的地 
			"servername=" + takeoffDate + "\r\n" +	//出发日期
			"appname=" + returnDate + "\r\n" +		//返程日期
			"state=查询\r\n";
		WriteIndexFile();
	}
	return 1;
}


//国内单程 往返 提交订单
int plane::XiaMenAirLines::pcordercn(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//获取乘客的人数
	string temp = src_data.substr(src_data.rfind("passengers"));
	string temp2 = temp.substr(0,temp.find("="));
	string People = ms_->GetValueBetweenBeginEnd(temp2,"[","]");
	int PassengerCount = atoi(People.c_str());
	//单程 往返

    //联系人姓名
	string contactName = ms_->GetValueFromMapByKey("contact.name",true);
	//联系人电话
	string contactMobile = ms_->GetValueFromMapByKey("contact.mobile",true);
	//联系人邮箱
	string contactEmail = ms_->GetValueFromMapByKey("contact.email",true);
	if (contactEmail.empty())
	{
		contactEmail = "Null";
	}
	for(int i =0;i<= PassengerCount;i++)
	{
		CString str;
		str.Format("%d",i);
		string strTemp = str.GetBuffer(0);
		//乘客姓名
		string publicTemp = "passengers";
		string passengerNameTemp = publicTemp + "[" + strTemp + "]" + ".name";
		string passengerName = ms_->GetValueFromMapByKey(passengerNameTemp,true);
		//乘客类型 0 成人 1 儿童
		string remarkTemp = publicTemp + "[" + strTemp + "]" + ".type";
		string remark = ms_->GetValueFromMapByKey(remarkTemp,true);

		string cateType,cateNo;
		if (remark == "0")
		{
			//乘客证件类型
			string cateTypeTemp = publicTemp + "[" + strTemp + "]" + ".idType";
			cateType = ms_->GetValueFromMapByKey(cateTypeTemp,true);
			//乘客证件号
			string cateNoTemp = publicTemp + "[" + strTemp + "]" + ".idNo";
			cateNo = ms_->GetValueFromMapByKey(cateNoTemp,true);
		}
		if (remark == "1")
		{
			//乘客证件类型
			string cateTypeTemp = publicTemp + "[" + strTemp + "]" + ".idType";
			cateType = ms_->GetValueFromMapByKey(cateTypeTemp,true);
			//乘客证件号
			
			string yearTemp = publicTemp + "[" + strTemp + "]" + ".year";
			string monthTemp = publicTemp + "[" + strTemp + "]" + ".month";
			string dayTemp = publicTemp + "[" + strTemp + "]" + ".day";
			cateNo = ms_->GetValueFromMapByKey(yearTemp,true) + "-" + ms_->GetValueFromMapByKey(monthTemp,true) + "-" + ms_->GetValueFromMapByKey(dayTemp,true);
		}
		//乘客电话
		string mobilePhoneTemp = publicTemp + "[" + strTemp + "]" + ".mobile";
		string mobilePhone = ms_->GetValueFromMapByKey(mobilePhoneTemp,true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//联系人姓名
			"oid=" + 	contactMobile + "\r\n" +	//联系人电话
			"imsi=" + 	contactEmail + "\r\n" +		//联系人邮箱
			"msgtype=" + passengerName + "\r\n" +	//乘客姓名
			"clientmsg=" + mobilePhone + "\r\n" +	//乘客电话
			"webname=" + cateType + "\r\n" +		//乘客证件类型
			"subject=" + cateNo + "\r\n" +			//乘客证件号
			"state=提交订单\r\n";
		WriteIndexFile();
	}
	return 1;
}

//国际单程 往返 提交订单
int plane::XiaMenAirLines::pcorderint(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("person"));
	ms_->SplitStringToMap(src_data,'&','=');
	//单程  往返 
	//联系人姓名
	string contactName = ms_->GetValueFromMapByKey("contactName",true);
	//联系人电话
	string contactMobile = ms_->GetValueFromMapByKey("contactMobile",true);
	//联系人邮箱
	string contactEmail = ms_->GetValueFromMapByKey("contactEmail",true);
	if (contactEmail.empty())
	{
		contactEmail = "Null";
	}
	//乘客人数
	int pos = src_data.rfind("person");
	int tos = src_data.find("&",pos);
	string passengerCountTemp = src_data.substr(pos,tos - pos);
	string passengerCount = ms_->GetValueBetweenBeginEnd(passengerCountTemp,"person","=");
	int nCount = atoi(passengerCount.c_str()); 
    nCount++;
	string passengerInfo = src_data.substr(src_data.find("person"),src_data.find("&contactName") - src_data.find("person"));
	//乘客信息 待优化 person0= ,mobile= ,person1= mobile=,	
	while (nCount) 
	{
		if (nCount > 1)
		{
			string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("&person"));	 
			//乘客姓名
			string passengerName = ms_->GetValueBetweenBeginEnd(passengerInfo,"familyName=","&") + 
								   ms_->GetValueBetweenBeginEnd(passengerInfo,"givenName=","&");
			//乘客证件类型
			string cateType = "护照";
			//乘客证件号
			string cateNo = ms_->GetValueBetweenBeginEnd(passengerInfo,"passNumber=","&");
			//乘客电话
			string mobilePhone = ms_->GetValueBetweenBeginEnd(passengerInfo,"mobile=","&");
			if (mobilePhone.empty())
			{
				mobilePhone = "Null";
			}
			output_text_ =
				"mid=" + 	contactName + "\r\n" +		//联系人姓名
				"oid=" + 	contactMobile + "\r\n" +	//联系人电话
				"imsi=" + 	contactEmail + "\r\n" +		//联系人邮箱
				"msgtype=" + passengerName + "\r\n" +	//乘客姓名
				"clientmsg=" + mobilePhone + "\r\n" +	//乘客电话
				"webname=" + cateType + "\r\n" +		//乘客证件类型
				"subject=" + cateNo + "\r\n" +			//乘客证件号
				"state=提交订单\r\n";
			WriteIndexFile();
		}	
		if (passengerInfo.find("&person") != string::npos)
		{
			passengerInfo = passengerInfo.substr(passengerInfo.find("&person") + strlen("&"));
		}
		if (nCount ==1 )
		{
			//乘客姓名
			string passengerName = ms_->GetValueBetweenBeginEnd(passengerInfo,"familyName=","&") + 
								   ms_->GetValueBetweenBeginEnd(passengerInfo,"givenName=","&");
			//乘客证件类型
			string cateType = "护照";
			//乘客证件号
			string cateNo = ms_->GetValueBetweenBeginEnd(passengerInfo,"passNumber=","&");
			//乘客电话
			string mobilePhone = passengerInfo.substr(passengerInfo.find("mobile=") + strlen("mobile="));
			if (mobilePhone.empty())
			{
				mobilePhone = "Null";
			}
			output_text_ =
				"mid=" + 	contactName + "\r\n" +		//联系人姓名
				"oid=" + 	contactMobile + "\r\n" +	//联系人电话
				"imsi=" + 	contactEmail + "\r\n" +		//联系人邮箱
				"msgtype=" + passengerName + "\r\n" +	//乘客姓名
				"clientmsg=" + mobilePhone + "\r\n" +	//乘客电话
				"webname=" + cateType + "\r\n" +		//乘客证件类型
				"subject=" + cateNo + "\r\n" +			//乘客证件号
				"state=提交订单\r\n";
			WriteIndexFile();
		}
		nCount--;
	}
	return 1;
}
