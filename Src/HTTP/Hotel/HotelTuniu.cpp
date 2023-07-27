#include "StdAfx.h"
#include "HotelTuniu.h"

hotel::HotelTuniu::HotelTuniu(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = ";ţ";
}

hotel::HotelTuniu::~HotelTuniu()
{

}

hotel::HotelTuniu::IsHotelTuniu(HttpSession *http_session)
{
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("hotel.tuniu.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/subscribe") != -1))	
	{
		return kWebNewOrder;
	}
	//web���ʾƵ�Ԥ�� 
	if( ( http_session->m_Requestline.m_Host.Find("globalhotel.tuniu.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/yii.php?r=/globalHotel/ajax/addOrder") != -1))	
	{
		return kWebIntNewOrder;
	}
	return 0;
}

hotel::HotelTuniu::Process(const char *packet,int action)
{
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

//web���ھƵ�Ԥ��
hotel::HotelTuniu::WebNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//�Ƶ�����
	string hotelname = ms_->GetValueFromMapByKey("hotelName",true);
	//�������� 
	string roomtype = ms_->GetValueFromMapByKey("roomName",true);
	//�������� 
	string roomcount = ms_->GetValueFromMapByKey("roomNum");
	//��ס���� 
	string livedate = ms_->GetValueFromMapByKey("checkInDate");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("checkOutDate");
	//ס������ 
	string name;
	int pos1 = src_data.find("contactName");
	int pos2 = src_data.find("&tel");
	string tempname = src_data.substr(pos1,pos2 - pos1);
	tempname = ms_->UTF8ToGBK(UrlDecode(tempname));
	while (tempname.find("&") != string::npos)
	{
		string strtemp = tempname.substr(0,tempname.find("&"));
		string str1 = strtemp.substr(strtemp.find("=") +1);
		name += str1;
		name += ",";
		tempname = tempname.substr(tempname.find("&") + 1);
	}
	name += tempname.substr(tempname.find("=") +1);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("intlCode") + ms_->GetValueFromMapByKey("tel");
	//��ϵ����
	string email = ms_->GetValueFromMapByKey("email",true);
	if (name.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" + 	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" +	name + "\r\n" + 		//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"webname=" + 	hotelname + "\r\n" +	//�Ƶ�����
		"clientmac=" + 	roomtype + "\r\n" +		//��������
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}

//web���ʾƵ�Ԥ��
hotel::HotelTuniu::WebIntNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string d = ms_->GetValueFromMapByKey("d",true);
	//��ס���� �˷����� ס������ ��ϵ�ֻ� ��ϵ���� ��������
	string livedate,leavedate,name,mobile,email,roomcount,totalprice;
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(d,value))
	{
		//��ס����
		livedate = value["checkInDate"].asString();
		//�˷�����
		leavedate = value["checkOutDate"].asString();
		//��ϵ�ֻ�
		mobile = value["contactPhoneNum"].asString();
		//��ϵ����
		email = value["contactEmail"].asString();
		//�۸�
		double price = value["totalPrice"].asDouble();
		CString strprice;
		strprice.Format("%f",price);
		totalprice = strprice.GetBuffer(strprice.GetLength());
		//��������
		int room = value["numberOfRooms"].asInt();
		CString strroom;
		strroom.Format("%d",room);
		roomcount = strroom.GetBuffer(strroom.GetLength());
		//ס������ 
		const Json::Value customerList = value["customerList"];
		for (int i=0;i<customerList.size();i++)
		{
			Json::Value customerInfo = customerList[i]["customerInfo"];
			for(int j=0;j<customerInfo.size();j++)
			{
				
				string tempname;
				tempname = customerInfo[j]["firstName"].asString() + customerInfo[j]["lastName"].asString();
				name += tempname;
				name += ",";
			}
		}
		if (name.rfind(",") != string::npos)
		{
			name = name.substr(0,name.rfind(","));
		}
	}
	if (name.empty() || mobile.empty() || email.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	name + "\r\n" + 		//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"mid=" + 	  	totalprice + "\r\n" + 	//�۸�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ʾƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}