
#include "StdAfx.h"
#include "QiChePiao.h"



QiChePiao::QiChePiao(HttpSession *http_session, const string &pro_name) :  Traffic(http_session, pro_name)
{
	
}

QiChePiao::~QiChePiao()
{
	
}

void QiChePiao::SplitString(const string& s, vector<string>& v, const string& c)
{
    string::size_type pos1, pos2;
    pos2 = s.find(c);
    pos1 = 0;
    while(string::npos != pos2)
    {
        v.push_back(s.substr(pos1, pos2-pos1));
		
        pos1 = pos2 + c.size();
        pos2 = s.find(c, pos1);
    }
    if(pos1 != s.length())
        v.push_back(s.substr(pos1));
}


int QiChePiao::Is(HttpSession *http_session)
{
	//pc��¼
	if((http_session->m_Requestline.m_Host.Find("passport.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login")!=-1))
	{
		return kDriverLogin;
	}
	//pc��ѯ��Ʊ
	if((http_session->m_Requestline.m_Host.Find("www.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/index/searchModel")!=-1))
	{
		return kNewVoiceOrder;
	}
	//pc�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/submitOrder")!=-1))
	{
		return kPreOrder;
	}
	//pc����
	if((http_session->m_Requestline.m_Host.Find("www.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/baoche/rentBusConfirmOrder")!=-1))
	{
		return kNewTextOrder;
	}

	//��׿��¼
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/captchaLogin")!=-1))
	{
		return kPassengerLogin;
	}

	//�ֻ��ύ����
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/confirmOrder")!=-1))
	{
		return kDriverWorking;
	}

	//�ֻ���ѯ��Ʊ
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/schedule/getLineList.srv")!=-1))
	{
		return kDriverRegister;
	}

	//�ֻ�����
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/confirmRentBusOrder")!=-1))
	{
		return kMobileBaoChe;
	}
	//�ֻ�Լ��
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/confirmZuCheOrderByPoi")!=-1))
	{
		return kMobileYueChe;
	}

	//�ֻ�����
	if((http_session->m_Requestline.m_Host.Find("webapi.12308.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bus/newConfirmBusOrder")!=-1))
	{
		return kMobileTrip;
	}

	return 0;
}

int QiChePiao::Process(const char *packet,int action)
{
	//pc��¼
	if (action == kDriverLogin)
	{
			return DriverLogin(packet);
	}
	//pc��ѯ��Ʊ
	if (action == kNewVoiceOrder)
	{
		return NewVoiceOrder(packet);
	}
	//pc�ύ����
	if (action == kPreOrder)
	{
		return PreOrder(packet);
	}
	//pc����
	if (action == kNewTextOrder)
	{
		return NewTextOrder(packet);
	}
	//��׿��¼
	if (action == kPassengerLogin)
	{
		return PassengerLogin(packet);
	}
	//�ֻ��ύ����
	if (action == kDriverWorking)
	{
		return DriverWorking(packet);
	}
	//�ֻ���ѯ��Ʊ
	if (action == kDriverRegister)
	{
		return DriverRegister(packet);
	}
	//�ֻ�����
	if (action == kMobileBaoChe)
	{
		return MobileBaoChe(packet);
	}
	//�ֻ�Լ��
	if (action == kMobileYueChe)
	{
		return MobileYueChe(packet);
	}
	//�ֻ��쳵
	if (action == kMobileTrip)
	{
		return MobileTrip(packet);
	}
	return 0;
}


