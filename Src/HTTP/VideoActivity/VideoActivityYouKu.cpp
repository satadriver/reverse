#include "StdAfx.h"
#include "VideoActivityYouKu.h"

videoactivity::YouKuVideo::YouKuVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "�ſ���Ƶ";	
}

videoactivity::YouKuVideo::~YouKuVideo()
{
	
}

int videoactivity::YouKuVideo::IsYouKuVideo(HttpSession *http_session)
{
	//��ҳ����
	//pc�ͻ�������
	if((http_session->m_Requestline.m_Host.Find("www.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/passport")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� �ظ�
	if((http_session->m_Requestline.m_Host.Find("p.comments.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ycp/comment/pc/add")!=-1)) 
	{
		return kPcWebComment;
	}
	//��ҳ��Ļ 
	if((http_session->m_Requestline.m_Host.Find("service.danmu.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add?t")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//pc�ͻ��˵�Ļ
	if((http_session->m_Requestline.m_Host.Find("service.danmu.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("add")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("add?") ==-1)) 
	{
		return kPcClientDanMu;
	}
	//android���� 
	if((http_session->m_Requestline.m_Host.Find("api.appsdk.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/i/s?")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android���� �ظ� 
	if((http_session->m_Requestline.m_Host.Find("p.comments.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ycp/comment/mobile/add")!=-1)) 
	{
		return kAndroidComment;
	}
	//android��Ļ
	if((http_session->m_Requestline.m_Host.Find("dmapp.youku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/common/danmu/send")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::YouKuVideo::Process(const char *packet,int action)
{
	//��ҳ����
	//pc�ͻ�������
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//��ҳ���� �ظ�
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//��ҳ��Ļ
	if (action == kPcWebDanMu)
	{
		return PcWebDanMu(packet);
	}
	//pc�ͻ��˵�Ļ
	if (action == kPcClientDanMu)
	{
		return PcClientDanMu(packet);
	}
	//android����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//Android���� �ظ�
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//Android��Ļ
	if (action == kAndroidDanmu)
	{
		return AndroidDanMu(packet);
	}
	return 1;
}
//��ҳ����
//pc�ͻ�������
int videoactivity::YouKuVideo::PcWebSearch(const char *packet)
{
	//��referer ����ȡ����
	string referer(http_session_->m_Requestline.m_Referer.GetBuffer(0));
	//��ҳ����  
	if (referer.find("search_video") != string::npos)  
	{
		string comment;
		if (referer.find("?f") == string::npos)
		{
			//����
			comment = referer.substr(referer.find("q_") + strlen("q_"));
			comment = ms_->UTF8ToGBK(UrlDecode(comment));
		}
		if (referer.find("?f") != string::npos)
		{
			//����
			comment = ms_->GetValueBetweenBeginEnd(referer,"q_","?f",true);  
			comment = ms_->UTF8ToGBK(UrlDecode(comment));
		}
		if (comment.empty())
		{
			return 0;
		}
		output_text_ =	
			"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";						//64	[19]  //״̬
		WriteIndexFile();
	}
	//pc�ͻ�������
	if (referer.find("search_ikuvideo") != string::npos)  
	{
		string comment;
		if (referer.find("?f") == string::npos)
		{
			//����
			comment = referer.substr(referer.find("q_") + strlen("q_"));
			comment = ms_->UTF8ToGBK(UrlDecode(comment));
		}
		if (referer.find("?f") != string::npos)
		{
			//����
			comment = ms_->GetValueBetweenBeginEnd(referer,"q_","?f",true);  
			comment = ms_->UTF8ToGBK(UrlDecode(comment));
		}
		if (comment.empty())
		{
			return 0;
		}
		output_text_ =	
			"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
			"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";						//64	[19]  //״̬
		WriteIndexFile();
	}
	return 1;
}

//��ҳ���� �ظ�
int videoactivity::YouKuVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����ƽ̨
	string app = ms_->GetValueFromMapByKey("app",true);
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	//�ж� ���� �ظ�
	string sourceCommentId = ms_->GetValueFromMapByKey("sourceCommentId",true);
	if (comment.empty())
	{
		return 0;
	}
	if (sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";						//64	[19]  //״̬
	}
	if (!sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";						//64	[19]  //״̬
	}					
	WriteIndexFile();
	return 1;
}

//��ҳ��Ļ
int videoactivity::YouKuVideo::PcWebDanMu(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";						//64	[19]  //״̬				
	WriteIndexFile();
	return 1;
}

//pc�ͻ��˵�Ļ
int videoactivity::YouKuVideo::PcClientDanMu(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";						//64	[19]  //״̬				
	WriteIndexFile();
	return 1;
}

//Android����
int videoactivity::YouKuVideo::AndroidSearch(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');

	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("keyword",true);  

	if (comment.empty())
	{
		return 0;
	}
	string platform = "Android��";

	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//Android���� �ظ�
int videoactivity::YouKuVideo::AndroidComment(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	//����ƽ̨
	string app = ms_->GetValueFromMapByKey("app",true);
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	//�ж� ���� �ظ�
	string sourceCommentId = ms_->GetValueFromMapByKey("sourceCommentId",true);
	if (comment.empty())
	{
		return 0;
	}
	if (sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";							//64	[19]  //״̬
	}
	if (!sourceCommentId.empty())
	{
		output_text_ =				
			"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" +	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";							//64	[19]  //״̬
	}					
	WriteIndexFile();
	return 1;
}

//Android��Ļ
int videoactivity::YouKuVideo::AndroidDanMu(const char *packet)
{
	//��cookie �л���ǳ�
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("u");
	if (nickname.empty())
	{
		nickname = "�ſ��û�null";
	}
	//�����ݲ��ֻ������
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//��ȡbase64���ܵ���Ϣ
	string msg_base64 = ms_->GetValueFromMapByKey("msg");
	int out_byte = 0;
	string msg = ms_->Base64Decode(msg_base64,out_byte);
	string content;
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(msg,value))
	{	
		//����
		content = value["content"].asString();
		content = ms_->UTF8ToGBK(UrlDecode(content));
	}
	if (content.empty())
	{
		return 0;
	}
	output_text_ =			
		"nick="	 +		nickname + "\r\n" + 			//58	[123] //�ǳ�
		"msg=" +		content + "\r\n" + 				//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +		//62	[126] //����ƽ̨
		"state=��Ļ\r\n";								//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}