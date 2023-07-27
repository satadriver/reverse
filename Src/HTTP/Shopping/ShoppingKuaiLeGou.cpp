#include "StdAfx.h"
#include "ShoppingKuaiLeGou.h"

shopping::KuaiLeGou::KuaiLeGou(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::KuaiLeGou::~KuaiLeGou()
{
	
}

int shopping::KuaiLeGou::IsKuaiLeGou(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("www.happigo.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/login")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址 修改地址 (购物过程中)
	if((http_session->m_Requestline.m_Host.Find("www.happigo.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/shop/index.php")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("act=member_address")==-1))
	{
		return kPCWebAddAddress;
	}
	//pc新增地址 修改地址 2 (地址管理)
	if((http_session->m_Requestline.m_Host.Find("www.happigo.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/shop/index.php?act=member_address&op=upaddress&inajax=1")!=-1))
	{
		return kPCWebModifyAddress;
	}
	return 0;
}

int shopping::KuaiLeGou::Process(const char *packet,int action)
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
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	return 0;
}

//pc登录信息
int shopping::KuaiLeGou::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("user_name",true);
	pass = ms_->GetValueFromMapByKey("password",true);
	
	if ( user.empty() || pass.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user,"上线");
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=登录\r\n";	
	WriteIndexFile();
	return 1;
}

//pc新增地址
int shopping::KuaiLeGou::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	
	name_ = ms_->GetValueFromMapByKey("true_name",true);
	phone_ = ms_->GetValueFromMapByKey("mob_phone");
	address_ =	ms_->GetValueFromMapByKey("statename_name",true) + 
				ms_->GetValueFromMapByKey("city_name",true) +
				ms_->GetValueFromMapByKey("county_name",true) +
				ms_->GetValueFromMapByKey("town_name",true) + 
				ms_->GetValueFromMapByKey("address",true) ;
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"state=新增地址\r\n";
	WriteIndexFile();
	return 0;
}

//pc修改地址
int shopping::KuaiLeGou::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	ms_->SplitStringToMap(src_data,'&','=');
	//id
	string id;
	id=ms_->GetValueFromMapByKey("receiver_seq_id",true);
	if (id.empty())
	{
		//用户名
		name_ = ms_->GetValueFromMapByKey("true_name",true);
		//地址(地址码)
		address_ = ms_->GetValueFromMapByKey("statename",true) + "," +
			       ms_->GetValueFromMapByKey("city",true) + "," + 
				   ms_->GetValueFromMapByKey("county",true) + "," +
				   ms_->GetValueFromMapByKey("town",true) + "+" +
				   ms_->GetValueFromMapByKey("address",true);
		//手机号
		string phone,mobile;
		mobile = ms_->GetValueFromMapByKey("mob_phone",true);
		phone = ms_->GetValueFromMapByKey("tel_phone",true);
		phone_ = mobile + "," +phone;
		if ( name_.empty() || phone_.empty() || address_.empty())
		{
			return 0;
		}
		
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=新增地址\r\n";
	}
	else
	{
		//用户名
		name_ = ms_->GetValueFromMapByKey("true_name",true);
		//地址(地址码)
		address_ = ms_->GetValueFromMapByKey("statename",true) + "," +
			       ms_->GetValueFromMapByKey("city",true) + "," + 
				   ms_->GetValueFromMapByKey("county",true) + "," +
				   ms_->GetValueFromMapByKey("town",true) + "," +
				   ms_->GetValueFromMapByKey("address",true);
		//手机号
		phone_ = ms_->GetValueFromMapByKey("tel_phone",true);
		if (name_.empty() || address_.empty() || phone_.empty())
		{
			return 0;
		}
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=修改地址\r\n";
	}
	WriteIndexFile();
	return 0;
}
