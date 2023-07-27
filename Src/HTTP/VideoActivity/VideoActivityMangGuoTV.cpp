#include "StdAfx.h"
#include "VideoActivityMangGouTV.h"

videoactivity::MangGuoVideo::MangGuoVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "â��TV��Ƶ";	
}

videoactivity::MangGuoVideo::~MangGuoVideo()
{
	
}

int videoactivity::MangGuoVideo::IsMangGuoVideo(HttpSession *http_session)
{
	//�û�����1
	if((http_session->m_Requestline.m_Host.Find("so.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so/k")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û�����2
	if((http_session->m_Requestline.m_Host.Find("so.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/so?k")!=-1)) 
	{
		return kPcWebSearcht;
	}
	//�û����� �ظ�
	if((http_session->m_Requestline.m_Host.Find("comment.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/video_comment")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("www.mgtv.com") != -1)) 
	{
		return kPcWebComment;
	}
	//pc�ͻ�������
	if((http_session->m_Requestline.m_Host.Find("pc.api.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v1/html/so/")!=-1)) 
	{
		return kPcClientSearch;
	}
	//pc�ͻ������� �ظ�
	if((http_session->m_Requestline.m_Host.Find("comment.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/video_comment")!=-1) &&
		(http_session->m_Requestline.m_Referer.Find("pc.api.mgtv.com") != -1)) 
	{
		return kPcClientComment;
	}
	//pc�ͻ��˲���
	if((http_session->m_Requestline.m_Host.Find("pc.da.mgtv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/pc/player")!=-1)) 
	{
		//return kPcClientPlay;
	}
	//�ֻ�����
	if((http_session->m_Requestline.m_Host.Find("mobile.api.hunantv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v6/search/getResult")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�ֻ����ۻظ�
	if ((http_session->m_Requestline.m_Host.Find("comment.mgtv.com")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/comment/write")!=-1))
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::MangGuoVideo::Process(const char *packet,int action)
{
	//�û�����1
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//�û�����2
	if (action == kPcWebSearcht)
	{
		return PcWebSearcht(packet);
	}
	//�û����� �ظ�
	if (action == kPcWebComment)
	{
		return PcWebComment(packet);
	}
	//pc������
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
	}
	//pc������ �ظ�
	if (action == kPcClientComment)
	{
		return PcClientComment(packet);
	}
	//pc�˲���
	if (action == kPcClientPlay)
	{
		return PcClientPlay(packet);
	}
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

//�û�����
int videoactivity::MangGuoVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string::size_type pos;
	pos = src_data.find("/so/k");
	if (pos == string::npos)
	{
		return 0;
	}
	string comment = src_data.substr(pos + strlen("/so/k")+1);
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
//�û�����2
int videoactivity::MangGuoVideo::PcWebSearcht(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"/so?k=","&");
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
int videoactivity::MangGuoVideo::PcWebComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string type = ms_->GetValueFromMapByKey("fid",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (type.empty() || comment.empty())
	{
		return 0;
	}
	if (type == "0")
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		    "state=����\r\n";						//64	[19]  //״̬
	}else
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		    "state=�ظ�\r\n";						 //64	[19]  //״̬
	}
	WriteIndexFile();
	return 1;
}

//pc������
int videoactivity::MangGuoVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
    ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("name",true);
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//pc������ �ظ�
int videoactivity::MangGuoVideo::PcClientComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	string type = ms_->GetValueFromMapByKey("fid",true);
	string comment = ms_->GetValueFromMapByKey("content",true);
	if (type.empty() || comment.empty())
	{
		return 0;
	}
	if (type == "0")
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";						//64	[19]  //״̬
	}else
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";						 //64	[19]  //״̬
	}
	WriteIndexFile();
	return 1;
}
//pc�˲���
int videoactivity::MangGuoVideo::PcClientPlay(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string v = ms_->GetValueFromMapByKey("v",true);
	string title;
	//ʹ��json ��ȡ����
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(v,value))
	{	
		//��Ƶ����
		title   = value["title"].asString();
	}
	if (title.empty())
	{
		return 0;
	}
	output_text_ =	
		"mid=" + 	  	title + "\r\n" + 		//56	[121] //��Ƶ����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
//�ֻ�����
int videoactivity::MangGuoVideo::AndroidSearch(const char *packet)
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
int videoactivity::MangGuoVideo::AndroidComment(const char *packet)
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