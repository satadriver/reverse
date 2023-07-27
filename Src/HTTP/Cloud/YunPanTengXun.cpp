
#include "StdAfx.h"
#include "YunPanTengXun.h"



yunpan::TengXunWeiYun::TengXunWeiYun(HttpSession *http_session, const string &pro_name)
: YunPan(http_session,pro_name)
{
	
}

yunpan::TengXunWeiYun::~TengXunWeiYun()
{
	
}

yunpan::TengXunWeiYun::IsTengXunWeiYun(HttpSession *http_session)
{	
	//web下载文件
	if(( http_session->m_Requestline.m_URI.Find("/ftn_handler/") != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("fname") != -1))	
	{
		return kDownloadFileCW;
	}
	
	if( ( http_session->m_Requestline.m_Host.Find("yun.ftn.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/ftn_handler/") != -1 )/* &&
		( http_session->m_Requestline.m_URI.Find("fid=") != -1)*/)	
	{
		return kDownloadFileCC;
	}
	
	return 0;
}

yunpan::TengXunWeiYun::Process(const char *packet,int action)
{
	if (action == kDownloadFileCW)
	{
		return DownloadFileCW(packet);
	}	
	if (action == kDownloadFileCC)
	{
		return DownloadFileCC(packet);
	}
	return 0;
}
//web下载
yunpan::TengXunWeiYun::DownloadFileCW(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));	
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');	
	user_ = ms_->GetValueFromMapByKey("uin",true);
	filename_ = ms_->GetValueFromMapByKey("fname",true);
    if (filename_.empty())
    {
		return 0;
    }
	output_text_ = 
		"user=" +user_ + "\r\n" +		
		"msg=" + filename_ + "\r\n" + 
		"state=下载\r\n";
	WriteIndexFile();
	return 1;
}

yunpan::TengXunWeiYun::DownloadFileCC(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	
	uri = ms_->UrlDecode(uri);
	
	filename_ = ms_->GetValueBetweenBeginEnd(uri,"/","?",true);


	if (filename_.empty())
	{
		return 0;
	}
	
	string::size_type pos = filename_.rfind("/");
	if (pos != string::npos)
	{
		filename_ = filename_.substr(pos + 1);
	}
	
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=下载\r\n";
	WriteIndexFile();
	return 1;
}
