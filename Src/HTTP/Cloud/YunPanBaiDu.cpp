
#include "StdAfx.h"
#include "YunPanBaiDu.h"

yunpan::BaiDuYunPan::BaiDuYunPan(HttpSession *http_session, const string &pro_name)
 : YunPan(http_session,pro_name)
{

}

yunpan::BaiDuYunPan::~BaiDuYunPan()
{

}

yunpan::BaiDuYunPan::IsBaiDuYunPan(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("pan.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/create") != -1 ) )	
	{
		return kUploadFileCW;
	}

	if( ( http_session->m_Requestline.m_Host.Find(".baidupcs.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/file/") != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("fn=") != -1))	
	{
		return kDownloadFileCW;
	}

	/*
	if( ( http_session->m_Requestline.m_Host.Find("pan.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/precreate") != -1 ) )	
	{
		return kUploadFileM_CC;
	}
*/
	if( ( http_session->m_Requestline.m_Host.Find("pcs.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("pcs/file") != -1 ) &&
		( http_session->m_Requestline.m_URI.Find("path=") != -1) && 
		( http_session->m_Requestline.m_URI.Find("download") != -1))	
	{
		return kDownloadFileM_CC;
	}
	return 0;
}

yunpan::BaiDuYunPan::Process(const char *packet,int action)
{
	if (action == kUploadFileCW)
	{
		return UploadFileCW(packet);
	}

	if (action == kDownloadFileCW)
	{
		return DownloadFileCW(packet);
	}

	if (action == kUploadFileM_CC)
	{
		return UploadFileM_CC(packet);
	}

	if (action == kDownloadFileM_CC)
	{
		return DownloadFileM_CC(packet);
	}

	return 0;
}

yunpan::BaiDuYunPan::UploadFileCW(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	filename_ = ms_->GetValueFromMapByKey("path",true);
//	ms_->Replace(filename_,"\\\\","");

	if (filename_.empty())
	{
		return 0;
	}

	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=上传\r\n";
	WriteIndexFile();

	return 1;

}

yunpan::BaiDuYunPan::DownloadFileCW(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);

	uri = ms_->UrlDecode(uri);

	ms_->SplitStringToMap(uri,'&','=');
	filename_ = ms_->GetValueFromMapByKey("fn",true);

	if (filename_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=下载\r\n";
	WriteIndexFile();
	return 1;
}

yunpan::BaiDuYunPan::UploadFileM_CC(const char *packet)
{
	string src_data = GetPostData_tmp(packet);

	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	filename_ = ms_->GetValueFromMapByKey("path",true);
	//	ms_->Replace(filename_,"\\\\","");
	
	if (filename_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=上传\r\n";
	WriteIndexFile();
	
	return 1;
	
}

yunpan::BaiDuYunPan::DownloadFileM_CC(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	uri = ms_->UrlDecode(uri);
	
	ms_->SplitStringToMap(uri,'&','=');
	filename_ = ms_->GetValueFromMapByKey("path",true);
	
	if (filename_.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=下载\r\n";
	WriteIndexFile();
	return 1;
}