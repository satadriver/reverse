#include "StdAfx.h"

#include "ShoppingPaiPai.h"

shopping::PaiPai::PaiPai(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::PaiPai::~PaiPai()
{
	
}

int shopping::PaiPai::IsPaiPai(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("auction.paipai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cgi-bin/auction_fixup_confirm/confirm")!=-1))
	{
		return kPCWebAddAddress;
	}

	if((http_session->m_Requestline.m_Host.Find("api.m.paipai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/recvaddr/add.xhtml")!=-1))
	{
		return kAndroidAddAddress;
	}
	return 0;
}

int shopping::PaiPai::Process(const char *packet,int action)
{
	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	return 0;
}

int shopping::PaiPai::PCWebAddAddress(const char *packet)
{
	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');

	string temp_data = ms_->GetValueFromMapByKey("addressStr",true);

	if (temp_data.empty())
	{ 
		return 0;
	}
	
	ms_->Replace(temp_data,"，","&");
	ms_->Replace(temp_data,"：","=");
	ms_->SplitStringToMap(temp_data,'&','=');
	
	string::size_type pos = temp_data.find("&");
	if (pos == string::npos)
	{
		return 0;
	}

	address_ = temp_data.substr(0,pos);
	name_ = ms_->GetValueFromMapByKey("收货人");
	phone_ = ms_->GetValueFromMapByKey("联系电话");

	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 1;
}

int shopping::PaiPai::AndroidAddAddress(const char *packet)
{
	string src_data(packet);	
	ms_->SplitStringToMap(src_data,'&','=');
	
	name_ = ms_->GetValueFromMapByKey("name",true);
	phone_ = ms_->GetValueFromMapByKey("mobile");
	address_ = ms_->GetValueFromMapByKey("address",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=添加地址\r\n";
	
	WriteIndexFile();
	return 1;
}


