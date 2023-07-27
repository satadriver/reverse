#include "StdAfx.h"
#include "hardwarexunlei.h"

hardwareinfo::HardWareXunLei::HardWareXunLei(HttpSession *http_session, const string &pro_name) : HardwareInfo(http_session,pro_name)
{
	app_ = "迅雷";
}

hardwareinfo::HardWareXunLei::~HardWareXunLei()
{
	
}

int hardwareinfo::HardWareXunLei::IsHardWareXunLei(HttpSession *http_session)
{
	//迅雷 7.9 极速版
	//GET /install_stat?appname=xl_thunder_pc&appversion=1.0.34.360&processid=5168&peerid=000C29C9189C1BMQ&channel=00000&filename=ThunderSpeed1.0.34.360.exe&osversion=6.1.1.0.1&slience=0&offline=1&new=0 HTTP/1.1
	//Host: 02.rcv.sandai.net
	if((http_session->m_Requestline.m_Host.Find("02.rcv.sandai.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/install_stat?appname=")!=-1))
	{
		return khardwareinfomation;
	}
	//迅雷 9
	//GET /boxserver/clientreport?op=Niux_Service_Start&peer_id=000C29C9189C1BMQ&xl_user_id=&xl_user_name=&ptime=1479178219&version_code=1.0.0.6&att1=&att2=9.0.19.482 HTTP/1.1
	//Host: report.boxsvr.niu.xunlei.com:6643
	if((http_session->m_Requestline.m_Host.Find("report.boxsvr.niu.xunlei.com:6643")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/boxserver/clientreport?op=")!=-1))
	{
		return khardwareinfomation;
	}
	//手机迅雷
	//GET /device/associate HTTP/1.1
	//Host: device.client.xunlei.com
	if((http_session->m_Requestline.m_Host.Find("device.client.xunlei.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/device/associate")!=-1))
	{
		return khardwareinfomation;
	}
	return 0;
}

int hardwareinfo::HardWareXunLei::Process(const char *packet,int action)
{
	if (action == khardwareinfomation)
	{
		return hardwareinfomation(packet);
	}
	return 1;
}

int hardwareinfo::HardWareXunLei::hardwareinfomation(const char *packet)
{
	//URL 中得到数据
	//数据部分
	string url(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//迅雷极速7.9
    if (url.find("/install_stat?appname=") != string::npos)
    {
		url = ms_->UrlDecode(url);
		if (url.find("uuid=") != string::npos)
		{
			url = url.substr(url.find("appname="));
		}
		ms_->SplitStringToMap(url,'&','=');
		mid = ms_->GetValueFromMapByKey("peerid"); 
    }
    //迅雷9
    if (url.find("/boxserver/clientreport?op=") != string::npos)
    {
		url = ms_->UrlDecode(url);
		if (url.find("op=") != string::npos)
		{
			url = url.substr(url.find("op="));
		}
		ms_->SplitStringToMap(url,'&','=');
		mid = ms_->GetValueFromMapByKey("peer_id"); 
    }
	//手机迅雷
    if (url.find("/device/associate") != string::npos)
    {
		//cookie
		string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
		cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
		ms_->SplitStringToMap(cookie, ';','=');	
		mid = ms_->GetValueFromMapByKey("phone_peerid"); 
    }
	if (mid.empty())
	{
		return 0;
	}
	output_text_ = 
		"mid=" + mid + "\r\n" +
		"nick=" + app_ + "\r\n" +
		"state=硬件特征信息\r\n";
	WriteIndexFile();
	return 1;
}