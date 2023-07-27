
#include "StdAfx.h"
#include "PlaneCaptialAirlines.h"

plane::CaptalAirLines::CaptalAirLines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::CaptalAirLines::~CaptalAirLines()
{
	
}
		
//NI ���֤ PP ��Ч���� JG ����֤ ID ����  --�׶�����
string plane::CaptalAirLines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "���֤";}
	if (idType == "PP"){idTypeTrue = "��Ч����";}
	if (idType == "JG"){idTypeTrue = "����֤";}
	if (idType == "ID"){idTypeTrue = "����";}
	return idTypeTrue;
}

int plane::CaptalAirLines::IsCaptalAirLines(HttpSession *http_session)
{	
	//���ڵ��� ���� ��ѯ
	if((http_session->m_Requestline.m_Host.Find("www.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/b2c-flight/searchflight.action")!=-1)) 
	{										
		return kpcticketcn;
	}	
	//���ʵ��� ���� ��ѯ
	if((http_session->m_Requestline.m_Host.Find("new.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jdair/ticket/airList")!=-1))
	{
		return kpcticketint;
	}	
	//���ڵ��� ���� �ύ����
	if((http_session->m_Requestline.m_Host.Find("www.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/b2c-flight/submitorder.action")!=-1) )
	{
		return kpcordercn;
	}
	//���ʵ��� ���� �ύ����
	if((http_session->m_Requestline.m_Host.Find("new.jdair.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jdair/ticket/flightOrder")!=-1) )
	{
		return kpcorderint;
	}
	return 0;
}

