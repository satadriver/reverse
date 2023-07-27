#include "StdAfx.h"
#include "VideoActivityKuLiu.h"

videoactivity::KuLiuVideo::KuLiuVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "ku6��Ƶ";	
}

videoactivity::KuLiuVideo::~KuLiuVideo()
{
	
}

int videoactivity::KuLiuVideo::IsKuLiuVideo(HttpSession *http_session)
{
	//��ҳ����  
	if((http_session->m_Requestline.m_Host.Find("video.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v?fr=ex_ku6")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� 
	if((http_session->m_Requestline.m_Host.Find("comment.ku6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/web_save.jhtm")!=-1)) 
	{
		//return kPcWebComment;
	}
	//android���� 
	if((http_session->m_Requestline.m_Host.Find("search.ku6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/iosSearch.htm")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android���� 
	if((http_session->m_Requestline.m_Host.Find("comment.ku6.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/save.jhtm")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::KuLiuVideo::Process(const char *packet,int action)
{
	//��ҳ����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//��ҳ���� 
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//android����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//Android���� 
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}
//��ҳ����
int videoactivity::KuLiuVideo::PcWebSearch(const char *packet)
{
	//uri ����ȡ����
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//��ҳ����  
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("word",true);
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

//��ҳ����
int videoactivity::KuLiuVideo::PcWebComment(const char *packet)
{
	//uri ����ȡ����
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//��ҳ����  
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("comment.commentContent",true);
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
//Android����
int videoactivity::KuLiuVideo::AndroidSearch(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("q",true);  
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
//Android���� 
int videoactivity::KuLiuVideo::AndroidComment(const char *packet)
{
	//uri ����ȡ����
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	//��ҳ����  
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("comment.commentContent",true);
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
