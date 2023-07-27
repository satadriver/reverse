#include "StdAfx.h"
#include "HotelQuner.h"

hotel::HotelQuner::HotelQuner(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "去哪儿";
}

hotel::HotelQuner::~HotelQuner()
{

}

hotel::HotelQuner::IsHotelQuner(HttpSession *http_session)
{
	//web国内 国际酒店搜索
	if( ( http_session->m_Requestline.m_Host.Find("hotel.qunar.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/render/listPageSnapshot.jsp") != -1 ))	
	{
		return kWebSearch;
	}
	return 0;
}

hotel::HotelQuner::Process(const char *packet,int action)
{
	//web国内 国际酒店搜索
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	return 0;
}

//web国内 国际酒店搜索
hotel::HotelQuner::WebSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("data=") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("data=") + strlen("data="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string cityname,livedate,leavedate,keyword;
	//使用 json读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		//for (int i=0;i < value.size();i++)
		//{
			int i = 0;
			//目的地
			cityname = value[i]["city_tag"].asString();
			//入住日期
			livedate = value[i]["checkin_Date"].asString();
			//退房日期
			leavedate = value[i]["checkout_Date"].asString();
			//关键词
			keyword = value[i]["query"].asString();
		//}
	}
	if (cityname.empty() || livedate.empty() || leavedate.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//目的地
		"appname=" + 	livedate + "\r\n" + 	//入住日期
		"msgtype=" + 	leavedate + "\r\n" +	//退房日期
		"nick="	 + 	  	keyword + "\r\n" + 		//关键词
		"netmark=" +	app_ + "\r\n" + 		//来源
		"clientmsg=" +  platformWeb + "\r\n" +  //操作平台
		"state=酒店搜索\r\n";					//状态
	WriteIndexFile();
	return 1;
}
