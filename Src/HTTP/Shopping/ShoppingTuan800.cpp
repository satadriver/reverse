
#include "StdAfx.h"

#include "ShoppingTuan800.h"

shopping::Tuan800::Tuan800(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::Tuan800::~Tuan800()
{
	
}

int shopping::Tuan800::IsTuan800(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("passport.tuan800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/m/sessions")!=-1))
	{
		return kAndroidLogin;
	}
	if((http_session->m_Requestline.m_Host.Find("zapi.zhe800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/profile/address")!=-1))
	{
		return kAndroidAddAddress;

	}
	if((http_session->m_Requestline.m_Host.Find("m.api.tuan800.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4/nearby")!=-1))
	{
		return kAndroidLocation;
	}
	return 0;
}

int shopping::Tuan800::Process(const char *packet,int action)
{
	
	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	return 0;
}

int shopping::Tuan800::AndroidAddAddress(const char *packet)
{
	char *p = strstr(packet,"\r\n\r\n");
	if (!p)
	{
		return 0;
	}
	string src_data(p);
	ms_->SplitStringToMap(src_data,'&','=',"\r\n");
	
	name_ = ms_->GetValueFromMapByKey("consignee_name",true);
	phone_ = ms_->GetValueFromMapByKey("phone_number");
	address_ = ms_->GetValueFromMapByKey("province",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("address_info",true) ;
	
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

int shopping::Tuan800::AndroidLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string user,pass,imei;
	user = ms_->GetValueFromMapByKey("phone_number",true);
	pass = ms_->GetValueFromMapByKey("password");
	imei = ms_->GetValueFromMapByKey("deviceId",true);
	
	if ( user.empty() || pass.empty() || imei.empty())
	{
		return 0;
	}
	
	WriteVirtualIdentity(user,"上线");

	output_text_ = "imei=" + imei + "\r\n" + 
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=登录\r\n";
	
	WriteIndexFile();
	return 1;
}

int shopping::Tuan800::AndroidLocation(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI);
	ms_->SplitStringToMap(src_data,'&','=');
	string lon,lat;
	lon = ms_->GetValueFromMapByKey("longitude");
	lat = ms_->GetValueFromMapByKey("latitude");
	
	if ( lon.empty() || lat.empty())
	{
		return 0;
	}
	
	output_text_ = "lon=" + lon + "\r\n" + 
		"lat=" + lat + "\r\n" + 
		"state=定位\r\n";
	
//	WriteIndexFile();
	return 1;
}

