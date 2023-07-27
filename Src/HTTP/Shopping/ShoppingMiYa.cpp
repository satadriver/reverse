#include "StdAfx.h"
#include "ShoppingMiYa.h"

shopping::MiYa::MiYa(HttpSession *http_session, const string &pro_name) : Shopping(http_session, pro_name)
{
	
}

shopping::MiYa::~MiYa()
{
	
}

int shopping::MiYa::IsMiYa(HttpSession *http_session)
{
	//pc登录信息
	if((http_session->m_Requestline.m_Host.Find("www.mia.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/instant/account/loginService")!=-1))
	{
		return kPCWebLogin;
	}
	//pc新增地址 //修改地址
	/************************************************************************/
	/*  IE save_address      Google	homeSaveAddress                         */
	/************************************************************************/
	if((http_session->m_Requestline.m_Host.Find("www.mia.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/instant/order/save_address")!=-1) ||
		(http_session->m_Requestline.m_URI.Find("/instant/order/homeSaveAddress")!=-1) ) 
	{
		return kPCWebAddAddress;
	}
	//android 登录\新增\修改地址
	if((http_session->m_Requestline.m_Host.Find("mlog.mia.com")!=-1))
	{
		return kAndroidLogonAddress;
	}
	return 0;
}

int shopping::MiYa::Process(const char *packet,int action)
{
	//pc登录信息
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc新增地址
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//android 登录\新增\修改地址
	if (action == kAndroidLogonAddress)
	{
		return AndroidLogonAddress(packet);
	}
	return 0;
}

//pc登录信息
int shopping::MiYa::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("username",true);
	//手机验证码
	pass = ms_->GetValueFromMapByKey("phonecode");
	
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

//pc新增地址 //修改地址
int shopping::MiYa::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string type; //当前记录地址的状态
	//用户名
	name_ = ms_->GetValueFromMapByKey("truename",true);
	//电话
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//固定电话
	tel_ = ms_->GetValueFromMapByKey("phone",true);
	//地址
	address_ =  ms_->GetValueFromMapByKey("prov",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("town",true) + 
				ms_->GetValueFromMapByKey("address",true);
	//地址id
	addressid_ = ms_->GetValueFromMapByKey("addressid",true);
	//当前状态
	type = ms_->GetValueFromMapByKey("type",true);

	if ( name_.empty() || phone_.empty() || address_.empty())
	{
		return 0;
	}
    if (addressid_.empty() && type == "add")
    {
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=添加地址\r\n";	
		WriteIndexFile();

    } 
    else if(!addressid_.empty() && type == "edit")
    {
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=修改地址\r\n";	
		WriteIndexFile();

    }
	return 0;
}


//android 登录\新增\修改地址
int shopping::MiYa::AndroidLogonAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//去除前面的 var
	src_data = src_data.substr(strlen("msg="));
	//存储登录 地址 信息
	string loginInfo,addAddressInfo,modifyAddressInfo;
    //使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{	
		//body
		Json::Value body = value["body"];
		Json::Value activities = body["activities"];

		for (int i = 0;i <activities.size();i++)
		{
			Json::Value current_param_id = activities[i]["current_param_id"];
			//登录
			loginInfo = current_param_id["/account/login/"].asString();	
			if (!loginInfo.empty())
			{
				Json::Reader readerlogin;
				Json::Value valuelogin;
				if (readerlogin.parse(loginInfo,valuelogin))
				{
					string user,pass,userend,passend;
					//解码需要的变量
					int n = 0;
					//用户名 base64加密
					user = valuelogin["name"].asString();
					userend = ms_->Base64Decode(user,n);
					userend = userend.substr(strlen("miababy"));
					//密码 base64加密
					pass = valuelogin["password"].asString();		
					passend = ms_->Base64Decode(pass,n);
					passend = passend.substr(strlen("miababy"));
					if ( userend.empty() || passend.empty())
					{
						return 0;
					}
					WriteVirtualIdentity(user,"上线");		
					output_text_ = 
						"user=" + userend + "\r\n" + 
						"pass=" + passend + "\r\n" + 
						"state=登录\r\n";	
					WriteIndexFile();
				}
			}
			//新增地址
			addAddressInfo = current_param_id["/address/add/"].asString();	
			if (!addAddressInfo.empty())
			{
				//使用json 读取数据
				Json::Reader readerAddAddress;
				Json::Value valueaddAddress;
				if (readerAddAddress.parse(addAddressInfo,valueaddAddress))
				{		
					//用户名
					name_ = valueaddAddress["name"].asString();
					//电话
					phone_ = valueaddAddress["mobile"].asString(); 
					//地址
					address_ =  valueaddAddress["prov"].asString() +
						valueaddAddress["city"].asString() +
						valueaddAddress["area"].asString() +
						valueaddAddress["street"].asString() +
						valueaddAddress["address"].asString();
					//地址id
					addressid_ = valueaddAddress["id"].asString();
				}
				if ( name_.empty() || phone_.empty() || address_.empty())
				{
					return 0;
				}
				output_text_ = 
					"nick=" + name_ + "\r\n" + 
					"oid=" + phone_ + "\r\n" +  
					"msg=" + address_ + "\r\n" +
					"state=添加地址\r\n";
				WriteIndexFile();
			}
			//修改地址
			modifyAddressInfo = current_param_id["/address/modify/"].asString();	
			if (!modifyAddressInfo.empty())
			{
				//使用json 读取数据
				Json::Reader readerModifyAddress;
				Json::Value valueModifyAddress;
				if (readerModifyAddress.parse(modifyAddressInfo,valueModifyAddress))
				{		
					//用户名
					name_ = valueModifyAddress["name"].asString();
					//电话
					phone_ = valueModifyAddress["mobile"].asString(); 
					//地址
					address_ =  valueModifyAddress["prov"].asString() +
						valueModifyAddress["city"].asString() +
						valueModifyAddress["area"].asString() +
						valueModifyAddress["street"].asString() +
						valueModifyAddress["address"].asString();
					//地址id
					addressid_ = valueModifyAddress["id"].asString();	
				}
				if ( name_.empty() || phone_.empty() || address_.empty())
				{
					return 0;
				}
				output_text_ = 
					"nick=" + name_ + "\r\n" + 
					"oid=" + phone_ + "\r\n" +  
					"msg=" + address_ + "\r\n" +
					"state=修改地址\r\n";
				WriteIndexFile();
			}	
		}
		//header
		Json::Value header = value["header"];
 	}    
	return 1;

}	   
