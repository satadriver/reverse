
#include "StdAfx.h"

#include "ZhaiJiSong.h"

kuaidi::ZhaiJiSong::ZhaiJiSong(HttpSession *http_session, const string &pro_name) : KuaiDi(http_session,pro_name)
{
	
}

kuaidi::ZhaiJiSong::~ZhaiJiSong()
{
	
}

int kuaidi::ZhaiJiSong::IsZhaiJiSong(HttpSession *http_session)
{
	//查询
	if((http_session->m_Requestline.m_Host.Find("www.zjs.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/api/tracking")!=-1))
	{
		return kzhaijisonggw;
	}
	//快递100 
	if((http_session->m_Requestline.m_Host.Find("www.kuaidi100.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/query?type=zhaijisong")!=-1))
	{
		return kzhaijisong;
	}
	return 0;
}

int kuaidi::ZhaiJiSong::Process(const char *packet,int action)
{
	//查询
	if (action == kzhaijisonggw)
	{
		return zhaijisonggw(packet);
	}
	//快递100 查询
	if (action == kzhaijisong)
	{
		return zhaijisong(packet);
	}
	return 1;
}

//快递100 查询
int kuaidi::ZhaiJiSong::zhaijisong(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	hd=ms_->GetValueFromMapByKey("postid",true);
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=查询\r\n";
	WriteIndexFile();
	return 1;
}

//查询
int kuaidi::ZhaiJiSong::zhaijisonggw(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	hd=ms_->GetValueBetweenBeginEnd(src_data,"orderNos=","&");
	if (hd.empty())
	{
		return 0;
	}
	output_text_ = 
		"servername=" + hd + "\r\n" +
		"state=查询\r\n";
	WriteIndexFile();
	return 1;
}
