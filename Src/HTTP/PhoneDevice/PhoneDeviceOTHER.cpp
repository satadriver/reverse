
#include "StdAfx.h"

#include "PhoneDeviceOTHER.h"

phonedevice::Other::Other(HttpSession *http_session, const string &pro_name) : PhoneDevice(http_session,pro_name)
{

}

phonedevice::Other::~Other()
{
	
}

int phonedevice::Other::IsOther(HttpSession *http_session)
{
	if(	(http_session->m_Requestline.m_Host.Find("info.antutu.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/info/info.php")!=-1))
	{
		return kantutu;
	}

	if(	(http_session->m_Requestline.m_Host.Find("susapi.lenovomm.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/adpserver/GetVIByPNBG")!=-1))
	{
		return kleanquan;
	}

	if(	(http_session->m_Requestline.m_Host.Find("www.talk007.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/talkforum/mobile.do")!=-1))
	{
		return kyingjianguanjia;
	}

	if(	(http_session->m_Requestline.m_Host.Find("seecom.zte.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/homenet/homenetPushMessage_pushMessage.action")!=-1))
	{
		return ksuopingdashi;
	}

	if(	(http_session->m_Requestline.m_Host.Find("cngift3.tcstat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/gift30/inform/index/1n")!=-1))
	{
		return khuadongsuoping;
	}
	return 0;
}

int phonedevice::Other::Process(const char *packet,int action)
{
	if (action == kantutu)
	{
		return antutu(packet);
	}

	if (action == kleanquan)
	{
		return leanquan(packet);
	}

	if (action == kyingjianguanjia)
	{
		return yingjianguanjia(packet);
	}

	if (action == ksuopingdashi)
	{
		return suopingdashi(packet);
	}

	if (action == khuadongsuoping)
	{
		return huadongsuoping(packet);
	}
	return 0;
}


int phonedevice::Other::antutu(const char *packet)
{
	app_ = "安兔兔评测";

	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');

	model_ = ms_->GetValueFromMapByKey("model");
	imei_ = ms_->GetValueFromMapByKey("imei");

	if (model_.empty() && imei_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	return 1;
}

int phonedevice::Other::leanquan(const char *packet)
{
	app_ = "乐安全";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	model_ = ms_->GetValueFromMapByKey("DModel");
	imei_ = ms_->GetValueFromMapByKey("dauuid");
	
	if (model_.empty() && imei_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	return 1;
}

int phonedevice::Other::yingjianguanjia(const char *packet)
{
	app_ = "硬件管家";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("IMEI");
	
	if (model_.empty() && imei_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	return 1;
}

int phonedevice::Other::suopingdashi(const char *packet)
{
	app_ = "锁屏大师";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("DeviceID");
	
	if (model_.empty() && imei_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	return 1;
}

int phonedevice::Other::huadongsuoping(const char *packet)
{
	app_ = "滑动锁屏";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	imei_ = ms_->GetValueFromMapByKey("imei");
	model_ = ms_->GetValueFromMapByKey("model");

	if (model_.empty() && imei_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	return 1;
}