//pc��¼��Ϣ
int QiChePiao::DriverLogin(const char *packet)
{	
	string src_data(packet);
	ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string username,password;
	//�û���
	username = ms_->GetValueFromMapByKey("username",true);
	//����
	password = ms_->GetValueFromMapByKey("password",true);
 
	if (username.empty() || password.empty())
	{
	 	return 0;
	}
	
	output_text_ = 
	 	"user=" + username + "\r\n" +
	 	"pass=" + password + "\r\n" + 
		"platform=���������\r\n" +
	 	"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}

//pc��ѯ��Ʊ��ַ����
int QiChePiao::NewVoiceOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//��ȡ��Ҫ������
	src_data = src_data.substr(src_data.rfind("startCityName="),src_data.length()-src_data.rfind("startCityName"));
	ms_->SplitStringToMap(src_data,'&','=');
    //������
	start_ = ms_->GetValueFromMapByKey("startCityName",true);	
	//Ŀ�ĵ�
	end_ = ms_->GetValueFromMapByKey("endCityName",true);
	//��������
	time_ = ms_->GetValueFromMapByKey("startDate",true);
		
	if (start_.empty() || end_.empty() || time_.empty())
	{
		return 0;
	}
	output_text_ = 
			   "nick=" + start_ + "\r\n" +
			   "msg=" + end_ + "\r\n" +
			   "servername=" + time_ + "\r\n" +
			   "platform=���������\r\n" +
			   "state=��ѯ����Ʊ\r\n";
	WriteIndexFile();	

	return 1;
}

//pc��ƱԤ���ύ����
int QiChePiao::PreOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ϵ����Ϣ (ȡƱ��)
	//����
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//�绰
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//���֤��
	string idcard = ms_->GetValueFromMapByKey("certificateNo",true); 

    //�˿���Ϣ
	//passengerInfo=������,17195279563,520322198712270862,01,0,0.0;���ʾ�,17195279564,640324198704213155,01,0,0.0;
	string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);
	vector<string> vPassenger;
	//�ָ��� ;
	SplitString(passengerInfo, vPassenger,";"); 
	for(vector<string>::size_type i = 0; i != vPassenger.size(); ++i)
	{
		vector<string> vInfo;
		//�ָ��� ,
		SplitString(vPassenger[i], vInfo,","); 
		//�˿��������˿͵绰�������֤��
		string passengerName,passengerTel,passengerIdcard;
		passengerName = vInfo[0];
		passengerTel = vInfo[1];
		passengerIdcard = vInfo[2];

		if (name_.empty() || tel_.empty() || idcard.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"mid=" + name_ + "\r\n" +				//��ϵ������
			"oid=" + tel_ + "\r\n" +				//��ϵ�˵绰
			"userid=" + idcard + "\r\n" +			//��ϵ��֤����
			"msgtype=" + passengerName + "\r\n" +	//�˿�����
			"clientip=" + passengerTel + "\r\n" +	//�˿͵绰
			"netmark=" + passengerIdcard + "\r\n" + //�˿�֤����
			"platform=���������\r\n" +
			"state=����Ʊ\r\n";	
		WriteIndexFile();		
	}   
	return 1;
}

//pc����
int QiChePiao::NewTextOrder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//��ϵ����Ϣ
	//����
    name_ = ms_->GetValueFromMapByKey("userName",true);
	//�绰
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//�����ص�
	start_ = ms_->GetValueFromMapByKey("startCity",true) + ms_->GetValueFromMapByKey("startPlace",true);	
	//����ص�
	end_ = ms_->GetValueFromMapByKey("endCity",true) + ms_->GetValueFromMapByKey("arrivalPlace",true);		
	//����ʱ�� (�ó�ʱ��)
	time_ = ms_->GetValueFromMapByKey("startTime",true);

	//�Ƿ�����
	string isBack,arrivalTime;
	isBack = ms_->GetValueFromMapByKey("isBack",true);
	//����
	if (isBack == "1")
	{
		arrivalTime = "��";
	}
	//����
	if (isBack == "2")
	{
		//����ʱ�� (����ʱ��)
		arrivalTime = ms_->GetValueFromMapByKey("arrivalTime",true);
	}
	//��������
	number_ = ms_->GetValueFromMapByKey("personCount",true);
	//�������� (����7�� 2-1 ,С�ͳ�22�� 1-1 ,�аͳ�33�� 3-1,��ͳ�53�� 4-1)
	type_  = ms_->GetValueFromMapByKey("rentBusInfo",true);
    
	//�����ص�����(����) ���� 
	string startpos = ms_->GetValueFromMapByKey("startLongLat",true);
	//����ص�����(����)
	string endpos = ms_->GetValueFromMapByKey("endLongLat",true);
	//��Ʊ��Ϣ(����) ��Ʊ���⣬�ˣ���ַ
    string invoice = ms_->GetValueFromMapByKey("invoice",true);
	if (invoice == "1")
	{
		string invoiceInfo = ms_->GetValueFromMapByKey("invoiceTitle",true) + ms_->GetValueFromMapByKey("recipients",true) + ms_->GetValueFromMapByKey("address",true);
	}
	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"appname=" + arrivalTime + "\r\n" + 
		"fileurl=" + number_ + "\r\n" +
		"subject=" + type_ + "\r\n" + 
		"platform=���������\r\n" +
		"state=����\r\n";
	WriteIndexFile();

	return 1;
}


