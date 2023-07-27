#include "StdAfx.h"
#include "SearchSoChina.h"

search::SoChina::SoChina(HttpSession *http_session, const string &pro_name) : Search(http_session, pro_name)
{
	
}

search::SoChina::~SoChina()
{
	
}

string search::SoChina::GetType(string idType)
{
	string strType;
	if (idType == "0"){strType = "ȫ��";}
	if (idType == "1"){strType = "ȫ��";}
	if (idType == "2"){strType = "����";}
	if (idType == "3"){strType = "����";}
	if (idType == "4"){strType = "����";}
	if (idType == "5"){strType = "��Ϸ";}
	if (idType == "6"){strType = "�Ƽ�";}
	if (idType == "7"){strType = "����";}
	if (idType == "8"){strType = "����";}
	if (idType == "9"){strType = "��Ƶ";}
	if (idType == "10"){strType = "�Ļ�";}
	return strType;
}

int search::SoChina::IsSoChina(HttpSession *http_session)
{
	//�ѱ�վ
	if((http_session->m_Requestline.m_Host.Find("so.china.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/cse/search")!=-1))
	{
		return kWWW;
	}
	//��ȫ��
	if((http_session->m_Requestline.m_Host.Find("so.china.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/customsearch/click")!=-1))
	{
		return kWWW;
	}
	return 0;
}

int search::SoChina::Process(const char *packet,int action)
{
	if (action == kWWW)
	{
		return WWW(packet);
	}
	return 0;
}

int search::SoChina::WWW(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//��������
	string nsid = ms_->GetValueFromMapByKey("nsid",true);
	type_ = GetType(nsid);
	//�ؼ���
	keyword_ = ms_->GetValueFromMapByKey("q",true);	

	if (keyword_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + keyword_ + "\r\n" +
		"state=" + type_ + "\r\n";
	WriteIndexFile();
	return 1;
}