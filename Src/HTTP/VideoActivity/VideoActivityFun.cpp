#include "StdAfx.h"
#include "VideoActivityFun.h"

videoactivity::FunVideo::FunVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "������Ƶ";	
}

videoactivity::FunVideo::~FunVideo()
{
	
}

int videoactivity::FunVideo::IsFunVideo(HttpSession *http_session)
{
	//��ҳ���� 1
	if((http_session->m_Requestline.m_Host.Find("api1.fun.tv")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/guess_like/search")!=-1)) 
	{
		return kPcWebSearch;
	}
	//��ҳ���� 
	if((http_session->m_Requestline.m_Host.Find("api1.fun.tv")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/update")!=-1)) 
	{
		return kPcWebComment;
	}
    //pc���� 
	if((http_session->m_Requestline.m_Host.Find("ps.funshion.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v7/search/video")!=-1)) 
	{
		return kPcClientSerch;
	}
	//android���� 
	if((http_session->m_Requestline.m_Host.Find("ps.funshion.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/v5/search/video")!=-1)) 
	{
		return kAndroidSearch;
	}
	//android����  
	if((http_session->m_Requestline.m_Host.Find("pinter.funshion.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment")!=-1)) 
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::FunVideo::Process(const char *packet,int action)
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
	//pc����
	if (action == kPcClientSerch)
	{
		return PcClientSearch(packet);
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
int videoactivity::FunVideo::PcWebSearch(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
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
int videoactivity::FunVideo::PcWebComment(const char *packet)
{
	//��cookie �л���ǳ�
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	string nickname = ms_->GetValueFromMapByKey("funshion_username",true);
	if (nickname.empty())
	{
		nickname = "�����û�null";
	} 
	//��ȡ����
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =			
		"nick="	 + 	  	nickname + "\r\n" + 	//58	[123] //�ǳ�	
		"msg=" + 		comment + "\r\n" + 		//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 		//59	[124] //��Դ
		"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬			
	WriteIndexFile();
	return 1;
}

//pc����
int videoactivity::FunVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("q",true);  
	comment = ms_->UTF8ToGBK(UrlDecode(comment));
	if (comment.empty())
	{
		return 0;
	}
	output_text_ =	
		"msg=" + 		comment + "\r\n" + 			//63	[128] //����	
		"netmark=" +	app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + 	platformPC + "\r\n" +		//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}

//Android����
int videoactivity::FunVideo::AndroidSearch(const char *packet)
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
int videoactivity::FunVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n") != -1)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
    src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//����
	string comment = ms_->GetValueFromMapByKey("content",true);
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
