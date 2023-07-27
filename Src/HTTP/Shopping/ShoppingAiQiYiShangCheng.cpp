
#include "StdAfx.h"
#include "ShoppingAiQiYiShangCheng.h"

shopping::AiQiYiShangCheng::AiQiYiShangCheng(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::AiQiYiShangCheng::~AiQiYiShangCheng()
{
	
}

int shopping::AiQiYiShangCheng::IsAiQiYiShangCheng(HttpSession *http_session)
{
	//pc新增地址
	if((http_session->m_Requestline.m_Host.Find("mall.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/apis/address/add.action")!=-1))
	{
		return kPCWebAddAddress;
	}
	//pc修改地址
	if((http_session->m_Requestline.m_Host.Find("mall.iqiyi.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/apis/address/update.action")!=-1))
	{
		return kPCWebModifyAddress;
	}
	return 0;
}

int shopping::AiQiYiShangCheng::Process(const char *packet,int action)
{	
    //pc新增地址
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
    //pc修改地址
	if (action == kPCWebModifyAddress)
	{
		return PCWebModifyAddress(packet);
	}
	return 0;
}

//pc新增地址
int shopping::AiQiYiShangCheng::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);	
	//手机号码
	phone_ = ms_->GetValueFromMapByKey("receiver_mobile",true);
	//联系电话
	tel_ =	ms_->GetValueFromMapByKey("receiver_phone",true);
	//城市地址编码
	string receiver_state,receiver_city,receiver_district,receiver_address;		
	receiver_state = ms_->GetValueFromMapByKey("receiver_state",true);
	receiver_city = ms_->GetValueFromMapByKey("receiver_city",true);
	receiver_district = ms_->GetValueFromMapByKey("receiver_district",true);
	receiver_address = ms_->GetValueFromMapByKey("receiver_address",true);
    //获取地址
	addressName_ = GetAddress(receiver_district);
	address_ = addressName_ + receiver_address;
    //邮编
	postcode_ = ms_->GetValueFromMapByKey("receiver_zip",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" +
		"state=添加地址\r\n";
	WriteIndexFile();
	return 0;
}

//pc修改地址
int shopping::AiQiYiShangCheng::PCWebModifyAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	
	ms_->SplitStringToMap(src_data,'&','=');
	//姓名
	name_ = ms_->GetValueFromMapByKey("receiver_name",true);	
	//手机号码
	phone_ = ms_->GetValueFromMapByKey("receiver_mobile",true);
	//联系电话
	tel_ =	ms_->GetValueFromMapByKey("receiver_phone",true);
	//城市地址编码
	string receiver_state,receiver_city,receiver_district,receiver_address;		
	receiver_state = ms_->GetValueFromMapByKey("receiver_state",true);
	receiver_city = ms_->GetValueFromMapByKey("receiver_city",true);
	receiver_district = ms_->GetValueFromMapByKey("receiver_district",true);
	receiver_address = ms_->GetValueFromMapByKey("receiver_address",true);
    //获取地址
	addressName_ = GetAddress(receiver_district);
	address_ = addressName_ + receiver_address;
    //邮编
	postcode_ = ms_->GetValueFromMapByKey("receiver_zip",true);
	
	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	output_text_ =  
		"nick=" + name_ + "\r\n" + 
		"oid=" + phone_ + "\r\n" + 
		"clientmsg=" + tel_ + "\r\n" + 
		"msg=" + address_ + "\r\n" + 
		"msgtype=" + postcode_ + "\r\n" +
		"state=修改地址\r\n";
	WriteIndexFile();
	return 0;
}