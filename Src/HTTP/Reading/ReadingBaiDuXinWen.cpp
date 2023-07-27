
#include "StdAfx.h"
#include "ReadingBaiDuXinWen.h"

reading::BaiDuXinWen::BaiDuXinWen(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::BaiDuXinWen::~BaiDuXinWen()
{
	
}

int reading::BaiDuXinWen::IsBaiDuXinWen(HttpSession *http_session)
{
	//android评论内容
	if(  ( http_session->m_Requestline.m_Host.Find("api.baiyue.baidu.com")  != -1 ) &&  
		( http_session->m_Requestline.m_URI.Find("/sn/api/comment") != -1 ))	
	{
		return kAddCommentM;
	}
	return 0;
}

int reading::BaiDuXinWen::Process(const char *packet,int action)
{
	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}


int reading::BaiDuXinWen::AddCommentM(const char *packet)
{
    //数据部分
	string src_data(packet);
	string src_dataEnd;
	if (src_data.find("text=") != string::npos )
	{
		src_dataEnd = src_data.substr(src_data.find("text="));
	}	
	src_dataEnd = ms_->UTF8ToGBK(UrlDecode(src_dataEnd));
	ms_->SplitStringToMap(src_dataEnd,'&','=');
    //内容
	content_ = ms_->GetValueFromMapByKey("text");
  
	if (content_.empty() /*| url_.empty()*/)
	{
		return 0;
	}
	output_text_ =
		"msg=" + content_ + "\r\n" +
		"platform=手机\r\n"
		"state=评论\r\n";
	WriteIndexFile();
	return 1;
}
