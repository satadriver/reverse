// Xiecheng.cpp: implementation of the Xiecheng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Xiecheng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Xiecheng::Xiecheng(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

Xiecheng::~Xiecheng()
{

}

int Xiecheng::Is(HttpSession *http_session)
{
	////////////////////////////
/*
	FILE* f = fopen("xiecheng_sp_airplane.txt", "rb");
	int len = filelength(fileno(f));
	char* p = new char[len];
	memset(p, 0, len);
	fread(p, 1, len, f);
	fclose(f);
	Xiecheng tn(NULL, "");
	tn.Process(p, PC_AIRPLANE);
*/	
	////////////////////////////
	
	// PC 汽车票
	if((http_session->m_Requestline.m_Host.Find("bus.ctrip.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?param=/buy/ajaxCommitOrder")!=-1))
	{
		return PC_BUS;
	}
	// PC 火车票
	if((http_session->m_Requestline.m_Host.Find("trains.ctrip.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/TrainBooking/InputPassengers.aspx")!=-1))
	{
		return PC_TRAIN;
	}
	// PC 飞机票
	if((http_session->m_Requestline.m_Host.Find("flights.ctrip.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Book/Book")!=-1))
	{
		return PC_AIRPLANE;
	}
	return 0;
}

int Xiecheng::Process(const char *packet,int action)
{
	if (action == PC_TRAIN)
	{
		string sContent = ms_->UrlDecode(packet);
		char* cur_pos = (char*)sContent.c_str();
		tel_     = middle_text(sContent.c_str(), "hidContactMobile=", "&", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"TrainName\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"DepartureDate\":\"", "\"", &cur_pos) + " " + middle_text(sContent.c_str(), "\"DepartureTime\":\"", "\"", &cur_pos);	
		start_   = middle_text(sContent.c_str(), "\"DepartureStation\":\"", "\"", &cur_pos);
		end_     = middle_text(sContent.c_str(), "\"ArrivalStation\":\"", "\"", &cur_pos);
		name_    = middle_text(sContent.c_str(), "passengerName=", ",", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "passengerCardID=", ",", &cur_pos);
		price_  = middle_text(sContent.c_str(), "\"TicketPrice\":\"", "\"", &cur_pos);
		type_   = "火车票";
		Write();		
		return 0;
	}
	if (action == PC_BUS)
	{
		// 此处需要将 unicode码转换成 汉字
		string sContent = ms_->UrlDecode(packet);
		sContent = ms_->UrlDecode(sContent);
		sContent = ms_->UTF8ToGBK(sContent);
		char* cur_pos = NULL;	
		tel_     = middle_text(sContent.c_str(), "contactMobile=", "&", &cur_pos);
		number_  = middle_text(sContent.c_str(), "busNumber=", "&", &cur_pos);
		time_    = middle_text(sContent.c_str(), "ticketDate=", "&", &cur_pos) + " " + middle_text(sContent.c_str(), "ticketTime=", "&", &cur_pos);
        start_   = middle_text(sContent.c_str(), "fromCityName=", "&", &cur_pos) + "(" + middle_text(sContent.c_str(), "fromStationName=", "&", &cur_pos) + ")";
		end_     = middle_text(sContent.c_str(), "toCityName=", "&", &cur_pos) + "(" + middle_text(sContent.c_str(), "toStationName=", "&", &cur_pos) + ")";
		name_    = middle_text(sContent.c_str(), "passenger_info=", ",", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "二代身份证,", ",", &cur_pos);
		price_   = middle_text(sContent.c_str(), "fullPrice=", "&", &cur_pos);
		type_    = "汽车票";
		Write();			
		return 0;
	}
	if (action == PC_AIRPLANE)
	{
		// 此处需要将 unicode码转换成 汉字
		string sContent = ms_->UrlDecode(packet);
	
		char* cur_pos = NULL;
		tel_     = middle_text(sContent.c_str(), "\"MobileNo\":\"", "\"", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"Flight\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"DepartTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"DepartCity\":\"", "\"", &cur_pos);
		end_     = middle_text(sContent.c_str(), "\"ArriveCity\":\"", "\"", &cur_pos);
		name_    = middle_text(sContent.c_str(), "\"name\":\"", "\"", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "\"IDCard\":\"", "\"", &cur_pos);
		price_   = middle_text(sContent.c_str(), "\"Price\":", ",", &cur_pos);
		type_    = "飞机票";
		Write();
		return 0;
	}
	return 0; 
}
