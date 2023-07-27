
#include "StdAfx.h"

#include "PhoneInfoZhuShou91.h"

phoneinfo::ZhuShou91::ZhuShou91(HttpSession *http_session, const string &pro_name) : PhoneInfo(http_session,pro_name)
{
	app_ = "91ÖúÊÖ";
}

phoneinfo::ZhuShou91::~ZhuShou91()
{
	
}

int phoneinfo::ZhuShou91::IsZhuShou91(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("bbx2.sj.91.com") != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/softs.ashx") != -1 ) )	
	{
		return kUploadPhoneInfo;
	}
	return 0;
}

int phoneinfo::ZhuShou91::Process(const char *packet,int action)
{
	if (action == kUploadPhoneInfo)
	{
		return UploadPhoneInfo(packet);
	}
	return 0;
}


int phoneinfo::ZhuShou91::UploadPhoneInfo(const char *packet)
{
	char *p = (char *)packet;
	if (!p)
	{
		return 0;
	}
	int len = http_session_->m_Requestline.m_nseq - http_session_->m_Requestline.m_Ssequence;
	int i = 0;
	for (;i <= len ; i++,p++)
	{
		if ( 0 == *p)
		{
			*p = '&';
		}
	}
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
//	string os,version,model,imsi;
//	version = ms_->GetValueFromMapByKey("osv");
//	model = ms_->GetValueFromMapByKey("dm");
	imei_ = ms_->GetValueFromMapByKey("imei");
	imsi_ = ms_->GetValueFromMapByKey("imsi");
	
	if ( imsi_.empty() || imei_.empty() /*|| version.empty() || model.empty()*/)
	{
		return 0;
	}	
	WriteIndexFile();
	return 1;
}

