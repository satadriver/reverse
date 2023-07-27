#include "StdAfx.h"
#include "YunPanWangYi.h"

yunpan::WangYiWangPan::WangYiWangPan(HttpSession *http_session, const string &pro_name)
: YunPan(http_session,pro_name)
{
	
}

yunpan::WangYiWangPan::~WangYiWangPan()
{
	
}

yunpan::WangYiWangPan::IsWangYiWangPan(HttpSession *http_session)
{	
	//web上传
	if( ( http_session->m_Requestline.m_Host.Find("wp.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/filehub/nf/file") != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("op=proxyUpLoad") != -1))	
	{
		return kUploadFile;
	}
	//web下载
	if( ( http_session->m_Requestline.m_Host.Find("wp.163.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/filehub/nf/file") != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("op=downLoadFile") != -1))	
	{
		return kUploadFile;
	}	
	return 0;
}

yunpan::WangYiWangPan::Process(const char *packet,int action)
{
	//web上传
	if (action == kUploadFile)
	{
		return UploadFile(packet);
	}
	//web下载
	if (action == kDownloadFile)
	{
		return DownloadFile(packet);
	}
	return 0;
}

//web上传
yunpan::WangYiWangPan::UploadFile(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	user_ = ms_->GetValueFromMapByKey("uid",true);

	string boundary = http_session_->m_Requestline.m_strBoundary.GetBuffer(0);
	boundary = "\r\n--" + boundary;	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	filename_ = ms_-> GetValueBetweenBeginEnd(src_data,"name=\"Filename\"\r\n\r\n",boundary,true);
	if (filename_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_+ "\r\n" +		
		"msg=" +  filename_ + "\r\n" + 
		"state=上传\r\n";
	WriteIndexFile();
	return 0;
}
//web下载
yunpan::WangYiWangPan::DownloadFile(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	user_ = ms_->GetValueFromMapByKey("uid",true);
    filename_ = ms_->GetValueFromMapByKey("name",true);

	if (filename_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_+ "\r\n" +		
		"msg=" +  filename_ + "\r\n" + 
		"state=下载\r\n";
	WriteIndexFile();
	return 0;
}
