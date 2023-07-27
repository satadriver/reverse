
#include "StdAfx.h"

#include "ShenTong.h"

kuaidi::Shentong::Shentong(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::Shentong::~Shentong()
{
	
}

int kuaidi::Shentong::IsShenTong(HttpSession *http_session)
{
	// ���ڿ����ѯ /chaxun/result?express_no=3316288983985
	if((http_session->m_Requestline.m_Host.Find("q1.sto.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/chaxun/result")!=-1))  
	{
		return kShenTong;
	}
	// ���ʿ����ѯ /Home/Api?bagNo=3316288983985
	if((http_session->m_Requestline.m_Host.Find("intl.sto.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Home/Api")!=-1))  
	{
		return kShenTongInt;
	}
	//Android app��ͨ
	if((http_session->m_Requestline.m_Host.Find("appinterface.sto.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/StoAppPro/QueryOrderList")!=-1))
	{
		return kAndroidST;
	}
	//pcLogin
	if((http_session->m_Requestline.m_Host.Find("q1.sto.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/reg/login")!=-1))
	{
		return kPClogin;
	}
	return 0;
}

int kuaidi::Shentong::Process(const char *packet,int action)
{
	//���ڿ����ѯ
	if (action == kShenTong)
	{
		return ShenTong(packet);
	}
	//���ʿ����ѯ
	if (action == kShenTongInt)
	{
		return ShenTongInt(packet);
	}
	if (action==kAndroidST)
	{
		return AndroidST(packet);
	}

	if (action == kPClogin)
	{
		return PClogin(packet);
	}
	return 0;

}
int kuaidi::Shentong::ShenTong(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	int n=uri.find("express_no=");
	if (n<0)
	{
		return 0;
	}
	hd=uri.substr(n+11);
	ms_->Replace(hd,"\r\n",",");
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=��ѯ\r\n";
	WriteIndexFile();
	return 1;
}

//���ʿ����ѯ
int kuaidi::Shentong::ShenTongInt(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UrlDecode(uri);
	int n=uri.find("bagNo=");
	if (n<0)
	{
		return 0;
	}
	hd=uri.substr(n+ strlen("bagNo="));
	ms_->Replace(hd,"\r\n",",");
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=��ѯ\r\n";
	WriteIndexFile();
	return 1;
}

int kuaidi::Shentong::AndroidST(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	uri = ms_->UTF8ToGBK(UrlDecode(uri));
	ms_->Replace(uri,"?","&");
	ms_->SplitStringToMap(uri,'&','=');
	hd=ms_->GetValueFromMapByKey("mailNo",true);
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=��ѯ\r\n";

	WriteIndexFile();
	return 1;
}
int kuaidi::Shentong::PClogin(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string username=ms_->GetValueFromMapByKey("username",true);
	string pass = ms_->GetValueFromMapByKey("password",true);
	if (username.empty() || pass.empty())
	{
		return 0;
	}

	output_text_ =  
		"user=" + username + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";
	WriteIndexFile();
	return 1;
}