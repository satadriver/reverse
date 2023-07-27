// Lvmamalvyou.cpp: implementation of the Lvmamalvyou class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Lvmamalvyou.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Lvmamalvyou::Lvmamalvyou(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

Lvmamalvyou::~Lvmamalvyou()
{

}

//�����֤������
char* credential_type[][2] = 
{{"ID","���֤"},
{"ID_CARD","���֤"},
{"PASSPORT","����"},
{"GANGAO","�۰�ͨ��֤"},
{"HUIXIANG","����֤"},
{"TAIBAO","̨��ͨ��֤"},
{"OFFICER","����֤"},
{"SOLDIER","ʿ��֤"},
{"TAIBAOZHENG","̨��֤"},
{"OTHER","����"}};

//��ȡ֤������
string Lvmamalvyou::GetCredentialType(string passengerIDCardTypeTemp)
{
	for (int iCode = 0; iCode < sizeof(credential_type)/sizeof(credential_type[0]); iCode ++)
	{
		if (passengerIDCardTypeTemp.compare(credential_type[iCode][0]) == 0)
		{
			return credential_type[iCode][1];;
		}
	}
	return passengerIDCardTypeTemp;
}

//ʵ��
void Lvmamalvyou::SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
		
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


int Lvmamalvyou::Is(HttpSession *http_session)
{
	//pc ��¼
	if((http_session->m_Requestline.m_Host.Find("login.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/nsso/login")!=-1))
	{
		return PC_LOGIN;
	}
	// PC ��Ʊ
	if((http_session->m_Requestline.m_Host.Find("train.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/book/booking")!=-1))
	{
		return PC_TRAIN;
	}
	// PC �ɻ�Ʊ
	if((http_session->m_Requestline.m_Host.Find("flight.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/submitOrder")!=-1))
	{
		return PC_AIRPLANE;
	}
	// SP ��Ʊ
	if((http_session->m_Requestline.m_Host.Find("m.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/new/api/router/rest.do?method=api.com.train.order.submitorder")!=-1))
	{
		return SP_TRAIN;
	}
	// SP �ɻ�Ʊ
	if((http_session->m_Requestline.m_Host.Find("m.lvmama.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/client-service/router/rest.do?method=api.com.flight.order.booking")!=-1))
	{
		return SP_AIRPLANE;
	}

	return 0;
}


