
#include "StdAfx.h"
#include "PlaneXiaMenAirlines.h"

plane::XiaMenAirLines::XiaMenAirLines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::XiaMenAirLines::~XiaMenAirLines()
{
	
}
		
int plane::XiaMenAirLines::IsXiaMenAirLines(HttpSession *http_session)
{	
	//���ڵ��� ���� ��ѯ
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/book/findFlights.json")!=-1)) 
	{										
		return kpcticketcn;
	}	
	//���ʵ��� ���� ��ѯ
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/intlbook/intlFindFlights.json")!=-1))
	{
		return kpcticketint;
	}	
	//���ڵ��� ���� �ύ����
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/book/view.action")!=-1) )
	{
		return kpcordercn;
	}
	//���ʵ��� ���� �ύ����
	if((http_session->m_Requestline.m_Host.Find("et.xiamenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/xiamenair/intlbook/intlConfirmOrder.action")!=-1) )
	{
		return kpcorderint;
	}
	return 0;
}

int plane::XiaMenAirLines::Process(const char *packet,int action)
{
	//���ڵ��� ���� ��ѯ
	if (action == kpcticketcn)
	{
		return pcticketcn(packet);
	}
	//���ʵ��� ���� ��ѯ
	if(action == kpcticketint)
	{
		return pcticketint(packet);
	}
	//���ڵ��� ���� �ύ����
	if (action == kpcordercn)
	{
		return pcordercn(packet);
	}
	//���ʵ��� ���� �ύ����
	if (action == kpcorderint)
	{
		return pcorderint(packet);
	}
	return 1;
}

