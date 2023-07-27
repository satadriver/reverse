
#include "StdAfx.h"

#include "PlaneChunQiuAirlines.h"

plane::ChunQiuAirlines::ChunQiuAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ChunQiuAirlines::~ChunQiuAirlines()
{
	
}

// NI ���֤��PP ���գ�ORI ����֤��MIL ����֤��OTHER ������HTPP �۰�ͨ��֤��BR ��������
string plane::ChunQiuAirlines::GetIdType(string idType)
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

int plane::ChunQiuAirlines::IsChunQiuAirlines(HttpSession *http_session)
{
	//�г�Ԥ��
	if((http_session->m_Requestline.m_Host.Find("flights.ch.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/default/SearchByTime")!=-1))
	{
		return kpcticket;
	}
	return 0;
}

int plane::ChunQiuAirlines::Process(const char *packet,int action)
{
	//�г�Ԥ��
	if (action == kpcticket)
	{
		return pcticket(packet);
	}
	return 1;
}
// �г�Ԥ��
int plane::ChunQiuAirlines::pcticket(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->Replace(src_data,"?","&");
	ms_->SplitStringToMap(src_data,'&','=');
	string IfRet = ms_->GetValueFromMapByKey("IfRet",true);
	//����
	if (IfRet=="false")
	{
		deptCdTxt = ms_->GetValueFromMapByKey("OriCity",true);
		arrCdTxt = ms_->GetValueFromMapByKey("DestCity",true);
		deptDt =ms_->GetValueFromMapByKey("FDate",true);
		adtCount = ms_->GetValueFromMapByKey("ANum",true);
		chdCount = ms_->GetValueFromMapByKey("CNum",true);
		infCount = ms_->GetValueFromMapByKey("INum",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
		{
			return 0;
		}
		output_text_ =
			"netmark=" +adtCount + "\r\n" +		//��������
			"onick=" +	chdCount + "\r\n" +		//��ͯ����
			"sex=" +	infCount + "\r\n" +		//Ӥ������
			"nick=" +	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" +	//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	else if(IfRet == "true")
	{
		deptCdTxt = ms_->GetValueFromMapByKey("OriCity",true);
		arrCdTxt = ms_->GetValueFromMapByKey("DestCity",true);
		deptDt =ms_->GetValueFromMapByKey("FDate",true);
		string arrdt = ms_->GetValueFromMapByKey("RetFDate",true);
		adtCount = ms_->GetValueFromMapByKey("ANum",true);
		chdCount = ms_->GetValueFromMapByKey("CNum",true);
		infCount = ms_->GetValueFromMapByKey("INum",true);
		if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty() || arrdt.empty())
		{
			return 0;
		}

		output_text_ =
			"netmark=" +adtCount + "\r\n" +		//��������
			"onick=" +	chdCount + "\r\n" +		//��ͯ����
			"sex=" +	infCount + "\r\n" +		//Ӥ������
			"nick=" +	deptCdTxt + "\r\n" +	//������
			"msg=" + 	arrCdTxt + "\r\n" +		//Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" +	//��������
			"appname=" +arrdt +"\r\n" +			//��������
			"state=��ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}