#include "StdAfx.h"
#include "VideoActivityTuDou.h"

videoactivity::TuDouVideo::TuDouVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "������Ƶ";	
}

videoactivity::TuDouVideo::~TuDouVideo()
{
	
}

int videoactivity::TuDouVideo::IsTuDouVideo(HttpSession *http_session)
{
	//��ҳ���� 1
	if((http_session->m_Requestline.m_Host.Find("www.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/t/nisearch")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/t/nisearch.do")==-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� 2
	if((http_session->m_Requestline.m_Host.Find("www.soku.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/t/nisearch.do?kw=")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� �ظ�
	if((http_session->m_Requestline.m_Host.Find("www.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comments/itemnewcomment.srv?method=add")!=-1)) 
	{
		return kPcWebComment;
	}
	//��ҳ��Ļ 
	if((http_session->m_Requestline.m_Host.Find("service.danmu.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add?t")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//��ҳ���� 1  ������¼
	if((http_session->m_Requestline.m_Host.Find("log.mmstat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/yt.gif?")!=-1)) 
	{
		return kPcWebPlay;
	}
	//��ҳ���� 2  �鿴��һ����¼
	if((http_session->m_Requestline.m_Host.Find("gm.mmstat.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/yt/preclk")!=-1)) 
	{
		return kPcWebPlay;
	}
	//pc�ͻ������� 
	if((http_session->m_Requestline.m_Host.Find("itudoupage.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/itudoupage/search.action?")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc�ͻ��˵�Ļ
	if((http_session->m_Requestline.m_Host.Find("service.danmu.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/add?ver")!=-1)) 
	{
		return kPcClientDanMu;
	}
	//android���� (��Ƶ) /v4/search/spread?pidetwork=WIFI&keyword=%E9%82%A3%E5%A5%B3%E4%BA%BA%E7%9A%84%E9%80%89%E6%8B%A924
	if((http_session->m_Requestline.m_Host.Find("search.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4/search/spread")!=-1)) 
	{
		//return kAndroidSearch;
	}
	//android���� (��Ƶ��) /v4/search/spread?pidetwork=WIFI&keyword=%E9%82%A3%E5%A5%B3%E4%BA%BA%E7%9A%84%E9%80%89%E6%8B%A924
	if((http_session->m_Requestline.m_Host.Find("search.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4/search/spread")!=-1)) 
	{
		//return kAndroidSearch;
	}
	//android���� �ظ� /v3_1/comment?pid=2bee25aa5485c2bb&_t_=1486533089&_e_=md5&_s_=383c1b65d80ae3c16c406edc244e5c33&guid=6347920e68ce220dc429967e72160ca6&ver=5.9&network=WIFI&itemCode=dDGzEnDSXzI&comment=hello
	if((http_session->m_Requestline.m_Host.Find("user.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v3_1/comment")!=-1)) 
	{
		//return kAndroidComment;
	}
	//android��Ļ /v4_5/danmu/add?pid=2bee25aa5485c2bb&_t_=1486532241&_e_=md5&_s_=70420a583be4d46065acb865c87983f9&guid=6347920e68ce220dc429967e72160ca6&item_codes=dDGzEnDSXzI&playat=21580&propertis={"pos":3,"alpha":1,"size":1,"effect":0,"color":4294967295}&content=nihao&network=WIFI&ver=5.9
	if((http_session->m_Requestline.m_Host.Find("dm.api.3g.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v4_5/danmu/add")!=-1)) 
	{
		//return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::TuDouVideo::Process(const char *packet,int action)
{
	//��ҳ����
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
	//��ҳ����
	if (action == kPcWebPlay)
	{
		return PcWebPlay(packet);
	}
	//pc�ͻ�������
    if (action == kPcClientSearch)
    {
		return PcClientSearch(packet);
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
int videoactivity::TuDouVideo::PcWebSearch(const char *packet)
{
	//��referer ����ȡ����
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//��ҳ����  
	string comment;
	if (uri.find("?f") == string::npos)
	{
		//����
		comment = uri.substr(uri.find("kw=") + strlen("kw="));
	}
	if (uri.find("?f") != string::npos)
	{
		//����
		comment = ms_->GetValueBetweenBeginEnd(uri,"nisearch/","?f",true);  
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
	return 1;
}

//��ҳ���� �ظ�
int videoactivity::TuDouVideo::PcWebComment(const char *packet)
{
	//��cookie �л���ǳ�
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("u_nick");
	if (nickname.empty())
	{
		nickname = "�����û�null";
	}
	//��ȡ����
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	//�ж� ���� �ظ�
	string sourceCommentId = ms_->GetValueFromMapByKey("parent",true);
	if (comment.empty())
	{
		return 0;
	}
	if (sourceCommentId == "0")
	{
		output_text_ =				
			"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //�ǳ�
			"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";						//64	[19]  //״̬
	}
	if (sourceCommentId != "0")
	{
		output_text_ =				
			"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //�ǳ�
			"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
			"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";						//64	[19]  //״̬
	}					
	WriteIndexFile();
	return 1;
}

//��ҳ��Ļ
int videoactivity::TuDouVideo::PcWebDanMu(const char *packet)
{
	//��cookie �л���ǳ�
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("u_nick");
	if (nickname.empty())
	{
		nickname = "�����û�null";
	}
	//���ݲ���
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
		"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //�ǳ�
		"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";						//64	[19]  //״̬				
	WriteIndexFile();
	return 1;
}
//��ҳ����
int videoactivity::TuDouVideo::PcWebPlay(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("title",true);  
	ms_->Replace(comment,"%u","");
	comment = ms_->UnicodeToGBK(comment);
	if (comment.empty())
	{
		return 0;
	}	
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//pc�ͻ�������
int videoactivity::TuDouVideo::PcClientSearch(const char *packet)
{
	//��referer ����ȡ����
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//��ҳ����  
	string comment;
	if (uri.find("keyword=") != string::npos)
	{
		//����
		comment = uri.substr(uri.find("keyword=") + strlen("keyword="));
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
	return 1;
}
//pc�ͻ��˵�Ļ
int videoactivity::TuDouVideo::PcClientDanMu(const char *packet)
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
int videoactivity::TuDouVideo::AndroidSearch(const char *packet)
{
	//��cookie �л���ǳ�
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("u_nick");
	if (nickname.empty())
	{
		nickname = "�����û�null";
	}
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
	output_text_ =	
		"nick="	 + 	  	nickname + "\r\n" + 		//58	[123] //�ǳ�
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//Android���� �ظ�
int videoactivity::TuDouVideo::AndroidComment(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
AfxMessageBox(uri.c_str());
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
int videoactivity::TuDouVideo::AndroidDanMu(const char *packet)
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