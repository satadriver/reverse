
#include "StdAfx.h"

#include "ReadingTianTianKuaiBao.h"

reading::TianTianKuaiBao::TianTianKuaiBao(HttpSession *http_session, const string &pro_name) : Reading(http_session,pro_name)
{
	
}

reading::TianTianKuaiBao::~TianTianKuaiBao()
{
	
}

int reading::TianTianKuaiBao::IsTianTianKuaiBao(HttpSession *http_session)
{
	if( ( http_session->m_Requestline.m_Host.Find("w.cnews.qq.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/shareQQNewsMulti") != -1 ) )
	{
		return kAddCommentM;
	}

	return 0;
}

int reading::TianTianKuaiBao::Process(const char *packet,int action)
{

	if (action == kAddCommentM)
	{
		return AddCommentM(packet);
	}
	return 0;
}



int reading::TianTianKuaiBao::AddCommentM(const char *packet)
{		
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UrlDecode(cookie);
	ms_->SplitStringToMap(cookie, ';','=');

	user_ = ms_->GetValueBetweenBeginEnd(cookie,"uin=o",";");

	string src_date(packet);
	ms_->UrlDecode(src_date);
	ms_->SplitStringToMap(src_date,'&','=');
	title_ =  ms_->GetValueFromMapByKey("title",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	url_ = ms_->GetValueFromMapByKey("url",true);

	ms_->Replace(url_,"\r","");
	ms_->Replace(url_,"\n","");

	if (title_.empty() || content_.empty() || url_.empty() || user_.empty())
	{
		return 0;
	}
	output_text_ =
		"user=" + user_ + "\r\n" +
		"mid=" + title_ + "\r\n" + 
		"oid=" + url_ + "\r\n" +
		"msg=" + content_ + "\r\n" +
		"platform=ÊÖ»ú\r\n"
		"state=ÆÀÂÛ\r\n";
	WriteIndexFile();
	return 1;
}