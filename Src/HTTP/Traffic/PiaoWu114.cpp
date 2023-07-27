// PiaoWu114.cpp: implementation of the PiaoWu114 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PiaoWu114.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//�����֤������(���ڷɻ�Ʊ)
string credential_type_piaowu[6] = {"���֤","����","����֤","ʿ��֤","̨��֤","����"};

PiaoWu114::PiaoWu114(HttpSession *http_session, const string &pro_name) : Traffic(http_session, pro_name)
{

}

PiaoWu114::~PiaoWu114()
{

}

int PiaoWu114::Is(HttpSession *http_session)
{
	//pc ��̬��¼
	if((http_session->m_Requestline.m_Host.Find("ucenter.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/datum/userdata/userlogin.html")!=-1))
	{
		return PC_LOGIN;
	}
	//pc ��̬��¼
	if((http_session->m_Requestline.m_Host.Find("ucenter.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/notVip/queryorder/loginMyOrder.html")!=-1))
	{
		return PC_LOGIN_DYNAMIC;
	}
	// PC ��Ʊ ��ȡƱ δ����ר��ͨ��
	if((http_session->m_Requestline.m_Host.Find("huochepiao.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/newTrainOrder")!=-1))
	{
		return PC_TRAIN;
	}
	// PC ����Ʊ 
	if((http_session->m_Requestline.m_Host.Find("qiche.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/indexAjax/verifyOrder.action")!=-1))
	{
		return PC_BUS;
	}
	// PC ���ڷɻ�Ʊ ���ʷɻ�Ʊ
	if((http_session->m_Requestline.m_Host.Find("jipiao.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/indexAjax/verifyOrder.action")!=-1))
	{
		return PC_AIRPLANE;
	}
	// SP ��̬��¼
	if((http_session->m_Requestline.m_Host.Find("appservice.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ucenter/userBasic_loginByPassword.services")!=-1))
	{
		return SP_LOGIN;
	}
	// SP ����Ʊ 1
	if((http_session->m_Requestline.m_Host.Find("m.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qiche/qicheYuDing_newOrder.action")!=-1))
	{
		return SP_BUS;
	}
	//SP ����Ʊ 2
	if((http_session->m_Requestline.m_Host.Find("appservice.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qiche/query/qiCheQuery_newOrder.services")!=-1))
	{
		return SP_BUSSECOND;
	}
	// SP ��Ʊ
	if((http_session->m_Requestline.m_Host.Find("appservice.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/train/booking/singleOrder_ordinary.services")!=-1))
	{
		return SP_TRAIN;
	}
	// SP �ɻ�Ʊ
	if((http_session->m_Requestline.m_Host.Find("m.114piaowu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jipiao/jipiaoYuDing_newOrder.action")!=-1))
	{
		return SP_AIRPLANE;
	}
	return 0;
}


