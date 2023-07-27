#include "StdAfx.h"
#include "HotelCtrip.h"

hotel::HotelCtrip::HotelCtrip(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "Я��";
}

hotel::HotelCtrip::~HotelCtrip()
{

}

hotel::HotelCtrip::IsHotelCtrip(HttpSession *http_session)
{
	//web���ھƵ�����
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotel") != -1 ))	
	{
		return kWebSearch;
	}
	//web���ʾƵ�����
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/international") != -1 ))	
	{
		return kWebIntSearch;
	}
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/DomesticBook/DomeInputNewOrderCS.aspx") != -1))	
	{
		return kWebNewOrder;
	}
	//web���ʾƵ�Ԥ�� ���������
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/internationalbook/inputneworder.aspx") != -1))	
	{
		//return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelCtrip::Process(const char *packet,int action)
{
	//web���ھƵ�����
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	//web���ʾƵ�����
	if (action == kWebIntSearch)
	{
		return WebIntSearch(packet);
	}
	//web���ھƵ�Ԥ��
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	//web���ʾƵ�Ԥ��
	if (action == kWebIntNewOrder)
	{
		return WebIntNewOrder(packet);
	}
	return 0;
}

//web���ھƵ�����
hotel::HotelCtrip::WebSearch(const char *packet)
{
	//��cookie �л������
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,'&','=');
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
//���� 1 
    if (referer.find("hotel") == string::npos)
    {
		//Ŀ�ĵ�
		string cityname = ms_->GetValueFromMapByKey("CityName",true);
		//��ס����
		string livedate = ms_->GetValueFromMapByKey("checkIn");
		//�˷�����
		string leavedate = ms_->GetValueFromMapByKey("checkOut");
		//�ؼ���
		string keyword = ms_->GetValueFromMapByKey("keywordNew",true);
		if (keyword.empty())
		{
			keyword = "null";
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
			"clientip=" + 	placelogin + "\r\n" + 	//�����ص�
			"netmark=" +	app_ + "\r\n" + 		//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
			"state=���ھƵ�����\r\n";				//״̬
		WriteIndexFile();
    }
//���� 2
	if (referer.find("hotel") != string::npos)
	{
		//Ŀ�ĵ�
		string cityname = ms_->GetValueFromMapByKey("cityName",true);
		//��ס����
		string livedate = ms_->GetValueFromMapByKey("StartTime");
		//�˷�����
		string leavedate = ms_->GetValueFromMapByKey("DepTime");
		//�ؼ���
		string keyword = ms_->GetValueFromMapByKey("txtkeyword",true);
		//����
		string longitude = ms_->GetValueFromMapByKey("cityLng");
		//γ��
		string latitude = ms_->GetValueFromMapByKey("cityLat");
		if (keyword.empty())
		{
			keyword = "null";
		}
		if (cityname.empty() || livedate.empty() || leavedate.empty() || longitude.empty() || latitude.empty())
		{
			return 0;
		}
		output_text_ =
			"servername=" + cityname + "\r\n" + 	//Ŀ�ĵ�
			"appname=" + 	livedate + "\r\n" + 	//��ס����
			"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
			"nick="	 + 	  	keyword + "\r\n" + 		//�ؼ���
			"from=" + 	  	longitude + "\r\n" + 	//���� 
			"to=" + 	  	latitude + "\r\n" + 	//γ��
			"clientip=" + 	placelogin + "\r\n" + 	//�����ص�
			"netmark=" +	app_ + "\r\n" + 		//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
			"state=���ھƵ�����\r\n";				//״̬
		WriteIndexFile();
	}
	return 1;
}

//web���ʾƵ�����
hotel::HotelCtrip::WebIntSearch(const char *packet)
{
	//��cookie �л������
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	//���� 1 
	if (referer == "http://hotels.ctrip.com/international/")
    {
		//Ŀ�ĵ�
		string cityname = ms_->GetValueFromMapByKey("CityName",true);
		//��ס����
		string livedate = ms_->GetValueFromMapByKey("checkIn");
		//�˷�����
		string leavedate = ms_->GetValueFromMapByKey("checkOut");
		//�ؼ���
		string keyword = ms_->GetValueFromMapByKey("keywordNew",true);
		if (keyword.empty())
		{
			keyword = "null";
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
			"clientip=" + 	placelogin + "\r\n" + 	//�����ص�
			"netmark=" +	app_ + "\r\n" + 		//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
			"state=���ʾƵ�����\r\n";				//״̬
		WriteIndexFile();
    }
	//���� 2
	if (referer != "http://hotels.ctrip.com/international/")
	{
		//Ŀ�ĵ�
		string cityname = ms_->GetValueFromMapByKey("cityName",true);
		//��ס����
		string livedate = ms_->GetValueFromMapByKey("StartTime");
		//�˷�����
		string leavedate = ms_->GetValueFromMapByKey("DepTime");
		//�ؼ���
		string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
		string keyword = uri.substr(uri.rfind("/") + strlen("/"));
		if (keyword.empty())
		{
			keyword = "null";
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
			"clientip=" + 	placelogin + "\r\n" + 	//�����ص�
			"netmark=" +	app_ + "\r\n" + 		//��Դ
			"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
			"state=���ʾƵ�����\r\n";				//״̬
		WriteIndexFile();
	}
	return 1;
}
//web���ھƵ�Ԥ��
hotel::HotelCtrip::WebNewOrder(const char *packet)
{
	//��cookie �л������
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�����ص�
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("Arrival");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("Departure");
	//ס������
	string guestname = ms_->GetValueFromMapByKey("GuestName",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("MobilePhone");
	//��ϵ���� 
	string email = ms_->GetValueFromMapByKey("ContactEmail");
	//��ϵ�̻� 
	string tel = ms_->GetValueFromMapByKey("ContactTelArea") + "-" + 
				 ms_->GetValueFromMapByKey("ContactTelNum");
	if (tel.empty())
	{
		tel = "null";
	}
	//�۸�
	string price = ms_->GetValueFromMapByKey("Price") + ms_->GetValueFromMapByKey("Currency");
	//��������
	string roomcount = ms_->GetValueFromMapByKey("selectedRoomCount");
	//��������
	string roomtype = ms_->GetValueFromMapByKey("RoomName",true);
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	guestname + "\r\n" + 	//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"onick=" + 	  	tel + "\r\n" + 			//��ϵ�̻�
		"mid=" + 	  	price + "\r\n" + 		//�۸�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"clientmac=" + 	roomtype + "\r\n" + 	//��������
		"clientip=" + 	placelogin + "\r\n" + 	//�����ص�
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}

//web���ʾƵ�Ԥ��
hotel::HotelCtrip::WebIntNewOrder(const char *packet)
{
	//��cookie �л������
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	//�����ص�
	string placelogin = ms_->GetValueFromMapByKey("adscityen");
	//���ʾƵ�
	//IntHotelCityID=73split%E6%96%B0%E5%8A%A0%E5%9D%A1%EF%BC%8C%E6%96%B0%E5%8A%A0%E5%9D%A1splitsingaporesplit2017-03-15split2017-03-16split2017-03-14%2014%3A49%3A51split0split2split2
	string strTemp = ms_->GetValueFromMapByKey("IntHotelCityID",true);
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("Arrival");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("Departure");
	//ס������
	string guestname = ms_->GetValueFromMapByKey("GuestName",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("MobilePhone");
	//��ϵ���� 
	string email = ms_->GetValueFromMapByKey("ContactEmail");
	//��ϵ�̻� 
	string tel = ms_->GetValueFromMapByKey("ContactTelArea") + "-" + 
		ms_->GetValueFromMapByKey("ContactTelNum");
	if (tel.empty())
	{
		tel = "null";
	}
	//�۸�
	string price = ms_->GetValueFromMapByKey("Price") + ms_->GetValueFromMapByKey("Currency");
	//��������
	string roomcount = ms_->GetValueFromMapByKey("selectedRoomCount");
	//��������
	string roomtype = ms_->GetValueFromMapByKey("RoomName",true);
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	guestname + "\r\n" + 	//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"onick=" + 	  	tel + "\r\n" + 			//��ϵ�̻�
		"mid=" + 	  	price + "\r\n" + 		//�۸�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"clientmac=" + 	roomtype + "\r\n" + 	//��������
		"clientip=" + 	placelogin + "\r\n" + 	//�����ص�
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ʾƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}