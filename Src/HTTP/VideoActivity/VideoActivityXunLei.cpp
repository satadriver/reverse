#include "StdAfx.h"
#include "VideoActivityXunLei.h"

videoactivity::XunLeiVideo::XunLeiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "Ѹ����Ƶ";	
}

videoactivity::XunLeiVideo::~XunLeiVideo()
{
	
}

int videoactivity::XunLeiVideo::IsXunLeiVideo(HttpSession *http_session)
{
	//�û����� 1
	if((http_session->m_Requestline.m_Host.Find("search.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� 2
	if((http_session->m_Requestline.m_Host.Find("kkpgv2.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/?u=search_page")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("backend.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/weibo_new.json")!=-1)) 
	{
		return kPcWebcomment;
	}
	//�û��ظ�
	if((http_session->m_Requestline.m_Host.Find("backend.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment_new.json")!=-1)) 
	{
		return kPcWebReply;
	}
	//�û���Ļ
	if((http_session->m_Requestline.m_Host.Find("backend3.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/point.json")!=-1)) 
	{
		return kPcWebDanmu;
	}
	//pc������	
	if((http_session->m_Requestline.m_Host.Find("search.kkp.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/?word=")!=-1)) 
	{
		return kPcClientSearch;
	}
	//�ֻ��û�����
	if((http_session->m_Requestline.m_Host.Find("search.pad.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search4android.php")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�ֻ��û�����
	if((http_session->m_Requestline.m_Host.Find("backend.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/weibo_new_client.json")!=-1)) 
	{
		return kAndroidComment;
	}
	//�ֻ��û���Ļ
	if((http_session->m_Requestline.m_Host.Find("backend3.t.kankan.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/point_client.json")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::XunLeiVideo::Process(const char *packet,int action)
{
	//�û�����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//�û�����
	if (action == kPcWebcomment)
	{
		return PcWebComment(packet);
	}
	//�û��ظ�
	if (action == kPcWebReply)
	{
		return PcWebReply(packet);
	}
	//�û���Ļ
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//pc������
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//�ֻ��û�����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//�ֻ��û�����
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//�ֻ��û���Ļ
	if (action == kAndroidDanmu)
	{
		return AndroidDanmu(packet);
	}
	return 1;
}

//�û�����
int videoactivity::XunLeiVideo::PcWebSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string content;
	if (uri.find("/?u=search_page") != -1) 
	{
		ms_->Replace(uri,"?","&");
		ms_->SplitStringToMap(uri,'&','=');
		content = ms_->GetValueFromMapByKey("u1",true);
	}
	if (uri.find("/search.php") != -1)
	{
		ms_->Replace(uri,"?","&");
		ms_->SplitStringToMap(uri,'&','=');
		content = ms_->GetValueFromMapByKey("keyword",true);
	}
	if (content.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û�����
int videoactivity::XunLeiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("subject_title",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"mid=" + title + "\r\n" + 				//56	[121] //��Ƶ����  
		"msg=" + comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û��ظ�
int videoactivity::XunLeiVideo::PcWebReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=�ظ�\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û���Ļ
int videoactivity::XunLeiVideo::PcWebDanmu(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + comment + "\r\n"+
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"netmark=" + app_ + "\r\n" +
		"state=��Ļ\r\n";
	WriteIndexFile();
	return 1;
}
//pc������
int videoactivity::XunLeiVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string content;
	if (uri.find("word=") != string::npos)
	{
		content = uri.substr(uri.find("word=") + strlen("word="));
		content = ms_->UTF8ToGBK(UrlDecode(content));
	}
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//�ֻ��û�����
int videoactivity::XunLeiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string content = ms_->GetValueBetweenBeginEnd(src_data,"keyword=","&");
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//�ֻ��û�����
int videoactivity::XunLeiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string title = ms_->GetValueFromMapByKey("subject_title",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
		output_text_ = 
		"form=" + title + "\r\n" +
		"msg=" + comment + "\r\n"+
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"netmark=" + app_ + "\r\n" +
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//�ֻ��û���Ļ
int videoactivity::XunLeiVideo::AndroidDanmu(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string content = ms_->GetValueFromMapByKey("content",true);
	if (content.empty())
	{
		return 0;
	}
		output_text_ = 
		"msg=" + content + "\r\n"+
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"netmark=" + app_ + "\r\n" +
		"state=��Ļ\r\n";
	WriteIndexFile();
	return 1;
}
