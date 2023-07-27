#include "StdAfx.h"
#include "VideoActivityTouTiao.h"

videoactivity::TouTiaoVideo::TouTiaoVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "ͷ����Ƶ";	
}

videoactivity::TouTiaoVideo::~TouTiaoVideo()
{
	
}

int videoactivity::TouTiaoVideo::IsTouTiaoVideo(HttpSession *http_session)
{

	//android����
	if((http_session->m_Requestline.m_Host.Find("ic.snssdk.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/2/wap/search/")!=-1)) 
	{
		return kAndroidSearch;
	}
	return 0;
}

int videoactivity::TouTiaoVideo::Process(const char *packet,int action)
{
	//android����
	if (action == kAndroidSearch)
	{
		return AndroidSearch(packet);
	}
	return 1;
}

//Android����
int videoactivity::TouTiaoVideo::AndroidSearch(const char *packet)
{
	//��uri �л�ȡ����
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, '&', '=');
	string comment;
	//����
	comment = ms_->GetValueFromMapByKey("keyword",true);  
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
