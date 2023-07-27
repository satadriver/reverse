// Yilong.cpp: implementation of the Yilong class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Yilong.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Yilong::Yilong(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

Yilong::~Yilong()
{

}

//�����֤������
string credential_type[6] = {"���֤","����","����֤","����֤","̨��֤","�۰�ͨ��֤"};


int Yilong::Is(HttpSession *http_session)
{
	// PC ��Ʊ
	if((http_session->m_Requestline.m_Host.Find("train.elong.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/isajax/Order/CreateOrder")!=-1))
	{
		return PC_TRAIN;
	}
	// PC �ɻ�Ʊ ���ڻ�Ʊ
	if((http_session->m_Requestline.m_Host.Find("flight.elong.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/isajax/FillOrder/SaveOrder")!=-1))
	{
		return PC_AIRPLANE;
	}
    // PC  �ɻ�Ʊ ���ʼ��۰�̨��Ʊ
	if((http_session->m_Requestline.m_Host.Find("iflight.elong.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/saveOrder")!=-1))
	{
		return PC_AIRPLANE_INTER;
	}
	return 0;
}

int Yilong::Process(const char *packet,int action)
{
	//���ڻ�Ʊ
	if (action == PC_AIRPLANE)
	{
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//��ϵ����������ϵ�˵绰��ʼ����(����)��Ŀ�ĵ�(����)����������ţ��������ڣ����̺���ţ��������ڣ��˿��������˿�Ʊ���ͣ��˿�֤�����ͣ��˿�֤���ţ��˿����գ�������ַ)	
		string Name,MobilePhone,FlightNumber,BackflightNumber,DepartDate,BackDate,DepartCityName,ArriveCityName;
		string PassengerName,PassengerType,PassengerCertType,PassengerCertNumber,PassengerBirthday,OrderUrl,FlightType;
		//��ȡ�˿͵�����
		string temp = sContent.substr(sContent.rfind("PassengerList"),sContent.length()-sContent.rfind("PassengerList"));
		int pos = temp.rfind("PassengerList");
		string People = temp.substr(pos+ strlen("PassengerList") + strlen("["),1);
	    int PassengerCount = atoi(People.c_str());
		//��ϵ������
		Name = ms_->GetValueFromMapByKey("Contact.Name",true);
		//��ϵ�˵绰
		MobilePhone = ms_->GetValueFromMapByKey("Contact.MobilePhone",true); 

		//ʼ����(����)
		DepartCityName = ms_->GetValueFromMapByKey("RequestInfo.DepartCityName",true); 
		//Ŀ�ĵ�(����))
		ArriveCityName = ms_->GetValueFromMapByKey("RequestInfo.ArriveCityName",true); 

		//��������
		FlightType = ms_->GetValueFromMapByKey("RequestInfo.FlightType",true);

		if (strcmp(FlightType.c_str(),"OneWay") == 0)
		{
			//����
			//���������
			FlightNumber = ms_->GetValueFromMapByKey("flightnum",true); 
			//��������
			DepartDate = ms_->GetValueFromMapByKey("RequestInfo.DepartDate",true); 

			type_   = "���ڵ��̻�Ʊ";
		}
		if (strcmp(FlightType.c_str(),"RoundTrip") == 0)
		{
			//����
			//���������
			FlightNumber = ms_->GetValueFromMapByKey("flightnumber",true); 
			//��������
			DepartDate = ms_->GetValueFromMapByKey("RequestInfo.DepartDate",true); 
			//���̺����
			BackflightNumber = ms_->GetValueFromMapByKey("backflightnumber",true);
			//��������
			BackDate = ms_->GetValueFromMapByKey("RequestInfo.BackDate",true); 

			type_   = "����������Ʊ";
		}
		
		//������ַ
		OrderUrl = ms_->GetValueFromMapByKey("Url",true); 

        for (int j=0;j < PassengerCount + 1;j++)
        {
			//ת��
			CString str;
			str.Format("%d",j);
			string strEnd = str.GetBuffer(0);
			//�˿�����
			string strTempName = "PassengerList[" + strEnd + "].Name";
			PassengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//�˿�Ʊ����	
			string strTempType = "PassengerList[" + strEnd + "].PassengerType";
			string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
			if (PassengerTypeTemp == "1")
			{
				PassengerType = "����Ʊ";
			}
			if (PassengerTypeTemp == "0")
			{
				PassengerType = "��ͯƱ";
			}
			//�˿�֤������
			string strCertType = "PassengerList[" + strEnd + "].CertType";
			string PassengerCertTypeTemp = ms_->GetValueFromMapByKey(strCertType,true); 
			int temp = atoi(PassengerCertTypeTemp.c_str());
			PassengerCertType = credential_type[temp];
			//�˿�֤����
			string strCertNumber = "PassengerList[" + strEnd + "].CertNumber";
			PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
			//�˿�����
			string strBirthday = "PassengerList[" + strEnd + "].Birthday";
			PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true);
			
			if (strcmp(FlightType.c_str(),"OneWay") == 0)
			{
				output_text_ = 
					"mid=" + 		Name + "\r\n" +					//��ϵ������
					"oid=" + 		MobilePhone + "\r\n" +			//��ϵ�˵绰
					"nick=" + 		DepartCityName + "\r\n" +		//������
					"msg=" + 		ArriveCityName + "\r\n" +		//Ŀ�ĵ� 
					"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
					"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
					"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
					"servername=" + DepartDate + "\r\n" +			//��������
					"islogin=" + 	FlightNumber + "\r\n" +			//�����
					"platform=���������\r\n" +
					"state=" + type_ + "\r\n";		
				WriteIndexFile();
			}
			if (strcmp(FlightType.c_str(),"RoundTrip") == 0)
			{
				output_text_ = 
					"mid=" + 		Name + "\r\n" +					//��ϵ������
					"oid=" + 		MobilePhone + "\r\n" +			//��ϵ�˵绰
					"nick=" + 		DepartCityName + "\r\n" +		//������
					"msg=" + 		ArriveCityName + "\r\n" +		//Ŀ�ĵ� 
					"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
					"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
					"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
					"servername=" + DepartDate + "\r\n" +			//��������
					"appname=" + 	BackDate + "\r\n" +				//��������
					"islogin=" + 	FlightNumber + "\r\n" +			//�����
					"platform=���������\r\n" +
					"state=" + type_ + "\r\n";	
				WriteIndexFile();
			}
        }		
		return 0;
	}
	//���ʼ��۰�̨��Ʊ  
	if (action == PC_AIRPLANE_INTER)
	{
		string sContent = ms_->UrlDecode(packet);   
		ms_->SplitStringToMap(sContent,'&','=');

		//���� (��ϵ����������ϵ�˵绰����ϵ�����䣬������(Ӣ����)��Ŀ�ĵ�(Ӣ����)����������) + ����
		//���� (��ϵ����������ϵ�˵绰����ϵ�����䣬������(Ӣ����)��Ŀ�ĵ�(Ӣ����)���������ڣ����̺���ţ���������) + ����
        //���� (�˿�����(Ӣ��)���˿����ͣ��˿͹���(����)���˿��Ա𣬳˿����գ��˿�֤�����ͣ��˿�֤���ţ��˿�֤����Ч��)	
	
		//��ȡ������Ϣ
		string InfoNessary;
		InfoNessary = ms_->GetValueFromMapByKey("orderDataStr",true);

		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(InfoNessary,value))
		{          
			//��ϵ����Ϣ
			string name,mobilePhone,email;
            Json::Value contact = value["contact"];
			//��ϵ������
			name = contact["name"].asString();
			//��ϵ�˵绰
			mobilePhone = contact["mobilePhone"].asString();
			//��ϵ������
			email = contact["email"].asString();

			//������Ϣ
			int flyType;
			string departCityNameEn,arriveCityNameEn,departDate,goFlightNo,backDate;
            Json::Value requestInfo = value["requestInfo"];
			//��������
			flyType = requestInfo["flyType"].asInt();

			if (flyType == 0)
			{
				//����
				//������
				departCityNameEn = requestInfo["departCityNameEn"].asString();
				//Ŀ�ĵ�
				arriveCityNameEn = requestInfo["arriveCityNameEn"].asString();
				//��������
				departDate = requestInfo["departDate"].asString();

				type_   = "���ʵ��̻�Ʊ";
			}
			if (flyType == 1)
			{
				//����
				//������
				departCityNameEn = requestInfo["departCityNameEn"].asString();
				//Ŀ�ĵ�
				arriveCityNameEn = requestInfo["arriveCityNameEn"].asString();
				//��������
				departDate = requestInfo["departDate"].asString();
				//���̺����
				goFlightNo = requestInfo["goFlightNo"].asString();
				//��������
				backDate = requestInfo["backDate"].asString();

				type_   = "����������Ʊ";
			}

			//�˿���Ϣ
			string PassengerTypeTemp,SexTemp,certTypeTemp;
			string Name,PassengerType,Country,Birthday,Sex,certType,CertNumber,CertPeriod;
			Json::Value passengerList = value["passengerList"];

			for (int k = 0;k <passengerList.size();k++)
			{
				Json::Value flightOrderPassengersPlit = passengerList[k];

				//�˿�����(Ӣ��)
				Name  = flightOrderPassengersPlit["Name"].asString();
				//�˿�����
				PassengerTypeTemp  = flightOrderPassengersPlit["PassengerType"].asString();
				if (PassengerTypeTemp == "0")
				{
					PassengerType = "����Ʊ";
				}
				if (PassengerTypeTemp == "1")
				{
					PassengerType = "��ͯƱ";
				}
				//�˿͹���(����)
				Country  = flightOrderPassengersPlit["Country"].asString();
				//�˿�����
				Birthday  = flightOrderPassengersPlit["Birthday"].asString();
				//�˿��Ա�
				SexTemp  = flightOrderPassengersPlit["Sex"].asString();
				if (SexTemp == "0")
				{
					Sex = "��";
				}
				if (SexTemp == "1")
				{
					Sex = "Ů";
				}
				//�˿�֤������
				certTypeTemp  = flightOrderPassengersPlit["certType"].asString();
				if (certTypeTemp == "2")
				{
					certType = "����";
				}
				if (certTypeTemp == "7")
				{
					certType = "����֤";
				}
				if (certTypeTemp == "8")
				{
					certType = "̨��֤";
				}
				if (certTypeTemp == "10")
				{
					certType = "�۰�ͨ��֤";
				}
				if (certTypeTemp == "22")
				{
					certType = "̨��ͨ��֤";
				}

				//�˿�֤����
				CertNumber  = flightOrderPassengersPlit["CertNumber"].asString();
				//�˿�֤����Ч��
				CertPeriod  = flightOrderPassengersPlit["CertPeriod"].asString();
				
				//����
				if(flyType == 0)
				{
					output_text_ = 
						"mid=" + 		name + "\r\n" +				//��ϵ������
						"oid=" + 		mobilePhone + "\r\n" +		//��ϵ�˵绰
						"nick=" + 		departCityNameEn + "\r\n" + //������
						"msg=" + 		arriveCityNameEn + "\r\n" + //Ŀ�ĵ� 
						"msgtype=" + 	Name + "\r\n" +				//�˿�����
						"webname=" + 	certType + "\r\n" +			//�˿�֤������
						"netmark=" + 	CertNumber + "\r\n" +		//�˿�֤����
						"servername=" + departDate + "\r\n" +		//��������
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";			
					WriteIndexFile();
				}
				//����
				if(flyType == 1)
				{
					output_text_ = 
						"mid=" + 		name + "\r\n" +				//��ϵ������
						"oid=" + 		mobilePhone + "\r\n" +		//��ϵ�˵绰
						"nick=" + 		departCityNameEn + "\r\n" + //������
						"msg=" + 		arriveCityNameEn + "\r\n" + //Ŀ�ĵ� 
						"msgtype=" + 	Name + "\r\n" +				//�˿�����
						"webname=" + 	certType + "\r\n" +			//�˿�֤������
						"netmark=" + 	CertNumber + "\r\n" +		//�˿�֤����
						"servername=" + departDate + "\r\n" +		//��������
						"appname=" + 	backDate + "\r\n" +			//��������
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
        }		
		return 0;
	}
	if (action == PC_TRAIN)
	{
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//(��ϵ�˵绰�����Σ��������ڣ�ʼ����(����)��Ŀ�ĵ�(����)���˿��������˿�Ʊ���ͣ��˿�֤�����ͣ��˿�֤���ţ��˿�����)
		string contactMobile,trainNumber,startDate,fromStationName,toStationName;
		string PassengerName,PassengerType,PassengerCertType,PassengerCertNumber,PassengerBirthday;	
		//��ȡ�˿͵�����
		string temp = sContent.substr(sContent.rfind("passengers"),sContent.length()-sContent.rfind("passengers"));
		int pos = temp.rfind("passengers");
		string People = temp.substr(pos+ strlen("passengers") + strlen("["),1);
		int PassengerCount = atoi(People.c_str());
		//��ϵ�˵绰
		contactMobile = ms_->GetValueFromMapByKey("contactMobile",true); 		
		//����
		trainNumber = ms_->GetValueFromMapByKey("trainNumber",true);	
		//��������
		startDate = ms_->GetValueFromMapByKey("startDate",true); 
		//ʼ����(����)
		fromStationName = ms_->GetValueFromMapByKey("fromStationName",true); 
		//Ŀ�ĵ�(����))
		toStationName = ms_->GetValueFromMapByKey("toStationName",true); 
		
		type_   = "��Ʊ";
        for (int j=0;j < PassengerCount + 1;j++)
        {
			//ת��
			CString str;
			str.Format("%d",j);
			string strEnd = str.GetBuffer(0);
			//�˿�����
			string strTempName = "passengers[" + strEnd + "].name";
			PassengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//�˿�Ʊ����
			string strTempType = "passengers[" + strEnd + "].passengerType";
			string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
			if (PassengerTypeTemp == "1")
			{
				PassengerType = "����Ʊ";
			}
			if (PassengerTypeTemp == "0")
			{
				PassengerType = "��ͯƱ";
			}
			//�˿�֤������
			string strCertType = "passengers[" + strEnd + "].certType";
			string PassengerCertTypeTemp = ms_->GetValueFromMapByKey(strCertType,true); 
			if (PassengerCertTypeTemp == "1")
			{
				PassengerCertType = "�������֤";
			}
			if (PassengerCertTypeTemp == "B")
			{
				PassengerCertType = "����";
			}
			if (PassengerCertTypeTemp == "C")
			{
				PassengerCertType = "�۰�ͨ��֤";
			}
			//�˿�֤����
			string strCertNumber = "passengers[" + strEnd + "].certNumber";
			PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
			//�˿�����
			string strBirthday = "passengers[" + strEnd + "].birthday";
			PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true); 
			
			output_text_ = 
				"oid=" + 		contactMobile + "\r\n" +		//��ϵ�˵绰
				"nick=" + 		fromStationName + "\r\n" +		//������
				"msg=" + 		toStationName + "\r\n" +		//Ŀ�ĵ� 
				"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
				"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
				"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
				"clientmsg=" + 	trainNumber + "\r\n" +			//���� 
				"servername=" + startDate + "\r\n" +			//��������
				"platform=���������\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
        }
		return 0;
	}
	return 0; 
}
