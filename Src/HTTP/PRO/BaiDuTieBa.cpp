#include "StdAfx.h"

#include "BaiduTieBa.h"

map<string,string>BaiDuTieBa::bduss_user_ = map<string,string>();

BaiDuTieBa::BaiDuTieBa()
{
	software_ = "百度贴吧";
	pro_ = "forum_baidutieba";
}

BaiDuTieBa::~BaiDuTieBa()
{
	
}

int BaiDuTieBa::Is(HttpSession *http_session)
{
	
	if( (http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/f/commit/thread/add")!=-1))
	{
		return kNewPostCW;
	}

	if((http_session->m_Requestline.m_Host.Find("c.tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/c/c/thread/add")!=-1))
	{
		return kNewPostM;
	}

	if((http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/f/commit/post/add")!=-1))
	{
		return kNewCommentCW;
	}

	if((http_session->m_Requestline.m_Host.Find("c.tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/c/c/post/add")!=-1))
	{
		return kNewCommentM;
	}

	if((http_session->m_Requestline.m_Host.Find("tieba.baidu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/tb/img/track.gif")!=-1))
	{
		return kLoginCW;
	}

	if( ( http_session->m_Requestline.m_Host.Find("c.tieba.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/c/s/inpv") != -1 ) )	
	{
		return kUploadDeviceInfoM;
	}
	return 0;
}

int BaiDuTieBa::Process(const char *packet,int action)
{
	
	if (action == kNewPostCW)
	{
		return NewPostCW(packet);
	}

	if (action == kNewPostM)
	{
		return NewPostM(packet);
	}

	if (action == kNewCommentCW)
	{
		return NewCommentCW(packet);
	}

	if (action == kNewCommentM)
	{
		return NewCommentM(packet);
	}

	if (action == kLoginCW)
	{
		return LoginCW(packet);
	}

	if (action == kUploadDeviceInfoM)
	{
		return UploadDeviceInfoM(packet);
	}
	return 0;
}

int BaiDuTieBa::NewPostCW(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string bar_name = ms_->GetValueFromMapByKey("kw",true);
	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("content",true);

	if (bar_name.empty() || title.empty() || content.empty())
	{
		return 0;
	}
	
	string user = GetUserName(GetBDUSS());
	
	WriteCaseInfo(content);
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"webname=" + bar_name + "\r\n" + 
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"platform=电脑\r\n" +
		"state=发帖\r\n";
	
	WriteIndexFile1();
	return 1;
}

int BaiDuTieBa::NewPostM(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string bduss = ms_->GetValueFromMapByKey("BDUSS");
	string bar_name = ms_->GetValueFromMapByKey("kw",true);
	string title = ms_->GetValueFromMapByKey("title",true);
	string content = ms_->GetValueFromMapByKey("content",true);
	string imei = ms_->GetValueFromMapByKey("_phone_imei");

	if (bar_name.empty() || title.empty() || content.empty() || imei.empty())
	{
		return 0;
	}
	

	string user = GetUserName(bduss);
	WriteCaseInfo(content);
	output_text_ = 
		"user=" + user + "\r\n" + 
		"imei=" + imei + "\r\n" + 
		"webname=" + bar_name + "\r\n" + 
		"msgtype=" + title + "\r\n" +
		"msg=" + content + "\r\n" +
		"platform=手机\r\n" +
		"state=发帖\r\n";
	
	WriteIndexFile1();
	return 1;
}

int BaiDuTieBa::NewCommentCW(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string bar_name = ms_->GetValueFromMapByKey("kw",true);
	string post_id = ms_->GetValueFromMapByKey("tid",true);
	string content = ms_->GetValueFromMapByKey("content",true);

	if (bar_name.empty() || content.empty() || post_id.empty())
	{
		return 0;
	}

	post_id = "http://tieba.baidu.com/p/" + post_id;

	string user = GetUserName(GetBDUSS());
	
	WriteCaseInfo(content);
	output_text_ = 
		"user=" + user + "\r\n" + 
		"webname=" + bar_name + "\r\n" + 
		"oid=" + post_id + "\r\n" + 
		"msg=" + content + "\r\n" +
		"platform=电脑\r\n" +
		"state=跟帖\r\n";
	
	WriteIndexFile1();
	return 1;
}

int BaiDuTieBa::NewCommentM(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	
	string bduss = ms_->GetValueFromMapByKey("BDUSS");
	string bar_name = ms_->GetValueFromMapByKey("kw",true);
	string content = ms_->GetValueFromMapByKey("content",true);
	string post_id = ms_->GetValueFromMapByKey("tid");
	string imei = ms_->GetValueFromMapByKey("_phone_imei");

	if (bar_name.empty() || content.empty() || imei.empty() || post_id.empty())
	{
		return 0;
	}
	
	string user = GetUserName(bduss);
	
	post_id = "http://tieba.baidu.com/p/" + post_id;
	WriteCaseInfo(content);
	output_text_ = 
		"user=" + user + "\r\n" + 
		"imei=" + imei + "\r\n" + 
		"webname=" + bar_name + "\r\n" + 
		"oid=" + post_id + "\r\n" + 
		"msg=" + content + "\r\n" +
		"platform=手机\r\n" +
		"state=跟帖\r\n";
	
	WriteIndexFile1();
	return 1;
}


int BaiDuTieBa::LoginCW(const char *packet)
{
	string src_data(http_session_->m_Requestline.m_URI.GetBuffer(0));
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');

	string user = ms_->GetValueFromMapByKey("uname",true);

	string bduss = GetBDUSS();
	if (!bduss.empty())
	{
		bduss_user_.insert(std::make_pair(bduss,user));
	}
	
	WriteVirtualIdentity(user,"登录");
	return 0;
}

int BaiDuTieBa::UploadDeviceInfoM(const char *packet)
{
	string src_data(packet);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	string version = ms_->GetValueFromMapByKey("os_version");
	string model = ms_->GetValueFromMapByKey("model",true);
	string imei = ms_->GetValueFromMapByKey("_phone_imei");
	DeviceInfo device_info;
	string os = device_info.GetOS(model,version);
	
	if (imei.empty() || version.empty() || model.empty() || os.empty())
	{
		return 0;
	}
	
	output_text_ = 
		"imei=" + imei + "\r\n" +
		"webname=" + model + "\r\n" +
		"from=" + os + "\r\n" +
		"to=" + version + "\r\n" +
		"state=" + software_ + "\r\n";
	
	WriteIndexFile();
	return 1;
}

string BaiDuTieBa::GetBDUSS()
{
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	ms_->SplitStringToMap(cookie,';','=');
	return  ms_->GetValueFromMapByKey("BDUSS");	
}

string BaiDuTieBa::GetUserName(const string &bduss)
{
	map<string,string>::iterator iter = bduss_user_.find(bduss);
	if (iter != bduss_user_.end())
	{
		return iter->second;
	}
	return "";
}