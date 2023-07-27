
#include "StdAfx.h"

#include "XieChengLvXing.h"

XieChengLvXing::XieChengLvXing()
{
	software_ = "携程旅行";
}

XieChengLvXing::~XieChengLvXing()
{
	
}

int XieChengLvXing::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/DomesticBook/Tool/AjaxResidentBlackList.aspx") != -1 ) )	
	{
		return kBookHotelCW;
	}

	if( ( http_session->m_Requestline.m_Host.Find("trains.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/TrainBooking/CommitOrder.aspx") != -1 ) )	
	{
		return kBookTrainTicketCW;
	}

	if( ( http_session->m_Requestline.m_Host.Find("flights.ctrip.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Book/Review") != -1 ) )	
	{
		return kBookPlaneTicketCW;
	}
	return 0;
}

int XieChengLvXing::Process(const char *packet,int action)
{

	if (action == kBookHotelCW)
	{
		return BookHotelCW(packet);
	}

	if (action == kBookPlaneTicketCW)
	{
		return BookPlaneTicketCW(packet);
	}

	if (action == kBookTrainTicketCW)
	{
		return BookTrainTicketCW(packet);
	}

	return 0;
}


int XieChengLvXing::BookHotelCW(const char *packet)
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
		string phone;
		string name;
		if (tmp == "txtContactMobilePhone")
		{
			phone = ms_->GetValueFromMapByKey("checkValue");
		}
		if (tmp == "J_resident_1")
		{
			name = ms_->GetValueFromMapByKey("checkValue",true);
		}

		if (name.empty() || phone.empty())
		{
			continue;
		}
		
		output_text_ = 
			"user=" + name + "\r\n" +
			"webname=" + phone + "\r\n" +
			"state=" + software_ + "\r\n";

		WriteIndexFile();
		return 1;
		
	}

	return 0;
}

int XieChengLvXing::BookTrainTicketCW(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string name = ms_->GetValueFromMapByKey("hidContactName",true);
	string phone = ms_->GetValueFromMapByKey("hidContactMobile");
	string id = ms_->GetValueFromMapByKey("passengerCardID[]");

	if (name.empty() ||  phone.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + name + "\r\n" +
		"to=" + id + "\r\n" +
		"webname=" + phone + "\r\n" +
		"state=" + software_ + "\r\n";

	WriteIndexFile();
	return 1;
}

int XieChengLvXing::BookPlaneTicketCW(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=',"\"");
	string name = ms_->GetValueFromMapByKey("contactName",true);
	string phone = ms_->GetValueFromMapByKey("contactPhone");

	string passager_info = ms_->GetValueFromMapByKey("PassengerInfoList");

	ms_->Replace(passager_info,"[{","&");
	ms_->Replace(passager_info,"}]","&");
	ms_->Replace(passager_info,"\"","");
	ms_->SplitStringToMap(passager_info,',',':');
	string id = ms_->GetValueFromMapByKey("CardNo");

	
	output_text_ = 
		"user=" + name + "\r\n" +
		"to=" + id + "\r\n" +
		"webname=" + phone + "\r\n" +
		"state=" + software_ + "\r\n";

	WriteIndexFile();
	return 1;
}