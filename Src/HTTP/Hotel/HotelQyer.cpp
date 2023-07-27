#include "StdAfx.h"
#include "HotelQyer.h"

hotel::HotelQyer::HotelQyer(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "穷游网";
}

hotel::HotelQyer::~HotelQyer()
{

}

hotel::HotelQyer::IsHotelQyer(HttpSession *http_session)
{
	//web酒店搜索
	if( ( http_session->m_Requestline.m_Host.Find("www.qyer.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/goto.php") != -1 ))	
	{
		return kWebSearch;
	}
	return 0;
}

hotel::HotelQyer::Process(const char *packet,int action)
{
	//web酒店搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	return 0;
}

//web酒店搜索
hotel::HotelQyer::WebSearch(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url  = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');
	string livedate = ms_->GetValueFromMapByKey("checkin_year",true) + "-" +
		              ms_->GetValueFromMapByKey("checkin_month",true) + "-" + 
					  ms_->GetValueFromMapByKey("checkin_monthday",true);
	string leavedate = ms_->GetValueFromMapByKey("checkout_year",true) + "-" +
					   ms_->GetValueFromMapByKey("checkout_month",true) + "-" +
		               ms_->GetValueFromMapByKey("checkout_monthday",true);

	string cityname = ms_->GetValueFromMapByKey("ss",true);
	if (livedate.empty() || leavedate.empty() || cityname.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//目的地
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=酒店搜索\r\n";			      	//状态
		WriteIndexFile();
	return 1;
}
