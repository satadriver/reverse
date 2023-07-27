
#include "StdAfx.h"

#include "RealNameZhiFuBao.h"

realname::ZhiFuBao::ZhiFuBao(HttpSession *http_session, const string &pro_name) : RealName(http_session,pro_name)
{
	app_ = "Ö§¸¶±¦";
}

realname::ZhiFuBao::~ZhiFuBao()
{
	
}

int realname::ZhiFuBao::IsZhiFuBao(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("wgo.mmstat.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("tbjipiao") != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("gokey=FillOrderInfoTrack") != -1 ))	
	{
		return kBuyFlightTicket;
	}

	if( ( http_session->m_Requestline.m_Host.Find("wgo.mmstat.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/tbtrip") != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("gokey=param") != -1 ))	
	{
		return kBuyTrainTicket;
	}

	return 0;
}

int realname::ZhiFuBao::Process(const char *packet,int action)
{
	if (action == kBuyFlightTicket)
	{
		return BuyFlightTicket(packet);
	}

	if (action == kBuyTrainTicket)
	{
		return BuyTrainTicket(packet);
	}
	return 0;
}


int realname::ZhiFuBao::BuyFlightTicket(const char *packet)
{	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,'&','=');
	
	string param;
	
	string::size_type pos = uri.find("FillOrderInfoTrack=");
	
	if (pos != string::npos)
	{
		param = uri.substr(pos);
	}
	else
	{
		return 0;
	}

	string contact = ms_->GetValueBetweenBeginEnd(param,"contact\":\"{","\"}\"",true);

	ms_->Replace(contact,"[","");
	ms_->Replace(contact,"{","");
	ms_->Replace(contact,"]","");
	ms_->Replace(contact,"}","");
	ms_->Replace(contact,"\\","");
	ms_->Replace(contact,"\"","");

	ms_->SplitStringToMap(contact,',',':');
	phone_ = ms_->GetValueFromMapByKey("phone",true);
	
	string passenger = ms_->GetValueBetweenBeginEnd(param,"passenger\":\"[{","}]",true);
	
	ms_->Replace(passenger,"[","");
	ms_->Replace(passenger,"{","");
	ms_->Replace(passenger,"]","");
	ms_->Replace(passenger,"}","");
	ms_->Replace(passenger,"\\","");
	ms_->Replace(passenger,"\"","");
	ms_->SplitStringToMap(passenger,',',':');
	
	name_ = ms_->GetValueFromMapByKey("fullname",true);
	id_ = ms_->GetValueFromMapByKey("certNo",true);

	if (name_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();

	return 1;
}

int realname::ZhiFuBao::BuyTrainTicket(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	uri = ms_->UrlDecode(uri);
	ms_->SplitStringToMap(uri,'&','=');

	string param;

	string::size_type pos = uri.find("param=");

	if (pos != string::npos)
	{
		param = uri.substr(pos);
	}
	else
	{
		return 0;
	}


	param = ms_->UrlDecode(param);
	ms_->SplitStringToMap(param,',',':',"\"");

	address_ = ms_->GetValueFromMapByKey("receiverAddress",true);
	phone_ = ms_->GetValueFromMapByKey("contactMobile",true);

	string passengers = ms_->GetValueBetweenBeginEnd(param,"passengers\":\"[{","}]",true);

	ms_->Replace(passengers,"[","");
	ms_->Replace(passengers,"{","");
	ms_->Replace(passengers,"]","");
	ms_->Replace(passengers,"}","");
	ms_->Replace(passengers,"\\","");
	ms_->Replace(passengers,"\"","");
	
	ms_->SplitStringToMap(passengers,',',':');
	name_ = ms_->GetValueFromMapByKey("passengerName",true);
	id_ = ms_->GetValueFromMapByKey("certificateNum",true);


	if (name_.empty() || id_.empty() || phone_.empty())
	{
		return 0;
	}
	output_text_ = GetOutputText();
	WriteIndexFile();


	return 1;
}