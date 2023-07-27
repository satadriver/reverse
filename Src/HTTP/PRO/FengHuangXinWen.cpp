
#include "StdAfx.h"

#include "FengHuangXinWen.h"

FengHuangXinWen::FengHuangXinWen()
{
	software_ = "·ï»ËÐÂÎÅ";
	pro_ = "phoneread_newsfh";
	
}

FengHuangXinWen::~FengHuangXinWen()
{
	
}

int FengHuangXinWen::Is(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("comment.ifeng.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/post.php?format=new") != -1 ) )
	{
		return kAddCommentC;
	}	

	/*
	if( ( http_session->m_Requestline.m_Host.Find("icomment.ifeng.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wappost.php") != -1 ) )
	{
		return kAddCommentM;
	}
*/
	if(	(http_session->m_Requestline.m_Host.Find("stadig.ifeng.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/appsta.js?")!=-1))
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int FengHuangXinWen::Process(const char *packet,int action)
{
	if (action == kAddCommentC)
	{
		return AddCommentC(packet);
	}

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}

	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}


int FengHuangXinWen::AddCommentC(const char *packet)
{	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');
	string user = ms_->GetValueFromMapByKey("userid");

	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string title =  ms_->GetValueFromMapByKey("docName",true);
	string content = ms_->GetValueFromMapByKey("content",true);
	string url = ms_->GetValueFromMapByKey("docUrl");
	
	if (title.empty() || content.empty() || url.empty())
	{
		return 0;
	}

//	output_text_ = GetOutputText(kC);
	WriteIndexFile();
	return 1;
}

int FengHuangXinWen::AddCommentM(const char *packet)
{		
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	ms_->SplitStringToMap(src_data,'&','=');

	string title =  ms_->GetValueFromMapByKey("docName",true);
	string content = ms_->GetValueFromMapByKey("content",true);
	string url = ms_->GetValueFromMapByKey("docUrl",true);


	if (title.empty() || content.empty() || url.empty())
	{
		return 0;
	}

//	output_text_ = GetOutputText(kM);
	WriteIndexFile();
	return 1;
}

int FengHuangXinWen::UploadDeviceInfoM(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	ms_->SplitStringToMap(uri,'&','=');
	
	string model = ms_->GetValueFromMapByKey("ua");
	string imei = ms_->GetValueFromMapByKey("userkey");
	string tmp = ms_->GetValueFromMapByKey("mos");
	string::size_type pos = tmp.find("_");
	string os;
	string version;
	if (pos != string::npos)
	{
		os = os.substr(0, pos);
		version = os.substr(pos + 1);
	}
	
	if (model.empty() || imei.empty())
	{
		return 0;
	}
	
//	output_text_ = GetOutputText();
	
	WriteIndexFile();
	return 1;
}

string FengHuangXinWen::GetOutputText(int platform)
{
	return "";
}