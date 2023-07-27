
#include "StdAfx.h"

#include "PlaneLianHeAirlines.h"

plane::LianHeAirlines::LianHeAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::LianHeAirlines::~LianHeAirlines()
{
	
}

// ID_CARD ���֤��PASSPORT ���գ�OTHER ����֤��
string plane::LianHeAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "ID_CARD"){idTypeTrue = "���֤";}
	if (idType == "PASSPORT"){idTypeTrue = "����";}
	if (idType == "OTHER"){idTypeTrue = "����֤��";}
	return idTypeTrue;
}

int plane::LianHeAirlines::IsLianHeAirlines(HttpSession *http_session)
{
	//�г�Ԥ��
	if((http_session->m_Requestline.m_Host.Find("www.flycua.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/otabooking/flight-search!doFlightSearch.shtml")!=-1))
	{
		return kpcticket;
	}
	return 0;
}

int plane::LianHeAirlines::Process(const char *packet,int action)
{
	//�г�Ԥ��
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	return 1;
}

// �г�Ԥ��
int plane::LianHeAirlines::pcticket(const char *packet)
{
	string src_data(packet);
	string tookit = "earchCond=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit) + tookit.length());
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));	
	//��ȡjson
	Json::Value value;
	Json::Reader reader;
	if (reader.parse(src_data,value))
	{
		//����
		triptype_ = value["tripType"].asString();
		CString str;
		//��������
		int adt = value["adtCount"].asInt();
		str.Format("%d",adt);
		adtCount = str.GetBuffer(0);
		//��ͯ����
		int chd = value["chdCount"].asInt();
		str.Format("%d",chd);
		chdCount = str.GetBuffer(0);
		//Ӥ������
		int inf = value["infCount"].asInt();
		str.Format("%d",inf);
		infCount = str.GetBuffer(0);
		//
		Json::Value segmentList = value["segmentList"];
		int one = 0,two = 1;
		//������
		deptCdTxt = segmentList[one]["deptCd"].asString();
		//Ŀ�ĵ�
		arrCdTxt = segmentList[one]["arrCd"].asString();
		//��������
		deptDt = segmentList[one]["deptDt"].asString();
		//Ŀ������ 
		deptDtRe = segmentList[two]["deptDt"].asString();
		if ("OW" == triptype_)
		{
			output_text_ =
				"netmark=" + adtCount + "\r\n" +   //��������    
				"onick=" + chdCount + "\r\n" +	   //��ͯ����    
				"sex=" + infCount + "\r\n" +	   //Ӥ������ 
				"nick=" + deptCdTxt + "\r\n" +	   //������
				"msg=" + arrCdTxt + "\r\n" +	   //Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +  //��������
				"state=���ڵ��̲�ѯ\r\n";
			WriteIndexFile();
		}
		else if ("RT" == triptype_)
		{
			output_text_ =
				"netmark=" + adtCount + "\r\n" +   //��������    
				"onick=" + chdCount + "\r\n" +	   //��ͯ����    
				"sex=" + infCount + "\r\n" +	   //Ӥ������ 
				"nick=" + deptCdTxt + "\r\n" +	   //������
				"msg=" + arrCdTxt + "\r\n" +	   //Ŀ�ĵ� 
				"servername=" + deptDt + "\r\n" +  //��������
				"appname=" + deptDtRe + "\r\n" +   //�������� 
				"state=����������ѯ\r\n";
			WriteIndexFile();
		}
	}
	return 1;
}