int Lvmamalvyou::Process(const char *packet,int action)
{
	//pc ��¼
	if (action == PC_LOGIN)
	{
		string src_data(packet);
		src_data=ms_->UrlDecode(src_data);
		ms_->SplitStringToMap(src_data,'&','=');
		string user,pass;
		user=ms_->GetValueFromMapByKey("username",true);
		pass=ms_->GetValueFromMapByKey("password",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=��¼\r\n";	
		WriteIndexFile();
	}
	//�ɻ�Ʊ
	if (action == PC_AIRPLANE)
	{	
		//����-��ϵ�˵绰����ϵ�������������أ�Ŀ�ĵأ� �����������ﵽ����������ţ��������ڣ�
		//����-��ϵ�˵绰����ϵ�������������أ�Ŀ�ĵأ� ȥ�̳���������ȥ�̴ﵽ������ȥ�̺���ţ�ȥ�����ڣ����̳������������̵�����������̺���ţ��������ڣ�
        //����-�˿��������˿����ͣ��˿�֤�����ͣ��˿��������˿�֤�����ͣ��˿�֤�����룬�˿����գ��ܼ۸�
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(sContent,value))
		{          
			//��ϵ����Ϣ
            Json::Value flightOrderContacter = value["flightOrderContacter"];
			//��ϵ������
			name_    = flightOrderContacter["name"].asString();
			//��ϵ�˵绰
			tel_     = flightOrderContacter["cellphone"].asString();

			//OW ���̻�Ʊ RT ������Ʊ 
			string routeTypeTemp,routeType;
			routeTypeTemp  = value["routeType"].asString();
			if (routeTypeTemp == "OW")
			{
				routeType == "���̻�Ʊ";
			}
			if (routeTypeTemp == "RT")
			{
				routeType == "������Ʊ";
			}
			//������
			start_ = value["deptCityName"].asString();
			//Ŀ�ĵ�
			end_  = value["arrvCityName"].asString();

            //������Ϣ
			string flightTripType,seatClassCode;
			string departureAirOw,arrivalAirOw,flightNoOw,departureDateOw;
			string departureAirRt,arrivalAirRt,flightNoRt,departureDateRt;

			//ȥ��
			if (routeTypeTemp == "OW")
			{
				Json::Value flightOrderBookingDetailRequests = value["flightOrderBookingDetailRequests"];
				//ע�� ����ط���json Ŀǰֻ��һ�� 0 ȥ�� 1 ����
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = flightOrderBookingDetailRequests[f];
				//��������
				departureAirOw = value["owdepartureAirName"].asString() + value["owdepartureAirBulding"].asString();
				//�ﵽ����
				arrivalAirOw  = value["owarrivalAirName"].asString() + value["owarrivalAirBulding"].asString();
				//�����
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//��������
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();

				type_   = "���ڵ��̻�Ʊ";
			} 
			//����
			if(routeTypeTemp == "RT")
			{
				Json::Value flightOrderBookingDetailRequests = value["flightOrderBookingDetailRequests"];
				//ע�� ����ط���json Ŀǰֻ��һ�� 0 ȥ�� 1 ����
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = flightOrderBookingDetailRequests[f];
				//ȥ�̳�������
				departureAirOw = value["owdepartureAirName"].asString() + value["owdepartureAirBulding"].asString();
				//ȥ�̴ﵽ����
				arrivalAirOw  = value["owarrivalAirName"].asString() + value["owarrivalAirBulding"].asString();
				//ȥ�̺����
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//ȥ������
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();
				
				int g = 1;
				Json::Value flightOrderBookingDetailRequestsPlitRT = flightOrderBookingDetailRequests[g];
				//���̳�������
				departureAirRt = value["rtdepartureAirName"].asString() + value["rtdepartureAirBulding"].asString();	
				//���̴ﵽ����
				arrivalAirRt  = value["rtarrivalAirName"].asString() + value["rtarrivalAirBulding"].asString();
				//���̺����
				flightNoRt = flightOrderBookingDetailRequestsPlitRT["flightNo"].asString();
				//��������
				departureDateRt  = flightOrderBookingDetailRequestsPlitRT["departureDate"].asString();

				type_   = "����������Ʊ";
			}

			//�ܼ۸�
			price_  = value["orderTotalSalesAmount"].asString();

			//�˿���Ϣ
			string strTemp,passengerName,passengerIDCardType,passengerIDCardTypeTemp,passengerIDCardNo,gender,passengerType,passengerBirthday;
			Json::Value flightOrderPassengers = value["flightOrderPassengers"];
			for (int k = 0;k <flightOrderPassengers.size();k++)
			{
				Json::Value flightOrderPassengersPlit = flightOrderPassengers[k];
				//�˿�����
				passengerName  = flightOrderPassengersPlit["passengerName"].asString();
				//�˿�֤������
				passengerIDCardTypeTemp = flightOrderPassengersPlit["passengerIDCardType"].asString();
				passengerIDCardType = GetCredentialType(passengerIDCardTypeTemp);
				//�˿�֤������
				passengerIDCardNo = flightOrderPassengersPlit["passengerIDCardNo"].asString();
				//�˿��Ա�
				gender = flightOrderPassengersPlit["gender"].asString();
				//�˿�����
				passengerType = flightOrderPassengersPlit["passengerType"].asString();
				//�˿�����
				passengerBirthday = flightOrderPassengersPlit["passengerBirthday"].asString();
		
				//����
				if(routeTypeTemp == "OW")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//��ϵ������
						"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
						"nick=" + 		start_ + "\r\n" +				//������
						"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
						"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
						"webname=" + 	passengerIDCardType + "\r\n" +	//�˿�֤������
						"netmark=" + 	passengerIDCardNo + "\r\n" +	//�˿�֤����
						"onick=" + 		departureAirOw + "\r\n" +		//��������
						"lang=" + 		arrivalAirOw + "\r\n" +			//�������
						"servername=" + departureDateOw + "\r\n" +		//��������
						"islogin=" + 	flightNoOw + "\r\n" +			//�����
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				//����
				if(routeTypeTemp == "RT")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//��ϵ������
						"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
						"nick=" + 		start_ + "\r\n" +				//������
						"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
						"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
						"webname=" + 	passengerIDCardType + "\r\n" +	//�˿�֤������
						"netmark=" + 	passengerIDCardNo + "\r\n" +	//�˿�֤����
						"onick=" + 		departureAirOw + "\r\n" +		//��������
						"mid=" + 		arrivalAirOw + "\r\n" +			//�������
						"servername=" + departureDateOw + "\r\n" +		//��������
						"appname=" + 	departureDateRt + "\r\n" +		//��������
						"islogin=" + 	flightNoOw + "\r\n" +			//�����
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";	
					WriteIndexFile();
				}
			}
		}
		return 0;
	}
    //��Ʊ
	if (action == PC_TRAIN)
	{
		//��ϵ�˵绰����ϵ�����������Σ����ţ�ʼ����(����)��Ŀ�ĵ�(����)������ʱ�䣬��վʱ�䣬�˿��������˿����ͣ��˿�֤�����ͣ��˿����գ��ܼ۸�)
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(sContent,value))
		{          
			//��ϵ������
			name_    = value["contact"].asString();
			//��ϵ�˵绰
			tel_     = value["contactphone"].asString();
			//������Ϣ
			string trainNo;
			//����
			number_ = value["traincode"].asString();
			//����
			trainNo = value["trainNo"].asString();
			//ʼ����(����)
			start_   = value["fromstationname"].asString();
			//Ŀ�ĵ�(����)
			end_     = value["tostationname"].asString();
			//����ʱ��  
			time_ = value["startdate"].asString() + " " + value["fromtime"].asString();
			//��վʱ��
			string arriveTime;  
			arriveTime = value["arrivedate"].asString() + " " + value["totime"].asString();
			//�ܼ۸�
			price_  = value["totalprice"].asString();
			type_   = "��Ʊ";

			//�˿���Ϣ
			string strTemp,passengerName,passengerType,passengerIDCardType,passengerIDCardNo,passengerBirthday;   
			Json::Value passengers = value["passengers"];
			for (int k = 0;k <passengers.size();k++)
			{
				Json::Value passengersPlit = passengers[k];
				//�˿�����
				passengerName  = passengersPlit["passengerName"].asString();
				//�˿�����
				passengerType = passengersPlit["passengerType"].asString();
				//�˿�֤������
				passengerIDCardType = passengersPlit["cardType"].asString();
				if (passengerIDCardType == "ID_CARD")
				{
					passengerIDCardType = "���֤";
				}
				if (passengerIDCardType == "GANGAO")
				{
					passengerIDCardType = "�۰�ͨ��֤";
				}
				if (passengerIDCardType == "TAIBAO")
				{
					passengerIDCardType = "̨��ͨ��֤";
				}
				if (passengerIDCardType == "HUZHAO")
				{
					passengerIDCardType = "����";
				}
				//�˿�֤������
				passengerIDCardNo = passengersPlit["cardNumbers"].asString();
				//�˿�����
				passengerBirthday = passengersPlit["passengerBirthday"].asString();

				output_text_ = 
					"mid=" + 		name_ + "\r\n" +				//��ϵ������
					"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
					"nick=" + 		start_ + "\r\n" +				//������
					"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
					"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
					"webname=" + 	passengerIDCardType + "\r\n" +	//�˿�֤������
					"netmark=" + 	passengerIDCardNo + "\r\n" +	//�˿�֤����
					"servername=" + time_ + "\r\n" +				//��������
					"platform=���������\r\n" +
					"state=" + type_ + "\r\n";		
				WriteIndexFile();
			}
		}		
		return 0;
	}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ɻ�Ʊ
	if (action == SP_AIRPLANE)
	{

		//���� (��ϵ�˵绰����ϵ����������������(����)��Ŀ�Ļ���(����)������ţ���������) + ����
		//���� (��ϵ�˵绰����ϵ��������ȥ�̳�������(����)��ȥ��Ŀ�Ļ���(����)��ȥ�̺���ţ�ȥ�����ڣ����̳������������̵�����������̺���ţ���������) + ����
		//���� (�˿��������˿����ͣ��˿�֤�����ͣ��˿����գ��ܼ۸�)
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
		//��ȡ��������
		string sContent;
		sContent = ms_->GetValueFromMapByKey("param",true); 
		//ʹ��json ��ȡ����
		Json::Reader reader;
		Json::Value value; 
		if (reader.parse(sContent,value))
		{          
			  
			//��ϵ����Ϣ
            Json::Value orderContacter = value["orderContacter"];
			//��ϵ������
			name_    = orderContacter["name"].asString();
			//��ϵ�˵绰
			tel_     = orderContacter["cellphone"].asString();
			
			//OW ���̻�Ʊ RT ������Ʊ 
			string routeTypeTemp,routeType;
			routeTypeTemp  = value["routeType"].asString();
			if (routeTypeTemp == "OW")
			{
				routeType == "���̻�Ʊ";
			}
			if (routeTypeTemp == "RT")
			{
				routeType == "������Ʊ";
			}

			//������Ϣ
			string flightTripType,seatClassCode;
			string departureAirOw,arrivalAirOw,flightNoOw,departureDateOw;
			string departureAirRt,arrivalAirRt,flightNoRt,departureDateRt;
			
			//ȥ��
			if (routeTypeTemp == "OW")
			{
				Json::Value orderBookingDetailRequests = value["orderBookingDetailRequests"];
				//ע�� ����ط���json Ŀǰֻ��һ�� 0 ȥ�� 1 ����
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = orderBookingDetailRequests[f];
				//ȥ�̳�������
				departureAirOw = flightOrderBookingDetailRequestsPlit["departureAirportCode"].asString();
				//ȥ�̴ﵽ����
				arrivalAirOw  = flightOrderBookingDetailRequestsPlit["arrivalAirportCode"].asString();
				//ȥ�̺����
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//ȥ������
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();
				
				type_   = "���ڵ��̻�Ʊ";
			} 
			//����
			if(routeTypeTemp == "RT")
			{
				Json::Value orderBookingDetailRequests = value["orderBookingDetailRequests"];
				//ע�� ����ط���json Ŀǰֻ��һ�� 0 ȥ�� 1 ����
				int f = 0;
				Json::Value flightOrderBookingDetailRequestsPlit = orderBookingDetailRequests[f];
				//ȥ�̳�������
				departureAirOw = flightOrderBookingDetailRequestsPlit["departureAirportCode"].asString();
				//ȥ�̴ﵽ����
				arrivalAirOw  = flightOrderBookingDetailRequestsPlit["arrivalAirportCode"].asString();
				//ȥ�̺����
				flightNoOw = flightOrderBookingDetailRequestsPlit["flightNo"].asString();
				//ȥ������
				departureDateOw  = flightOrderBookingDetailRequestsPlit["departureDate"].asString();
				
				int g = 1;
				Json::Value flightOrderBookingDetailRequestsPlitRT = orderBookingDetailRequests[g];
				//���̳�������
				departureAirRt = flightOrderBookingDetailRequestsPlitRT["departureAirportCode"].asString();	
				//���̴ﵽ����
				arrivalAirRt  = flightOrderBookingDetailRequestsPlitRT["arrivalAirportCode"].asString();
				//���̺����
				flightNoRt = flightOrderBookingDetailRequestsPlitRT["flightNo"].asString();
				//��������
				departureDateRt  = flightOrderBookingDetailRequestsPlitRT["departureDate"].asString();
				
				type_   = "����������Ʊ";
			}		
			//�ܼ۸�
			price_  = value["totalSalesAmount"].asString();

			//�˿���Ϣ
			string strTemp,passengerName,passengerIDCardType,passengerIDCardTypeTemp,passengerIDCardNo,gender,passengerType,passengerBirthday;
			Json::Value flightOrderPassengers = value["orderPassengers"];
			for (int k = 0;k <flightOrderPassengers.size();k++)
			{
				Json::Value flightOrderPassengersPlit = flightOrderPassengers[k];
				//�˿�����
				passengerName  = flightOrderPassengersPlit["passengerName"].asString();
				//�˿�����
				passengerType = flightOrderPassengersPlit["passengerType"].asString();
				//�˿�֤������
				passengerIDCardTypeTemp = flightOrderPassengersPlit["passengerIDCardType"].asString();
				passengerIDCardType = GetCredentialType(passengerIDCardTypeTemp);
				//�˿�֤������
				passengerIDCardNo = flightOrderPassengersPlit["passengerIDCardNo"].asString();
				//�˿�����
				passengerBirthday = flightOrderPassengersPlit["passengerBirthday"].asString();
			
				//����
				if(routeTypeTemp == "OW")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +					//��ϵ������
						"oid=" + 		tel_ + "\r\n" +						//��ϵ�˵绰
						"msgtype=" + 	passengerName + "\r\n" +			//�˿�����
						"webname=" + 	passengerIDCardType + "\r\n" +		//�˿�֤������
						"netmark=" + 	passengerIDCardNo + "\r\n" +		//�˿�֤����
						"onick=" + 		departureAirOw + "\r\n" +			//��������
						"mid=" + 		arrivalAirOw + "\r\n" +				//�������
						"servername=" + departureDateOw + "\r\n" +			//��������
						"islogin=" + 	flightNoOw + "\r\n" +				//�����
						"platform=�ֻ�app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				//����
				if(routeTypeTemp == "RT")
				{
					output_text_ = 
						"mid=" + 		name_ + "\r\n" +					//��ϵ������
						"oid=" + 		tel_ + "\r\n" +						//��ϵ�˵绰
						"msgtype=" + 	passengerName + "\r\n" +			//�˿�����
						"webname=" + 	passengerIDCardType + "\r\n" +		//�˿�֤������
						"netmark=" + 	passengerIDCardNo + "\r\n" +		//�˿�֤����
						"onick=" + 		departureAirOw + "\r\n" +			//��������
						"mid=" + 		arrivalAirOw + "\r\n" +				//�������
						"servername=" + departureDateOw + "\r\n" +			//��������
						"appname=" + 	departureDateRt + "\r\n" +			//��������
						"islogin=" + 	flightNoOw + "\r\n" +				//�����
						"platform=�ֻ�app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
		}	
		return 0;
	}
	//��Ʊ
	if (action == SP_TRAIN)
	{

		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
	
		//��ϵ������
		name_    = ms_->GetValueFromMapByKey("contact",true);
		//��ϵ�˵绰
		tel_     = ms_->GetValueFromMapByKey("contactphone",true); 	
		//�𳵳��� 
		number_ = ms_->GetValueFromMapByKey("traincode",true);
		//�𳵱��
		string trainNo = ms_->GetValueFromMapByKey("trainNo",true);
		//������
		start_   = ms_->GetValueFromMapByKey("fromstationname",true);
		//Ŀ�ĵ�
		end_     = ms_->GetValueFromMapByKey("tostationname",true);	
		//����ʱ��
		time_ = ms_->GetValueFromMapByKey("startdate",true) + " " + ms_->GetValueFromMapByKey("fromtime",true);
		//����ʱ��
		string arrTime = ms_->GetValueFromMapByKey("arrivedate",true) + " " + ms_->GetValueFromMapByKey("totime",true);

		//�ܼ۸�
		price_  = ms_->GetValueFromMapByKey("totalprice",true);
		type_   = "��Ʊ";	

		//�˿���Ϣ
		//names=����&names=Τ�ٵ�&types=CONTACT&types=CONTACT&cardNumbers=G19782981&cardNumbers=320684198109198745&cardTypes=HUZHAO&cardTypes=ID_CARD
		string passengerName,passengerIsAdult,passengerCardType,passengerCardNumber,passengerBirthday;
		vector<string> vPassengerName,vPassengerIsadult,vPassengerCardType,vPassengerCardNum,vPassengerBirthday;
		//�˿�����
        string headcount = ms_->GetValueFromMapByKey("headcounts",true);
		int headNumber = atoi(headcount.c_str());

		//�˿�����
		string strNameTemp = src_data.substr(src_data.find("names"),(src_data.find("types") - src_data.find("names")) - 1);
		ms_->Replace(strNameTemp,"names=","");
		//�ָ��� &
		SplitString(strNameTemp, vPassengerName,"&"); 

		//�˿�����
		string strIsAdult = src_data.substr(src_data.find("isAdults"),(src_data.find("insuranceProId") - src_data.find("isAdults")) - 1);
		ms_->Replace(strIsAdult,"isAdults=","");
		//�ָ��� &
		SplitString(strIsAdult, vPassengerIsadult,"&"); 

		//�˿�֤������
		string strCardTypeTemp = src_data.substr(src_data.find("cardTypes"),(src_data.find("passengerSexs") - src_data.find("cardTypes")) - 1);
		ms_->Replace(strCardTypeTemp,"cardTypes=","");
		//�ָ��� &
		SplitString(strCardTypeTemp, vPassengerCardType,"&"); 

		//�˿�֤����
		string strCardNumTemp = src_data.substr(src_data.find("cardNumbers"),(src_data.find("cardTypes") - src_data.find("cardNumbers")) - 1);
		ms_->Replace(strCardNumTemp,"cardNumbers=","");
		//�ָ��� &
		SplitString(strCardNumTemp, vPassengerCardNum,"&"); 
       
		//�˿�����
		string strbirthdaysTemp = src_data.substr(src_data.find("birthdays"),(src_data.find("isInsurance") - src_data.find("birthdays")) - 1);
		ms_->Replace(strbirthdaysTemp,"birthdays=","");
		//�ָ��� &
		SplitString(strbirthdaysTemp, vPassengerBirthday,"&"); 

		for (int i = 0;i< headNumber;i++)
		{
			passengerName = vPassengerName[i];
			passengerIsAdult = vPassengerIsadult[i];
			if (passengerIsAdult == "0")
			{
				passengerIsAdult = "��ͯƱ";
			}
			if (passengerIsAdult == "1")
			{
				passengerIsAdult = "����Ʊ";
			}
			passengerCardType = vPassengerCardType[i];
			if (passengerCardType == "ID_CARD")
			{
				passengerCardType = "���֤";
			}
			if (passengerCardType == "GANGAO")
			{
				passengerCardType = "�۰�ͨ��֤";
			}
			if (passengerCardType == "TAIBAO")
			{
				passengerCardType = "̨��ͨ��֤";
			}
			if (passengerCardType == "HUZHAO")
			{
				passengerCardType = "����";
			}
			passengerCardNumber = vPassengerCardNum[i];
			passengerBirthday = vPassengerBirthday[i];

			output_text_ = 
				"mid=" + 		name_ + "\r\n" +				//��ϵ������
				"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
				"nick=" + 		start_ + "\r\n" +				//������
				"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
				"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
				"webname=" + 	passengerCardType + "\r\n" +	//�˿�֤������
				"netmark=" + 	passengerCardNumber + "\r\n" +	//�˿�֤����
				"clientmsg=" + 	number_ + "\r\n" +				//���� 
				"servername=" + time_ + "\r\n" +				//��������
				"platform=�ֻ�app\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;
	}
	return 0; 
}
