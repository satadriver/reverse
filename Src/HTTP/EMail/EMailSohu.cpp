
#include "StdAfx.h"

#include "EMailSohu.h"
#include "../BoundaryParser.h"

email::EMailSohu::EMailSohu(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMailSohu::~EMailSohu()
{
	
}


int email::EMailSohu::IsEMailSohu(HttpSession *http_session)
{
//搜狐邮箱
	/************************************************************************/
	/* 登录形式 POST /fe/anoy/login HTTP/1.1   mail.sohu.com   2016-09-05   */
	/************************************************************************/
	if((http_session->m_Requestline.m_Host.Find("mail.sohu.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/fe/anoy/login")!=-1))
	{
		return kLogin;
	}
	/************************************************************************/
	/* 邮件形式 PUT /fe/mail HTTP/1.1   Host: mail.sohu.com   2016-08-13   */
	/************************************************************************/
	if((http_session->m_Requestline.m_Host.Find("mail.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/mail")!=-1)) 
	{
		return kPCWebSendMail;
	}
	/************************************************************************/
	/*附件形式	POST /fe/attach HTTP/1.1	Host: mail.sohu.com   2016-08-13 */
	/************************************************************************/
	if( (http_session->m_Requestline.m_Host.Find("mail.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/fe/attach")!=-1))
	{
		return kPCWebUploadAttachment;
	}
//搜狐vip邮箱
	/************************************************************************/
	/* 登录形式 POST /apiv2/login HTTP/1.1   passport.sohu.com  2016-09-05   */
	/************************************************************************/
	if((http_session->m_Requestline.m_Host.Find("passport.sohu.com")!=-1) &&  
		(http_session->m_Requestline.m_URI.Find("/apiv2/login")!=-1))
	{
		return kVipLogin;
	}
	/************************************************************************/
	/* 邮件形式 POST /bapp/6/mail HTTP/1.1  Host: vip.sohu.com   2016-08-13   */
	/************************************************************************/
	if((http_session->m_Requestline.m_Host.Find("vip.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bapp/6/mail")!=-1) 
		&& (http_session->m_Requestline.m_URI.Find("/bapp/6/mail/att/flash") == -1)) 
	{
		return kPCVipWebSendMail;
	}
	/************************************************************************/
	/*附件形式	POST POST /bapp/6/mail/att/flash	Host: vip.sohu.com  2016-08-13 */ 
	/************************************************************************/
	if( (http_session->m_Requestline.m_Host.Find("vip.sohu.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/bapp/6/mail/att/flash")!=-1))
	{
		return kPCVipWebUploadAttachment;
	}
	return 0;
}

int email::EMailSohu::Process(const char *packet,int action)
{
	//搜狐邮箱
	if (action == kLogin)
	{
		return PCWebLogin(packet);
	}
	if (action == kPCWebSendMail)
	{
		version_ = "电脑浏览器";
		return PCWebSendMail(packet);
	}
	if (action == kPCWebUploadAttachment)
	{
		return PCWebUploadAttachment(packet);
	}
	//搜狐vip邮箱
	if (action == kVipLogin)
	{
		return PCVipWebLogin(packet);
	}
	if (action == kPCVipWebSendMail)
	{
		version_ = "电脑浏览器";
		return PCVipWebSendMail(packet);
	}
	if (action == kPCVipWebUploadAttachment)
	{
		return PCVipWebUploadAttachment(packet);
	}
	return 0;
}

//登录信息
int email::EMailSohu::PCWebLogin(const char *packet)
{
	string src_data(packet);
	string user,pass;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	//判断是添加还是更改
	string keyId;
	if (reader.parse(src_data,value))
	{	
		user = value["username"].asString();
		//此处的密码使用md5 加密
		pass = value["password"].asString();	
	}	
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

//邮件
int email::EMailSohu::PCWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//用于区别 草稿 发送
	int send = -1;
	string timing;
	string attachmentName;
	//使用json 读取数据
	Json::Reader reader;
	Json::Value value;
	if (reader.parse(src_data,value))
	{
		//提取from
		from_ = value["from"].asString();
		//提取to JsonArray
		Json::Value arrayObj = value["to"];
		for (int h = 0;h <arrayObj.size();h++)
		{
			string out1 = arrayObj[h].asString();
			to_ = to_ + out1;
			to_ = to_ + ",";	
		}
		//提取主题
		title_ = value["subject"].asString();
		//提取内容
		content_ = value["content"].asString();
		//判断草稿 发送
		send = value["send"].asInt();
		//定时发送邮件
		timing = value["timing"].asString();
        //提取附件 名称
		
		Json::Value orderForm = value["attachments"];
		for (int i = 0;i <orderForm.size();i++)
		{				
			string out2 = orderForm[i]["name"].asString();
			attachmentName = attachmentName + out2;
			attachmentName = attachmentName + ",";	
		}
	}	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	if (send == 1)
	{
		WriteVirtualIdentity(from_,"发送邮件");
		ConstructSendMailOutputString();
		WriteIndexFile();
	}
// 	else if (send == 0)
// 	{
// 		WriteVirtualIdentity(from_,"保存草稿");
// 		ConstructSendMailOutputString();
// 		WriteIndexFile();
// 	}
	return 1;
}

//附件
int email::EMailSohu::PCWebUploadAttachment(const char *packet)
{	
	//目前获取不到
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	string user = ms_->GetValueFromMapByKey("email", true);

	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"file\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}


//vip登录
int email::EMailSohu::PCVipWebLogin(const char *packet)
{
	string src_data(packet);
	ms_->SplitStringToMap(src_data,'&','=');
	string user,pass;
	user = ms_->GetValueFromMapByKey("userid",true);
	pass = ms_->GetValueFromMapByKey("password");
	
	if ( user.empty() || pass.empty())
	{
		return 0;
	}
	WriteVirtualIdentity(user,"上线");
	
	output_text_ = 
		"user=" + user + "\r\n" + 
		"pass=" + pass + "\r\n" + 
		"state=vip登录\r\n";	
	WriteIndexFile();
	return 1;
}				

//vip邮件
int email::EMailSohu::PCVipWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	//存入map
	ms_->SplitStringToMap(src_data,'&','=');   
    //取值
	from_ = ms_->GetValueFromMapByKey("from", true);	 	
	to_ = ms_->GetValueFromMapByKey("to", true);	
	title_ = ms_->GetValueFromMapByKey("subject", true); 	
	content_ = ms_->GetValueFromMapByKey("text", true);   	
	//用于区别 草稿 发送
	string is_send,sot_time;
	is_send = ms_->GetValueFromMapByKey("is_send", true);
	//定时发送时间
	sot_time = ms_->GetValueFromMapByKey("sot_time", true);

	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	if (is_send == "1")
	{
		WriteVirtualIdentity(from_,"发送邮件");
		ConstructSendMailOutputString();
		WriteIndexFile();
	}
// 	else if (send == 0)
// 	{
// 		WriteVirtualIdentity(from_,"保存草稿");
// 		ConstructSendMailOutputString();
// 		WriteIndexFile();
// 	}
	return 1;
}

//vip附件
int email::EMailSohu::PCVipWebUploadAttachment(const char *packet)
{	
	//从cookie中提取数据
	string cookie = http_session_->m_Requestline.m_Cookie.GetBuffer(0); 
	string userTemp = ms_->GetValueBetweenBeginEnd(cookie,"VIPID",";");
    from_ = ms_->GetValueBetweenBeginEnd(userTemp,"|","|");
    //附件部分
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"upload_file\"");          
	bv = bp.GetBoundaryValue(key);	
	if (bv == NULL || key.empty())
	{
		return 0;
	}
	attachment_ = ms_->GetValueBetweenBeginEnd(key, "filename=\"", "\"", true);	
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}
