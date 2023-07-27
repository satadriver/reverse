#include "StdAfx.h"
#include "HotelQyer.h"

hotel::HotelQyer::HotelQyer(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "������";
}

hotel::HotelQyer::~HotelQyer()
{

}

hotel::HotelQyer::IsHotelQyer(HttpSession *http_session)
{
	//web�Ƶ�����
	if( ( http_session->m_Requestline.m_Host.Find("www.qyer.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/goto.php") != -1 ))	
	{
		return kWebSearch;
	}
	return 0;
}

hotel::HotelQyer::Process(const char *packet,int action)
{
	//web�Ƶ�����
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	return 0;
}

//web�Ƶ�����
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
		"servername=" + cityname + "\r\n" + 	//Ŀ�ĵ�
		"appname=" + 	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=�Ƶ�����\r\n";			      	//״̬
		WriteIndexFile();
	return 1;
}
