#include "StdAfx.h"

#include "ShoppingSuNingYiGou.h"

shopping::SuNingYiGou::SuNingYiGou(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::SuNingYiGou::~SuNingYiGou()
{
	
}

int shopping::SuNingYiGou::IsSuNingYiGou(HttpSession *http_session)
{
	
	if((http_session->m_Requestline.m_Host.Find("cart.suning.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/emall/SNEasilyBuyCmd?")!=-1))
	{
		return kPCWebAddAddress;
	}

	if((http_session->m_Requestline.m_Host.Find("www.suning.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/webapp/wcs/stores/servlet/SNMTConfirmDeliveryAddress")!=-1))
	{
		return kAndroidAddAddress;
	}

	if((http_session->m_Requestline.m_Host.Find("mobts.suning.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/suning-web-mobile/store/getNearStoreList.do")!=-1))
	{
		return kAndroidLocation;
	}
	return 0;
}

int shopping::SuNingYiGou::Process(const char *packet,int action)
{
	
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}

	if (action == kAndroidLocation)
	{
		return AndroidLocation(packet);
	}
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	
	return 0;
}

int shopping::SuNingYiGou::PCWebAddAddress(const char *packet)
{
	
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');
	
	name_ = ms_->GetValueFromMapByKey("receiveName",true);
	phone_ = ms_->GetValueFromMapByKey("cellphone");
	address_ = ms_->GetValueFromMapByKey("detail",true);

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
	return 0;
}

int shopping::SuNingYiGou::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string telephone;
	name_ = ms_->GetValueFromMapByKey("receiveName",true);
	phone_ = ms_->GetValueFromMapByKey("cellPhone");
	address_ = ms_->GetValueFromMapByKey("detail",true) ;
	telephone = ms_->GetValueFromMapByKey("telePhone",true);
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

int shopping::SuNingYiGou::AndroidLocation(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string lon,lat;
	lon = ms_->GetValueFromMapByKey("gpsLongitude");
	lat = ms_->GetValueFromMapByKey("gpsLatitude");
	
	if ( lon.empty() || lat.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"lon=" + lon + "\r\n" + 
		"lat=" + lat + "\r\n" + 
		"state=定位\r\n";
	
//	WriteIndexFile();
	return 1;
}