//���ڵ��� ���� ��ѯ
int plane::XiaMenAirLines::pcticketcn(const char *packet)
{	
	//��url����ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->SplitStringToMap(uri,'&','=');
	//���� ���� 
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (tripType == "0")
	{
		//������(������)
		string orgCity = ms_->GetValueFromMapByKey("orgCity",true);
        //Ŀ�ĵ�(������)
		string dstCity = ms_->GetValueFromMapByKey("dstCity",true);
		//��������
		string takeoffDate = ms_->GetValueFromMapByKey("takeoffDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //������
			"msg=" + 	dstCity + "\r\n" +    //Ŀ�ĵ� 
			"servername=" + takeoffDate + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	if (tripType == "1")
	{
		//������(������)
		string orgCity = ms_->GetValueFromMapByKey("orgCity",true);
        //Ŀ�ĵ�(������)
		string dstCity = ms_->GetValueFromMapByKey("dstCity",true);
		//��������
		string takeoffDate = ms_->GetValueFromMapByKey("takeoffDate",true);
		//��������
		string returnDate = ms_->GetValueFromMapByKey("returnDate",true);
		output_text_ =
			"nick=" + 	orgCity + "\r\n" +    //������
			"msg=" + 	dstCity + "\r\n" +    //Ŀ�ĵ� 
			"servername=" + takeoffDate + "\r\n" + //��������
			"appname=" + 	returnDate + "\r\n" + //��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}

//���ʵ��� ���� ��ѯ
int plane::XiaMenAirLines::pcticketint(const char *packet)
{	
	//��refer����ȡ����
	string refer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	ms_->Replace(refer,"?","&");
	refer = ms_->UTF8ToGBK(UrlDecode(refer));
	ms_->SplitStringToMap(refer,'&','=');
	//���� ���� 
	string tripType = ms_->GetValueFromMapByKey("tripType",true);
	if (tripType == "0")
	{
	//�˿���Ϣ 0,1;1,1
		string psgrInfo = ms_->GetValueFromMapByKey("psgrInfo",true);
        //��������
		string adultTemp = psgrInfo.substr(0,psgrInfo.find(";"));
		string adult = adultTemp.substr(adultTemp.find(",") + strlen(","));
        //��ͯ����
		string childTemp = psgrInfo.substr(psgrInfo.find(";"));
		string child = childTemp.substr(childTemp.find(",") + strlen(","));

	//������Ϣ XMN,HKG,2016-12-09
		string queryFlightInfo = ms_->GetValueFromMapByKey("queryFlightInfo",true);
		//������(������)
		string orgCity = queryFlightInfo.substr(0,queryFlightInfo.find(","));	
        //Ŀ�ĵ�(������)
		string dstCity =  queryFlightInfo.substr(queryFlightInfo.find(",") + strlen(","),queryFlightInfo.rfind(",") - queryFlightInfo.find(",") - strlen(","));
		//��������
		string takeoffDate = queryFlightInfo.substr(queryFlightInfo.rfind(",") + strlen(","));

		output_text_ =
			"netmark=" + 	adult + "\r\n" +	//��������    
			"onick=" + 	child + "\r\n" +		//��ͯ���� 
			"nick=" + 	orgCity + "\r\n" +		//������
			"msg=" + 	dstCity + "\r\n" +		//Ŀ�ĵ� 
			"servername=" + takeoffDate + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	
	if (tripType == "1")
	{
	//�˿���Ϣ 0,1;1,1
		string psgrInfo = ms_->GetValueFromMapByKey("psgrInfo",true);
        //��������
		string adultTemp = psgrInfo.substr(0,psgrInfo.find(";"));
		string adult = adultTemp.substr(adultTemp.find(",") + strlen(","));
        //��ͯ����
		string childTemp = psgrInfo.substr(psgrInfo.find(";"));
		string child = childTemp.substr(childTemp.find(",") + strlen(","));
		
		//������Ϣ AMS,HGH,2016-12-09;HGH,AMS,2016-12-10
		string queryFlightInfo = ms_->GetValueFromMapByKey("queryFlightInfo",true);
		string queryFlightInfoOw = queryFlightInfo.substr(0,queryFlightInfo.find(";"));
		string queryFlightInfoRe = queryFlightInfo.substr(queryFlightInfo.rfind(";"));
		//������(������)
		string orgCity = queryFlightInfoOw.substr(0,queryFlightInfoOw.find(","));	
        //Ŀ�ĵ�(������)
		string dstCity =  queryFlightInfoOw.substr(queryFlightInfoOw.find(",") + strlen(","),queryFlightInfoOw.rfind(",") - queryFlightInfoOw.find(",") - strlen(","));
		//��������
		string takeoffDate = queryFlightInfoOw.substr(queryFlightInfoOw.rfind(",") + strlen(","));
		//��������
		string returnDate = queryFlightInfoRe.substr(queryFlightInfoRe.rfind(",") + strlen(","));	
		output_text_ =
			"netmark=" + adult + "\r\n" +		//��������    
			"onick=" + 	child + "\r\n" +		//��ͯ���� 
			"nick=" + 	orgCity + "\r\n" +		//������
			"msg=" + 	dstCity + "\r\n" +		//Ŀ�ĵ� 
			"servername=" + takeoffDate + "\r\n" +	//��������
			"appname=" + returnDate + "\r\n" +		//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}


//���ڵ��� ���� �ύ����
int plane::XiaMenAirLines::pcordercn(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��ȡ�˿͵�����
	string temp = src_data.substr(src_data.rfind("passengers"));
	string temp2 = temp.substr(0,temp.find("="));
	string People = ms_->GetValueBetweenBeginEnd(temp2,"[","]");
	int PassengerCount = atoi(People.c_str());
	//���� ����

    //��ϵ������
	string contactName = ms_->GetValueFromMapByKey("contact.name",true);
	//��ϵ�˵绰
	string contactMobile = ms_->GetValueFromMapByKey("contact.mobile",true);
	//��ϵ������
	string contactEmail = ms_->GetValueFromMapByKey("contact.email",true);
	if (contactEmail.empty())
	{
		contactEmail = "Null";
	}
	for(int i =0;i<= PassengerCount;i++)
	{
		CString str;
		str.Format("%d",i);
		string strTemp = str.GetBuffer(0);
		//�˿�����
		string publicTemp = "passengers";
		string passengerNameTemp = publicTemp + "[" + strTemp + "]" + ".name";
		string passengerName = ms_->GetValueFromMapByKey(passengerNameTemp,true);
		//�˿����� 0 ���� 1 ��ͯ
		string remarkTemp = publicTemp + "[" + strTemp + "]" + ".type";
		string remark = ms_->GetValueFromMapByKey(remarkTemp,true);

		string cateType,cateNo;
		if (remark == "0")
		{
			//�˿�֤������
			string cateTypeTemp = publicTemp + "[" + strTemp + "]" + ".idType";
			cateType = ms_->GetValueFromMapByKey(cateTypeTemp,true);
			//�˿�֤����
			string cateNoTemp = publicTemp + "[" + strTemp + "]" + ".idNo";
			cateNo = ms_->GetValueFromMapByKey(cateNoTemp,true);
		}
		if (remark == "1")
		{
			//�˿�֤������
			string cateTypeTemp = publicTemp + "[" + strTemp + "]" + ".idType";
			cateType = ms_->GetValueFromMapByKey(cateTypeTemp,true);
			//�˿�֤����
			
			string yearTemp = publicTemp + "[" + strTemp + "]" + ".year";
			string monthTemp = publicTemp + "[" + strTemp + "]" + ".month";
			string dayTemp = publicTemp + "[" + strTemp + "]" + ".day";
			cateNo = ms_->GetValueFromMapByKey(yearTemp,true) + "-" + ms_->GetValueFromMapByKey(monthTemp,true) + "-" + ms_->GetValueFromMapByKey(dayTemp,true);
		}
		//�˿͵绰
		string mobilePhoneTemp = publicTemp + "[" + strTemp + "]" + ".mobile";
		string mobilePhone = ms_->GetValueFromMapByKey(mobilePhoneTemp,true);

		output_text_ =
			"mid=" + 	contactName + "\r\n" +		//��ϵ������
			"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
			"imsi=" + 	contactEmail + "\r\n" +		//��ϵ������
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"clientmsg=" + mobilePhone + "\r\n" +	//�˿͵绰
			"webname=" + cateType + "\r\n" +		//�˿�֤������
			"subject=" + cateNo + "\r\n" +			//�˿�֤����
			"state=�ύ����\r\n";
		WriteIndexFile();
	}
	return 1;
}

//���ʵ��� ���� �ύ����
int plane::XiaMenAirLines::pcorderint(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("person"));
	ms_->SplitStringToMap(src_data,'&','=');
	//����  ���� 
	//��ϵ������
	string contactName = ms_->GetValueFromMapByKey("contactName",true);
	//��ϵ�˵绰
	string contactMobile = ms_->GetValueFromMapByKey("contactMobile",true);
	//��ϵ������
	string contactEmail = ms_->GetValueFromMapByKey("contactEmail",true);
	if (contactEmail.empty())
	{
		contactEmail = "Null";
	}
	//�˿�����
	int pos = src_data.rfind("person");
	int tos = src_data.find("&",pos);
	string passengerCountTemp = src_data.substr(pos,tos - pos);
	string passengerCount = ms_->GetValueBetweenBeginEnd(passengerCountTemp,"person","=");
	int nCount = atoi(passengerCount.c_str()); 
    nCount++;
	string passengerInfo = src_data.substr(src_data.find("person"),src_data.find("&contactName") - src_data.find("person"));
	//�˿���Ϣ ���Ż� person0= ,mobile= ,person1= mobile=,	
	while (nCount) 
	{
		if (nCount > 1)
		{
			string passengerInfoTemp = passengerInfo.substr(0,passengerInfo.find("&person"));	 
			//�˿�����
			string passengerName = ms_->GetValueBetweenBeginEnd(passengerInfo,"familyName=","&") + 
								   ms_->GetValueBetweenBeginEnd(passengerInfo,"givenName=","&");
			//�˿�֤������
			string cateType = "����";
			//�˿�֤����
			string cateNo = ms_->GetValueBetweenBeginEnd(passengerInfo,"passNumber=","&");
			//�˿͵绰
			string mobilePhone = ms_->GetValueBetweenBeginEnd(passengerInfo,"mobile=","&");
			if (mobilePhone.empty())
			{
				mobilePhone = "Null";
			}
			output_text_ =
				"mid=" + 	contactName + "\r\n" +		//��ϵ������
				"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
				"imsi=" + 	contactEmail + "\r\n" +		//��ϵ������
				"msgtype=" + passengerName + "\r\n" +	//�˿�����
				"clientmsg=" + mobilePhone + "\r\n" +	//�˿͵绰
				"webname=" + cateType + "\r\n" +		//�˿�֤������
				"subject=" + cateNo + "\r\n" +			//�˿�֤����
				"state=�ύ����\r\n";
			WriteIndexFile();
		}	
		if (passengerInfo.find("&person") != string::npos)
		{
			passengerInfo = passengerInfo.substr(passengerInfo.find("&person") + strlen("&"));
		}
		if (nCount ==1 )
		{
			//�˿�����
			string passengerName = ms_->GetValueBetweenBeginEnd(passengerInfo,"familyName=","&") + 
								   ms_->GetValueBetweenBeginEnd(passengerInfo,"givenName=","&");
			//�˿�֤������
			string cateType = "����";
			//�˿�֤����
			string cateNo = ms_->GetValueBetweenBeginEnd(passengerInfo,"passNumber=","&");
			//�˿͵绰
			string mobilePhone = passengerInfo.substr(passengerInfo.find("mobile=") + strlen("mobile="));
			if (mobilePhone.empty())
			{
				mobilePhone = "Null";
			}
			output_text_ =
				"mid=" + 	contactName + "\r\n" +		//��ϵ������
				"oid=" + 	contactMobile + "\r\n" +	//��ϵ�˵绰
				"imsi=" + 	contactEmail + "\r\n" +		//��ϵ������
				"msgtype=" + passengerName + "\r\n" +	//�˿�����
				"clientmsg=" + mobilePhone + "\r\n" +	//�˿͵绰
				"webname=" + cateType + "\r\n" +		//�˿�֤������
				"subject=" + cateNo + "\r\n" +			//�˿�֤����
				"state=�ύ����\r\n";
			WriteIndexFile();
		}
		nCount--;
	}
	return 1;
}