int plane::CaptalAirLines::Process(const char *packet,int action)
{
	//���ڵ��� ���� ��ѯ
	if (action == kpcticketcn)
	{
		return pcticketcn(packet);
	}
	//���ʵ��� ���� ��ѯ
	if(action == kpcticketint)
	{
		return pcticketint(packet);
	}
	//���ڵ��� ���� �ύ����
	if (action == kpcordercn)
	{
		return pcordercn(packet);
	}
	//���ʵ��� ���� �ύ����
	if (action == kpcorderint)
	{
		return pcorderint(packet);
	}
	return 1;
}
//���ڵ��� ���� ��ѯ
int plane::CaptalAirLines::pcticketcn(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//���� ���� 
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (tripType == "ONEWAY")
	{
		//������
		string orgCity = ms_->GetValueFromMapByKey("orgCity_m",true);
        //Ŀ�ĵ�
		string dstCity = ms_->GetValueFromMapByKey("dstCity_m",true);
		//��������
		string flightDate = ms_->GetValueFromMapByKey("flightDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //������
			"msg=" + 	dstCity + "\r\n" +    //Ŀ�ĵ� 
			"servername=" +flightDate + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}

	if (tripType == "ROUNDTRIP")
	{
		//������
		string orgCity = ms_->GetValueFromMapByKey("orgCity_m",true);
        //Ŀ�ĵ�
		string dstCity = ms_->GetValueFromMapByKey("dstCity_m",true);
		//��������
		string flightDate = ms_->GetValueFromMapByKey("flightDate",true);
		//��������
		string returnDate = ms_->GetValueFromMapByKey("returnDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //������
			"msg=" + 	dstCity + "\r\n" +    //Ŀ�ĵ� 
			"servername=" + flightDate + "\r\n" + //��������
			"appname=" + 	returnDate + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}

//���ʵ��� ���� ��ѯ
int plane::CaptalAirLines::pcticketint(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	//�ж�����ӻ��Ǹ���
	if (reader.parse(src_data,value))
	{	
		string tripType = value["tripType"].asString();
		if (tripType == "OW")
		{
			//��������
			string adtNum = value["adtNum"].asCString();
			//��ͯ����
			string chdNum = value["chdNum"].asCString();
			//������(������)
			string originCode = value["originCode"].asCString();
			//Ŀ�ĵ�(������)
			string destCode = value["destCode"].asCString();
			//��������
			string departureDate = value["departureDate"].asCString();
			output_text_ =
				"netmark=" + adtNum + "\r\n" +		//��������    
				"onick=" + 	chdNum + "\r\n" +		//��ͯ���� 
				"nick=" + 	originCode + "\r\n" +	//������
				"msg=" + 	destCode + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + departureDate + "\r\n" + //��������
				"state=��ѯ\r\n";
			WriteIndexFile();
		}
		if (tripType == "RT")
		{
			//��������
			string adtNum = value["adtNum"].asCString();
			//��ͯ����
			string chdNum = value["chdNum"].asCString();
			//������(������)
			string originCode = value["originCode"].asCString();
			//Ŀ�ĵ�(������)
			string destCode = value["destCode"].asCString();
			//��������
			string departureDate = value["departureDate"].asCString();
			//��������
			string returnDate = value["returnDate"].asCString();
			output_text_ =
				"netmark=" + adtNum + "\r\n" +		//��������    
				"onick=" + 	chdNum + "\r\n" +		//��ͯ���� 
				"nick=" + 	originCode + "\r\n" +	//������
				"msg=" + 	destCode + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + departureDate + "\r\n" + //��������
				"appname=" + returnDate + "\r\n" +	//��������
				"state=��ѯ\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}


//���ڵ��� ���� �ύ����
int plane::CaptalAirLines::pcordercn(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��ȡ�˿͵�����
	string temp = src_data.substr(src_data.rfind("passengerList"),src_data.find("&contactName") - src_data.rfind("passengerList"));
	string People = ms_->GetValueBetweenBeginEnd(temp,"[","]");
	int PassengerCount = atoi(People.c_str());
    string depCodes,arrCodes;
	//���� ����
	string segmentSize = ms_->GetValueFromMapByKey("segmentSize",true);
	if (segmentSize == "1") //����
	{ 
		//������(������)
		depCodes = ms_->GetValueFromMapByKey("depCodeS",true);
		//Ŀ�ĵ�(������)
		arrCodes = ms_->GetValueFromMapByKey("arrCodeS",true);
	}
	if (segmentSize == "2") //���� -- ���Ż�
	{ 
		string tempSegment = src_data.substr(src_data.find("cabinClass"),src_data.find("&passengerList") - src_data.find("cabinClass"));

		//������(������)
		depCodes = ms_->GetValueFromMapByKey("depCodeS",true);
		//Ŀ�ĵ�(������)
		arrCodes = ms_->GetValueFromMapByKey("arrCodeS",true);
	}
    //��ϵ������
	string contactName = ms_->GetValueFromMapByKey("contactName",true);
	//��ϵ�˵绰
	string contactMobile = ms_->GetValueFromMapByKey("contactMobile",true);

	for(int i =1;i<= PassengerCount;i++)
	{
		CString str;
		str.Format("%d",i);
		string strTemp = str.GetBuffer(0);
		//�˿�����
		string publicTemp = "passengerList";
		string passengerNameTemp = publicTemp + "[" + strTemp + "]" + ".name";
		string passengerName = ms_->GetValueFromMapByKey(passengerNameTemp,true);
		//�˿�����
		string remarkTemp = publicTemp + "[" + strTemp + "]" + ".remark";
		string remark = ms_->GetValueFromMapByKey(remarkTemp,true);

		string cateType,cateNo;
		if (remark == "ADULT" || remark == "CHILD")
		{
			//�˿�֤������
			string cateTypeTemp = publicTemp + "[" + strTemp + "]" + ".certificateType";
			cateType = ms_->GetValueFromMapByKey(cateTypeTemp,true);
			cateType = GetIdType(cateType);
			//�˿�֤����
			string cateNoTemp = publicTemp + "[" + strTemp + "]" + ".certificateNo";
			cateNo = ms_->GetValueFromMapByKey(cateNoTemp,true);
		}
		if (remark == "INFANT")
		{
			//�˿�֤������
			cateType = "��";
			//�˿�֤����
			string cateNoTemp = publicTemp + "[" + strTemp + "]" + ".birthday";
			cateNo = ms_->GetValueFromMapByKey(cateNoTemp,true);
		}
		//�˿͵绰
		string mobilePhoneTemp = publicTemp + "[" + strTemp + "]" + ".mobilePhone";
		string mobilePhone = ms_->GetValueFromMapByKey(mobilePhoneTemp,true);
		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"nick=" + 	depCodes + "\r\n" +			//������
			"msg=" + 	arrCodes + "\r\n" +			//Ŀ�ĵ� 
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"clientmsg=" + mobilePhone + "\r\n" +	//�˿͵绰
			"webname=" + cateType + "\r\n" +		//�˿�֤������
			"subject=" + cateNo + "\r\n" +			//�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	}
	return 1;
}

//���ʵ��� ���� �ύ����
int plane::CaptalAirLines::pcorderint(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
	//���� OW ���� RT
		string tripType = value["request.tripType"].asCString();
		if (tripType == "OW")
		{
			//��ϵ������
			string contactName = value["contactName"].asCString();
			//��ϵ�˵绰
			string contactTel = value["contactTel"].asCString();
			//��ϵ������
			string contactEmail = value["contactEmail"].asCString();
			//������(������)
			string originCode = value["request.originCode"].asCString();
			//Ŀ�ĵ�(������)
			string destCode = value["request.destCode"].asCString();
			//��������
			string departureDate = value["request.departureDate"].asCString();    
			
// 			//������Ϣ
// 			Json::Value flightSegments = value["flightSegments"];
// 			//��������(������ + վ̨)
// 			int k = 0;
// 			string departureAir = flightSegments[k]["departureAirport"].asString() + " " + flightSegments[k]["departureTerminal"].asString();
// 			//�������(������ + վ̨)
// 			string arrvalAir = flightSegments[k]["arrivalAirport"].asString() + " " + flightSegments[k]["arrivalTerminal"].asString();
// 			//�����
// 			string flightNumber = flightSegments[k]["flightNumber"].asCString();
			//�˿���Ϣ
			Json::Value passengers = value["passengers"];
			for (int i=0;i< passengers.size();i++)
			{
				//�˿�����
				string lastName = passengers[i]["lastName"].asCString();
				string firstName = passengers[i]["firstName"].asCString();
				string passengerName =  lastName + firstName;
				//�˿�֤������
				string certTypeTemp = passengers[i]["certType"].asCString();
				string certType = GetIdType(certTypeTemp);
				//�˿�֤����
				string certNo = passengers[i]["certNo"].asCString();
				output_text_ =
					"mid=" + 	  contactName + "\r\n" +	//��ϵ������
					"oid=" + 	  contactTel + "\r\n" +		//��ϵ�˵绰
					"imsi=" + 	  contactEmail + "\r\n" +	//��ϵ������
					"nick=" + 	  originCode + "\r\n" +		//������
					"msg=" + 	  destCode + "\r\n" +		//Ŀ�ĵ� 
					"msgtype=" +  passengerName + "\r\n" +	//�˿�����
					"webname=" +  certType + "\r\n" +		//�˿�֤������
					"netmark=" +  certNo + "\r\n" +			//�˿�֤����
					"servername=" + departureDate + "\r\n" +//��������
					"state=�ύ����\r\n";
				WriteIndexFile();
			}
		}
		if (tripType == "RT")
		{
			//��ϵ������
			string contactName = value["contactName"].asCString();
			//��ϵ�˵绰
			string contactTel = value["contactTel"].asCString();
			//��ϵ������
			string contactEmail = value["contactEmail"].asCString();
			//������(������)
			string originCode = value["request.originCode"].asCString();
			//Ŀ�ĵ�(������)
			string destCode = value["request.destCode"].asCString();
			//��������
			string departureDate = value["request.departureDate"].asCString();   
			//��������
			string departureDateRt = value["request.returnDate"].asCString(); 
			
			//������Ϣ
// 			Json::Value flightSegments = value["flightSegments"];
// 			int k = 0;
// 			//��������(������ + վ̨)
// 			string departureAir = flightSegments[k]["departureAirport"].asString() + " " + flightSegments[k]["departureTerminal"].asString();
// 			//�������(������ + վ̨)
// 			string arrvalAir = flightSegments[k]["arrivalAirport"].asString() + " " + flightSegments[k]["arrivalTerminal"].asString();
// 			//�����
// 			string flightNumber = flightSegments[k]["flightNumber"].asCString();
			//�˿���Ϣ
			Json::Value passengers = value["passengers"];
			for (int i=0;i<passengers.size();i++)
			{
				//�˿�����
				string lastName = passengers[i]["lastName"].asCString();
				string firstName = passengers[i]["firstName"].asCString();
				string passengerName =  lastName + firstName;
				//�˿�֤������
				string certTypeTemp = passengers[i]["certType"].asCString();
				string certType = GetIdType(certTypeTemp);
				//�˿�֤����
				string certNo = passengers[i]["certNo"].asCString();
				output_text_ =
					"mid=" + 	  contactName + "\r\n" +	//��ϵ������
					"oid=" + 	  contactTel + "\r\n" +		//��ϵ�˵绰
					"imsi=" + 	  contactEmail + "\r\n" +	//��ϵ������
					"nick=" + 	  originCode + "\r\n" +		//������
					"msg=" + 	  destCode + "\r\n" +		//Ŀ�ĵ� 
					"msgtype=" +  passengerName + "\r\n" +	//�˿�����
					"webname=" +  certType + "\r\n" +		//�˿�֤������
					"netmark=" +  certNo + "\r\n" +			//�˿�֤����
					"servername=" + departureDate + "\r\n" +//��������
					"appname=" + departureDateRt + "\r\n" + //��������
					"state=�ύ����\r\n";
				WriteIndexFile();
			}
		}
	}
	return 1;
}
