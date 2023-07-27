#include "StdAfx.h"
#include "VideoActivityBiLiBiLi.h"

videoactivity::BiLiBiLiVideo::BiLiBiLiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "����������Ļ��Ƶ";	
}

videoactivity::BiLiBiLiVideo::~BiLiBiLiVideo()
{
	
}

int videoactivity::BiLiBiLiVideo::IsBiLiBiLiVideo(HttpSession *http_session)
{
	//��ҳ����	
	if((http_session->m_Requestline.m_Host.Find("search.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/all")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� �ظ�
	if((http_session->m_Requestline.m_Host.Find("www.tudou.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comments/itemnewcomment.srv?method=add")!=-1)) 
	{
		//return kPcWebComment;
	}
	//��ҳ��Ļ 
	if((http_session->m_Requestline.m_Host.Find("interface.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/dmpost")!=-1)) 
	{
		return kPcWebDanMu;
	}
	//��ҳ���� 1 /v/web/web_page_view?mid=&fts=1486603842&url=http%253A%252F%252Fwww.bilibili.com%252Fvideo%252Fav8482736%252F&proid=1&ptype=1&module=main&title=%5BAMV%E6%A8%B1%E8%8A%B1%E9%A3%98%E8%90%BD%E7%9A%84%E7%9E%AC%E9%97%B4%2F%E5%8F%AF%E6%83%9C%E6%B2%A1%E5%A6%82%E6%9E%9C-%E7%A7%92%E4%BA%94%5D_%E5%8E%9F%E5%88%9B%E9%9F%B3%E4%B9%90_%E9%9F%B3%E4%B9%90_bilibili_%E5%93%94%E5%93%A9%E5%93%94%E5%93%A9%E5%BC%B9%E5%B9%95%E8%A7%86%E9%A2%91%E7%BD%91&ajaxtag=&ajaxid=&page_ref=http%3A%2F%2Fwww.bilibili.com%2Fvideo%2Fmusic-original-1.html&_=1486605859546         ������¼
	if((http_session->m_Requestline.m_Host.Find("data.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v/web/web_page_view")!=-1)) 
	{
		//return kPcWebPlay;
	}
	//android���� /x/v2/search?duration=0&keyword=naruto%E6%80%A5&pn=1&ps=20&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&appkey=1d8b6e7d45233436&build=434000&mobi_app=android&platform=android
	if((http_session->m_Requestline.m_Host.Find("app.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/x/v2/search?duration")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android���� �ظ� /x/v2/reply/add?_device=android&_hwid=440b7ad385b965a8&_ulv=5000&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&appkey=1d8b6e7d45233436&build=434000&mobi_app=android&platform=android&sign=eba0a80697b01d493e86b2d8e261d0ff
	if((http_session->m_Requestline.m_Host.Find("api.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/x/v2/reply/add")!=-1)) 
	{
		return kAndroidComment;
	}
	//android��Ļ /comment/post?_device=android&_ulv=5000&access_key=51bda0ac5c7f743b0ecf4795c85d95d0&aid=4270788&appkey=1d8b6e7d45233436&build=434000&cid=6907160&mobi_app=android&pid=88935161&platform=android&sign=4ffb15b41d86a17b10324a5633d69575
	if((http_session->m_Requestline.m_Host.Find("api.bilibili.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/post")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::BiLiBiLiVideo::Process(const char *packet,int action)
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
int videoactivity::BiLiBiLiVideo::PcWebSearch(const char *packet)
{
	//��referer ����ȡ����	/all?keyword
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//��ҳ����  
	string comment;
	if (uri.find("keyword") != string::npos)
	{
		//����
		comment = uri.substr(uri.find("keyword") + strlen("keyword"));
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
int videoactivity::BiLiBiLiVideo::PcWebComment(const char *packet)
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
int videoactivity::BiLiBiLiVideo::PcWebDanMu(const char *packet)
{
	//��ȡ����
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("message",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
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
//��ҳ����
int videoactivity::BiLiBiLiVideo::PcWebPlay(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
//AfxMessageBox(uri.c_str());
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("title",true);  
//AfxMessageBox(comment.c_str());
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

//Android����
int videoactivity::BiLiBiLiVideo::AndroidSearch(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("keyword",true);  
	comment = ms_->UTF8ToGBK(comment);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//Android���� �ظ�
int videoactivity::BiLiBiLiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	//��ȡ����
	if (src_data.find("parent=") != string::npos)
	{
		src_data = src_data.substr(src_data.find("parent="));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("message",true);
	//�ж� ���� �ظ�
	string sourceCommentId = ms_->GetValueFromMapByKey("parent",true);
	
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
int videoactivity::BiLiBiLiVideo::AndroidDanMu(const char *packet)
{
	//�����ݲ��ֻ������
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("msg");
	if (content.empty())
	{
		return 0;
	}
	output_text_ =			
		"msg=" +		content + "\r\n" + 				//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" +	platformAndroid + "\r\n" +		//62	[126] //����ƽ̨
		"state=��Ļ\r\n";								//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}