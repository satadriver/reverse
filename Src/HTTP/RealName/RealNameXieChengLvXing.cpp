
#include "StdAfx.h"

#include "RealNameXieChengLvXing.h"

realname::XieChengLvXing::XieChengLvXing(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "携程旅行";
}

realname::XieChengLvXing::~XieChengLvXing()
{
	
}

int realname::XieChengLvXing::IsXieChengLvXing(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/DomesticBook/Tool/AjaxResidentBlackList.aspx") != -1 ) )	
	{
		return kBookHotel;
	}

	if( ( http_session->m_Requestline.m_Host.Find("trains.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/TrainBooking/CommitOrder.aspx") != -1 ) )	
	{
		return kBookTrainTicket;
	}

	if( ( http_session->m_Requestline.m_Host.Find("flights.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Book/Review") != -1 ) )	
	{
		return kBookPlaneTicket;
	}
	return 0;
}

int realname::XieChengLvXing::Process(const char *packet,int action)
{

	if (action == kBookHotel)
	{
		return BookHotel(packet);
	}

	if (action == kBookPlaneTicket)
	{
		return BookPlaneTicket(packet);
	}

	if (action == kBookTrainTicket)
	{
		return BookTrainTicket(packet);
	}

	return 0;
}


int realname::XieChengLvXing::BookHotel(const char *packet)
{	
	string src_data(packet);
	/*
	[{"elemId":"txtContactMobilePhone","checkValue":"18660156478","checkType":"MobilePhone"},{"elemId":"J_resident_1","checkValue":"郑大钱","checkType":"Name"},{"elemId":"J_resident_2","checkValue":"郑小钱","checkType":"Name"}]
	*/

	ms_->Replace(src_data,"[{","&");
	ms_->Replace(src_data,"}]","&");
	ms_->Replace(src_data,"},{","&");
	ms_->Replace(src_data,"\"","");
	int retval = 0;
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;
	while(1)
	{
		item_end_pos = src_data.find("&",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = src_data.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = src_data.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + strlen("&");
		}

		ms_->SplitStringToMap(item,',',':');
		string tmp = ms_->GetValueFromMapByKey("elemId");
		if (tmp == "txtContactMobilePhone")
		{
			phone_ = ms_->GetValueFromMapByKey("checkValue");
		}
		if (tmp == "J_resident_1")
		{
			name_ = ms_->GetValueFromMapByKey("checkValue",true);
		}

		if (name_.empty() || phone_.empty())
		{
			continue;
		}
		
		output_text_ = GetOutputText();
		WriteIndexFile();
		return 1;
		
	}

	return 0;
}

int realname::XieChengLvXing::BookTrainTicket(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	name_ = ms_->GetValueFromMapByKey("hidContactName",true);
	phone_ = ms_->GetValueFromMapByKey("hidContactMobile");
	id_ = ms_->GetValueFromMapByKey("passengerCardID[]");
	if (name_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

int realname::XieChengLvXing::BookPlaneTicket(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=',"\"");
	name_ = ms_->GetValueFromMapByKey("contactName",true);
	phone_ = ms_->GetValueFromMapByKey("contactPhone");

	string passager_info = ms_->GetValueFromMapByKey("PassengerInfoList");

	ms_->Replace(passager_info,"[{","&");
	ms_->Replace(passager_info,"}]","&");
	ms_->Replace(passager_info,"\"","");
	ms_->SplitStringToMap(passager_info,',',':');
	id_ = ms_->GetValueFromMapByKey("CardNo");

	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}