int PiaoWu114::Process(const char *packet,int action)
{
	//pc ��¼ 
	if (action == PC_LOGIN)
	{
		string src_data(packet);
		string src_dataEnd = src_data.substr(src_data.rfind("username="));
		src_dataEnd = ms_->UrlDecode(src_dataEnd);
		ms_->SplitStringToMap(src_dataEnd,'&','=');
		
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
		return 0;
	}
	if (action == PC_LOGIN_DYNAMIC)
	{
		//���ݲ���
		string src_data(packet);
		string src_dataEnd;
		if (src_data.rfind("regPhone") != string::npos )
		{
			src_dataEnd = src_data.substr(src_data.rfind("regPhone"));
		}	
		src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
		ms_->SplitStringToMap(src_dataEnd,'&','=');

		string user,pass;
		user=ms_->GetValueFromMapByKey("regPhone",true);
		pass=ms_->GetValueFromMapByKey("randCode",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=��¼\r\n";	
		WriteIndexFile();
		return 0;
	}
	//PC ���ڻ�Ʊ ���ʻ�Ʊ
	if (action == PC_AIRPLANE)
	{	
		//��cookie ����ȡ������ Ŀ�ĵ�
		string depCity,arrCity;
		string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
		cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
		ms_->SplitStringToMap(cookie, ';','=');	
		//������
		depCity = ms_->GetValueFromMapByKey("depCity");
		//Ŀ�ĵ�
		arrCity = ms_->GetValueFromMapByKey("arrCity");

		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		ms_->SplitStringToMap(sContent,'&','=');
		//��ȡ�˿͵�����
		string temp = sContent.substr(sContent.rfind("passengers"),sContent.length()-sContent.rfind("passengers"));
		int pos = temp.rfind("passengers");
		string People = temp.substr(pos+ strlen("passengers") + strlen("["),1);
		int PassengerCount = atoi(People.c_str());

		string lickManName,lickManMobileNum;
		//��ϵ������
		lickManName = ms_->GetValueFromMapByKey("lickManName",true);
		//��ϵ�˵绰
		lickManMobileNum = ms_->GetValueFromMapByKey("lickManMobileNum",true); 
      
		//��Ʊ���� (���� GN/���� GW)
		string catype = ms_->GetValueFromMapByKey("catype",true); 
		if (catype == "GN")
		{
			//�������� (���� 0 /���� 1) 
			string searchType = ms_->GetValueFromMapByKey("searchType",true);
			
			if (searchType == "0")
			{
				//����
				type_   = "���ڵ��̻�Ʊ";
			}
			if (searchType == "1")
			{
				//����
				type_   = "����������Ʊ";
			}
			//��ϵ����������ϵ�˵绰�������أ�Ŀ�ĵأ��˿��������˿����ͣ��˿�֤�����ͣ��˿�֤���ţ��˿ͳ�������
			string PassengerName,PassengerType,PassengerCertType,PassengerCertNumber,PassengerBirthday;
			for (int j=0;j < PassengerCount + 1;j++)
			{
				//ת��
				CString str;
				str.Format("%d",j);
				string strEnd = str.GetBuffer(0);
				//�˿�����
				string strTempName = "passengers[" + strEnd + "].name";
				PassengerName = ms_->GetValueFromMapByKey(strTempName,true); 
				//�˿�����	
				string strTempType = "passengers[" + strEnd + "].type";
				string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
				if (PassengerTypeTemp == "0")
				{
					PassengerType = "����Ʊ";
				}
				if (PassengerTypeTemp == "1")
				{
					PassengerType = "��ͯƱ";
				}
				//�˿�֤������
				string strCertType = "passengers[" + strEnd + "].identityType";
				string PassengerCertTypeTemp = ms_->GetValueFromMapByKey(strCertType,true); 
				int temp = atoi(PassengerCertTypeTemp.c_str());
				PassengerCertType = credential_type_piaowu[temp - 1];
				//�˿�֤���� 
				string strCertNumber = "passengers[" + strEnd + "].identityNo";
				PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
				//�˿ͳ������� (���֤��)
				if (temp == 1)
				{
					string year = PassengerCertNumber.substr(6,4);
					string month = PassengerCertNumber.substr(10,2);
					string day = PassengerCertNumber.substr(12,2);
					PassengerBirthday = year + "-" + month + "-" + day;
				}
				else if(temp > 1)
				{
					string strBirthday = "passengers[" + strEnd + "].birthday";
					PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true);
				}			
				if (searchType == "0")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//��ϵ������
						"oid=" + 		lickManMobileNum + "\r\n" +		//��ϵ�˵绰
						"nick=" + 		depCity + "\r\n" +				//������
						"msg=" + 		arrCity + "\r\n" +				//Ŀ�ĵ� 
						"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
						"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				if (searchType == "1")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//��ϵ������
						"oid=" + 		lickManMobileNum + "\r\n" +		//��ϵ�˵绰
						"nick=" + 		depCity + "\r\n" +				//������
						"msg=" + 		arrCity + "\r\n" +				//Ŀ�ĵ� 
						"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
						"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
		}
		else if (catype == "GW")
		{
			//�������� (���� 0 /���� 1) 
			string searchType = ms_->GetValueFromMapByKey("searchType",true);
			
			if (searchType == "0")
			{
				//����
				type_   = "���ʵ��̻�Ʊ";
			}
			if (searchType == "1")
			{
				//����
				type_   = "����������Ʊ";
			}
			//��ϵ����������ϵ�˵绰�������أ�Ŀ�ĵأ��������ڣ��������ڣ�
			string godate,godatefy;
			//��������
			godate = ms_->GetValueFromMapByKey("godate");
			//��������
			godatefy = ms_->GetValueFromMapByKey("godatefy");
			//�˿�����(Ӣ����)���˿��Ա𣬳˿����ͣ��˿�֤�����ͣ��˿�֤���ţ��˿�֤��ǩ�������������˿ͳ������ڣ��˿�֤����Ч��
			string PassengerName,PassengerGender,PassengerType,PassengerCertType,PassengerCertNumber,PassengerCardIssuePlaceName,passengerNationality,PassengerBirthday,passengerCardExpired;
			for (int j=0;j < PassengerCount + 1;j++)
			{
				//ת��
				CString str;
				str.Format("%d",j);
				string strEnd = str.GetBuffer(0);
				//�˿�����
				string strTempFirstName = "passengers[" + strEnd + "].firstName";
				string strTemplastName = "passengers[" + strEnd + "].lastName";
				PassengerName = ms_->GetValueFromMapByKey(strTempFirstName,true) + ms_->GetValueFromMapByKey(strTemplastName,true); 
				//�˿��Ա�
				string strTempGender = "passengers[" + strEnd + "].gender";
				PassengerGender = ms_->GetValueFromMapByKey(strTempGender,true); 
				if (PassengerGender == "F")
				{
					PassengerGender = "Ů";
				}
				if (PassengerGender == "M")
				{
					PassengerGender = "��";
				}
				//�˿�����	
				string strTempType = "passengers[" + strEnd + "].type";
				string PassengerTypeTemp = ms_->GetValueFromMapByKey(strTempType,true); 
				if (PassengerTypeTemp == "0")
				{
					PassengerType = "����Ʊ";
				}
				if (PassengerTypeTemp == "1")
				{
					PassengerType = "��ͯƱ";
				}
				//�˿�֤������
				string strTempCardType = "passengers[" + strEnd + "].identityType";
				PassengerCertType = ms_->GetValueFromMapByKey(strTempCardType,true); 
				if (PassengerCertType == "2")
				{
					PassengerCertType = "����";
				}
				if (PassengerCertType == "7")
				{
					PassengerCertType = "�۰�ͨ��֤";
				}
				if (PassengerCertType == "8")
				{
					PassengerCertType = "����֤";
				}
				if (PassengerCertType == "5")
				{
					PassengerCertType = "̨��֤";
				}
				//�˿�֤���� 
				string strCertNumber = "passengers[" + strEnd + "].identityNo";
				PassengerCertNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
				//�˿�֤��ǩ����
				string strCertPlaceName = "passengers[" + strEnd + "].cardIssuePlaceName";
				PassengerCardIssuePlaceName = ms_->GetValueFromMapByKey(strCertPlaceName,true);  
				//����
				string strCertNationName = "passengers[" + strEnd + "].nationalityName";
				passengerNationality = ms_->GetValueFromMapByKey(strCertNationName,true); 
				//�˿ͳ�������
				string strBirthday = "passengers[" + strEnd + "].birthday";
				PassengerBirthday = ms_->GetValueFromMapByKey(strBirthday,true);
				//�˿�֤����Ч��
				string strExpired = "passengers[" + strEnd + "].cardExpired";
				passengerCardExpired = ms_->GetValueFromMapByKey(strExpired,true);

				if (searchType == "0")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//��ϵ������
						"oid=" + 		lickManMobileNum + "\r\n" +		//��ϵ�˵绰
						"nick=" + 		depCity + "\r\n" +				//������
						"msg=" + 		arrCity + "\r\n" +				//Ŀ�ĵ� 
						"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
						"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";			
					WriteIndexFile();
				}
				if (searchType == "1")
				{
					output_text_ = 
						"mid=" + 		lickManName + "\r\n" +			//��ϵ������
						"oid=" + 		lickManMobileNum + "\r\n" +		//��ϵ�˵绰
						"nick=" + 		depCity + "\r\n" +				//������
						"msg=" + 		arrCity + "\r\n" +				//Ŀ�ĵ� 
						"msgtype=" + 	PassengerName + "\r\n" +		//�˿�����
						"webname=" + 	PassengerCertType + "\r\n" +	//�˿�֤������
						"netmark=" + 	PassengerCertNumber + "\r\n" +	//�˿�֤����
						"servername=" + godate + "\r\n" +				//��������
						"appname=" + 	godatefy + "\r\n" +				//��������
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
		//��ϵ����������ϵ�˵绰��ʼ���أ�Ŀ�ĵأ����Σ��������ڣ�����ʱ��(ֻ��ʱ��)��
		//�˿��������˿����ͣ��˿�֤�����ͣ��˿�֤���ţ�Ʊ���ͣ��ܼ۸�)
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');

		//��ϵ������
		name_    = ms_->GetValueFromMapByKey("oUserName",true);
		//��ϵ�˵绰
		tel_     = ms_->GetValueFromMapByKey("oUserMobile",true); 	
		//������
		start_   = ms_->GetValueFromMapByKey("fromStation",true);
		//Ŀ�ĵ�
		end_     = ms_->GetValueFromMapByKey("toStation",true);	
		//�𳵳��� 
		number_ = ms_->GetValueFromMapByKey("trainCode",true);
		//����ʱ��
		time_ = ms_->GetValueFromMapByKey("goDate",true) + " " + ms_->GetValueFromMapByKey("startTime",true);
		//����ʱ��
		string arrTime = ms_->GetValueFromMapByKey("endTime",true);
		//Ʊ����
		string ticketType = ms_->GetValueFromMapByKey("oTicketType",true);
		//�ܼ۸�
		price_  = ms_->GetValueFromMapByKey("payTotal",true);
		type_   = "��Ʊ";	

		//�˿���Ϣ
		string passengerName,passengerIsAdult,passengerCardType,passengerCardNumber,passengerBirthday;
		//�˿�����
        string headcount = ms_->GetValueFromMapByKey("ticketNum",true);
		int headNumber = atoi(headcount.c_str());

		//�˿��������˿����ͣ��˿�Ʊ���ͣ��˿�֤�����ͣ��˿�֤���ţ��ܼ۸�)
		for (int i = 0;i< headNumber;i++)
		{
			CString str;
			str.Format("%d",i);
			string strEnd = str.GetBuffer(0);
			//�˿�����
			string strTempName = "passengers[" + strEnd + "].haveName";
			passengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//�˿�����	
			string strTempType = "passengers[" + strEnd + "].type";
			passengerIsAdult = ms_->GetValueFromMapByKey(strTempType,true); 
			if (passengerIsAdult == "1")
			{
				passengerIsAdult = "����Ʊ";
			}
			if (passengerIsAdult == "2")
			{
				passengerIsAdult = "��ͯƱ";
			}
			//�˿�֤������
			string strCertType = "passengers[" + strEnd + "].cardType";
			passengerCardType = ms_->GetValueFromMapByKey(strCertType,true); 
			//�˿�֤���� 
			string strCertNumber = "passengers[" + strEnd + "].haveId";
			passengerCardNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
				
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
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;	
	}
	//����Ʊ
	if (action == PC_BUS)
	{
		//��ϵ����������ϵ�˵绰�������أ�Ŀ�ĵأ�����վ��������վ��������ʱ�䣬�˿��������˿�֤���ţ��ܼ۸�(��������)
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
	//��ȡ�˿͵�����
		string temp = src_data.substr(src_data.rfind("passengers"),src_data.length()-src_data.rfind("passengers"));
		int pos = temp.rfind("passengers");
		string People = temp.substr(pos+ strlen("passengers") + strlen("["),1);
		int PassengerCount = atoi(People.c_str());
	//��ϵ����Ϣ
		//��ϵ������
		name_    = ms_->GetValueFromMapByKey("lickManName",true);
		//��ϵ�˵绰
		tel_     = ms_->GetValueFromMapByKey("lickManMobileNum",true); 	   
	//������Ϣ
		string orderTrain = ms_->GetValueFromMapByKey("orderTrain",true); 	
		//ʹ��json ��ȡ����
		string startStationName,belongPlaceName;
		Json::Reader reader;
		Json::Value value;
		//�ж�����ӻ��Ǹ���
		if (reader.parse(orderTrain,value))
		{	
			//������
			start_   = value["fromCityName"].asString();
			//Ŀ�ĵ�
			end_     = value["toCityName"].asString();
			//����վ��
			startStationName = value["startStationName"].asString();
			//����վ��
			belongPlaceName  = value["belongPlaceName"].asString();
			//����ʱ��
			time_ = value["startDate"].asString() + " " + value["startTimestamp"].asString();
			//�ܼ۸�
			int price = value["fullPrice"].asInt();
			CString strTempPrice;
			strTempPrice.Format("%d",price);
			price_  = strTempPrice.GetBuffer(0);
		}
		type_   = "����Ʊ";		
	//�˿���Ϣ
		string passengerName,passengerCardType,passengerCardNumber;	
		
		for (int i = 0;i<= PassengerCount;i++)
		{
			CString str;
			str.Format("%d",i);
			string strEnd = str.GetBuffer(0);
			//�˿�����
			string strTempName = "passengers[" + strEnd + "].name";
			passengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//�˿�֤������
			passengerCardType = "���֤";
			//�˿�֤���� 
			string strCertNumber = "passengers[" + strEnd + "].identityNo";
			passengerCardNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  
			
			output_text_ = 
						"mid=" + 		name_ + "\r\n" +				//��ϵ������
						"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
						"nick=" + 		start_ + "\r\n" +				//������
						"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
						"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
						"netmark=" + 	passengerCardNumber + "\r\n" +	//�˿�֤����
						"imsi=" + 		startStationName + "\r\n" +		//����վ��
						"clientmac=" + belongPlaceName + "\r\n" +		//����վ�� 
						"servername=" + time_ + "\r\n" +				//��������
						"platform=���������\r\n" +
						"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;	
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//app ��¼ 
	if (action == SP_LOGIN)
	{
		string src_data(packet);
		src_data=ms_->UrlDecode(src_data);
		ms_->SplitStringToMap(src_data,'&','=');
		
		string user,pass;
		user=ms_->GetValueFromMapByKey("userBean.userName",true);
		pass=ms_->GetValueFromMapByKey("userBean.password",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=��¼\r\n";	
		WriteIndexFile();
		return 0;
	}
	if (action == SP_LOGIN_DYNAMIC)
	{
		//���ݲ���
		string src_data(packet);
		string src_dataEnd;
		if (src_data.rfind("regPhone") != string::npos )
		{
			src_dataEnd = src_data.substr(src_data.rfind("regPhone"));
		}	
		src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
		ms_->SplitStringToMap(src_dataEnd,'&','=');
		
		string user,pass;
		user=ms_->GetValueFromMapByKey("regPhone",true);
		pass=ms_->GetValueFromMapByKey("randCode",true);
		if ( user.empty() || pass.empty())
		{
			return 0;
		}	
		output_text_ = 
			"user=" + user + "\r\n" + 
			"pass=" + pass + "\r\n" + 
			"state=��¼\r\n";	
		WriteIndexFile();
		return 0;
	}
	//�ɻ�Ʊ
	if (action == SP_AIRPLANE)
	{	
		//���� (��ϵ�˵绰�������أ�Ŀ�ĵأ���������������������������ڣ������) + ����
		//���� (��ϵ�˵绰�������أ�Ŀ�ĵأ�ȥ�̳���������ȥ�̵��������ȥ�̳������ڣ�ȥ�̺���ţ����̳������������̵�����������̳������ڣ����̺����) + ����
		//���� (�˿��������˿����ͣ��˿�֤�����ͣ��˿�֤���ţ��˿����գ��ܼ۸�)
	//��ϵ����Ϣ
		string telNum;
	//������Ϣ
		string flight,orgCityNameC,dstCityNameC,orgCityName,dstCityName,depDate,arrDate,airwaysCn,flightNo;
		string flightfy,orgCityNameCfy,dstCityNameCfy,orgCityNamefy,dstCityNamefy,depDatefy,arrDatefy,airwaysCnfy,flightNofy;
	//�˿���Ϣ
		string passengerName,passengerType,passengerCardType,passengerCard,passengerBirth,totalPrice;
	//��Ʊ���� GN ���� GW ����
		string catype;
	//�������� 1 ���� 0 ����
		string searchType;
		string sContent = ms_->UTF8ToGBK(UrlDecode(packet));   
		//�˿���Ϣ
		string passengerInfo = sContent.substr(sContent.find("passengerName"),sContent.find("telNum") - sContent.find("passengerName") - strlen("&"));
		ms_->SplitStringToMap(sContent,'&','=');
		//��ϵ�˵绰
		telNum = ms_->GetValueFromMapByKey("telNum",true);    
		//�������� (���� 0 /���� 1) 
		searchType = ms_->GetValueFromMapByKey("searchType",true);	
		//��Ʊ���� (���� GN/���� GW)
		catype = ms_->GetValueFromMapByKey("catype",true); 
		//�ܼ۸�
		totalPrice = ms_->GetValueFromMapByKey("totalPrice",true);	

		if (searchType == "0")
		{
			//����
			flight = ms_->GetValueFromMapByKey("flight",true);
			//ʹ��json ��ȡ����
			Json::Reader reader;
			Json::Value value;
			//�ж�����ӻ��Ǹ���
			if (reader.parse(flight,value))
			{	
				//������
				orgCityNameC = value["orgCityNameC"].asString();
				//Ŀ�ĵ�
				dstCityNameC = value["dstCityNameC"].asString();
				//ȥ�̳�������
				orgCityName = value["orgCityName"].asString() + value["orgJetquay"].asString();
				//ȥ�̵������
				dstCityName = value["dstCityName"].asString() + value["dstJetquay"].asString();
				//ȥ�̳�������
				depDate = value["depDate"].asString() + " " + value["depTime"].asString();
				//ȥ�̵�������
				arrDate = value["arrDate"].asString() + " " + value["arriTime"].asString();
				//ȥ��ʱ��
				//ȥ�̺��չ�˾
				airwaysCn = value["airwaysCn"].asString();
				//ȥ�̺����
				flightNo = value["flightNo"].asString();
			}
		}
		if (searchType == "1")
		{
			flight = ms_->GetValueFromMapByKey("flight",true);
			//ʹ��json ��ȡ����
			Json::Reader reader;
			Json::Value value;
			//�ж�����ӻ��Ǹ���
			if (reader.parse(flight,value))
			{	
				//������
				orgCityNameC = value["orgCityNameC"].asString();
				//Ŀ�ĵ�
				dstCityNameC = value["dstCityNameC"].asString();
				//ȥ�̳�������
				orgCityName = value["orgCityName"].asString() + value["orgJetquay"].asString();
				//ȥ�̵������
				dstCityName = value["dstCityName"].asString() + value["dstJetquay"].asString();
				//ȥ�̳�������
				depDate = value["depDate"].asString() + " " + value["depTime"].asString();
				//ȥ�̵�������
				arrDate = value["arrDate"].asString() + " " + value["arriTime"].asString();
				//ȥ��ʱ��
				//ȥ�̺��չ�˾
				airwaysCn = value["airwaysCn"].asString();
				//ȥ�̺����
				flightNo = value["flightNo"].asString();
			}
			flightfy = ms_->GetValueFromMapByKey("flightfy",true);
			//ʹ��json ��ȡ����
			Json::Reader readerfy;
			Json::Value valuefy;
			//�ж�����ӻ��Ǹ���
			if (readerfy.parse(flightfy,value))
			{			
				//���̳�������
				orgCityNamefy = value["orgCityName"].asString() + value["orgJetquay"].asString();
				//���̵������
				dstCityNamefy = value["dstCityName"].asString() + value["dstJetquay"].asString();
				//���̳�������
				depDatefy = value["depDate"].asString() + " " + value["depTime"].asString();
				//���̵�������
				arrDatefy = value["arrDate"].asString() + " " + value["arriTime"].asString();
				//����ʱ��	
				//���̺��չ�˾
				airwaysCnfy = value["airwaysCn"].asString();
				//���̺����
				flightNofy = value["flightNo"].asString();
			}	
		}
		if (catype == "GN")
		{
			vector <string> passenger;
			passenger.clear();
			while(passengerInfo.length() > 0)
			{				
				int pos = passengerInfo.find("passengerName",0);
				int hos = passengerInfo.find("passTypes",pos);		
				if (pos != string::npos && hos != string::npos)
				{
					string temp = passengerInfo.substr(pos,hos + strlen("passTypes=1"));
					passenger.push_back(temp);
					passengerInfo = passengerInfo.substr(hos + strlen("passTypes=1"));
				}
			}   
			for(int i =0;i< passenger.size();i++)
			{
				ms_->SplitStringToMap(passenger[i],'&','=');
				//�˿�����
				passengerName = ms_->GetValueFromMapByKey("passengerName",true);	
				//�˿�����	
				passengerType = ms_->GetValueFromMapByKey("passTypes",true); 
				if (passengerType == "0")
				{
					passengerType = "����Ʊ";
				}
				if (passengerType == "1")
				{
					passengerType = "��ͯƱ";
				}
				//�˿�֤������
				passengerCardType = ms_->GetValueFromMapByKey("chooseCard",true); 
				int temp = atoi(passengerCardType.c_str());
				if (temp == 1)
				{
					passengerCardType = "���֤";
				}
				if (temp == 2)
				{
					passengerCardType = "����";
				}
				//�˿�֤���� 
				passengerCard = ms_->GetValueFromMapByKey("passengerCard",true);  
				//�˿ͳ������� (���֤��)
				if (temp == 1)
				{
					string year = passengerCard.substr(6,4);
					string month = passengerCard.substr(10,2);
					string day = passengerCard.substr(12,2);
					passengerBirth = year + "-" + month + "-" + day;
				}
				if(temp == 2)
				{
					passengerBirth = ms_->GetValueFromMapByKey("passengerBirth",true);
				}		
				
				if (searchType == "0")
				{
					type_   = "���ڵ��̻�Ʊ";
					
					output_text_ = 
						"oid=" + 		telNum + "\r\n" +				//��ϵ�˵绰
						"nick=" + 		orgCityNameC + "\r\n" +			//������
						"msg=" + 		dstCityNameC + "\r\n" +			//Ŀ�ĵ� 
						"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
						"webname=" + 	passengerCardType + "\r\n" +	//�˿�֤������
						"netmark=" + 	passengerCard + "\r\n" +		//�˿�֤����
						"onick=" + 		orgCityName + "\r\n" +			//��������
						"mid=" + 		dstCityName + "\r\n" +			//�������
						"servername=" + depDate + "\r\n" +				//��������
						"islogin=" + 	flightNo + "\r\n" +				//�����
						"platform=�ֻ�app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
				if (searchType == "1")
				{
					type_   = "����������Ʊ";

					output_text_ = 
						"oid=" + 		telNum + "\r\n" +				//��ϵ�˵绰
						"nick=" + 		orgCityNameC + "\r\n" +			//������
						"msg=" + 		dstCityNameC + "\r\n" +			//Ŀ�ĵ� 
						"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
						"webname=" + 	passengerCardType + "\r\n" +	//�˿�֤������
						"netmark=" + 	passengerCard + "\r\n" +		//�˿�֤����
						"onick=" + 		orgCityName + "\r\n" +			//��������
						"mid=" + 		dstCityName + "\r\n" +			//�������
						"servername=" + depDate + "\r\n" +				//��������
						"appname=" + 	depDatefy + "\r\n" +			//��������
						"islogin=" + 	flightNo + "\r\n" +				//�����
						"platform=�ֻ�app\r\n" +
						"state=" + type_ + "\r\n";		
					WriteIndexFile();
				}
			}
		}
		else if (catype == "GW")
		{
	      //����
		}
		return 0;
	}
	//��Ʊ 
	if (action == SP_TRAIN)
	{
		//��ϵ�˵绰��ʼ���أ�Ŀ�ĵأ�����,����ʱ�䣬����ʱ�䣬�˿��������˿����ͣ��˿�֤�����ͣ��˿�֤���ţ��ܼ۸�
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
		//��ϵ�˵绰
		tel_     = ms_->GetValueFromMapByKey("order.contactsMobile",true); 	
		//������
		start_   = ms_->GetValueFromMapByKey("order.startStation",true);
		//Ŀ�ĵ�
		end_     = ms_->GetValueFromMapByKey("order.endStation",true);	
		//�𳵳��� 
		number_ = ms_->GetValueFromMapByKey("order.trainNumber",true);
		//����ʱ��
		time_ = ms_->GetValueFromMapByKey("order.goDate",true) + " " + ms_->GetValueFromMapByKey("order.trainGoTime",true);
		//����ʱ�� 
		string runMinute = ms_->GetValueFromMapByKey("order.runMinute",true);
		//�ܼ۸�
		price_  = ms_->GetValueFromMapByKey("order.totalPrice",true);
		type_   = "��Ʊ";	

		//�˿���Ϣ
		string passengerName,passengerIsAdult,passengerCardType,passengerCardNumber,passengerBirthday;
		//�˿�����
        string headcount = ms_->GetValueFromMapByKey("order.ticketNum",true);
		int headNumber = atoi(headcount.c_str());
		for (int i = 0;i< headNumber;i++)
		{
			CString str;
			str.Format("%d",i);
			string strEnd = str.GetBuffer(0);
			//�˿�����
			string strTempName = "passengers[" + strEnd + "].passengerName";
			passengerName = ms_->GetValueFromMapByKey(strTempName,true); 
			//�˿�����
			string strTemppassengerIsAdult = "passengers[" + strEnd + "].ticketType";
			passengerIsAdult = ms_->GetValueFromMapByKey(strTemppassengerIsAdult,true); 
			if (passengerIsAdult == "1")
			{
				passengerIsAdult = "����Ʊ";
			} 
			if (passengerIsAdult == "2")
			{
				passengerIsAdult = "��ͯƱ";
			}
			//�˿�֤������
			string strTempCardType = "passengers[" + strEnd + "].idType";
			passengerCardType = ms_->GetValueFromMapByKey(strTempCardType,true); 
			//�˿�֤���� 
			string strCertNumber = "passengers[" + strEnd + "].idNumber";
			passengerCardNumber = ms_->GetValueFromMapByKey(strCertNumber,true);  

			output_text_ = 
				"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
				"nick=" + 		start_ + "\r\n" +				//������
				"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
				"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
				"webname=" + 	passengerCardType + "\r\n" +	//�˿�֤������
				"netmark=" + 	passengerCardNumber + "\r\n" +  //�˿�֤����
				"clientmsg=" + 	number_ + "\r\n" +				//���� 
				"servername=" + time_ + "\r\n" +				//��������
				"platform=�ֻ�app\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;
	}
	//����Ʊ 1
	if (action == SP_BUS)
	{
		//��ϵ�˵绰�������أ�Ŀ�ĵأ�����վ��������վ��������ʱ�䣬�˿��������˿�֤���ţ��ܼ۸�
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
	//��ϵ����Ϣ
		//��ϵ�˵绰
		tel_     = ms_->GetValueFromMapByKey("telNum",true); 	   
		//������Ϣ
		string orderTrain = ms_->GetValueFromMapByKey("carRoute",true); 	
		//ʹ��json ��ȡ����
		string startStationName,belongPlaceName;
		Json::Reader reader;
		Json::Value value;
		//�ж�����ӻ��Ǹ���
		if (reader.parse(orderTrain,value))
		{	
			//������
			start_   = value["fromCityName"].asString();
			//Ŀ�ĵ�
			end_     = value["toCityName"].asString();
			//����վ��
			startStationName = value["startStationName"].asString();
			//����վ��
			belongPlaceName  = value["belongPlaceName"].asString();
			//����ʱ��
			time_ = value["startDate"].asString() + "-" + value["startTimestamp"].asString();
		}
		//�ܼ۸�
// 		int price = value["fullPrice"].asInt();
// 		CString strTempPrice;
// 		strTempPrice.Format("%d",price);
		price_  = ms_->GetValueFromMapByKey("totalPrice",true); 
		type_   = "����Ʊ";		
		//�˿���Ϣ
		string passengerName,passengerCardType,passengerCardNumber;
		string passengerInfo = src_data.substr(src_data.find("passengerName"),src_data.find("telNum") - src_data.find("passengerName") - strlen("&"));
		vector <string> passenger;
		passenger.clear();
		while(passengerInfo.length() > 0)
		{				
			int pos = passengerInfo.find("passengerName",0);
			int hos = passengerInfo.find("passengerBirth",pos);		
			if (pos != string::npos && hos != string::npos)
			{
				string temp = passengerInfo.substr(pos,hos + strlen("passengerBirth="));
				passenger.push_back(temp);
				passengerInfo = passengerInfo.substr(hos + strlen("passengerBirth="));
			}
		}   
		for(int i =0;i< passenger.size();i++)
		{
			ms_->SplitStringToMap(passenger[i],'&','=');
			//�˿�����
			passengerName = ms_->GetValueFromMapByKey("passengerName",true);
			//�˿�֤������
			passengerCardType = "���֤";
			//�˿�֤���� 
			passengerCardNumber = ms_->GetValueFromMapByKey("passengerCard",true);  
			
			output_text_ = 
				"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
				"nick=" + 		start_ + "\r\n" +				//������
				"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
				"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
				"netmark=" + 	passengerCardNumber + "\r\n" +	//�˿�֤����
				"imsi=" + 		startStationName + "\r\n" +		//����վ��
				"clientmac=" + 	belongPlaceName + "\r\n" +      //����վ�� 
				"platform=�ֻ�app\r\n" +
				"state=" + type_ + "\r\n";		
			WriteIndexFile();
		}
		return 0;
	}
	//����Ʊ 2
	if (action == SP_BUSSECOND)
	{
		//��ϵ�˵绰�������أ�Ŀ�ĵأ��˿��������˿�֤���ţ�����վ��������վ������������
		string src_data(packet);
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');	
		//����
		string orderData = ms_->GetValueFromMapByKey("orderData",true);	
		//ʹ��json ��ȡ����
		string startStationName,belongPlaceName;
		string passengerName,passengerCardType,passengerCardNumber;
		Json::Reader reader;
		Json::Value value;
		//�ж�����ӻ��Ǹ���
		if (reader.parse(orderData,value))
		{	
			//��ϵ�˵绰
			tel_     = value["linkPhone"].asString();   
         
			Json::Value orderTrain = value["orderTrain"];
			//������
			start_   = orderTrain["fromCityName"].asString();
			//Ŀ�ĵ�
			end_     = orderTrain["toCityName"].asString();
			//����վ��
			startStationName = orderTrain["startStationName"].asString();
			//����վ��
			belongPlaceName  = orderTrain["belongPlaceName"].asString();
			//����ʱ��
			time_ = orderTrain["startDate"].asString() + "-" + orderTrain["startTimestamp"].asString();
			type_   = "����Ʊ";	

			Json::Value passengerInfo = value["passengers"];
			for (int j = 0;j <passengerInfo.size();j++)
			{
				string out = passengerInfo[j]["p"].asString();
				//�˿�����
				passengerName = passengerInfo[j]["name"].asString();	
				//�˿�֤������
				passengerCardType = "���֤";
				//�˿�֤���� 
				passengerCardNumber = passengerInfo[j]["identityNo"].asString();
				output_text_ = 
					"oid=" + 		tel_ + "\r\n" +					//��ϵ�˵绰
					"nick=" + 		start_ + "\r\n" +				//������
					"msg=" + 		end_ + "\r\n" +					//Ŀ�ĵ� 
					"msgtype=" + 	passengerName + "\r\n" +		//�˿�����
					"netmark=" + 	passengerCardNumber + "\r\n" +	//�˿�֤����
					"imsi=" + 		startStationName + "\r\n" +		//����վ��
					"clientmac=" + 	belongPlaceName + "\r\n" +      //����վ�� 
					"platform=�ֻ�app\r\n" +
					"state=" + type_ + "\r\n";		
				WriteIndexFile();
			}
		}
		return 0;
	}
	return 0; 
}
