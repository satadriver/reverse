
#include "StdAfx.h"
#include "PlaneShanDongAirlines.h"

plane::ShanDongAirlines::ShanDongAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ShanDongAirlines::~ShanDongAirlines()
{
	
}

int plane::ShanDongAirlines::IsShanDongAirlines(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("218.56.48.139:8084")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/login")!=-1) )
	{
		return kpclogin;
	}
	//���ڵ��� ����
	if((http_session->m_Requestline.m_Host.Find("sc.travelsky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/scet/queryAv.do")!=-1))
	{
		return kpcticket;
	}
	//���ʵ��� ����
	if((http_session->m_Requestline.m_Host.Find("sc.travelsky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/scet/queryAvInternational.do")!=-1))
	{
		return kpcticket;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("sc.travelsky.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/scet/bookInfoConfirm.do")!=-1) )
	{
		return kpcorder;
	}
	return 0;
}

int plane::ShanDongAirlines::Process(const char *packet,int action)
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
int plane::ShanDongAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�˺�
	string username = ms_->GetValueFromMapByKey("username",true);
	//����
	string password = ms_->GetValueFromMapByKey("password",true);
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

//���� ���� ���� ����
int plane::ShanDongAirlines::pcticket(const char *packet)
{	
	string src_data(packet);
	//��ȷ��ȡ����
	if (src_data.rfind("\r\n") != 0)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//�ж��ǹ��ڻ��ǹ���
	string countrytype = ms_->GetValueFromMapByKey("countrytype",true);
	string adutcount,childcount,infantcount;
	//����
	if (countrytype == "0")
	{
		//�жϵ��̻�������
		string travelType = ms_->GetValueFromMapByKey("travelType",true);
		if (travelType == "0")
		{		
			adutcount = ms_->GetValueFromMapByKey("adultNum",true);
			childcount = ms_->GetValueFromMapByKey("childNum",true);
			//��������
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//������
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//Ŀ�ĵ�
			string arrCdTxt = ms_->GetValueFromMapByKey("cityNameDes",true);
			output_text_ =
				"netmark=" +adutcount + "\r\n" +	//��������
				"onick=" +	childcount + "\r\n" +	//��ͯ����
				"nick=" +	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +	//��������
				"state=���ڵ��̲�ѯ\r\n";
			WriteIndexFile();
		}
		if (travelType == "1")
		{
			adutcount = ms_->GetValueFromMapByKey("adultNum",true);
			childcount = ms_->GetValueFromMapByKey("childNum",true);
			//��������
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//��������
			string deptDtRe = ms_->GetValueFromMapByKey("returnDate",true);
			//������
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//Ŀ�ĵ�
			string arrCdTxt = ms_->GetValueFromMapByKey("cityNameDes",true);
			output_text_ =
				"netmark=" +	adutcount + "\r\n" + //��������
				"onick=" +	childcount + "\r\n" +	//��ͯ����
				"nick=" +	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +	//��������
				"appname=" + deptDtRe + "\r\n" +	//��������
				"state=����������ѯ\r\n";
			WriteIndexFile();
		}
	}
	//����
	if (countrytype == "1")
	{
		//�жϵ��̻�������
		string travelType = ms_->GetValueFromMapByKey("travelType",true);
		if (travelType == "0")
		{		
			//��������
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//������
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//Ŀ�ĵ�
			string arrCdTxt = ms_->GetValueFromMapByKey("'cityNameDes",true);
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" +	//������
				"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +	//��������
				"state=���ʵ��̲�ѯ\r\n";
			WriteIndexFile();
		}
		if (travelType == "1")
		{
			//��������
			string deptDt = ms_->GetValueFromMapByKey("takeoffDate",true);
			//��������
			string deptDtRe = ms_->GetValueFromMapByKey("returnDate",true);
			//������
			string deptCdTxt = ms_->GetValueFromMapByKey("cityNameOrg",true);
			//Ŀ�ĵ�
			string arrCdTxt = ms_->GetValueFromMapByKey("'cityNameDes",true);
			output_text_ =
				"nick=" +	deptCdTxt + "\r\n" + //������
				"msg=" + 	arrCdTxt + "\r\n" + //Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +//��������
				"appname=" + deptDtRe + "\r\n" + //��������
				"state=����������ѯ\r\n";
			WriteIndexFile();
		}		
	}
	return 1;
}

//�ύ����
int plane::ShanDongAirlines::pcorder(const char *packet)
{
	//��Ϣ��ȡ������
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ϵ����Ϣ
	string contactName,contactMobile,contackEmail;
	string contactInfo = src_data.substr(src_data.find("contactName"));
	ms_->SplitStringToMap(contactInfo,'&','=');

	contactName = ms_->GetValueFromMapByKey("contactName",true);
	contactMobile = ms_->GetValueFromMapByKey("contactmobile",true);
	contackEmail = ms_->GetValueFromMapByKey("contactemail",true);
	//�˿���Ϣ 
	string passengerName,passengerIDtypeTemp,passengerIDtype,passengerIDcard,passengerTel;
	string passengerInfo = src_data.substr(0,src_data.find("&passType"));
	while(passengerInfo.find("luggo") != string::npos)
	{
		string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("luggo"));
		ms_->SplitStringToMap(passengerInfoTemp,'&','=');
		//�˿�����
		passengerName = ms_->GetValueFromMapByKey("passName",true);
		//�˿͵绰
		passengerTel = ms_->GetValueFromMapByKey("contactMobilePerson",true);
		//�˿�֤����
		passengerIDcard = ms_->GetValueFromMapByKey("idNo",true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"clientmsg=" + passengerTel + "\r\n" +	//�˿͵绰
			"subject=" + passengerIDcard + "\r\n" + //�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
		passengerInfo = passengerInfo.substr(passengerInfo.find("luggo") + strlen("luggo"));
	}
	return 1;
}
