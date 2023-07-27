
#include "StdAfx.h"
#include "ShoppingYinYueShangCheng.h"

shopping::YinYueShangCheng::YinYueShangCheng(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::YinYueShangCheng::~YinYueShangCheng()
{
	
}

int shopping::YinYueShangCheng::IsYinYueShangCheng(HttpSession *http_session)
{
	
	//pc新增地址 //pc修改地址
	if((http_session->m_Requestline.m_Host.Find("trade.yinyuetai.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/create-address")!=-1))
	{
		return kPCWebAddAddress;
	}
	return 0;
}

int shopping::YinYueShangCheng::Process(const char *packet,int action)
{	
    //pc新增地址 //pc修改地址
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	return 0;
}

//pc新增地址 //pc修改地址
int shopping::YinYueShangCheng::PCWebAddAddress(const char *packet)
{	
	/************************************************************************/
	/* u_inf=51421065.LoveMusic897.xiaoxiaolong989@126.com.normal_user.http://img3.yytcdn.com/uploads/default/persons/header999_100x100.jpg;                                                                     */
	/************************************************************************/
	//从cookie 中获得user_
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	cookie = ms_->UTF8ToGBK(UrlDecode(cookie));
	ms_->SplitStringToMap(cookie, ';','=');
	//u_inf 中包含用户账户 头像url
	string u_inf = ms_->GetValueFromMapByKey("u_inf");
	//数据部分
	/************************************************************************/
	/* id=309657&name=鼬&province=13;;山东&city=1000;;济南市&country=4277;;高新区&address=78号&
	code=252004&tel=18550338765                                                             */
	/************************************************************************/
    //数据部分
	string src_data(packet);
	string src_dataEnd;
	//存在 post 信息部分
	if (src_data.find("\r\nid=") != string::npos )
	{
		src_data = src_data.substr(src_data.find("\r\nid=") + strlen("\r\n"));
	}
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//姓名
	name_ = ms_->GetValueFromMapByKey("name",true);	
	//手机
	phone_ = ms_->GetValueFromMapByKey("tel",true);

	string temp1,temp2;
	string province,city,country;
	temp1 = ms_->GetValueFromMapByKey("province",true);
	province = temp1.substr(temp1.find_last_of(";") + strlen(";"));

	temp1 = ms_->GetValueFromMapByKey("city",true);
	city = temp1.substr(temp1.find_last_of(";") + strlen(";"));

	temp1 = ms_->GetValueFromMapByKey("country",true);
	country = temp1.substr(temp1.find_last_of(";") + strlen(";"));

	//获取地址
	address_ = province + city + country + ms_->GetValueFromMapByKey("address",true);
    //邮编
	postcode_ = ms_->GetValueFromMapByKey("code",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
	//判读修改 新增地址
	addressid_ = ms_->GetValueFromMapByKey("id",true);
	if (!addressid_.empty())
	{
		output_text_ =  
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n"  
			"state=修改地址\r\n";
		WriteIndexFile();
	} 
	else
	{
		output_text_ =  
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"msgtype=" + postcode_ + "\r\n" 
			"state=添加地址\r\n";
		WriteIndexFile();
	}
	return 0;
}
