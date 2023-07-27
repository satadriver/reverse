#include "StdAfx.h"
#include "VideoActivityMiaoPai.h"

videoactivity::MiaoPaiVideo::MiaoPaiVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "����";	
}

videoactivity::MiaoPaiVideo::~MiaoPaiVideo()
{
	
}

int videoactivity::MiaoPaiVideo::IsMiaoPaiVideo(HttpSession *http_session)
{
	//�ֻ�����
	if((http_session->m_Requestline.m_Host.Find("mobile.api.hunantv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v6/search/getResult")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�ֻ����ۻظ�--���ݰ� ��ȡ����
	if ((http_session->m_Requestline.m_Host.Find("c.miaopai.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/m/comment.json")!=-1))
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::MiaoPaiVideo::Process(const char *packet,int action)
{
	//�ֻ�����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	//�ֻ����ۻظ�
	if (action == kAndroidComment)
	{
		return AndroidComment(packet);
	}
	return 1;
}


//�ֻ�����
int videoactivity::MiaoPaiVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	string comment = ms_->GetValueFromMapByKey("name",true);
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

//�ֻ����ۻظ�
int videoactivity::MiaoPaiVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.find("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string type = ms_->GetValueFromMapByKey("commentId",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (comment.empty())
	{
		return 0;
	}
	if (type.empty())
	{
		output_text_ =	
			"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ֻ�����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	}else
	{
		output_text_ =	
			"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
			"clientmsg=" + 	platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ֻ��ظ�\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	}
	return 1;
}