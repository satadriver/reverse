#include "StdAfx.h"
#include "VideoActivityHuaShuTv.h"

videoactivity::HuaShuTvVideo::HuaShuTvVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "����TV��Ƶ";	
}

videoactivity::HuaShuTvVideo::~HuaShuTvVideo()
{
	
}
int videoactivity::HuaShuTvVideo::IsHuaShuTvVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("www.wasu.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Search/show/")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����ۻظ�
	if ((http_session->m_Requestline.m_Host.Find("changyan.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/comment/submit")!=-1)&&
		(http_session->m_Requestline.m_Referer.Find("www.wasu.cn/")))
	{
		return kPcWebComment;
	}
	//pc�ͻ�������
	if((http_session->m_Requestline.m_Host.Find("clientapi.wasu.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Client/search/get/")!=-1)) 
	{
		return kPcClientSearch;
	}
	//�ֻ��û�����
	if((http_session->m_Requestline.m_Host.Find("clientapi.wasu.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/AggPhone/search/keyword")!=-1)) 
	{
		return kAndroidSearch;
	}
	return 0;
}

int videoactivity::HuaShuTvVideo::Process(const char *packet,int action)
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
	//pc�ͻ�������
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//�ֻ�����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}

	return 1;
}

//�û�����
int videoactivity::HuaShuTvVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string::size_type pos;
	pos = src_data.find("show/k/");
	if (pos == string::npos)
	{
		return 0;
	}
	string comment = src_data.substr(pos+strlen("show/k/"));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�û����ۻظ�
int videoactivity::HuaShuTvVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string type = ms_->GetValueFromMapByKey("reply_id",true);
	string title = ms_->GetValueFromMapByKey("topic_title",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (title.empty() || comment.empty())
	{
		return 0;
	}
	if (type == "0")
	{
		output_text_ =	
			"mid=" +  title + "\r\n" + 					//56	[121] //��Ƶ����
			"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //����ƽ̨
			"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	}
	else
	{
		output_text_ =	
			"mid=" +  title + "\r\n" + 					//56	[121] //��Ƶ����
			"msg=" +  comment + "\r\n" + 				//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +		//62	[126] //����ƽ̨
			"state=�ظ�\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	}
	return 1;
}
//pc�ͻ�������
int videoactivity::HuaShuTvVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string comment;
	if (uri.rfind("/") != string::npos)
	{
		comment = uri.substr(uri.rfind("/") + strlen("/"));
		comment = ms_->UTF8ToGBK(UrlDecode(comment));
	}
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +		//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ��û�����
int videoactivity::HuaShuTvVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string::size_type pos;
	pos = src_data.find("/search/keyword/");
	if (pos == string::npos)
	{
		return 0;
	}
	string text = src_data.substr(pos+strlen("/search/keyword/"));
	if (text.empty())
	{
		return 0;
	}
	output_text_ =				
		"msg=" + text + "\r\n" +     			//63	[128] //����
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformAndroid + "\r\n" +//62	[126] //����ƽ̨
		"state=�ֻ�����\r\n";					//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
