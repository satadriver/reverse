
#include "StdAfx.h"
#include "YunPan360.h"
#include "../BoundaryParser.h"
yunpan::YunPan360::YunPan360(HttpSession *http_session, const string &pro_name)
 : YunPan(http_session,pro_name)
{

}

yunpan::YunPan360::~YunPan360()
{

}

yunpan::YunPan360::IsYunPan360(HttpSession *http_session)
{
	//上传 不可解 2017-07-18
	if( ( http_session->m_Requestline.m_Host.Find("up48.yunpan.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/webupload?devtype") != -1 ))	
	{
		return kUploadFileCW;
	}
	//web下载 
	if( ( http_session->m_Requestline.m_Host.Find("c-c.yunpan.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/file/download") != -1 ))	
	{
		return kDownloadFileCW;
	}
	//pc下载 
	if( ( http_session->m_Requestline.m_Host.Find("api-c.yunpan.360.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/intf.php?method=File.getThumb") != -1 ))	
	{
		return kDownloadFileCC;
	}
	return 0;
}

yunpan::YunPan360::Process(const char *packet,int action)
{
	if (action == kUploadFileCW)
	{
		return UploadFileCW(packet);
	}
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

yunpan::YunPan360::UploadFileCW(const char *packet)
{
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key,path;
	key = bp.GetBoundaryKey("name=\"file\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	boundary = "\r\n--" + boundary;  
	path=ms_-> GetValueBetweenBeginEnd(src_data,"name=\"path\"\r\n\r\n",boundary,true);
	filename_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);
	string real_file_name = WriteTempRealFile(bv->ptr,filename_.c_str(),bv->len);
	filename_=path + filename_;
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=上传\r\n";
	WriteIndexFile(real_file_name);
	return 1;
}

//web下载
yunpan::YunPan360::DownloadFileCW(const char *packet)
{
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie,";","=");
	//用户名
	user_ = ms_->GetValueFromMapByKey("YUNPAN_USER",true);
    //
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
    ms_->SplitStringToMap(src_data,"&","=");
	//文件名
	filename_ = ms_->GetValueFromMapByKey("fname",true);
	if (filename_.empty())
	{
		return 0;
	}
	output_text_ = 
		"user=" + user_ + "\r\n" +		//用户名
		"msg=" + filename_ + "\r\n" + 
		"state=下载\r\n";
	WriteIndexFile();
	return 1;
}

//pc下载
yunpan::YunPan360::DownloadFileCC(const char *packet)
{
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0);
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie,";","=");
	//文件名
	filename_ = ms_->GetValueFromMapByKey("file_path",true);
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
