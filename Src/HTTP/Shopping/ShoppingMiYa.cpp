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
	//pc��¼��Ϣ
	if((http_session->m_Requestline.m_Host.Find("www.mia.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/instant/account/loginService")!=-1))
	{
		return kPCWebLogin;
	}
	//pc������ַ //�޸ĵ�ַ
	/************************************************************************/
	/*  IE save_address      Google	homeSaveAddress                         */
	/************************************************************************/
	if((http_session->m_Requestline.m_Host.Find("www.mia.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/instant/order/save_address")!=-1) ||
		(http_session->m_Requestline.m_URI.Find("/instant/order/homeSaveAddress")!=-1) ) 
	{
		return kPCWebAddAddress;
	}
	//android ��¼\����\�޸ĵ�ַ
	if((http_session->m_Requestline.m_Host.Find("mlog.mia.com")!=-1))
	{
		return kAndroidLogonAddress;
	}
	return 0;
}

int shopping::MiYa::Process(const char *packet,int action)
{
	//pc��¼��Ϣ
	if (action == kPCWebLogin)
	{
		return PCWebLogin(packet);
	}	
	//pc������ַ
	if (action == kPCWebAddAddress)
	{
		return PCWebAddAddress(packet);
	}
	//android ��¼\����\�޸ĵ�ַ
	if (action == kAndroidLogonAddress)
	{
		return AndroidLogonAddress(packet);
	}
	return 0;
}

//pc��¼��Ϣ
int shopping::MiYa::PCWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');

	string user,pass;
	user = ms_->GetValueFromMapByKey("username",true);
	//�ֻ���֤��
	pass = ms_->GetValueFromMapByKey("phonecode");
	
	if ( user.empty() || pass.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user,"����");
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=��¼\r\n";	
	WriteIndexFile();
	return 1;
}

//pc������ַ //�޸ĵ�ַ
int shopping::MiYa::PCWebAddAddress(const char *packet)
{	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,'&','=');
	string type; //��ǰ��¼��ַ��״̬
	//�û���
	name_ = ms_->GetValueFromMapByKey("truename",true);
	//�绰
	phone_ = ms_->GetValueFromMapByKey("mobile",true);
	//�̶��绰
	tel_ = ms_->GetValueFromMapByKey("phone",true);
	//��ַ
	address_ =  ms_->GetValueFromMapByKey("prov",true) + 
				ms_->GetValueFromMapByKey("city",true) +
				ms_->GetValueFromMapByKey("area",true) +
				ms_->GetValueFromMapByKey("town",true) + 
				ms_->GetValueFromMapByKey("address",true);
	//��ַid
	addressid_ = ms_->GetValueFromMapByKey("addressid",true);
	//��ǰ״̬
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
			"state=��ӵ�ַ\r\n";	
		WriteIndexFile();

    } 
    else if(!addressid_.empty() && type == "edit")
    {
		output_text_ = 
			"nick=" + name_ + "\r\n" + 
			"oid=" + phone_ + "\r\n" + 
			"clientmsg=" + tel_ + "\r\n" + 
			"msg=" + address_ + "\r\n" + 
			"state=�޸ĵ�ַ\r\n";	
		WriteIndexFile();

    }
	return 0;
}


//android ��¼\����\�޸ĵ�ַ
int shopping::MiYa::AndroidLogonAddress(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//ȥ��ǰ��� var
	src_data = src_data.substr(strlen("msg="));
	//�洢��¼ ��ַ ��Ϣ
	string loginInfo,addAddressInfo,modifyAddressInfo;
    //ʹ��json ��ȡ����
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
			//��¼
			loginInfo = current_param_id["/account/login/"].asString();	
			if (!loginInfo.empty())
			{
				Json::Reader readerlogin;
				Json::Value valuelogin;
				if (readerlogin.parse(loginInfo,valuelogin))
				{
					string user,pass,userend,passend;
					//������Ҫ�ı���
					int n = 0;
					//�û��� base64����
					user = valuelogin["name"].asString();
					userend = ms_->Base64Decode(user,n);
					userend = userend.substr(strlen("miababy"));
					//���� base64����
					pass = valuelogin["password"].asString();		
					passend = ms_->Base64Decode(pass,n);
					passend = passend.substr(strlen("miababy"));
					if ( userend.empty() || passend.empty())
					{
						return 0;
					}
					WriteVirtualIdentity(user,"����");		
					output_text_ = 
						"user=" + userend + "\r\n" + 
						"pass=" + passend + "\r\n" + 
						"state=��¼\r\n";	
					WriteIndexFile();
				}
			}
			//������ַ
			addAddressInfo = current_param_id["/address/add/"].asString();	
			if (!addAddressInfo.empty())
			{
				//ʹ��json ��ȡ����
				Json::Reader readerAddAddress;
				Json::Value valueaddAddress;
				if (readerAddAddress.parse(addAddressInfo,valueaddAddress))
				{		
					//�û���
					name_ = valueaddAddress["name"].asString();
					//�绰
					phone_ = valueaddAddress["mobile"].asString(); 
					//��ַ
					address_ =  valueaddAddress["prov"].asString() +
						valueaddAddress["city"].asString() +
						valueaddAddress["area"].asString() +
						valueaddAddress["street"].asString() +
						valueaddAddress["address"].asString();
					//��ַid
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
					"state=��ӵ�ַ\r\n";
				WriteIndexFile();
			}
			//�޸ĵ�ַ
			modifyAddressInfo = current_param_id["/address/modify/"].asString();	
			if (!modifyAddressInfo.empty())
			{
				//ʹ��json ��ȡ����
				Json::Reader readerModifyAddress;
				Json::Value valueModifyAddress;
				if (readerModifyAddress.parse(modifyAddressInfo,valueModifyAddress))
				{		
					//�û���
					name_ = valueModifyAddress["name"].asString();
					//�绰
					phone_ = valueModifyAddress["mobile"].asString(); 
					//��ַ
					address_ =  valueModifyAddress["prov"].asString() +
						valueModifyAddress["city"].asString() +
						valueModifyAddress["area"].asString() +
						valueModifyAddress["street"].asString() +
						valueModifyAddress["address"].asString();
					//��ַid
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
					"state=�޸ĵ�ַ\r\n";
				WriteIndexFile();
			}	
		}
		//header
		Json::Value header = value["header"];
 	}    
	return 1;

}	   
