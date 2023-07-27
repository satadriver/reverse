#include "StdAfx.h"
#include "VideoActivityPPs.h"

videoactivity::PPsVideo::PPsVideo(HttpSession *http_session, const string &pro_name) : VideoActivity(http_session,pro_name)
{
	app_ = "PPs影音视频";	
}

videoactivity::PPsVideo::~PPsVideo()
{
	
}

int videoactivity::PPsVideo::IsPPsVideo(HttpSession *http_session)
{
	//用户搜索
	if((http_session->m_Requestline.m_Host.Find("so.iqiyi.com")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/pps/")!=-1)) 
	{
		return kPcWebSearch;
	}
	return 0;
}

int videoactivity::PPsVideo::Process(const char *packet,int action)
{
	//用户搜索
	if (action == kPcWebSearch)
	{
		return PcWebSearch(packet);
	}
	return 1;
}

//用户搜索
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
		"msg=" + 	 content + "\r\n" + 		//63	[128] //内容	
		"netmark=" + app_ + "\r\n" + 			//59	[124] //来源
		"clientmsg=" + platformWeb + "\r\n" +	//62	[126] //操作平台
		"state=搜索\r\n";						//64	[19]  //状态
	WriteIndexFile();
	return 1;
}
