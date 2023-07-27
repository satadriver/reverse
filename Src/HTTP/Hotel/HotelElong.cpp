#include "StdAfx.h"
#include "HotelElong.h"

hotel::HotelElong::HotelElong(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "����";
}

hotel::HotelElong::~HotelElong()
{

}

hotel::HotelElong::IsHotelElong(HttpSession *http_session)
{
	//web���ھƵ�����
	if( ( http_session->m_Requestline.m_Host.Find("hotel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/ajax/list/getpageonedata") != -1 ))	
	{
		//return kWebSearch;
	}
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("hotels.ctrip.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("hotel.elong.com") != -1))	
	{
		//return kWebNewOrder;
	}
	//web���ʾƵ�Ԥ�� ���������
	if( ( http_session->m_Requestline.m_Host.Find("ihotel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/submit") != -1))	
	{
		return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelElong::Process(const char *packet,int action)
{
	//web���ھƵ�����
	if (action == kWebSearch)
	{
		return WebSearch(packet);
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
hotel::HotelElong::WebSearch(const char *packet)
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

//web���ھƵ�Ԥ��
hotel::HotelElong::WebNewOrder(const char *packet)
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
hotel::HotelElong::WebIntNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//Ŀ�ĵ� ��ס���� �˷����� ס������ ��ϵ�ֻ� ��ϵ���� ���� γ�� �������� �������� �۸�

	string cityname,livedate,leavedate,name,mobile,email,longitude,latitude,roomcount,roomtype,price;
	//��ס����
	livedate = ms_->GetValueFromMapByKey("check_in");
	//�˷����� 
	leavedate = ms_->GetValueFromMapByKey("check_out");
	//��������
	roomcount = ms_->GetValueFromMapByKey("room_num");

	//������Ϣ
	string orderData = ms_->GetValueFromMapByKey("orderData",true);
	//
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(orderData,value))
	{
		//�۸�
		Json::Value paymentInfo = value["paymentInfo"];
		double dbprice = paymentInfo["roomAmount"].asDouble(); 
		CString strPrice;
		strPrice.Format("%f",dbprice);
		price = strPrice.GetBuffer(0);

		//��ϵ��Ϣ
		Json::Value contactPerson = value["contactPerson"];
		//��ϵ�ֻ�
		mobile = contactPerson["mobileTelephone"].asString();
		//��ϵ����
		email = contactPerson["email"].asString();
		//������Ϣ
		Json::Value interHotelOrder = value["interHotelOrder"];
		Json::Value interHotelProduct = interHotelOrder["interHotelProducts"];
		for(int j= 0;j < interHotelProduct.size();j++)
		{
			//Ŀ�ĵ�
			cityname = interHotelProduct[j]["hotelCountry"].asString() +
							  interHotelProduct[j]["hotelCity"].asString() +
							  interHotelProduct[j]["hotelName"].asString();
			//���� 
			longitude = interHotelProduct[j]["hotelLongitude"].asString();
			//γ��
			latitude = interHotelProduct[j]["hotelLatitude"].asString();
			//��������
			roomtype = interHotelProduct[j]["addValues"].asString();
		}
		//ס����Ϣ
		Json::Value travelers = interHotelOrder["travelers"];
		for (int k = 0;k< travelers.size();k++)
		{
			string tempName;
			tempName = travelers[k]["firstname"].asString() + travelers[k]["surname"].asString();
			name += tempName;
			name += ",";
		}
	}
	if (cityname.empty() || livedate.empty() || leavedate.empty() || name.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//Ŀ�ĵ�
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	name + "\r\n" + 		//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"from=" + 	  	longitude + "\r\n" + 	//���� 
		"to=" + 	  	latitude + "\r\n" + 	//γ��
		"mid=" + 	  	price + "\r\n" + 		//�۸�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"clientmac=" + 	roomtype + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ʾƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}