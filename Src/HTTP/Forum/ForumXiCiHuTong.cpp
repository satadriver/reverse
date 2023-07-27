#include "StdAfx.h"

#include "ForumXiCiHuTong.h"

forum::XiCiHuTong::XiCiHuTong(HttpSession *http_session, const string &pro_name) : Forum(http_session, pro_name)
{
	
}

forum::XiCiHuTong::~XiCiHuTong()
{
	
}

int forum::XiCiHuTong::IsXiCiHuTong(HttpSession *http_session)
{
	//pc �˵�½
	if((http_session->m_Requestline.m_Host.Find("account.xici.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc �� ���� ����
	if((http_session->m_Requestline.m_Host.Find("www.xici.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/b")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/put.asp")!=-1))
	{
		return kPCWebFaTie;
	}
	//pc ������
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("life.xici.net/news") != -1))
	{
		return kPCWebNewComment;
	}
	return 0;
}

int forum::XiCiHuTong::Process(const char *packet,int action)
{
	//pc �˵�½
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}
	//pc �˸��� ����
	if (action == kPCWebFaTie)
	{
		return PCWebFaTie(packet);
	}
	//pc ������
	if (action == kPCWebNewComment)
	{
		return PCWebNewComment(packet);
	}
	return 0;
}


//pc �˵�½
int forum::XiCiHuTong::PCWebLogin(const char *packet)
{
	//��ȡ ���ݲ���
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("refer") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("refer"));
	}	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');

	//�û���
	user_ = ms_->GetValueFromMapByKey("username",true);
	//����
	password_ = ms_->GetValueFromMapByKey("password",true);
	//Ŀ�� url
	string url;
	url = ms_->GetValueFromMapByKey("refer",true);
	if ( user_.empty() || password_.empty())
	{
		return 0;
	}
	output_text_ =	
		"user=" + user_ + "\r\n" +
		"pass=" + password_ + "\r\n" +
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}


//pc �˸��� ����
int forum::XiCiHuTong::PCWebFaTie(const char *packet)
{
	//�ж��� ���� ����
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	if (referer.find(".htm") != -1)
	{
		//����
		string src_data(packet);
		ms_->SplitStringToMap(src_data,'&','=');		
		//���±��	
		string lDocId;
		lDocId = ms_->GetValueFromMapByKey("lDocId",true);
		//����
		content_ = ms_->GetValueFromMapByKey("doc_text",true);
		
		if (lDocId.empty() || content_.empty())
		{
			return 0;
		}		
		WriteCaseInfo(content_);
		output_text_ =
			"msg=" + content_ + "\r\n" +
			"state=����\r\n";	
		WriteIndexFile();
	}
	else
	{
		//����
		string src_data(packet);	
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string sTitle,doc_title;
		//����
		sTitle = ms_->GetValueFromMapByKey("sTitle",true);
		//����
		doc_title = ms_->GetValueFromMapByKey("doc_title",true);
		//����
		content_ = ms_->GetValueFromMapByKey("doc_text",true);
		if (sTitle.empty() || doc_title.empty() || content_.empty())
		{
			return 0;
		}
		WriteCaseInfo(content_);
		output_text_ = 
			"msgtype=" + doc_title + "\r\n" +
			"msg=" + content_ + "\r\n" +
			"state=����\r\n";
		WriteIndexFile();
	}
	return 1;
}

//pc ������
int forum::XiCiHuTong::PCWebNewComment(const char *packet)
{
	//����
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("client_id=") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("client_id="));
	}	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
	
	string topic_id,topic_title,topic_url;
	//����ID
	topic_id = ms_->GetValueFromMapByKey("topic_id",true);
	//��������
	topic_title = ms_->GetValueFromMapByKey("topic_title",true);
	//�����ַ
	topic_url = ms_->GetValueFromMapByKey("topic_url",true);
	//��������
	content_ = ms_->GetValueFromMapByKey("content",true);	
	if (topic_title.empty() || content_.empty())
	{
		return 0;
	}
	WriteCaseInfo(content_);
	output_text_ =	
		"msgtype=" + topic_title + "\r\n" +
		"nick=" + topic_url + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

