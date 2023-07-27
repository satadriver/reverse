#include "StdAfx.h"
#include "HotelRujia.h"

hotel::HotelRujia::HotelRujia(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "���";
}

hotel::HotelRujia::~HotelRujia()
{

}

hotel::HotelRujia::IsHotelRujia(HttpSession *http_session)
{
	//webע��
	if( ( http_session->m_Requestline.m_Host.Find("www.homeinns.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/member/register") != -1 ))	
	{
		return kWebRegister;
	}
	//web���ھƵ�����
	if( http_session->m_Requestline.m_Host.Find("www.homeinns.com") != -1 && 
		(http_session->m_Requestline.m_URI.Find("/list/") != -1 || http_session->m_Requestline.m_URI.Find("/map/") != -1))	
	{

		return kWebSearch;
	}
	//web���ھƵ�Ԥ�� 1 
	if( http_session->m_Requestline.m_Host.Find("www.homeinns.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/order/OrderCreateSubmit") != -1)	
	{
		return kWebNewOrder;
	}
	//web���ھƵ�Ԥ�� 2
	if( http_session->m_Requestline.m_Host.Find("www.bthhotels.com") != -1 && 
		http_session->m_Requestline.m_URI.Find("/order/OrderCreateSubmit") != -1)	
	{
		return kWebNewOrder;
	}
	//web���ʾƵ�Ԥ�� ���������
	if( ( http_session->m_Requestline.m_Host.Find("ihotel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/submit") != -1))	
	{
		return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelRujia::Process(const char *packet,int action)
{
	//webע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
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
//webע��
hotel::HotelRujia::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("TrueName") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("TrueName"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("TrueName",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//���� 
	string password = ms_->GetValueFromMapByKey("password");
	string password2 = ms_->GetValueFromMapByKey("password2");
	if (password != password2)
	{
		return 0;
	}
	//�Ա� 
	string sex = ms_->GetValueFromMapByKey("sex",true);	
	//��ϵ����
	string email = ms_->GetValueFromMapByKey("email",true);
    if (mobile.empty() || password.empty())
    {
		return 0;
    }
	output_text_ =
		"wifissid=" + 	name + "\r\n" + 		//����
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"pass=" + 		password + "\r\n" +		//����
		"sex=" +		sex + "\r\n" +			//�Ա�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}
//web���ھƵ�����
hotel::HotelRujia::WebSearch(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("cityName_Chinese") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("cityName_Chinese"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//Ŀ�ĵ�
	string cityname = ms_->GetValueFromMapByKey("cityName_Chinese",true);
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("beginDate");
	//�˷�����
	string leavedate = ms_->GetValueFromMapByKey("endDate");
	//�ؼ���
	string keyword = ms_->GetValueFromMapByKey("keyDescript",true);
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
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�����\r\n";				//״̬
	WriteIndexFile();
	return 1;
}

//web���ھƵ�Ԥ��
hotel::HotelRujia::WebNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//Ŀ�ĵ�
	string cityname = ms_->GetValueFromMapByKey("HotelNm",true);
	//��ס���� 
	string livedate = ms_->GetValueFromMapByKey("s_ArrDate");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("s_DepDate");
	//ס������ 
	string name = ms_->GetValueFromMapByKey("s_GustNm",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("s_Tel");
	//��ϵ���� 
	string email = ms_->GetValueFromMapByKey("s_Email");
	if (email.empty())
	{
		email = "null";
	}
	//��������
	string roomcount = ms_->GetValueFromMapByKey("s_RmNum");
	if (cityname.empty() || livedate.empty() || leavedate.empty() || name.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" +		//Ŀ�ĵ�
		"appname=" + 	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	name + "\r\n" + 		//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}

//web���ʾƵ�Ԥ��
hotel::HotelRujia::WebIntNewOrder(const char *packet)
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