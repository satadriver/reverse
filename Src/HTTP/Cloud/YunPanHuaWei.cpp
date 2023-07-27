
#include "StdAfx.h"
#include "YunPanHuaWei.h"

yunpan::HuaWeiYunPan::HuaWeiYunPan(HttpSession *http_session, const string &pro_name)
: YunPan(http_session,pro_name)
{
	
}

yunpan::HuaWeiYunPan::~HuaWeiYunPan()
{
	
}

yunpan::HuaWeiYunPan::IsHuaWeiYunPan(HttpSession *http_session)
{
	if( 
		( http_session->m_Requestline.m_URI.Find("/upload/up.php") != -1 ) )	
	{
		return kUploadFile;
	}
	
	
	if( 
		( http_session->m_Requestline.m_URI.Find("/file/") != -1 ) &&
		(http_session->m_Requestline.m_URI.Find("mode=download") != -1))	
	{
		return kDownloadFile;
	}
	
	return 0;
}

yunpan::HuaWeiYunPan::Process(const char *packet,int action)
{
	if (action == kUploadFile)
	{
		return UploadFile(packet);
	}
	
	if (action == kDownloadFile)
	{
		return DownloadFile(packet);
	}
	return 0;
}

yunpan::HuaWeiYunPan::UploadFile(const char *packet)
{
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
		"msg=" + filename_ + "\r\n" + 
		"state=ио╢╚\r\n";
	WriteIndexFile();
	
	return 1;
	
}

yunpan::HuaWeiYunPan::DownloadFile(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
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
		"state=обть\r\n";
	WriteIndexFile();
	return 1;

}
