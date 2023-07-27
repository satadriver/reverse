
#include "StdAfx.h"

#include "ShoppingTMall.h"

shopping::TMall::TMall(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::TMall::~TMall()
{
	
}

int shopping::TMall::IsTMall(HttpSession *http_session)
{

	if((http_session->m_Requestline.m_Host.Find("buy.tmall.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/auction/confirm_order.htm")!=-1))
	{
		return kPCWebNewOrder;
	}

	if((http_session->m_Requestline.m_Host.Find("unit.buy.tmall.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/auction/json/async_linkage.do")!=-1))
	{
	
		return kSellerDeliverGoods;
	}

	if((http_session->m_Requestline.m_Host.Find("buy.taobao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/auction/json/async_linkage.do")!=-1))
	{
		
		return kSellerDeliverGoods;
	}
	return 0;
}

int shopping::TMall::Process(const char *packet,int action)
{

	if (action == kPCWebNewOrder)
	{
		return PCWebNewOrder(packet);
	}

	if (action == kSellerDeliverGoods)
	{
		return SellerDeliverGoods(packet);
	}
	
	return 0;
}

int shopping::TMall::PCWebNewOrder(const char *packet)
{
	char *data = strstr(packet,"_tb_token_");
	if (!data)
	{
		return 0;
	}
	
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	
	ms_->SplitStringToMap(cookie,';','=');
	user_ = ms_->GetValueFromMapByKey("lgc");

	if ( user_.find("\\u") != string::npos)
	{
		ms_->Replace(user_,"\\u","");
		user_ = ms_->UnicodeToGBK(user_);
	}

	string src_data(data);
	src_data = ms_->UrlDecode(src_data);
	
	string address_data = ms_->GetValueBetweenBeginEnd(src_data,"\",\"options\":[{","}],");
	
	ms_->Replace(address_data,"\"","");
	ms_->Replace(address_data,"},{",";");
	
	
	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;
	
	string other_addresses;
	
	while(1)
	{
		item_end_pos = address_data.find(";",item_beg_pos);
		string item;
		if(item_end_pos == string::npos)
		{
			if (item_beg_pos == string::npos)
			{
				break;
			}
			else
			{
				item = address_data.substr(item_beg_pos);
				item_beg_pos = string::npos;
			}
		}
		else
		{
			item = address_data.substr(item_beg_pos,item_end_pos - item_beg_pos);
			item_beg_pos = item_end_pos + 1/*strlen(";")*/;
		}
		
		ms_->SplitStringToMap(item,',',':');
		
		if (ms_->GetValueFromMapByKey("addrMakerUrl") != "")
		{
			name_ = ms_->GetValueFromMapByKey("fullName",true);
			
			phone_ = ms_->GetValueFromMapByKey("mobile");
			
			string province(ms_->GetValueFromMapByKey("provinceName",true));
			string city(ms_->GetValueFromMapByKey("cityName",true));
			string area(ms_->GetValueFromMapByKey("areaName",true));
			string town(ms_->GetValueFromMapByKey("townName",true));
			string detail(ms_->GetValueFromMapByKey("addressDetail",true));
			
			address_ = province + city + area + town + detail;
		}
		else
		{
			string other_name(ms_->GetValueFromMapByKey("fullName",true));
			
			string other_phone(ms_->GetValueFromMapByKey("mobile"));
			
			string other_province(ms_->GetValueFromMapByKey("provinceName",true));
			string other_city(ms_->GetValueFromMapByKey("cityName",true));
			string other_area(ms_->GetValueFromMapByKey("areaName",true));
			string other_town(ms_->GetValueFromMapByKey("townName",true));
			string other_detail(ms_->GetValueFromMapByKey("addressDetail",true));
			
			string other_address = 
				other_province + other_city + other_area + other_town + other_detail;
			
			other_addresses = other_addresses + other_name + "," + other_phone + "," + other_address + ";";
			
		}
	}
	
	
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"mid=" + fee_ + "\r\n" + 
		"webname=" + other_addresses + "\r\n" + 
		"state=提交订单\r\n";
	
	
	WriteIndexFile();

	return 1;
}

int shopping::TMall::SellerDeliverGoods(const char *packet)
{
	string src_data(packet);
	
	src_data = ms_->UrlDecode(src_data);
	src_data = ms_->GetValueBetweenBeginEnd(src_data,"\"addrMakerUrl\":","}]");

	ms_->Replace(src_data,"[","");
	ms_->Replace(src_data,"]","");
	ms_->Replace(src_data,"}","&");
	ms_->Replace(src_data,"{","&");
	ms_->Replace(src_data,"\"","");

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
			item_beg_pos = item_end_pos + 1/*strlen("&")*/;
		}
		
		ms_->SplitStringToMap(item,',',':');

		string to_name(ms_->GetValueFromMapByKey("fullName",true)); 

		string to_phone(ms_->GetValueFromMapByKey("mobile",true)); 

		string to_province(ms_->GetValueFromMapByKey("provinceName",true));
		string to_city(ms_->GetValueFromMapByKey("cityName",true));
		string to_area(ms_->GetValueFromMapByKey("areaName",true));
		string to_town(ms_->GetValueFromMapByKey("townName",true));
		string to_detail(ms_->GetValueFromMapByKey("addressDetail",true));

		string to_address = to_province + to_city + to_area + to_town + to_detail;
		
		if (to_name.empty() || to_phone.empty() || to_address.empty())
		{
			continue ;
		}

		output_text_ =
			"nick=" + to_name + "\r\n" + 
			"oid=" + to_phone + "\r\n" + 
			"msg=" + to_address + "\r\n" + 
			"state=卖家发货\r\n";

		WriteIndexFile();
			
	}
	return 1;
}