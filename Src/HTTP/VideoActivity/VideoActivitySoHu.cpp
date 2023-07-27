#include "StdAfx.h"
#include "VideoActivitySoHu.h"

videoactivity::SoHuVideo::SoHuVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "�Ѻ���Ƶ";	
}

videoactivity::SoHuVideo::~SoHuVideo()
{
	
}

int videoactivity::SoHuVideo::IsSoHuVideo(HttpSession *http_session)
{
	//�û����� 1	
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mts?wd=")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� 2
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mts?box")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� �ظ� 
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit?")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("tv.sohu.com")!=-1)) 
	{
		return kPcWebComment;
	}
	//��Ļ
	if((http_session->m_Requestline.m_Host.Find("api.danmu.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu?act=dmfire_v2&dct=1")!=-1)) 
	{
		return kPcWebDanmu;
	}
	//pc�ͻ�������	
	if((http_session->m_Requestline.m_Host.Find("so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/ifox/v5/search")!=-1)) 
	{
		return kPcClientSearch;
	}
	//�ֻ��û�����
	if((http_session->m_Requestline.m_Host.Find("m.so.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/new/keyword")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�ֻ��û����� �ظ�
	if((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit?")==-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidComment;
	}
	//�ֻ��û���Ļ
	if((http_session->m_Requestline.m_Host.Find("api.danmu.tv.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/?act=dmfire_v2")!=-1) &&	
		(http_session->m_Requestline.m_URI.Find("/danmu?act=dmfire_v2&dct=1") == -1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::SoHuVideo::Process(const char *packet,int action)
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
	//��Ļ
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//pc�ͻ�������
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//�ֻ��û�����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//�ֻ��û����� �ظ�
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
int videoactivity::SoHuVideo::PcWebSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
  	
	string content = ms_->GetValueFromMapByKey("wd",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û����� �ظ�
int videoactivity::SoHuVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	//�ж� ���� �ظ�
	string sourceCommentId = ms_->GetValueFromMapByKey("reply_id",true);
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
//��Ļ
int videoactivity::SoHuVideo::PcWebDanmu(const char *packet)
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
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";
	WriteIndexFile();
	return 1;
}
//pc�ͻ�������
int videoactivity::SoHuVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	
	string content = ms_->GetValueFromMapByKey("wd",true);
	content = ms_->UrlDecode(content);

	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}
//�ֻ��û�����
int videoactivity::SoHuVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("key",true);
	if (content.empty())
	{
		return 0;
	}
	output_text_ = 
		output_text_ = 
		"msg=" + content + "\r\n"+
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//�ֻ��û����� �ظ�
int videoactivity::SoHuVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	//�ж� ���� �ظ�
	string sourceCommentId = ms_->GetValueFromMapByKey("reply_id",true);
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
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";							//64	[19]  //״̬
	}					
	WriteIndexFile();
	return 1;
}
//�ֻ���Ļ
int videoactivity::SoHuVideo::AndroidDanmu(const char *packet)
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
		"netmark=" + app_ + "\r\n" +
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";
	WriteIndexFile();
	return 1;
}