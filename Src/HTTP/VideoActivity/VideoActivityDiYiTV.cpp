#include "StdAfx.h"
#include "VideoActivityDiYiTV.h"

videoactivity::DiYiVideo::DiYiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "��һ��Ƶ";	
}

videoactivity::DiYiVideo::~DiYiVideo()
{
	
}

int videoactivity::DiYiVideo::IsDiYiVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("www.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search/getList")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� 
	if((http_session->m_Requestline.m_Host.Find("www.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/addComment")!=-1)) 
	{
		return kPcWebComment;
	}
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("i.apps.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/web/search/action")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("dynamic.app.m.v1.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/www/dynamic.php")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::DiYiVideo::Process(const char *packet,int action)
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
	return 1;
}

//�û�����
int videoactivity::DiYiVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string::size_type pos = src_data.find("keyword=");
	if (pos == string::npos)
	{
		return 0;
	}
	string comment = src_data.substr(pos+strlen("keyword="));
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

//�û����� 
int videoactivity::DiYiVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("content",true);
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

//�ֻ��û�����
int videoactivity::DiYiVideo::AndroidSearch(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string content = ms_->GetValueFromMapByKey("keyword",true);
	if (content.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + 	 content + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//�ֻ�����
int videoactivity::DiYiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueFromMapByKey("comments",true);
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