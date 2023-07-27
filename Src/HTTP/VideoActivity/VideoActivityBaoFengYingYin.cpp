#include "StdAfx.h"
#include "VideoActivityBaoFengYingYin.h"

videoactivity::BaoFengVideo::BaoFengVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "����Ӱ����Ƶ";	
}

videoactivity::BaoFengVideo::~BaoFengVideo()
{
	
}

int videoactivity::BaoFengVideo::IsBaoFengVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("eventlog.hd.baofeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/logger.php?ltype=search")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� �ظ�
	if((http_session->m_Requestline.m_Host.Find("comments.baofeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/push")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.baofeng.com")!=-1))  
	{
		return kPcWebComment;
	}
	//pc�ͻ�������
	if((http_session->m_Requestline.m_Host.Find("search.moviebox.baofeng.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc�ͻ�������
	if((http_session->m_Requestline.m_Host.Find("comments.baofeng.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/push")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("moviebox.baofeng.net")!=-1)) 
	{
		return kPcClientComment;
	}
	//pc�ͻ��˵�Ļ
	if((http_session->m_Requestline.m_Host.Find("danmu.baofeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/d")!=-1)) 
	{
		return kPcClientDanmu;
	}
	//pc�ͻ��˲���
	if((http_session->m_Requestline.m_Host.Find("clicklog.moviebox.baofeng.net")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/click.php")!=-1)) 
	{
		return kPcClientPlay;
	}
	//�ֻ�����
	if ((http_session->m_Requestline.m_Host.Find("so.shouji.baofeng.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1))
	{
		return kAndroidSearch;
	}
	//�ֻ�����
	if ((http_session->m_Requestline.m_Host.Find("comments.baofeng.net")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/push")!=-1)&&
		(http_session->m_Requestline.m_Referer.GetLength() == 0)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::BaoFengVideo::Process(const char *packet,int action)
{
	//�û�����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//�û����� �ظ�
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//pc�ͻ�������
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//pc�ͻ�������
	if (action == kPcClientComment)
	{
		return PcClientComment(packet);
	}
	//pc�ͻ��˵�Ļ
	if (action == kPcClientDanmu)
	{
		return PcClientDanmu(packet);
	}
	//pc�ͻ��˲���
	if (action == kPcClientPlay)
	{
		return PcClientPlay(packet);
	}
	//�ֻ�����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//�û�����
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}

//�û�����
int videoactivity::BaoFengVideo::PcWebSearch(const char *packet)
{
	//��uri �л�ȡ����
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->GetValueFromMapByKey("msg",true);
	Json::Value value;
	Json::Reader reader;
	if (!reader.parse(src_data,value))
	{
		return 0;
	}
	string comment = value["keyword"].asString();
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//�û����� �ظ�
int videoactivity::BaoFengVideo::PcWebComment(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string title = ms_->GetValueFromMapByKey("xname",true);
	string comment = ms_->GetValueFromMapByKey("yestxt",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" +title +"\r\n" +
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//pc�ͻ�������
int videoactivity::BaoFengVideo::PcClientSearch(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string comment = ms_->GetValueFromMapByKey("keyword",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//pc�ͻ�������
int videoactivity::BaoFengVideo::PcClientComment(const char *packet)
{
	//��cookie ����ȡ����
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	ms_->SplitStringToMap(cookie,';','=');
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("bfuid");
	//�û�����
    string username = ms_->GetValueFromMapByKey("bfuname",true);
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	//��Ƶ����
	string title = ms_->GetValueFromMapByKey("xname",true);
    //����
	string content = ms_->GetValueFromMapByKey("yestxt",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));

	if (userid.empty() || username.empty() || title.empty() || content.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + userid + "\r\n" + 				//54	[119] //�û�ID --
		"onick=" + username + "\r\n" + 			//55	[120] //�û��� --
		"mid=" + title + "\r\n" + 				//56	[121] //��Ƶ����
		"msg=" + content + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//pc�ͻ��˵�Ļ
int videoactivity::BaoFengVideo::PcClientDanmu(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	
	string uid = ms_->GetValueFromMapByKey("uid");
	string content = ms_->GetValueFromMapByKey("text",true);
	if (uid.empty() || content.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + uid + "\r\n" + 				//54	[119] //�û�ID	
		"msg=" + content + "\r\n" + 			//63	[128] //����
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//pc�ͻ��˲���
int videoactivity::BaoFengVideo::PcClientPlay(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string uid = ms_->GetValueFromMapByKey("uid");
	string title = ms_->GetValueFromMapByKey("title",true);
	if (uid.empty() || title.empty())
	{
		return 0;
	}
	output_text_ =	
		"oid=" + uid + "\r\n" + 				//54	[119] //�û�ID	
		"mid=" + title + "\r\n" + 				//56	[121] //��Ƶ����
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//�ֻ�����
int videoactivity::BaoFengVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"query=","&");
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ�����
int videoactivity::BaoFengVideo::AndroidComment(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string title = ms_->GetValueFromMapByKey("xname",true);
	string comment = ms_->GetValueFromMapByKey("yestxt",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =
		"from=" + title + "\r\n" +
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
