3
#include "StdAfx.h"

#include "ShoppingTaoBao.h"


shopping::TaoBao::TaoBao(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::TaoBao::~TaoBao()
{
	
}

int shopping::TaoBao::IsTaoBao(HttpSession *http_session)
{
	if((http_session->m_Requestline.m_Host.Find("buy.taobao.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/auction/confirm_order.htm")!=-1))
	{
		return kPCWebNewOrder;
	}

	if(((http_session->m_Requestline.m_Host.Find("wuliu.taobao.com") != -1) && 
		(http_session->m_Requestline.m_URI.Find("consign.htm") != -1)) ||
		((http_session->m_Requestline.m_Host.Find("wuliu.taobao.com") != -1) && 
		 (http_session->m_Requestline.m_URI.Find("mailno_validate_sxe.htm") != -1)))  
	{
		return kSellerDeliverGoods;
	}
	return 0;
}

int shopping::TaoBao::Process(const char *packet,int action)
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

int shopping::TaoBao::PCWebNewOrder(const char *packet)
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
		user_ = ms_->UnicodeToGBKPrefix_u(user_);
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


int shopping::TaoBao::SellerDeliverGoods(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');

	string mail_no = ms_->GetValueFromMapByKey("mailNo");

	string company_code = ms_->GetValueFromMapByKey("companyCode");
	string company_name = GetLogsticsCompanyName(company_code);

	string to_name = ms_->GetValueFromMapByKey("_fmw.r._0.c",true);
	string to_phone = ms_->GetValueFromMapByKey("_fmw.r._0.mo");
	string to_address = ms_->GetValueFromMapByKey("_fmw.r._0.dd",true);

	string from_name = ms_->GetValueFromMapByKey("_fmw.f._0.c",true);
	string from_phone = ms_->GetValueFromMapByKey("_fmw.f._0.m");
	string from_address = ms_->GetValueFromMapByKey("_fmw.f._0.dd",true);

	ms_->Replace(from_phone," ","");
	

	if (to_name.empty() || to_phone.empty() || to_address.empty() || 
		from_name.empty() || from_phone.empty() || from_address.empty())
	{
		return 0;
	}

	output_text_ = 
		"from=" + mail_no +  "\r\n" + 
		"to=" + company_name +  "\r\n" + 
		"nick=" + to_name + "\r\n" + 
		"oid=" + to_phone + "\r\n" + 
		"msg=" + to_address + "\r\n" + 
		"netmark=" + from_name + "\r\n" + 
		"clientmsg=" + from_phone + "\r\n" + 
		"msgtype=" + from_address + "\r\n" + 
		"state=卖家发货\r\n";
	WriteIndexFile();
	return 1;
}


string shopping::TaoBao::GetLogsticsCompanyName(const string &company_code)
{
	if (company_code == "SF")
	{
		return "顺丰快递";
	}
	if (company_code == "STO")
	{
		return "申通快递";
	}
	if (company_code == "ZTO")
	{
		return "中通快递";
	}
	if (company_code == "YTO")
	{
		return "圆通快递";
	}
	if (company_code == "HTKY")
	{
		return "汇通快递";
	}
	if (company_code == "YUNDA")
	{
		return "韵达快递";
	}
	if (company_code == "TTKDEX")
	{
		return "天天快递";
	}
	if (company_code == "LB")
	{
		return "联邦快递";
	}
	if (company_code == "QFKD")
	{
		return "全峰快递";
	}
	if (company_code == "QFKD")
	{
		return "全峰快递";
	}
	if (company_code == "GTO")
	{
		return "国通快递";
	}
	if (company_code == "UC")
	{
		return "优速快递";
	}
	if (company_code == "NEDA")
	{
		return "能达快递";
	}
	if (company_code == "EYB")
	{
		return "E邮宝";
	}
	if (company_code == "POSTB")
	{
		return "邮局平邮";
	}
	return company_code;
}