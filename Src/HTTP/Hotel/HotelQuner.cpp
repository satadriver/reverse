#include "StdAfx.h"
#include "HotelQuner.h"

hotel::HotelQuner::HotelQuner(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "ȥ�Ķ�";
}

hotel::HotelQuner::~HotelQuner()
{

}

hotel::HotelQuner::IsHotelQuner(HttpSession *http_session)
{
	//web���� ���ʾƵ�����
	if( ( http_session->m_Requestline.m_Host.Find("hotel.qunar.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/render/listPageSnapshot.jsp") != -1 ))	
	{
		return kWebSearch;
	}
	return 0;
}

hotel::HotelQuner::Process(const char *packet,int action)
{
	//web���� ���ʾƵ�����
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	return 0;
}

//web���� ���ʾƵ�����
hotel::HotelQuner::WebSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("data=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("data=") + strlen("data="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string cityname,livedate,leavedate,keyword;
	//ʹ�� json��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		//for (int i=0;i < value.size();i++)
		//{
			int i = 0;
			//Ŀ�ĵ�
			cityname = value[i]["city_tag"].asString();
			//��ס����
			livedate = value[i]["checkin_Date"].asString();
			//�˷�����
			leavedate = value[i]["checkout_Date"].asString();
			//�ؼ���
			keyword = value[i]["query"].asString();
		//}
	}
	if (cityname.empty() || livedate.empty() || leavedate.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//Ŀ�ĵ�
		"appname=" + 	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"nick="	 + 	  	keyword + "\r\n" + 		//�ؼ���
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=�Ƶ�����\r\n";					//״̬
	WriteIndexFile();
	return 1;
}
