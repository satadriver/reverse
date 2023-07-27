
#include "StdAfx.h"
#include "RealNameTrafficTongChengLvYou.h"

realname::TongChengLvYou::TongChengLvYou(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "Í¬³ÌÂÃÓÎ";
}

realname::TongChengLvYou::~TongChengLvYou()
{
	
}

int realname::TongChengLvYou::IsTongChengLvYou(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("www.ly.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/hotels/hotel-book1.aspx") != -1 ) )	
	{
		return kBookHotel;
	}
	if(  ( http_session->m_Requestline.m_Host.Find("www.ly.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/huochepiao/Handlers/FavoriteContacts.ashx") != -1 ) )	
	{
		return kBookTrainTicket;
	}
	if(  ( http_session->m_Requestline.m_Host.Find("www.ly.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/Flight/FlightBookSubmit.aspx") != -1 ) )	
	{
		return kBookPlaneTicket;
	}
	return 0;
}

int realname::TongChengLvYou::Process(const char *packet,int action)
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


int realname::TongChengLvYou::BookHotel(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');

	name_ = ms_->GetValueFromMapByKey("txt_live_name_1",true);
	phone_ = ms_->GetValueFromMapByKey("txt_phone");

	if (name_.empty() || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

int realname::TongChengLvYou::BookTrainTicket(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->Replace(src_data,"{",",");
	ms_->Replace(src_data,"}",",");
	ms_->SplitStringToMap(src_data,',',':',"\"");

	name_ = ms_->GetValueFromMapByKey("name",true);
	phone_ = ms_->GetValueFromMapByKey("phone");
	id_ = ms_->GetValueFromMapByKey("idCard");
	
	if (name_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}

int realname::TongChengLvYou::BookPlaneTicket(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=',"\"");
	name_ = ms_->GetValueFromMapByKey("passengerName",true);
	phone_ = ms_->GetValueFromMapByKey("phone");
	id_ = ms_->GetValueFromMapByKey("passengerCertNo");

	if (name_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}