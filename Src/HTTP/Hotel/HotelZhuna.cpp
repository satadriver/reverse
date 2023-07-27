#include "StdAfx.h"
#include "HotelZhuna.h"

hotel::HotelZhuna::HotelZhuna(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "ס�Ķ�";
}

hotel::HotelZhuna::~HotelZhuna()
{

}

int hotel::HotelZhuna::IsHotelZhuna(HttpSession *http_session)
{
	//webע��
	if (http_session->m_Requestline.m_Host.Find("www.zhuna.cn") != -1 &&		
		http_session->m_Requestline.m_URI.Find("/account/registerInsert/") != -1)
	{
		return kWebRegister;
	}
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("www.zhuna.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/order/post?webpath=&style=") != -1))	
	{
		return kWebNewOrder;
	}
	//android��¼
	if (http_session->m_Requestline.m_Host.Find("app.api.zhuna.cn") != -1 && 
		http_session->m_Requestline.m_URI.Find("/v30/member/userLogin.php") != -1)
	{
		return kAndroidLogin;
	}
	//android����
	if (http_session->m_Requestline.m_Host.Find("app.api.zhuna.cn") != -1 &&
		http_session->m_Requestline.m_URI.Find("/v20/postOrder.php") != -1)
	{
		return kAndroidOrder;
	}
	return 0;
}

int hotel::HotelZhuna::Process(const char *packet,int action)
{
	//webע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web���ھƵ�Ԥ��
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	//android��¼
	if (action == kAndroidLogin)
	{
		return AndroidLogin(packet);
	}
	//android����
	if (action == kAndroidOrder)
	{
		return AndroidOrder(packet);
	}
	return 0;
}
//webע��
int hotel::HotelZhuna::WebRegister(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("phone") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("phone"));
	}
	ms_->SplitStringToMap(src_data,"&","=");
	//�û���
	string username = ms_->GetValueFromMapByKey("phone");
	//����
	string password = ms_->GetValueFromMapByKey("pwd");
	string repassword = ms_->GetValueFromMapByKey("r_pwd");

	if (password != repassword)
	{
		return -1;
	}
	if (username.empty() || repassword.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +		//�û���
		"pass=" + 		password + "\r\n" +		//����	
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=ע��\r\n";	
	WriteIndexFile();
	return 1;
}
//web���ھƵ�Ԥ��
int hotel::HotelZhuna::WebNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("roomnum") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("roomnum"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	
	//��ס����
	string livedate = ms_->GetValueFromMapByKey("ordertmp[CheckInDate]");
	//�˷�����
	string leavedate = ms_->GetValueFromMapByKey("ordertmp[CheckOutDate]");
	//ס������
	string name;
    string tempname = ms_->GetValueBetweenBeginEnd(src_data,"g_name","c_mobile");
	while (tempname.find("&") != string::npos)
	{
		string strtemp = tempname.substr(0,tempname.find("&"));
		string str1 = strtemp.substr(strtemp.find("=") +1);
		name += str1;
		name += ",";
		tempname = tempname.substr(tempname.find("&") + 1);
	}
	name += tempname.substr(tempname.find("=") +1);
	if (name.rfind(",") != string::npos)
	{
		name = name.substr(0,name.rfind(","));
	}
	//��ϵ�ֻ�
	string mobile = ms_->GetValueFromMapByKey("c_mobile");
	//��ϵ����
	string email = ms_->GetValueFromMapByKey("c_email");
	if (email.empty())
	{
		email = "null";
	}
	//�Ƶ�����
	string hotelname = ms_->GetValueFromMapByKey("z[hotelname]",true);
	//�������� 
	string roomtype = ms_->GetValueFromMapByKey("ordertmp[RoomName]",true);
	//�������� 
	string roomcount = ms_->GetValueFromMapByKey("roomnum");
    if (name.empty() || mobile.empty())
    {
		return -1;
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

//android��¼
int hotel::HotelZhuna::AndroidLogin(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("password") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("password"));
	}
    ms_->SplitStringToMap(src_data,"&","=");
	//�û���
	string username = ms_->GetValueFromMapByKey("mobile");
	//����
	string password = ms_->GetValueFromMapByKey("password");
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +			//�û���
		"pass=" + 		password + "\r\n" + 		//����
		"netmark=" +	app_ + "\r\n" + 			//��Դ
		"clientmsg=" +  platformAndroid + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";							//״̬
	WriteIndexFile();
	return 1;
}
//android����
int hotel::HotelZhuna::AndroidOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//��ס����
    string livedate = ms_->GetValueFromMapByKey("tm1");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("tm2");
	//ס������
	string name = ms_->GetValueFromMapByKey("guestname");
	//��ϵ�ֻ�
	string mobile = ms_->GetValueFromMapByKey("mobile");
	//��������
	string roomcount = ms_->GetValueFromMapByKey("roomNum");
	if (name.empty() || mobile.empty())
	{
		return -1;
	}
	output_text_ =
		"appname=" + 	  	livedate + "\r\n" + 		//��ס����
		"msgtype=" + 	 	leavedate + "\r\n" +		//�˷�����
		"subject=" + 	  	name + "\r\n" + 			//ס������
		"islogin=" + 	  	mobile + "\r\n" + 			//��ϵ�ֻ�
		"fileurl=" + 	  	roomcount + "\r\n" + 		//��������
		"netmark=" +		app_ + "\r\n" + 			//��Դ
		"clientmsg=" +		platformAndroid + "\r\n" +	//����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}