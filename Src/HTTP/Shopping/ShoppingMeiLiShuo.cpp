
#include "StdAfx.h"

#include "ShoppingMeiLiShuo.h"

shopping::MeiLiShuo::MeiLiShuo(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MeiLiShuo::~MeiLiShuo()
{
	
}

int shopping::MeiLiShuo::IsMeiLiShuo(HttpSession *http_session)
{
	//登录
	if((http_session->m_Requestline.m_Host.Find("www.meilishuo.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/user/aj/login")!=-1))
	{
		return kPcWebLogin;
	}
	//新增地址 Get reverse->解析不到
	if((http_session->m_Requestline.m_Host.Find("logistics.meilishuo.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jsonp/addReceiveAddressActionlet/1")!=-1))
	{
		return kPcWebAddAddress;
	}
	//修改地址 Get reverse->解析不到
	if((http_session->m_Requestline.m_Host.Find("logistics.meilishuo.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jsonp/modifyReceiveAddressActionlet/1")!=-1))
	{
		return kPcWebModifyAddress;
	}
	//android新增地址
	if(/*(http_session->m_Requestline.m_Host.Find("103.229.150.65")!=-1) && */
		(http_session->m_Requestline.m_URI.Find("/gw/mwp.TradeWebLogistics.AddReceiveAddressActionlet")!=-1))
	{
		return kAndroidAddAddress;
	}
	//android修改地址 
	if(/*(http_session->m_Requestline.m_Host.Find("103.229.150.65")!=-1) && */
		(http_session->m_Requestline.m_URI.Find("/gw/mwp.TradeWebLogistics.ModifyReceiveAddressActionlet")!=-1))
	{
		return kAndroidModifyAddress;
	}
	return 0;
}

int shopping::MeiLiShuo::Process(const char *packet,int action)
{
	if (action == kPcWebLogin)
	{
		return PcWebLogin(packet);
	}
	if (action == kPcWebAddAddress)
	{
		return PcWebAddAddress(packet);
	}
	if (action == kPcWebModifyAddress)
	{
		return PcWebModifyAddress(packet);
	}
	if (action == kAndroidAddAddress)
	{
		return AndroidAddAddress(packet);
	}
	if (action == kAndroidModifyAddress)
	{
		return AndroidModifyAddress(packet);
	}
	return 0;
}
//用户登录
int shopping::MeiLiShuo::PcWebLogin(const char *packet)
{
	string src_data(packet);	
	string tookit = "uname=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	ms_->SplitStringToMap(src_data,'&','=');
	user_ = ms_->GetValueFromMapByKey("uname",true);
	//密码加密(加密方式未知)
	string password = ms_->GetValueFromMapByKey("password",true);
	
	if ( user_.empty() || password.empty() )
	{
		return 0;
	}
	output_text_ =  
		"user=" + user_ + "\r\n" + 
		"pass=" + password +  "\r\n" + 
		"state=登录\r\n";
	WriteIndexFile();
	return 1;
}

//pc新增地址 
int shopping::MeiLiShuo::PcWebAddAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url=ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,'&','=');
	//姓名
	name_=ms_->GetValueFromMapByKey("realName",true);
	//电话
	phone_=ms_->GetValueFromMapByKey("mobile",true);
    //地址
	address_= ms_->GetValueFromMapByKey("province",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("address",true);
	//邮编
	postcode_=ms_->GetValueFromMapByKey("zip",true);	
	output_text_=
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" 
			"state=添加地址\r\n";
	WriteIndexFile();
	return 1;
}

//pc修改地址 
int shopping::MeiLiShuo::PcWebModifyAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url=ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,'&','=');
	//姓名
	name_=ms_->GetValueFromMapByKey("realName",true);
	//电话
	phone_=ms_->GetValueFromMapByKey("mobile",true);
    //地址
	address_= ms_->GetValueFromMapByKey("province",true) + 
		ms_->GetValueFromMapByKey("city",true) +
		ms_->GetValueFromMapByKey("area",true) +
		ms_->GetValueFromMapByKey("address",true);
	//邮编
	postcode_=ms_->GetValueFromMapByKey("zip",true);	
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" 
		"state=修改地址\r\n";
	WriteIndexFile();
	return 1;
}

//android新增地址 
int shopping::MeiLiShuo::AndroidAddAddress(const char *packet)
{
	string src_data(packet);
	string strTrack = "data=";
	if (src_data.rfind(strTrack) != -1)
	{
		src_data = src_data.substr(src_data.rfind(strTrack) + strTrack.length());
	}
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));

	//json提取数据
	Json::Value value;
	Json::Reader rader;
	if (rader.parse(src_data,value))
	{
		//姓名
		name_ = value["realName"].asString();
		//电话
		phone_ = value["mobile"].asString();
		//地址
		address_ =	value["province"].asString() + 
					value["city"].asString() +
					value["area"].asString() +
					value["address"].asString();
        //邮编
		postcode_ = value["zip"].asString();	
	}
    if (name_.empty() || phone_.empty())
    {
		return 0;
    }  	
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" 
		"state=添加地址\r\n";
	WriteIndexFile();
	return 1;
}

//android新增地址 
int shopping::MeiLiShuo::AndroidModifyAddress(const char *packet)
{
	string src_data(packet);
	string strTrack = "data=";
	if (src_data.rfind(strTrack) != -1)
	{
		src_data = src_data.substr(src_data.rfind(strTrack) + strTrack.length());
	}
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
	
	//json提取数据
	Json::Value value;
	Json::Reader rader;
	if (rader.parse(src_data,value))
	{
		//姓名
		name_ = value["realName"].asString();
		//电话
		phone_ = value["mobile"].asString();
		//地址
		address_ =	value["province"].asString() + 
			value["city"].asString() +
			value["area"].asString() +
			value["address"].asString();
        //邮编
		postcode_ = value["zip"].asString();	
	}
    if (name_.empty() || phone_.empty())
    {
		return 0;
    }  	
	output_text_=
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" 
		"state=修改地址\r\n";
	WriteIndexFile();
	return 1;
}