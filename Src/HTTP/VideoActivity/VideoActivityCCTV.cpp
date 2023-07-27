#include "StdAfx.h"
#include "VideoActivityCCTV.h"

videoactivity::CCTVVideo::CCTVVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "CCTV������Ƶ";	
}

videoactivity::CCTVVideo::~CCTVVideo()
{
	
}

int videoactivity::CCTVVideo::IsCCTVVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("search.cctv.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/search.php")!=-1)) 
	{
		return kPcWebSearch;
	}
	//�û����� �ظ�
	if((http_session->m_Requestline.m_Host.Find("newcomment.cntv.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/comment/post/app/cms_tvlm")!=-1)) 
	{
		return kPcWebComment;
	}
	//pc���Զ�����
	if((http_session->m_Requestline.m_Host.Find("so.cntv.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/cbox_cntv_port.php")!=-1)) 
	{
		return kPcClientSearch;
	}
	//�ֻ�����
	if((http_session->m_Requestline.m_Host.Find("so.cntv.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/service/cbox_mobile.php")!=-1)) 
	{
		return kAndroidSearch;
	}
	//�ֻ����ۻظ�(pc���Զ�)
	if ((http_session->m_Requestline.m_Host.Find("newcomment.cntv.cn")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("/comment/post")!=-1))
	{
		return kAndroidComment;
	}
	return 0;
}

int videoactivity::CCTVVideo::Process(const char *packet,int action)
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
	//pc���Զ�����
	if (action == kPcClientSearch)
	{
		return PcClientSearch(packet);
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
int videoactivity::CCTVVideo::PcWebSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));

	string comment = ms_->GetValueBetweenBeginEnd(src_data,"qtext=","&");
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
//�û����� �ظ�
int videoactivity::CCTVVideo::PcWebComment(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(src_data,'&','=');
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"message=","&");
	string::size_type pos;
	pos = src_data.find("replyid");
	if (comment.empty())
	{
		return 0;
	}
	if (pos==string::npos)
	{
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	}else
	{	
		output_text_ =				
			"msg=" + 	 comment + "\r\n" + 		//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
			"clientmsg=" + 	platformWeb + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	}
	return 1;
}

//pc���Զ�����
int videoactivity::CCTVVideo::PcClientSearch(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	string comment = ms_->GetValueFromMapByKey("qtext",true);
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

//�ֻ�����
int videoactivity::CCTVVideo::AndroidSearch(const char *packet)
{
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	string comment = ms_->GetValueBetweenBeginEnd(src_data,"qtext=","&");
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
int videoactivity::CCTVVideo::AndroidComment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.find("\r\n")+strlen("\r\n"));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	//�ж����� �ظ�
	string type = ms_->GetValueFromMapByKey("replyid",true);
	//�û�ID
	string userid = ms_->GetValueFromMapByKey("authorid");
	//�û���
	string username = ms_->GetValueFromMapByKey("author",true);
	username = ms_->UTF8ToGBK(UrlDecode(username));
	//����
	string comment = ms_->GetValueFromMapByKey("message",true);

	if (userid.empty() || comment.empty() || username.empty())
	{
		return 0;
	}
	if (type.empty())
	{
		output_text_ =	
			"oid=" + userid + "\r\n" + 					//54	[119] //�û�ID 
			"onick=" + username + "\r\n" + 				//55	[120] //�û��� 
			"msg=" + comment + "\r\n" + 				//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
			"clientmsg=" + 	platformAndPc + "\r\n" +	//62	[126] //����ƽ̨
			"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	}else
	{
		output_text_ =	
			"oid=" + userid + "\r\n" + 					//54	[119] //�û�ID 
			"onick=" + username + "\r\n" + 				//55	[120] //�û��� 
			"msg=" + 	 comment + "\r\n" + 			//63	[128] //����	
			"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
			"clientmsg=" + 	platformAndPc + "\r\n" +	//62	[126] //����ƽ̨
			"state=�ظ�\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	}
	return 1;
}