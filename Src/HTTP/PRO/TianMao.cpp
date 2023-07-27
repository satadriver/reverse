
#include "StdAfx.h"

#include "TianMao.h"

TianMao::TianMao()
{
	software_ = "天猫";
	pro_ = "shopping_tmall";

}

TianMao::~TianMao()
{
	
}

int TianMao::Is(HttpSession *http_session)
{

	if((http_session->m_Requestline.m_Host.Find("buy.TianMao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/auction/confirm_order.htm")!=-1))
	{
		return kNewOrderCW;
	}

	if((http_session->m_Requestline.m_Host.Find("unit.buy.TianMao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/auction/json/async_linkage.do")!=-1))
	{
	
	//	return kSellerDeliverGoods;
	}
	return 0;
}

int TianMao::Process(const char *packet,int action)
{

	if (action == kNewOrderCW)
	{
		return NewOrderCW(packet);
	}

	if (action == kSellerDeliverGoods)
	{
		return SellerDeliverGoods(packet);
	}
	
	return 0;
}

int TianMao::NewOrderCW(const char *packet)
{
	char *data = strstr(packet,"_tb_token_");
	if (!data)
	{
		return 0;
	}
	
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);

 	ms_->SplitStringToMap(cookie,';','=');
	string user = ms_->GetValueFromMapByKey("lgc");

	if ( user.find("\\u") != string::npos)
	{
		user = ms_->UnicodeToGBKPrefix_u(user);
	}

 	string src_data(data);
 	src_data = ms_->UrlDecode(src_data);

	string address_data = ms_->GetValueBetweenBeginEnd(src_data,"\",\"options\":[{","}],");

	ms_->Replace(address_data,"\"","");
	ms_->Replace(address_data,"},{",";");


	string::size_type item_beg_pos = 0;
	string::size_type item_end_pos = 0;

	string name;
	string phone;
	string address;
	string other_name;
	string other_phone;
	string other_address;
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
			name = ms_->GetValueFromMapByKey("fullName",true);

			phone = ms_->GetValueFromMapByKey("mobile");

			string province(ms_->GetValueFromMapByKey("provinceName",true));
			string city(ms_->GetValueFromMapByKey("cityName",true));
			string area(ms_->GetValueFromMapByKey("areaName",true));
			string town(ms_->GetValueFromMapByKey("townName",true));
			string detail(ms_->GetValueFromMapByKey("addressDetail",true));

			address = province + city + area + town + detail;
		}
		else
		{
			other_name = ms_->GetValueFromMapByKey("fullName",true);
			
			other_phone = ms_->GetValueFromMapByKey("mobile");
			
			string other_province(ms_->GetValueFromMapByKey("provinceName",true));
			string other_city(ms_->GetValueFromMapByKey("cityName",true));
			string other_area(ms_->GetValueFromMapByKey("areaName",true));
			string other_town(ms_->GetValueFromMapByKey("townName",true));
			string other_detail(ms_->GetValueFromMapByKey("addressDetail",true));
			
			other_address = 
				other_province + other_city + other_area + other_town + other_detail;

			other_addresses = other_addresses + other_name + "," + other_phone + "," + other_address + ";";

		}
	}

	output_text_ = 
		"user=" + user + "\r\n" + 
		"nick=" + name + "\r\n" + 
		"oid=" + phone + "\r\n" + 
		"msg=" + address + "\r\n" + 
		"webname=" + other_addresses + "\r\n" + 
		"state=提交订单\r\n";
	
	
	WriteIndexFile();

	return 1;
}

int TianMao::SellerDeliverGoods(const char *packet)
{
	string src_data(packet);
	
	src_data = ms_->GetValueBetweenBeginEnd(src_data,"\"options\":","}]");

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

		string to_name(ms_->GetValueFromMapByKey("fullname",true)); 

		string to_phone(ms_->GetValueFromMapByKey("mobile",true)); 

		string to_province(ms_->GetValueFromMapByKey("provincename",true));
		string to_city(ms_->GetValueFromMapByKey("cityname",true));
		string to_area(ms_->GetValueFromMapByKey("areaname",true));
		string to_town(ms_->GetValueFromMapByKey("townname",true));
		string to_detail(ms_->GetValueFromMapByKey("addressdetail",true));

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