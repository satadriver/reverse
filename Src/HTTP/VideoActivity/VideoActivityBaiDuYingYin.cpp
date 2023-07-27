#include "StdAfx.h"
#include "VideoActivityBaiDuYingYin.h"

videoactivity::BaiDuYingYinVideo::BaiDuYingYinVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "�ٶ�Ӱ����Ƶ";	
}

videoactivity::BaiDuYingYinVideo::~BaiDuYingYinVideo()
{
	
}

int videoactivity::BaiDuYingYinVideo::IsBaiDuYingYinVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("v.baidu.com")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/v")!=-1)) 
	{
		return kPcWebSearch;
	}
	//android����
	if((http_session->m_Requestline.m_Host.Find("app.video.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/app")!=-1)) 
	{
		return kAndroidSearch;
	}
	return 0;
}

int videoactivity::BaiDuYingYinVideo::Process(const char *packet,int action)
{
	//�û�����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	//android����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	return 1;
}

//�û�����
int videoactivity::BaiDuYingYinVideo::PcWebSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	string content = ms_->GetValueFromMapByKey("word",true);
	if (content.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformPC + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}


//�û�����
int videoactivity::BaiDuYingYinVideo::AndroidSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri,'&','=');
	string content = ms_->GetValueFromMapByKey("word",true);
	if (content.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 			//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 				//59	[124] //��Դ
		"clientmsg=" + platformAndroid + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";							//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}