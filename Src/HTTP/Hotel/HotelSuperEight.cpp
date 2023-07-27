#include "StdAfx.h"
#include "HotelSuperEight.h"

hotel::HotelSuperEight::HotelSuperEight(HttpSession *http_session, const string &pro_name):Hotel(http_session,pro_name)
{
	app_ = "�ٰ˾Ƶ�";
}

hotel::HotelSuperEight::~HotelSuperEight()
{

}

hotel::HotelSuperEight::IsHotelSuperEight(HttpSession *http_session)
{
	//webע��
	if( ( http_session->m_Requestline.m_Host.Find("www.super8.com.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Register/RegisterSm") != -1 ))	
	{
		return kWebRegister;
	}
	//web��¼
	if( ( http_session->m_Requestline.m_Host.Find("www.super8.com.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/MemInfo/Index") != -1 ))	
	{
		return kWebLogin;
	}
	//web���ھƵ�Ԥ��
	if( ( http_session->m_Requestline.m_Host.Find("www.super8.com.cn") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/Book/ToPay") != -1))	
	{
		return kWebNewOrder;
	}
	return 0;
}

hotel::HotelSuperEight::Process(const char *packet,int action)
{
	//webע��
	if (action == kWebRegister)
	{
		return WebRegister(packet);
	}
	//web��¼
	if (action == kWebLogin)
	{
		return WebLogin(packet);
	}
	//web���ھƵ�Ԥ��
	if (action == kWebNewOrder)
	{
		return WebNewOrder(packet);
	}
	return 0;
}
//webע��
hotel::HotelSuperEight::WebRegister(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string name = ms_->GetValueFromMapByKey("CustomeName",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("PhoneNum");
	//�Ա� 
	string sex = ms_->GetValueFromMapByKey("Gender",true);	
	if (sex == "1")
	{
		sex = "��";
	}
	else if (sex == "2")
	{
		sex = "Ů";
	}
	if (name.empty() || mobile.empty() || sex.empty())
	{
		return -1;
	}
	output_text_ =
		"wifissid=" + 	name + "\r\n" + 		//����
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"sex=" +		sex + "\r\n" +			//�Ա�
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=ע��\r\n";						//״̬
	WriteIndexFile();
	return 1;
}
//web��¼
hotel::HotelSuperEight::WebLogin(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	if (src_data.rfind("LoginName") != string::npos)
	{
		src_data = src_data.substr(src_data.rfind("LoginName"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
    string username = ms_->GetValueFromMapByKey("LoginName",true);
	string password = ms_->GetValueFromMapByKey("PassWd");

	if (username.empty() || password.empty())
	{
		return -1;
	}
	output_text_ =
		"user=" + 		username + "\r\n" +	    //�û���
		"pass=" + 		password + "\r\n" +		//����
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=��¼\r\n";						//״̬
	WriteIndexFile();
	return 1;
}

//web���ھƵ�Ԥ��
hotel::HotelSuperEight::WebNewOrder(const char *packet)
{
	//���ݲ���
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ס���� 
	string livedate = ms_->GetValueFromMapByKey("sdate");
	//�˷����� 
	string leavedate = ms_->GetValueFromMapByKey("edate");
	//ס������ 
	string name = ms_->GetValueFromMapByKey("gstname",true);
	//��ϵ����
	string contactname = ms_->GetValueFromMapByKey("conname",true);
	//��ϵ�ֻ� 
	string mobile = ms_->GetValueFromMapByKey("conmoblie");
	//��ϵ���� 
	string email = ms_->GetValueFromMapByKey("conemail",true);
	if (email.empty())
	{
		email = "δ��д";
	}
	//��������
	string roomcount = ms_->GetValueFromMapByKey("roomnum");

	if (name.empty() || contactname.empty() || mobile.empty())
	{
		return 0;
	}
	output_text_ =
		"appname=" + 	livedate + "\r\n" + 	//��ס����
		"msgtype=" + 	leavedate + "\r\n" +	//�˷�����
		"subject=" + 	name + "\r\n" + 		//ס������
		"clientpcname=" + contactname + "\r\n" +//��ϵ����
		"islogin=" + 	mobile + "\r\n" + 		//��ϵ�ֻ�
		"oid=" + 	  	email + "\r\n" + 		//��ϵ����
		"fileurl=" + 	roomcount + "\r\n" + 	//��������
		"netmark=" +	app_ + "\r\n" + 		//��Դ
		"clientmsg=" +  platformWeb + "\r\n" +  //����ƽ̨
		"state=���ھƵ�Ԥ��\r\n";				//״̬
	WriteIndexFile();
	return 1;
}
