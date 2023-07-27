#include "StdAfx.h"
#include "VideoActivityLeShi.h"

videoactivity::LeShiVideo::LeShiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "������Ƶ";	
}

videoactivity::LeShiVideo::~LeShiVideo()
{
	
}
int videoactivity::LeShiVideo::IsLeShiVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("dc.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/qy/")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� 
	if((http_session->m_Requestline.m_Host.Find("api.my.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/vcm/api/add")!=-1)) 
	{
		return kPcWebComment;
	}
	//�û��ظ� 
	if((http_session->m_Requestline.m_Host.Find("api.my.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/vcm/api/reply")!=-1)) 
	{
		return kPcWebReply;
	}
	//�û���Ļ	/danmu/add?vid=27639136&cid=2&pid=10022394&start=1122.965&txt=%E4%B8%89%E7%94%9F%E4%B8%89%E4%B8%96&font=m&position=4&tn=0.07311636907979846&type=txt&isIdentify=1&from=1&color=FFFFFF
	if((http_session->m_Requestline.m_Host.Find("hd.my.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/add")!=-1)) 
	{
		//return kPcWebDanmu;
	}
	//pc�ͻ������� 
	if((http_session->m_Requestline.m_Host.Find("client.pc.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc�ͻ��˵�Ļ 
	if((http_session->m_Requestline.m_Host.Find("hd.my.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/danmu/add")!=-1)) 
	{
		//return kPcClientDanMu;
	}
	//�ֻ��û�����
	if((http_session->m_Requestline.m_Host.Find("dc.so.le.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/data_collect.so?action_code")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�ֻ�����
	if((http_session->m_Requestline.m_Host.Find("api.mob.app.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/add")!=-1)) 
	{
		return kAndroidComment;
	}
	//�ֻ��ظ�
	if((http_session->m_Requestline.m_Host.Find("api.mob.app.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/reply")!=-1)) 
	{
		return kAndroidReply;
	}
	//�ֻ���Ļ
	if((http_session->m_Requestline.m_Host.Find("dynamic.meizi.app.m.letv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/android/dynamic.php")!=-1)) 
	{
		return kAndroidDanmu;
	}
	return 0;
}

int videoactivity::LeShiVideo::Process(const char *packet,int action)
{
	//�û�����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//�û�����
	if (action == kPcWebComment)
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
	//���Կͻ�������
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//���Կͻ��˵�Ļ
	if (action == kPcClientDanMu)
	{
		return PcClientDanMu(packet);
	}
	//�ֻ�����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//�ֻ�����
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	//�ֻ��ظ�
	if (action == kAndroidReply)
	{
		return AndroidReply(packet);
	}
	//�ֻ���Ļ
	if (action == kAndroidDanmu)
	{
		return AndroidDanmu(packet);
	}
	return 1;
}

//�û�����
int videoactivity::LeShiVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("query",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//�û�����
int videoactivity::LeShiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	string title = ms_->GetValueFromMapByKey("title",true);
	if (comment.empty() || title.empty())
	{
		return 0;
	}
	output_text_ =	
		"from" + title + "\r\n" +
		"msg=" + comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ظ�
int videoactivity::LeShiVideo::PcWebReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" +  comment + "\r\n" +     		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=�ظ�\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//�û���Ļ
int videoactivity::LeShiVideo::PcWebDanmu(const char *packet)
{
	return 1;
}
//���Կͻ�������
int videoactivity::LeShiVideo::PcClientSearch(const char *packet)
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
//���Կͻ��˵�Ļ
int videoactivity::LeShiVideo::PcClientDanMu(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	
	string content = ms_->GetValueFromMapByKey("txt",true);
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
		"state=��Ļ\r\n";
	WriteIndexFile();
	return 1;
}
//�ֻ��û�����
int videoactivity::LeShiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("query",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ�����
int videoactivity::LeShiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("content",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ��ظ�
int videoactivity::LeShiVideo::AndroidReply(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string text = ms_->GetValueFromMapByKey("content",true);
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=�ظ�\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ���Ļ
int videoactivity::LeShiVideo::AndroidDanmu(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string txt = ms_->GetValueFromMapByKey("txt",true);
	if (txt.empty())
	{
		return 0;
	}	
	output_text_ =				
		"msg=" + txt + "\r\n" +     				//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=��Ļ\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}