#include "StdAfx.h"

#include "realnameWeChat.h"


realname::WeChat::WeChat(HttpSession *http_session, const string &pro_name) : RealName(http_session, pro_name)
{
	app_ = "Î¢ÐÅ¹ºÆ±";
}

realname::WeChat::~WeChat()
{
	
}

int realname::WeChat::IsWeChat(HttpSession *http_session)
{
	
	if( (http_session->m_Requestline.m_Host.Find("wx.17u.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/train/CreateOrder")!=-1))
	{
		return kBuyTrainTicket;
	}

	if( (http_session->m_Requestline.m_Host.Find("wx.17u.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/flight/json/flightsubmitorder.html")!=-1))
	{
		return kBuyFlightTicket;
	}
	return 0;
}

int realname::WeChat::Process(const char *packet,int action)
{
	
	if (action == kBuyTrainTicket)
	{
		return BuyTrainTicket(packet);
	}

	if (action == kBuyFlightTicket)
	{
		return BuyFlightTicket(packet);
	}
	
	
	return 0;
}

int realname::WeChat::BuyTrainTicket(const char *packet)
{
	string data_buffer(packet);
	data_buffer = ms_->UrlDecode(data_buffer);
	ms_->SplitStringToMap(data_buffer,'&','=');

	string para = ms_->GetValueFromMapByKey("para",true);

	Json::Reader jreader;
	Json::Value jroot;
	if (jreader.parse(para,jroot))
	{
		
		string phone = jroot["contactInfo"]["phone"].asString();
	
		Json::Value passengers = jroot["passengers"];
		for (int unsigned i = 0; i < passengers.size(); ++i)
		{
			string name = passengers[i]["name"].asString();
			string idCard = passengers[i]["idCard"].asString();
			if (i == 0)
			{
				output_text_ =
					"user=" + name + "\r\n" +
					"to=" + idCard + "\r\n" +
					"webname=" + phone + "\r\n" +
					"state=" + app_ + "\r\n";
			}
			else
			{

				output_text_ =
					"user=" + name + "\r\n" +
					"to=" + idCard + "\r\n" +
					"state=" + app_ + "\r\n";
			}
			WriteIndexFile();
		}
		
	}
	
	
	return 1;
}

int realname::WeChat::BuyFlightTicket(const char *packet)
{
	
	string src_data(http_session_->m_databuffer);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	
	
	name_ = ms_->GetValueFromMapByKey("linkman",true);
	phone_ = ms_->GetValueFromMapByKey("linkmobile");
	string tmp = ms_->GetValueFromMapByKey("plist",true);

	id_ = ms_->GetValueBetweenBeginEnd(tmp,"/","/");

		
	if ( name_.empty() || phone_.empty() || id_.empty())
	{ 
		return 0;
	}
	
	output_text_ = GetOutputText();
	WriteIndexFile();
	return 1;
}