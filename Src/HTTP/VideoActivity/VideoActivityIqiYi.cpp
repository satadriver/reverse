#include "StdAfx.h"
#include "VideoActivityIqiYi.h"

videoactivity::IQiYiVideo::IQiYiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "��������Ƶ";	
}

videoactivity::IQiYiVideo::~IQiYiVideo()
{
	
}

int videoactivity::IQiYiVideo::IsIQiYiVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("s.360.cn")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/so/zz.gif")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û�����(��������)
	if((http_session->m_Requestline.m_Host.Find("api-t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/feed/publish")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		return kPcWebComment;
	}
	//�û��ظ�(���ݻظ�)
	if((http_session->m_Requestline.m_Host.Find("sns-comment.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v2/comment/publish.action")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		return kPcWebReply;
	}
    //�û�����(��ͨ����)
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qx_api/comment/publish")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		return kPcWebPublish;
	}
	//�û��ظ�(��ͨ�ظ�)  (Get���޷����)
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qx_api/comment/reply")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() != 0)) 
	{
		//return kPcWebReplyGeneral;
	}
	//�û���Ļ	
	if((http_session->m_Requestline.m_Host.Find("bar.i.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myna-api/publish")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("secure_p=GPhone") ==-1)) 
	{
		//return kPcWebDanmu;
	}
	//���Կͻ�������
	if((http_session->m_Requestline.m_Host.Find("search.video.qiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/o?if=pps")!=-1)) 
	{
		return kPcSearch;
	}
	//�ֻ��ͻ�������
	if((http_session->m_Requestline.m_Host.Find("msg.71.am")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v5/alt/act")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidSearch;
	}
	//�ֻ��ͻ�������
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/feed/publish")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidComment;
	}
	//�ֻ��ͻ��˻ظ� 
	if((http_session->m_Requestline.m_Host.Find("api.t.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/feed/comment")!=-1) &&
		((http_session->m_Requestline.m_Referer).GetLength() == 0)) 
	{
		return kAndroidReply;
	}
	//�ֻ��ͻ��˵�Ļ
	if((http_session->m_Requestline.m_Host.Find("bar.i.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myna-api/publish")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("secure_p=GPhone")!=-1))  
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::IQiYiVideo::Process(const char *packet,int action)
{
	//�û�����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//�û�����(��������)
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//�û�����(��ͨ����)
	if (action == kPcWebPublish)
	{
		return PcWebPublish(packet);
	}
	//�û��ظ�(���ݻظ�)
	if (action == kPcWebReply)
	{
		return PcWebReply(packet);
	}
	//�û��ظ�(��ͨ�ظ�)
	if (action == kPcWebReplyGeneral)
	{
		return PcWebReplyGeneral(packet);
	}
	//�û���Ļ
	if (action == kPcWebDanmu)
	{
		return PcWebDanmu(packet);
	}
	//���Կͻ�������
	if (action == kPcSearch)
	{
		return PcSearch(packet);
	}
	//�ֻ��ͻ�������
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//�ֻ��ͻ�������
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//�ֻ��ͻ��˻ظ�
	if (action == kAndroidReply)
	{
		return AndroidReply(packet);
	}
	//�ֻ��ͻ��˵�Ļ
	if (action == kAndroidDanmu)
	{
		return AndroidDanmu(packet);
	}
	return 1;
}

//�û�����
int videoactivity::IQiYiVideo::PcWebSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	string src_data = ms_->UTF8ToGBK(UrlDecode(uri));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"q_","?source");
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//�û�����(��������)
int videoactivity::IQiYiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("description",true);
	if (comment.empty())
	{
		return 1;
	}
	output_text_ =				
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û�����(��ͨ����)
int videoactivity::IQiYiVideo::PcWebPublish(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string title = ms_->GetValueFromMapByKey("title",true);
	string comment = ms_->GetValueFromMapByKey("text",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	output_text_ =
		"mid=" + title + "\r\n" + 				//56	[121] //��Ƶ����
		"msg=" + comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û��ظ�(���ݻظ�)
int videoactivity::IQiYiVideo::PcWebReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("text",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  text + "\r\n" + 				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=�ظ�\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û��ظ�(��ͨ�ظ�)
int videoactivity::IQiYiVideo::PcWebReplyGeneral(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("text",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));

	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=�ظ�\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û���Ļ
int videoactivity::IQiYiVideo::PcWebDanmu(const char *packet)
{
	return 1;
}
//pc�ͻ�������
int videoactivity::IQiYiVideo::PcSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("key",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ��ͻ�������
int videoactivity::IQiYiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string keyword = ms_->GetValueFromMapByKey("s_rq",true);
	if (keyword.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  keyword + "\r\n" + 				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ��ͻ�������
int videoactivity::IQiYiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("description",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));
	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  content + "\r\n" + 				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ��ͻ��˻ظ�
int videoactivity::IQiYiVideo::AndroidReply(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("text",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));

	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  content + "\r\n" + 				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=�ظ�\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ��ͻ��˵�Ļ
int videoactivity::IQiYiVideo::AndroidDanmu(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	string content = ms_->GetValueFromMapByKey("content",true);
	content = ms_->UTF8ToGBK(UrlDecode(content));

	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  content + "\r\n" + 				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