//�ֻ���¼
int QiChePiao::PassengerLogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
    
	string username,password;
	username = ms_->GetValueFromMapByKey("mobilePhone",true);
	//����Ϊmd5 ����
	password = ms_->GetValueFromMapByKey("password",true);

	if (username.empty() || password.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + username + "\r\n" +
		"pass=" + password + "\r\n" + 
		"platform=�ֻ�App\r\n" +
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}


//�ֻ���ѯ��Ʊ //���⻹�ɲ�ѯ��ϸ�ĳ���

int QiChePiao::DriverRegister(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	//��ȡ��Ҫ������
	src_data = src_data.substr(src_data.rfind("sign="),src_data.length()-src_data.rfind("sign"));
	ms_->SplitStringToMap(src_data,'&','=');
    //������
	start_ = ms_->GetValueFromMapByKey("startCityName",true);
	//Ŀ�ĵ�
	end_ = ms_->GetValueFromMapByKey("endCityName",true);
	//��������
	time_ = ms_->GetValueFromMapByKey("startDate",true);
	
	if (start_.empty() || end_.empty() || time_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"platform=�ֻ�App\r\n" +
		"state=��ѯ����Ʊ\r\n";
	WriteIndexFile();	
	
	return 1;
}

//�ֻ��ύ����
int QiChePiao::DriverWorking(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//��ϵ����Ϣ (ȡƱ��)
	//����
	name_ = ms_->GetValueFromMapByKey("realName",true);
	//�绰
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//���֤��
	string idcard = ms_->GetValueFromMapByKey("certificateNo",true); 
    //�˿���Ϣ
	//passengerInfo=������,17195279563,520322198712270862,01,0,0.0;���ʾ�,17195279564,640324198704213155,01,0,0.0
    string passengerInfo = ms_->GetValueFromMapByKey("passengerInfo",true);

	vector<string> vPassenger;
	//�ָ��� ;
	SplitString(passengerInfo, vPassenger,";"); 
	for(vector<string>::size_type i = 0; i != vPassenger.size(); ++i)
	{
		vector<string> vInfo;
		//�ָ��� ,
		SplitString(vPassenger[i], vInfo,","); 
		//�˿��������˿͵绰�������֤��
		string passengerName,passengerTel,passengerIdcard;
		passengerName = vInfo[0];
		passengerTel = vInfo[1];
		passengerIdcard = vInfo[2];
		
		if (name_.empty() || tel_.empty() || idcard.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"mid=" + name_ + "\r\n" +
			"oid=" + tel_ + "\r\n" +
			"userid=" + idcard + "\r\n" +
			"msgtype=" + passengerName + "\r\n" +
			"clientip=" + passengerTel + "\r\n" +
			"webname=" + passengerIdcard + "\r\n" +
			"platform=�ֻ�App\r\n" +
			"state=����Ʊ\r\n";	
		WriteIndexFile();		
	}   
	return 1;
}


//�ֻ�����
int QiChePiao::MobileBaoChe(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//��ϵ����Ϣ
	//����
    name_ = ms_->GetValueFromMapByKey("userName",true);
	//�绰
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);

	//����ʱ�� (�ó�ʱ��)
	time_ = ms_->GetValueFromMapByKey("startTime",true);
	//�����ص�
	start_ = ms_->GetValueFromMapByKey("startCity",true) + ms_->GetValueFromMapByKey("startPlace",true);	
	//����ص�
	end_ = ms_->GetValueFromMapByKey("endCity",true) + ms_->GetValueFromMapByKey("arrivalPlace",true);	

	//�Ƿ�����
	string isBack,arrivalTime;
	isBack = ms_->GetValueFromMapByKey("isBack",true);
	//����
	if (isBack == "1")
	{
		arrivalTime = "��";
	}
	//����
	if (isBack == "2")
	{
		//����ʱ�� (����ʱ��)
		arrivalTime = ms_->GetValueFromMapByKey("arrivalTime",true);
	}

	//��������
	number_ = ms_->GetValueFromMapByKey("personCount",true);
	//�������� (����7�� 2-1 ,С�ͳ�22�� 1-1 ,�аͳ�33�� 3-1,��ͳ�53�� 4-1)
	type_  = ms_->GetValueFromMapByKey("rentBusInfo",true);
    
	//�����ص�����(����) ���� 
	string startpos = ms_->GetValueFromMapByKey("startLongLat",true);
	//����ص�����(����)
	string endpos = ms_->GetValueFromMapByKey("endLongLat",true);
	//��Ʊ��Ϣ(����) ��Ʊ���⣬�ˣ���ַ
    string invoice = ms_->GetValueFromMapByKey("invoice",true);
	if (invoice == "1")
	{
		string invoiceInfo = ms_->GetValueFromMapByKey("invoiceTitle",true) + ms_->GetValueFromMapByKey("recipients",true) + ms_->GetValueFromMapByKey("address",true);
	}
	
	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"appname=" + arrivalTime + "\r\n" + 
		"fileurl=" + number_ + "\r\n" +
		"subject=" + type_ + "\r\n" + 
		"platform=�ֻ�App\r\n" +
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}


//�ֻ�Լ��
int QiChePiao::MobileYueChe(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//�û�id
	name_ = ms_->GetValueFromMapByKey("userId",true);
	//�绰
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//������
	start_ = ms_->GetValueFromMapByKey("getOnPlace",true);
	//Ŀ�ĵ�
	end_ = ms_->GetValueFromMapByKey("getOffPlace",true);
    //����ʱ��
	time_ = ms_->GetValueFromMapByKey("startTime",true);
	//BUS id
	number_ = ms_->GetValueFromMapByKey("busId",true);

	if (name_.empty() || start_.empty() || end_.empty() || start_.empty() || time_.empty())
	{
		return 0;
	}

	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"sex=" + number_ + "\r\n" +
		"platform=�ֻ�App\r\n" +
		"state=�ֻ�Լ�⳵\r\n";
	
	WriteIndexFile();
	
	return 1;

}
//�ֻ����� (����)
int QiChePiao::MobileTrip(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
 
	//ȡƱ����Ϣ
	//�û���
	name_ = ms_->GetValueFromMapByKey("userName",true);
	//�ֻ���
	tel_ = ms_->GetValueFromMapByKey("mobilePhone",true);
	//ʱ��
	time_ = ms_->GetValueFromMapByKey("startTime",true);
	//��ʼ��ַ
	start_ = ms_->GetValueFromMapByKey("startPlace",true);
	//�����ַ
	end_ = ms_->GetValueFromMapByKey("endPlace",true);
    //����
	number_ = ms_->GetValueFromMapByKey("count",true);

	if (name_.empty() || tel_.empty() || time_.empty() || start_.empty() || end_.empty())
	{
		return 0;
	}

	output_text_ =
		"mid=" + name_ + "\r\n" +
		"oid=" + tel_ + "\r\n" +
		"nick=" + start_ + "\r\n" +
		"msg=" + end_ + "\r\n" +
		"servername=" + time_ + "\r\n" +
		"fileurl=" + number_ + "\r\n" +
		"platform=�ֻ�App\r\n" +
		"state=�ֻ�����\r\n";
	WriteIndexFile();
	return 1;
}
