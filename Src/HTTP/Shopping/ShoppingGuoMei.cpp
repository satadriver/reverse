#include "StdAfx.h"

#include "ShoppingGuoMei.h"

shopping::GuoMei::GuoMei(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::GuoMei::~GuoMei()
{
	
}

int shopping::GuoMei::IsGuoMei(HttpSession *http_session)
{
	//购物过程中 新增地址
	if((http_session->m_Requestline.m_Host.Find("cart.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/home/api/consignee/addAddress")!=-1))
	{
		return kPCWebAddAddress;
	}
	//购物过程中 修改地址
	if((http_session->m_Requestline.m_Host.Find("cart.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/home/api/consignee/updateAddress")!=-1))
	{
		return kPCWebModifyAddress;
	}
	//地址管理中 新增地址 (reverse问题 Get包获取不到)
	if((http_session->m_Requestline.m_Host.Find("member.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myaccount/address/insertAddress")!=-1))
	{
		return kPCWebAccountInsertAddress;
	}
	//地址管理中 修改地址 (reverse问题 Get包获取不到)
	if((http_session->m_Requestline.m_Host.Find("member.gome.com.cn")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/myaccount/address/updateAddress")!=-1))
	{
		return kPCWebAccountUpdateAddress;
	}
	return 0;
}

int shopping::GuoMei::Process(const char *packet,int action)
{
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	if (action == kPCWebAccountInsertAddress)
	{
		return PCWebAccountInsertAddress(packet);
	}
	if (action == kPCWebAccountUpdateAddress)
	{
		return PCWebAccountUpdateAddress(packet);
	}
	return 0;
}

int shopping::GuoMei::PCWebAddAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//姓名
	name_ = ms_->GetValueFromMapByKey("name");
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//地址
	address_ = ms_->GetValueFromMapByKey("province") + "," +
			   ms_->GetValueFromMapByKey("city") + "," +
			   ms_->GetValueFromMapByKey("country") + "," +
			   ms_->GetValueFromMapByKey("town") + "," +
			   ms_->GetValueFromMapByKey("address");
	//邮箱
	email_ = ms_->GetValueFromMapByKey("email");
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//昵称
		"oid=" + 		phone_ + "\r\n" + 	//电话
		"msg=" + 		address_ + "\r\n" + //收货地址
		"appname=" +	email_ + "\r\n" + 	//邮箱
		"state=新增地址\r\n";		
	WriteIndexFile();
	return 1;
}

int shopping::GuoMei::PCWebModifyAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,"&","=");
	//姓名
	name_ = ms_->GetValueFromMapByKey("name");
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
	//地址
	address_ = ms_->GetValueFromMapByKey("province") + "," +
		ms_->GetValueFromMapByKey("city") + "," +
		ms_->GetValueFromMapByKey("country") + "," +
		ms_->GetValueFromMapByKey("town") + "," +
		ms_->GetValueFromMapByKey("address");
	//邮箱
	email_ = ms_->GetValueFromMapByKey("email");
	
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//昵称
		"oid=" + 		phone_ + "\r\n" + 	//电话
		"msg=" + 		address_ + "\r\n" + //收货地址
		"appname=" +	email_ + "\r\n" + 	//邮箱
		"state=修改地址\r\n";	
	WriteIndexFile();
	return 1;
}

int shopping::GuoMei::PCWebAccountInsertAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,"&","=");
	//姓名
	name_ = ms_->GetValueFromMapByKey("firstName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
    //地址
	address_ = ms_->GetValueFromMapByKey("state",true) + "," +
			   ms_->GetValueFromMapByKey("city",true) + "," +
			   ms_->GetValueFromMapByKey("county",true) + "," +
			   ms_->GetValueFromMapByKey("town",true) + "," +
			   ms_->GetValueFromMapByKey("address",true);
    //邮箱
	email_ = ms_->GetValueFromMapByKey("email",true);

	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//昵称
		"oid=" + 		phone_ + "\r\n" + 	//电话
		"msg=" + 		address_ + "\r\n" + //收货地址
		"appname=" +	email_ + "\r\n" + 	//邮箱
	"state=新增地址\r\n";
	WriteIndexFile();
	return 1;
}

int shopping::GuoMei::PCWebAccountUpdateAddress(const char *packet)
{
	string url = http_session_->m_Requestline.m_URI.GetBuffer(0);
	url = ms_->UrlDecode(url);
	ms_->SplitStringToMap(url,"&","=");
	//姓名
	name_ = ms_->GetValueFromMapByKey("firstName",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile");
    //地址
	address_ = ms_->GetValueFromMapByKey("state",true) + "," +
		ms_->GetValueFromMapByKey("city",true) + "," +
		ms_->GetValueFromMapByKey("county",true) + "," +
		ms_->GetValueFromMapByKey("town",true) + "," +
		ms_->GetValueFromMapByKey("address",true);
    //邮箱
	email_ = ms_->GetValueFromMapByKey("email",true);
	
	if (name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}	
	output_text_ = 
		"nick="	 + 		name_ + "\r\n" + 	//昵称
		"oid=" + 		phone_ + "\r\n" + 	//电话
		"msg=" + 		address_ + "\r\n" + //收货地址
		"appname=" +	email_ + "\r\n" + 	//邮箱
		"state=修改地址\r\n";
	WriteIndexFile();
	return 1;
}