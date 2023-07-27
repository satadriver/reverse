#include "StdAfx.h"
#include "Supply58.h"


supply::Supply58::Supply58(HttpSession *http_session, const string &pro_name):Supply(http_session,pro_name)
{
	app_ = "58同城";
}

supply::Supply58::~Supply58()
{

}

supply::Supply58::IsSupply58(HttpSession *http_session)
{
	//第一次发布招聘
	if( ( http_session->m_Requestline.m_Host.Find("qy.58.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/ajax_saveentall") != -1 ))	
	{
		return kWebPublicRecruitment;
	}
	//发布招聘信息
	if( ( http_session->m_Requestline.m_Host.Find("post.58.com") != -1) && 
		( http_session->m_Requestline.m_URI.Find("/zhaopin/265/") != -1 ))	
	{
		return kWebPublic;
	}
	return 0;
}

supply::Supply58::Process(const char *packet,int action)
{

	if (action == kWebPublicRecruitment)
	{
		return WebPublicRecruitment(packet);
	}
	if (action == kWebPublic)
	{
		return WebPublic(packet);
	}

	return 0;
}


int supply::Supply58::WebPublicRecruitment(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}

	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');

	//公司名称
	string enterpriseName = ms_->GetValueFromMapByKey("enterpriseName",true);
	//公司别称
	string enterpriseAlias = ms_->GetValueFromMapByKey("enterpriseAlias",true);
	//联系人
	string name = ms_->GetValueFromMapByKey("linkMan",true);
	//电话
	string phone = ms_->GetValueFromMapByKey("phone",true);
	//邮箱
	string email = ms_->GetValueFromMapByKey("email",true);   //可为空
	//公司简介
	string info = ms_->GetValueFromMapByKey("intro",true);
	//详细地址
	string address = ms_->GetValueFromMapByKey("address",true);
	//经度
	string longitude = ms_->GetValueFromMapByKey("longitude",true);
	//纬度
	string lat = ms_->GetValueFromMapByKey("latitude",true);
	if (enterpriseName.empty() || enterpriseAlias.empty() || name.empty() || phone.empty() || info.empty() || address.empty())
	{
		return 0;
	}
	if (email.empty())
	{
		email = "null";
	}
	output_text_ = 
		"user=" + 		name + "\r\n" +			            //联系人
		"servername=" + enterpriseName + "\r\n" + 			//公司名
		"appname=" + 	enterpriseAlias + "\r\n" + 			//公司别名
		"islogin=" + 	phone + "\r\n" + 					//电话
		"oid=" + 	  	email + "\r\n" + 					//邮箱
		"onick=" + 	  	info + "\r\n" + 					//公司简介
		"mid=" + 	  	address + "\r\n" + 					//地址
		"from=" +       longitude + "\r\n" +				//经度
		"to=" +         lat + "\r\n" +						//纬度
		"netmark=" +	app_ + "\r\n" + 					//来源
		"clientmsg=" +  platformWeb + "\r\n" +				//操作平台
		"state=招聘信息发布\r\n";							//状态
	WriteIndexFile();
	return 1;
}

int supply::Supply58::WebPublic(const char *packet)
{
	string src_data(packet);
	if (src_data.rfind("\r\n")!=string::npos)
	{
		src_data = src_data.substr(src_data.rfind("\r\n") + strlen("\r\n"));
	}
	if (src_data.find("xiaozhiwei")==string::npos)
	{
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string title = ms_->GetValueFromMapByKey("Title",true);				//职位名称
		string num = ms_->GetValueFromMapByKey("zhaopinrenshu",true);		//招聘人数
		string content = ms_->GetValueFromMapByKey("Content",true);			//兼职内容
		string name = ms_->GetValueFromMapByKey("goblianxiren",true);		//联系人
		string phone = ms_->GetValueFromMapByKey("Phone",true);				//手机号
		string email = ms_->GetValueFromMapByKey("Email",true);				//邮箱
		string address = ms_->GetValueFromMapByKey("address",true);			//地址
		if(title.empty() || num.empty() || content.empty() || name.empty() || phone.empty())
		{
			return 0;
		}
		if (email.empty())
		{
			email = "null";
		}
		if (address.empty())
		{
			address = "null";
		}
		output_text_ = 
			"user=" + 		name + "\r\n" +			    //联系人
			"islogin=" + 	phone + "\r\n" + 			//电话
			"oid=" + 	  	email + "\r\n" + 			//邮箱
			"fileurl=" + 	title + "\r\n" + 			//职位名称
			"msgtype=" + 	num  + "\r\n" +				//招聘人数
			"clientmac=" + 	content + "\r\n" + 			//兼职内容	
			"mid=" + 	  	address + "\r\n" + 			//地址
			"netmark=" +	app_ + "\r\n" + 			//来源
			"clientmsg=" +  platformWeb + "\r\n" +		//操作平台
			"state=兼职信息发布\r\n";					//状态
	WriteIndexFile();
	}
	else
	{
		src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
		ms_->SplitStringToMap(src_data,'&','=');
		string title = ms_->GetValueFromMapByKey("Title",true);				//职位名称
		string num = ms_->GetValueFromMapByKey("zhaopinrenshu",true);		//招聘人数
		string name = ms_->GetValueFromMapByKey("goblianxiren",true);       //联系人
		string phone = ms_->GetValueFromMapByKey("Phone",true);				//手机号
		string email = ms_->GetValueFromMapByKey("Email",true);             //邮箱
		if (title.empty() || num.empty() || name.empty() || phone.empty())
		{
			return 0;
		}
		if (email.empty())
		{
			email = "null";
		}
		
		output_text_ = 
			"user=" + 		name + "\r\n" +			            //联系人
			"islogin=" + 	phone + "\r\n" + 					//电话
			"oid=" + 	  	email + "\r\n" + 					//邮箱
			"fileurl=" + 	title + "\r\n" + 					//职位名称
			"msgtype=" + 	num  + "\r\n" +						//招聘人数
			"netmark=" +	app_ + "\r\n" + 					//来源
			"clientmsg=" +  platformWeb + "\r\n" +				//操作平台
			"state=全职招聘信息发布\r\n";						//状态
	WriteIndexFile();
	}
	return 1;
}
