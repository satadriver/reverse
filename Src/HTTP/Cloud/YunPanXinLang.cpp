
#include "StdAfx.h"
#include "YunPanXinLang.h"

yunpan::XinLangWeiPan::XinLangWeiPan(HttpSession *http_session, const string &pro_name)
: YunPan(http_session,pro_name)
{
	
}

yunpan::XinLangWeiPan::~XinLangWeiPan()
{
	
}

yunpan::XinLangWeiPan::IsXinLangWeiPan(HttpSession *http_session)
{	
	//����	
	if( ( http_session->m_Requestline.m_Host.Find("vdisk.weibo.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/search/") != -1 ))	
	{
		return kSearchFileCW;
	}
	//���� (url ��ȫ)
	if( ( http_session->m_Requestline.m_Host.Find("data.weipan.cn.wscdns.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("ip") != -1 ))	
	{
		return kDownloadFileCW;
	}
	return 0;
}

yunpan::XinLangWeiPan::Process(const char *packet,int action)
{
	//����
	if (action == kSearchFileCW)
	{
		return SearchFileCW(packet);
	}
	//����
	if (action == kDownloadFileCW)
	{
		return DownloadFileCW(packet);
	}
	return 0;
}

//����
yunpan::XinLangWeiPan::SearchFileCW(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	
	ms_->SplitStringToMap(uri,'&','=');	
	//�ļ���
	filename_ = ms_->GetValueFromMapByKey("keyword",true);
	
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

//����
yunpan::XinLangWeiPan::DownloadFileCW(const char *packet)
{
	string uri(http_session_->m_Requestline.m_URI.GetBuffer(0));
	uri = ms_->UrlDecode(uri);
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');	
    filename_ = ms_->GetValueFromMapByKey("fn",true);
	if (filename_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + filename_ + "\r\n" + 
		"state=����\r\n";
	WriteIndexFile();
	return 1;
}

