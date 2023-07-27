
#include "StdAfx.h"

#include "ChinaSouthern.h"

plane::ChinaSouthern::ChinaSouthern(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChinaSouthern::~ChinaSouthern()
{
	
}

int plane::ChinaSouthern::IsChinaSouthern(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("b2c.csair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/B2C40/user/login.ao")!=-1))
	{
		return kpclogin;
	}
	//���� ���� ����
	if((http_session->m_Requestline.m_Host.Find("b2c.csair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/B2C40/query/jaxb/direct/query.ao")!=-1))
	{
		return kpcticket;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("b2c.csair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/B2C40/data/order/direct/createOrder.xsql?")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ChinaSouthern::Process(const char *packet,int action)
{
	//��¼
	if (action == kpclogin)
	{
		return pclogin(packet);
	}
	//���� ���� ����
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

//�û���¼
int plane::ChinaSouthern::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��Ҫ��ȡ������
	src_data = src_data.substr(src_data.rfind("userId="),src_data.length()-src_data.rfind("userId="));
	ms_->SplitStringToMap(src_data,'&','=');

	username = ms_->GetValueFromMapByKey("userId",true);
	password = ms_->GetValueFromMapByKey("passWord",true);
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

//���� ���� ����
int plane::ChinaSouthern::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("json=")+strlen("json="));

	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		adtCount = value["adultnum"].asString();
		chdCount = value["childnum"].asString();
		infCount = value["infantnum"].asString();
		deptCdTxt = value["depcity"].asString();
		arrCdTxt = value["arrcity"].asString();
		deptDt = value["flightdate"].asString();
	}
	if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
	{
		return 0;
	}
	output_text_ =
		"netmark=" + 	adtCount + "\r\n" + //��������
		"onick=" + 	chdCount + "\r\n" +		//��ͯ����
		"sex=" + 	infCount + "\r\n" +		//Ӥ������
		"nick=" + 	deptCdTxt + "\r\n" +	//������
		"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
		"servername=" + deptDt + "\r\n" +	//��������
		"state=��ѯ\r\n";
	WriteIndexFile();
	return 1;
}

//�ύ����
int plane::ChinaSouthern::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ϵ����Ϣ
	string contactInfo;
	contactInfo = ms_->GetValueBetweenBeginEnd(src_data,"<ORDERCONTACT>","</ORDERCONTACT>",true);
	//��ϵ������
	string contactName,contactMobile,contackEmail;
	contactName = contactInfo.substr(0,contactInfo.find("|"));
	//��ϵ�˵绰
	contactMobile = contactInfo.substr(contactInfo.find("||") + strlen("||"),contactInfo.rfind("|") - contactInfo.find("||") - strlen("||"));
	//�˿���Ϣ
	string passengeInfo;
	passengeInfo = ms_->GetValueBetweenBeginEnd(src_data,"<PASSENGERS>","</PASSENGERS>",true);
	while (passengeInfo.find("</PASSENGER>") != -1)
	{
		string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
		string passengerInfoTemp = passengeInfo.substr(0,passengeInfo.find("</PASSENGER>") + strlen("</PASSENGER>"));
		//�˿�����
		passengerName = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"<NAME>","</NAME>",true);
		//�˿�֤������
		passengerIDtypeTemp = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"<IDTYPE>","</IDTYPE>",true);
		if (passengerIDtypeTemp == "NI"){passengerIDtype = "���֤";};
		if (passengerIDtypeTemp == "PP"){passengerIDtype = "����";};
		if (passengerIDtypeTemp == "ID"){passengerIDtype = "����֤��";};
		//�˿�֤����
		passengerIDcard = ms_->GetValueBetweenBeginEnd(passengerInfoTemp,"<IDCARD>","</IDCARD>",true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"webname=" + passengerIDtype + "\r\n" + //�˿�֤������
			"subject=" + passengerIDcard + "\r\n" + //�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	    passengeInfo = passengeInfo.substr(passengeInfo.find("</PASSENGER>") + strlen("</PASSENGER>"));
	}
	return 1;
}
