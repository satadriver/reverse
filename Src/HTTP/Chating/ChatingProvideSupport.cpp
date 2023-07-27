
#include "StdAfx.h"
#include "ChatingProvideSupport.h"

chating::ProvideSupport::ProvideSupport(HttpSession *http_session, const string &pro_name) : Chating(http_session,pro_name)
{
	
}

chating::ProvideSupport::~ProvideSupport()
{
	
}


int chating::ProvideSupport::IsProvideSupport(HttpSession *http_session)
{
	/************************************************************************/
	/*   |命令  ||区别网站的标志		    |
	GET /command/1xfq51il5pa7v1wyqn1t3pz5kn/visitor.json?connection-id=i9O5W3EakuO1_WVM&
	                               |消息内容																			  |
	callback=p_124&t=1475194909718&commands=1475194834145-MESSAGE-10-697512-1475194909245-.17.Helo_2CGood-Morning-129691421       
	 */  
	/************************************************************************/
	//ProvideSupport (官网) 
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1xfq51il5pa7v1wyqn1t3pz5kn") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcProvideSupportSendMessage;
	}
	//皇冠 (现金网)
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1l77cpk8a3jug1wrgvexenvluu") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcHuangGuanSendMessage;
	}
	//太阳城 
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("0jc1a4xktpcdc13iwpjrfqa7vt") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcTaiYangChengSendMessage;
	}
	//888真人 
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("0loqkm6dbwn5s083zoft54cukx") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPc888ZhenRen;
	}
	//广东会
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("0q850yuny5ihd0m82ffoyiw0rc") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcGuangDongHui;
	}
	//威尼斯人赌场
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1u4i7cohrc6va0sxbysdaway7a") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcWeiNiSiRenDuChang;
	}
	//bet365
	if( (http_session->m_Requestline.m_URI.Find("/command/") != -1) &&
		(http_session->m_Requestline.m_URI.Find("1t4oid3qvbvub0ak8l52dfhrwr") != -1) &&
		(http_session->m_Requestline.m_URI.Find("MESSAGE") != -1) &&
		(http_session->m_Requestline.m_Host.Find(".providesupport.com") != -1))	
	{
		return kPcBet365;
	}
	return 0;
}


int chating::ProvideSupport::Process(const char *packet,int action)
{
	//ProvideSupport (官网)
	if (action == kPcProvideSupportSendMessage)
	{
		return PcProvideSupportSendMessage(packet);
	}
	//皇冠 (现金网)
	if (action == kPcHuangGuanSendMessage)
	{
		return PcHuangGuanSendMessage(packet);
	}
	//太阳城
	if (action == kPcTaiYangChengSendMessage)
	{
		return PcTaiYangChengSendMessage(packet);
	}
	//888真人
	if (action == kPc888ZhenRen)
	{
		return Pc888ZhenRenSendMessage(packet);
	}
	//广东会
	if (action == kPcGuangDongHui)
	{
		return PcGuangDongHui(packet);
	}
	//威尼斯人赌场
	if (action == kPcWeiNiSiRenDuChang)
	{
		return PcWeiNiSiRenDuChang(packet);
	}
	//bet365
	if (action == kPcBet365)
	{
		return PcBet365(packet);
	}
	return 0;
}


//ProvideSupport (官网)
int chating::ProvideSupport::PcProvideSupportSendMessage(const char *packet)
{
	//消息来源
	string srcFrom  = "ProvideSupport(官网)";
	//网址
	string srcNet = "www.providesupport.com";
	
	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
	
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}

//皇冠 (现金网)
int chating::ProvideSupport::PcHuangGuanSendMessage(const char *packet)
{
	//消息来源
	string srcFrom  = "皇冠(现金网)";
	//网址
	string srcNet = "www.hg077088.com";

	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
	
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}

//太阳城
int chating::ProvideSupport::PcTaiYangChengSendMessage(const char *packet)
{
	//消息来源
	string srcFrom  = "太阳城";
	//网址
	string srcNet = "www.tyc7861.cc";
	
	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}


//888真人
int chating::ProvideSupport::Pc888ZhenRenSendMessage(const char *packet)
{
	//消息来源
	string srcFrom  = "888真人";
	//网址
	string srcNet = "www.zr18.cc";
	
	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
// 	if (message_.find("*")!=string::npos)
// 	{
// 		ms_->Replace(message_,"*","\\u");
// 		message_ = ms_->UnicodeToGBKPrefix_u(message_);
// 	}
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}

//广东会
int chating::ProvideSupport::PcGuangDongHui(const char *packet)
{
	//消息来源
	string srcFrom  = "广东会";
	//网址
	string srcNet = "www.gdh94.com";
	
	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}

//威尼斯人赌场
int chating::ProvideSupport::PcWeiNiSiRenDuChang(const char *packet)
{
	//消息来源
	string srcFrom  = "威尼斯人赌场";
	//网址
	string srcNet = "www.v086.cc";
	
	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}

//bet365
int chating::ProvideSupport::PcBet365(const char *packet)
{
	//消息来源
	string srcFrom  = "bet365";
	//网址
	string srcNet = "www.365a9.cc";
	
	//提取信息
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	string::size_type tmp_pos = uri.find("&");
	if (tmp_pos != string::npos)
	{
		uri = uri.substr(tmp_pos + 1);
	}
	else
	{
		return 0;
	}
	ms_->SplitStringToMap(uri,'&','=');
    //消息内容	
	message_ = ms_->GetValueFromMapByKey("commands");
	if (message_.empty())
	{
		return 0;
	}
	output_text_ = 
		"msg=" + message_ + "\r\n" + 
		"from=" + srcFrom + "\r\n" +
		"webname=" + srcNet + "\r\n" +
		"state=发送消息\r\n";
	WriteIndexFile();
	return 1;
}
