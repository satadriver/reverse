#include "StdAfx.h"
#include "VideoActivityPPs.h"

videoactivity::PPsVideo::PPsVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "PPsӰ����Ƶ";	
}

videoactivity::PPsVideo::~PPsVideo()
{
	
}

int videoactivity::PPsVideo::IsPPsVideo(HttpSession *http_session)
{
	//�û�����
	if((http_session->m_Requestline.m_Host.Find("so.iqiyi.com")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/pps/")!=-1)) 
	{
		return kPcWebSearch;
	}
	return 0;
}

int videoactivity::PPsVideo::Process(const char *packet,int action)
{
	//�û�����
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	return 1;
}

//�û�����
int videoactivity::PPsVideo::PcWebSearch(const char *packet)
{
	string uri=http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');

	string content = ms_->GetValueFromMapByKey("k",true);
	if (content.empty())
	{
		return 1;
	}
	output_text_ =	
		"msg=" + 	 content + "\r\n" + 		//63	[128] //����	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //��Դ
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //����ƽ̨
		"state=����\r\n";						//64	[19]  //״̬
	WriteIndexFile();
	return 1;
}
