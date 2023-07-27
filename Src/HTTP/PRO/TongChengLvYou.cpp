
#include "StdAfx.h"

#include "TongChengLvYou.h"

TongChengLvYou::TongChengLvYou()
{
	software_ = "Í¬³ÌÂÃÓÎ";
}

TongChengLvYou::~TongChengLvYou()
{
	
}

int TongChengLvYou::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("www.ly.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/hotels/hotel-book1.aspx") != -1 ) )	
	{
		return kBookHotelCW;
	}

	if( ( http_session->m_Requestline.m_Host.Find("www.ly.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/huochepiao/Handlers/FavoriteContacts.ashx") != -1 ) )	
	{
		return kBookTrainTicketCW;
	}

	if( ( http_session->m_Requestline.m_Host.Find("www.ly.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Flight/FlightBookSubmit.aspx") != -1 ) )	
	{
		return kBookPlaneTicketCW;
	}
	return 0;
}

int TongChengLvYou::Process(const char *packet,int action)
{

	if (action == kBookHotelCW)
	{
		return BookHotelCW(packet);
	}
	if (action == kBookTrainTicketCW)
	{
		return BookTrainTicketCW(packet);
	}
	if (action == kBookPlaneTicketCW)
	{
		return BookPlaneTicketCW(packet);
	}
	return 0;
}


int TongChengLvYou::BookHotelCW(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');

	string name = ms_->GetValueFromMapByKey("txt_live_name_1",true);
	string phone = ms_->GetValueFromMapByKey("txt_phone");

	if (name.empty() || phone.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"user=" + name + "\r\n" +
		"webname=" + phone + "\r\n" +
		"state=" + software_ + "\r\n";

	WriteIndexFile();
	return 1;
}

int TongChengLvYou::BookTrainTicketCW(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->SplitStringToMap(src_data,',',':',"\"");

	string name = ms_->GetValueFromMapByKey("name",true);
	string phone = ms_->GetValueFromMapByKey("phone");
	string id = ms_->GetValueFromMapByKey("idCard");
	
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

int TongChengLvYou::BookPlaneTicketCW(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=',"\"");
	string name = ms_->GetValueFromMapByKey("passengerName",true);
	string phone = ms_->GetValueFromMapByKey("phone");
	string id = ms_->GetValueFromMapByKey("passengerCertNo");

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