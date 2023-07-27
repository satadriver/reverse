
#include "StdAfx.h"

#include "RealNameYiLong.h"

realname::YiLong::YiLong(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "ÒÕÁú";
}

realname::YiLong::~YiLong()
{
	
}

int realname::YiLong::IsYiLong(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("hotel.elong.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/isajax/HotelFillOrder/SaveOrderNew") != -1 ) )	
	{
		return kBookHotel;
	}
	if(  ( http_session->m_Requestline.m_Host.Find("train.elong.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/isajax/Order/VerifyPassenger") != -1 ) )	
	{
		return kBookTrainTicket;
	}
	if(  ( http_session->m_Requestline.m_Host.Find("flight.elong.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/isajax/FillOrder/SaveOrder") != -1 ) )	
	{
		return kBookPlaneTicket;
	}
	return 0;
}

int realname::YiLong::Process(const char *packet,int action)
{

	if (action == kBookHotel)
	{
		return BookHotel(packet);
	}
	if (action == kBookTrainTicket)
	{
		return BookTrainTicket(packet);
	}
	if (action == kBookPlaneTicket)
	{
		return BookPlaneTicket(packet);
	}
	return 0;
}


int realname::YiLong::BookHotel(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	name_ = ms_->GetValueFromMapByKey("HotelContacter.Name",true);
	phone_ = ms_->GetValueFromMapByKey("HotelContacter.MobileString",true);
	if (name_.empty()  || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

int realname::YiLong::BookTrainTicket(const char *packet)
{	
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	ms_->SplitStringToMap(cookie,';','=');
	phone_ = ms_->GetValueFromMapByKey("member");

	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	name_ = ms_->GetValueFromMapByKey("passengerList[0].passengerName",true);
	id_ = ms_->GetValueFromMapByKey("passengerList[0].certNo");

	if (name_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

int realname::YiLong::BookPlaneTicket(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=',"\"");
	name_ = ms_->GetValueFromMapByKey("PassengerList[0].Name",true);
	phone_ = ms_->GetValueFromMapByKey("Contact.MobilePhone");
	id_ = ms_->GetValueFromMapByKey("PassengerList[0].CertNumber");
	if (name_.empty() ||  id_.empty() || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}