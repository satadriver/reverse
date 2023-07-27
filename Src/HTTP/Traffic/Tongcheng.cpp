// Tongcheng.cpp: implementation of the Tongcheng class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Tongcheng.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Tongcheng::Tongcheng(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

Tongcheng::~Tongcheng()
{

}

int Tongcheng::Is(HttpSession *http_session)
{
	// PC »ð³µÆ±
	if((http_session->m_Requestline.m_Host.Find("www.ly.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/huochepiao/Handlers/OrderSubmit.ashx")!=-1))
	{
		return PC_TRAIN;
	}	
	// PC Æû³µÆ±
	if((http_session->m_Requestline.m_Host.Find("bus.ly.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/CreateBusOrder")!=-1))
	{
		return PC_BUS;
	}
	// PC ·É»úÆ±
	if((http_session->m_Requestline.m_Host.Find("www.ly.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Flight/FlightBookSubmit.aspx")!=-1))
	{
		return PC_AIRPLANE;
	}
	// SP Æû³µÆ±
	if((http_session->m_Requestline.m_Host.Find("tcmobileapi.17usoft.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/OrderHandler.ashx")!=-1))
	{
		return SP_BUS;
	}
	// SP »ð³µÆ±
	if((http_session->m_Requestline.m_Host.Find("tcmobileapi.17usoft.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/train/orderhandler.ashx")!=-1))
	{
		return SP_TRAIN;
	}
	// SP ·É»úÆ±
	if((http_session->m_Requestline.m_Host.Find("tcmobileapi.17usoft.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/flight/orderhandler.ashx")!=-1))
	{
		return SP_AIRPLANE;
	}
	return 0;
}

int Tongcheng::Process(const char *packet,int action)
{
	if (action == PC_AIRPLANE)
	{
		string sContent = ms_->UrlDecode(packet);		
		char* cur_pos = (char*)sContent.c_str();	
		tel_     = middle_text(sContent.c_str(), "phone=", "&", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"trainNo\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"departureTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"fromCityPy\":\"", "\"", &cur_pos);
		end_     = middle_text(sContent.c_str(), "\"toCityPy\":\"", "\"", &cur_pos);
		name_    = middle_text(sContent.c_str(), "passengerName=", "&", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "passengerCertNo=", "&", &cur_pos);
		price_  = middle_text(sContent.c_str(), "\"ticketPrice\":\"", "\"", &cur_pos);
		type_   = "·É»úÆ±";	
		Write();		
		return 0;
	}
	if (action == PC_TRAIN)
	{
		string sContent = ms_->UrlDecode(packet);
		char* cur_pos = (char*)sContent.c_str();
		tel_     = middle_text(sContent.c_str(), "\"phone\":\"", "\"", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"trainNo\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"departureTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"fromCityPy\":\"", "\"", &cur_pos);
		end_     = middle_text(sContent.c_str(), "\"toCityPy\":\"", "\"", &cur_pos);
		name_    = middle_text(sContent.c_str(), "\"name\":\"", "\"", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "\"idCard\":\"", "\"", &cur_pos);
		price_  = middle_text(sContent.c_str(), "\"ticketPrice\":\"", "\"", &cur_pos);
		type_   = "»ð³µÆ±";
		Write();
		return 0;
	}
	if (action == PC_BUS)
	{
		string sContent = ms_->UrlDecode(packet);	
		char* cur_pos = NULL;		
		tel_     = middle_text(sContent.c_str(), "\"MobileNo\":\"", "\"", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"CoachNo\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"dptDateTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"Departure\":\"", "\"", &cur_pos) + "(" + middle_text(sContent.c_str(), "\"dptStation\":\"", "\"", &cur_pos) + ")";
		end_     = middle_text(sContent.c_str(), "\"Destination\":\"", "\"", &cur_pos) + "(" + middle_text(sContent.c_str(), "\"ArrStation\":\"", "\"", &cur_pos) + ")";
		name_    = middle_text(sContent.c_str(), "\"name\":\"", "\"", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "\"IDCard\":\"", "\"", &cur_pos);
		price_   = middle_text(sContent.c_str(), "\"ticketPrice\":", ",", &cur_pos);
		type_    = "Æû³µÆ±";
    	Write();
		return 0;
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (action == SP_AIRPLANE)
	{
		string sContent = ms_->UrlDecode(packet);	
		char* cur_pos = (char*)sContent.c_str();
		tel_     = middle_text(sContent.c_str(), "\"linkMobile\":\"", "\"", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"flightNo\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"flyOffTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"fromCityPy\":\"", "\"", &cur_pos);
		end_     = middle_text(sContent.c_str(), "\"arriveAirportCode\":\"", "\"", &cur_pos);
		name_    = middle_text(sContent.c_str(), "\"passengerName\":\"", "&", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "\"certNo\":\"", "&", &cur_pos);
		price_  = middle_text(sContent.c_str(), "\"ticketPrice\":\"", "\"", &cur_pos);
		type_   = "·É»úÆ±";	
		Write();	
		return 0;
	}
	if (action == SP_TRAIN)
	{
		string sContent = ms_->UrlDecode(packet);		
		char* cur_pos = (char*)sContent.c_str();	
		tel_     = middle_text(sContent.c_str(), "\"phone\":\"", "\"", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"trainNo\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"fromTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"fromCityPy\":\"", "\"", &cur_pos);
		end_     = middle_text(sContent.c_str(), "\"toCityPy\":\"", "\"", &cur_pos);
		name_    = middle_text(sContent.c_str(), ",\"name\":\"", "\"", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "\"idCard\":\"", "\"", &cur_pos);
		price_  = middle_text(sContent.c_str(), "\"ticketPrice\":\"", "\"", &cur_pos);
		type_   = "»ð³µÆ±";		
		Write();		
		return 0;
	}	
	if (action == SP_BUS)
	{
		string sContent = ms_->UrlDecode(packet);
		char* cur_pos = NULL;	
		tel_     = middle_text(sContent.c_str(), "\"mobileNo\":\"", "\"", &cur_pos);
		number_  = middle_text(sContent.c_str(), "\"coachNo\":\"", "\"", &cur_pos);
		time_    = middle_text(sContent.c_str(), "\"dptDateTime\":\"", "\"", &cur_pos);
        start_   = middle_text(sContent.c_str(), "\"departure\":\"", "\"", &cur_pos) + "(" + middle_text(sContent.c_str(), "\"dptStation\":\"", "\"", &cur_pos) + ")";
		end_     = middle_text(sContent.c_str(), "\"destination\":\"", "\"", &cur_pos) + "(" + middle_text(sContent.c_str(), "\"arrStation\":\"", "\"", &cur_pos) + ")";
		name_    = middle_text(sContent.c_str(), "\"name\":\"", "\"", &cur_pos);
		cardid_  = middle_text(sContent.c_str(), "\"IDCard\":\"", "\"", &cur_pos);
		price_   = middle_text(sContent.c_str(), "\"ticketPrice\":", ",", &cur_pos);
		type_    = "Æû³µÆ±";		
		Write();	
		return 0;
	}
	return 0; 
}
