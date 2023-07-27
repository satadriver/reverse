
#include "StdAfx.h"

#include "EMail163.h"

map<string, UploadFileBufferList * > email::EMail163::file_buffer_ = map<string, UploadFileBufferList * >();
email::EMail163::EMail163(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail163::~EMail163()
{
	
}


int email::EMail163::IsEMail163(HttpSession *http_session)
{
//手机浏览器
	if( (http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/m/s")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose") != -1))
	{
		return kMobileSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/m/compose/upload.jsp")!=-1))
	{
		return kMboileUploadAttachment;
	}
//163 企业版本
	if((http_session->m_Requestline.m_Host.Find("mail.ym.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy3/compose/compose.jsp")!=-1))
	{
		return kPCWebYmSendMail;
	}
	if( (http_session->m_Requestline.m_Host.Find("mail.ym.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy3/compose/flashUpload.jsp")!=-1))
	{
		return kPCWebUpYmloadAttachment;
	}
 //vip版本 
	//1.English 版 邮件
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jyen/compose/compose.jsp")!=-1))
	{
		return kPCWebEnVipSendMail;
	}
	if( (http_session->m_Requestline.m_Host.Find("webmail.vip.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jyen/compose/autoUploadForH.jsp")!=-1))
	{
		return kPCWebEnUpViploadAttachment;
	}
	//2.管家5.0 版 邮件
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
	{
		return kPCWeb5VipSendMail;
	}
	//t1hz.vip.163.com 
	if( (http_session->m_Requestline.m_Host.Find(".vip.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1))
	{
		return kPCWeb5UpViploadAttachment;
	}
	//3.极致6.0 版
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
	{
		return kPCWeb6VipSendMail;
	}
	//t1hz.vip.163.com 
	if( (http_session->m_Requestline.m_Host.Find(".vip.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1))
	{
		return kPCWeb6UpViploadAttachment;
	}
	//4.默认 版  目前 同 极致6.0 版 
// 	if((http_session->m_Requestline.m_Host.Find("webmail.vip.163.com")!=-1) && 
// 		(http_session->m_Requestline.m_URI.Find("/js6")!=-1) &&
// 		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
// 	{
// 		return kPCWebVipSendMail;
// 	}
// 	if( (http_session->m_Requestline.m_Host.Find(".vip.163.com")!=-1) && 
// 		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1))
// 	{
// 		return kPCWebUpViploadAttachment;
// 	}
//普通版本
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js6/h")!=-1))
	{
		return 0;
	}
	
	if((http_session->m_Requestline.m_Host.Find("entry.mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/corEMail/fcg/ntesdoor2")!=-1))
	{
		return kLogin;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("compose/compose")!=-1))
	{
		return kPCWebJY6SendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("compose/compose")!=-1))
	{
		return kPCWebJY5SendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose&")!=-1))
	{
		return kPCWeb6SendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose&")!=-1))
	{
		return kPCWeb5SendMail;
	}
	
	if((http_session->m_Requestline.m_Host.Find(".mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
	{
		return kMWebSendMail;
	}	
	//附件
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("ver=js")!=-1))
	{
		return kPCWebJSUploadAttachment;
	}	
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("ver=jy")!=-1))
	{
		return kPCWebJYUploadAttachment;
	}	
	if((http_session->m_Requestline.m_Host.Find("mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js6/compose/upload.jsp")!=-1) )
	{
		return kPCWebJSUploadAttachment2;
	}
	// modify by cxl
	/*
	if( (http_session->m_Requestline.m_Host.Find(".")!=-1))
	{
	return kReassembleFileBuffer;
	}
	*/	
	return 0;
}

int email::EMail163::Process(const char *packet,int action)
{
	switch (action)
	{
//普通版本 -邮件
	case kPCWebJY6SendMail :
		version_ = "(电脑浏览器简约6.0版)";
		return PCWebJY6SendMail(packet);
	case kPCWeb6SendMail :
		version_ = "(电脑浏览器6.0版 默认版本)";
		return PCWeb6SendMail(packet);
	case kPCWebJY5SendMail :
		version_ = "(电脑浏览器简约5.0版)";
		return PCWebJY5SendMail(packet);
	case kPCWeb5SendMail :
		version_ = "(电脑浏览器5.0版)";
		return PCWeb5SendMail(packet);
	case kMWebSendMail :
		version_ = "(手机浏览器)";
		return MWebSendMail(packet);
 //普通版本 -附件
	case kPCWebJSUploadAttachment :
		return PCWebJSUploadAttachment(packet);
	case kPCWebJSUploadAttachment2 :
		return PCWebJSUploadAttachment2(packet);
 //vip版本 -邮件
	case kPCWebEnVipSendMail:		
		version_ = "(电脑浏览器English 版)";
		return PCWebEnVipSendMail(packet);
	case kPCWeb5VipSendMail:	
		version_ = "(电脑浏览器管家5.0版)";
		return PCWeb5VipSendMail(packet);
	case kPCWeb6VipSendMail:	
		version_ = "(电脑浏览器极致6.0版)";
		return PCWeb6VipSendMail(packet);
	case kPCWebVipSendMail:			
		version_ = "(电脑浏览器默认版)";
		return PCWebVipSendMail(packet);
 //vip版本 -附件
	case kPCWebEnUpViploadAttachment:
		version_ = "(电脑浏览器English 版)";
		return PCWebEnUpViploadAttachment(packet);
	case kPCWeb5UpViploadAttachment:
		version_ = "(电脑浏览器管家5.0版)";
        return PCWeb5UpViploadAttachment(packet);
	case kPCWeb6UpViploadAttachment:
		version_ = "(电脑浏览器极致6.0版)";
		return PCWeb6UpViploadAttachment(packet);
	case kPCWebUpViploadAttachment:
		version_ = "(电脑浏览器默认版)";
		return PCWebUpViploadAttachment(packet);
//ym企业版本
	case kPCWebYmSendMail :						
		version_ = "(电脑浏览器简约6.0版)";
		return PCWebYmSendMail(packet);
	case kPCWebUpYmloadAttachment :            
		return PCWebJSYmUploadAttachment(packet);	
	// 	case kPCWebJYUploadAttachment :
	// //		return PCWebJYUploadAttachment(packet);
	// 		break;
	// 	case kReassembleFileBuffer:
	// 		ReassembleFileBuffer();
	// 		break;
//手机浏览器
	case kMobileSendMail :						
		version_ = "(手机浏览器)";
		return MobileSendMail(packet);
	case kMboileUploadAttachment :            
		return MboileUploadAttachment(packet);
	default:
		break;
	}
	return 0; 
}


//手机浏览器
int email::EMail163::MobileSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	
	ms_->SplitStringToMap(src_data,',',':',"\"");
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"<string name=\"account\">","</string>");
	from_ = ms_->GetValueBetweenBeginEnd(from_,"&lt;","&gt;");
    to_ = ms_->GetValueBetweenBeginEnd(src_data,"<array name=\"to\">","</array>");
	ms_->Replace(to_,"<string>","");
	ms_->Replace(to_,"</string>",","); 
    title_ = ms_->GetValueBetweenBeginEnd(src_data,"<string name=\"subject\">","</string>");
    content_ = ms_->GetValueBetweenBeginEnd(src_data,"<string name=\"content\">","</string>");
	ms_->Replace(content_,"\r","");
	ms_->Replace(content_,"\n","");
	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}	
	WriteVirtualIdentity(from_,"发送邮件");	
	ConstructSendMailOutputString();
	WriteIndexFile();	
	return 1;
}

int email::EMail163::MboileUploadAttachment(const char *packet)
{
	//从cookie 获得user
	StringManipulation sm;	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	sm.SplitStringToMap(cookie, ';', '=');

	from_ = sm.GetValueFromMapByKey("MAIL_PINFO");
	from_ = from_.substr(0,from_.find("|"));
    //得到文件名
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"Filedata\"");          
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

//1.English 版 vip邮件
int email::EMail163::PCWebEnVipSendMail(const char *packet)
{
	//格式为字符串 格式
	string src_data(packet);	
	src_data = ms_->UrlDecode(src_data);
	//存入map
	ms_->SplitStringToMap(src_data,'&','=');   
    //取值
	from_ = ms_->GetValueFromMapByKey("account", true);	 
	to_ = ms_->GetValueFromMapByKey("to", true);
	title_ = ms_->GetValueFromMapByKey("subject", true); 
	content_ = ms_->GetValueFromMapByKey("content", true);
    //判断
	if (from_.empty() || to_.empty() /*|| title_.empty()*/)
	{
	 	return 0;
	}
	//写入文件
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}
//2.管家5.0 版 vip邮件
int email::EMail163::PCWeb5VipSendMail(const char *packet)
{
	//格式为xml 格式
	string src_data(packet);	
	src_data = ms_->UrlDecode(src_data);
	//替换转义字符 < => &lt,> =>&gt,
	if ((src_data.find("&lt") != string::npos) && (src_data.find("&gt") != string::npos))
	{
		ms_->Replace(src_data,"&lt;","<");
		ms_->Replace(src_data,"&gt;",">");
		ms_->Replace(src_data,"&quot;","\"");
		ms_->Replace(src_data,"&amp;","&");
		ms_->Replace(src_data,"&nbsp;"," ");
	}

	//截取拆分
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"<array name=\"to\">","</array>",true);
		string strEnd;
		string::size_type iPos;
		string::size_type iTos;
		while((iPos = to_.find("<string>")) != string::npos && (iTos = to_.find("</string>")) != string::npos)
		{
			string strFirst = to_.substr(iPos +strlen("<string>"),iTos - strlen("<string>"));
			if (strFirst.find("<") != string::npos && strFirst.find(">") != string::npos)
			{
				strFirst = ms_->GetValueBetweenBeginEnd(strFirst,"<",">",true);
			}	
			strEnd += strFirst;
			strEnd += ";";
			to_ = to_.substr(iTos + strlen("</string>"),to_.length() - iTos + strlen("</string>"));
		}
				to_ = strEnd;

	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true); 
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);
	//判断
	if (from_.empty() || to_.empty() /*|| title_.empty()*/)
	{
		return 0;
	}
	//写入文件
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}
//3.极致6.0 版 vip邮件
int email::EMail163::PCWeb6VipSendMail(const char *packet)
{
	//格式为xml 格式
	string src_data(packet);	
	src_data = ms_->UrlDecode(src_data);
	//替换转义字符 < => &lt,> =>&gt,
	if ((src_data.find("&lt") != string::npos) && (src_data.find("&gt") != string::npos))
	{
		ms_->Replace(src_data,"&lt;","<");
		ms_->Replace(src_data,"&gt;",">");
		ms_->Replace(src_data,"&quot;","\"");
		ms_->Replace(src_data,"&amp;","&");
		ms_->Replace(src_data,"&nbsp;"," ");
	}
	//截取拆分
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true); 
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"<array name=\"to\">","</array>",true);
		string strEnd;
		string::size_type iPos;
		string::size_type iTos;
		while((iPos = to_.find("<string>")) != string::npos && (iTos = to_.find("</string>")) != string::npos)
		{
			string strFirst = to_.substr(iPos +strlen("<string>"),iTos - strlen("<string>"));
			if (strFirst.find("<") != string::npos && strFirst.find(">") != string::npos)
			{
				strFirst = ms_->GetValueBetweenBeginEnd(strFirst,"<",">",true);
			}	
			strEnd += strFirst;
			strEnd += ";";
			to_ = to_.substr(iTos + strlen("</string>"),to_.length() - iTos + strlen("</string>"));
		}
		to_ = strEnd;

	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true); 		
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);
	//判断
	if (from_.empty() || to_.empty() /*|| title_.empty()*/)
	{
		return 0;
	}
	//写入文件
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}
//4.默认 版 vip邮件	
int email::EMail163::PCWebVipSendMail(const char *packet)
{
	//默认版本 目前 同 极致6.0 版 
	return 1;
}		


//vip附件1.English 版 vip附件
int email::EMail163::PCWebEnUpViploadAttachment(const char *packet)
{
	//从cookie 获得user
	StringManipulation sm;	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	sm.SplitStringToMap(cookie, ';', '=');
	from_ = sm.GetValueFromMapByKey("mail_uid");
    //得到文件名
 	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
 	BoundaryParser bp;
 	bp.Parse(packet, http_session_->m_datalen, boundary); 
 	BoundaryValue *bv = NULL;
 	string key;
 	key = bp.GetBoundaryKey("name=\"localAttach\"");          
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
//2.管家5.0 版 vip附件
int email::EMail163::PCWeb5UpViploadAttachment(const char *packet)
{
	//从uri 中获取user
	int retval = 0;
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);

	sm.Replace(uri, "?", "&");
	sm.SplitStringToMap(uri, '&', '=');
	string uid = sm.GetValueFromMapByKey("uid");
	string sid = sm.GetValueFromMapByKey("sid");
	if (uid.empty())
	{
		return retval;
	}	
	//request->ProcessAuthentication(&auth_info_);	
	string attachment_name = sm.UTF8ToGBK(sm.UrlDecode(http_session_->m_Requestline.m_emailAttName.GetBuffer(0)));
	int file_size = sm.ToInt(http_session_->m_Requestline.m_emailAttSize.GetBuffer(0));	
	int offset = sm.ToInt(http_session_->m_Requestline.m_emailAttOffset.GetBuffer(0));
	int length = sm.ToInt(http_session_->m_Requestline.m_emailAttLegnth.GetBuffer(0));	
	if (file_size == length)
	{		
		string real_file_name = WriteTempRealFile(packet, attachment_name.c_str(), file_size);	
		stringstream ss;
		ss<<"user="<<uid<<"\r\n"
			<<"onick="<<attachment_name<<"\r\n"
			<<"state=上传附件(JS6)"<<"\r\n";
		string ot = ss.str();
		output_text_ = ot;
		WriteIndexFile(real_file_name);
		retval = 1;
	}
	else
	{	
		string file_key = sid;			
		UploadFileBuffer *ufb = new UploadFileBuffer();
		ufb->offset = offset;
		ufb->length = length;
		ufb->data = new char[length + 1];
		memset(ufb->data,0,length + 1);
		if (length != http_session_->m_datalen)
		{
			//todolog
			WriteLog(HTTPPRO, "163 upload att len err\r\n");
			//	log->Output("");
		}
		memcpy(ufb->data, packet, http_session_->m_datalen);
		map<string, UploadFileBufferList *>::iterator iter = file_buffer_.find(file_key);
		if (iter == file_buffer_.end())
		{
			UploadFileBufferList *ufbl = new UploadFileBufferList();
			//			request->ProcessAuthentication(&ufbl->authInfo);
			time_t timestamp = 0;
			ufb->offset = 0;
			ufbl->lastTime = ::time(&timestamp);
			ufbl->complete = false;
			ufbl->uid = uid;
			ufbl->fileName = attachment_name;
			ufbl->fileSize = file_size;
			ufbl->recieve_size += length; // add by cxl
			ufbl->buffers.push_back(ufb);
			file_buffer_.insert(make_pair(file_key, ufbl));
		}
		else
		{
			time_t timestamp = 0;
			iter->second->lastTime = ::time(&timestamp);
			iter->second->recieve_size += length; // add by cxl
			iter->second->buffers.push_back(ufb);
		}		
	}
	// begin add by cxl 
	map<string, UploadFileBufferList *>::iterator itFile = file_buffer_.begin();
	
	while (itFile != file_buffer_.end())
	{
		if (itFile->second->recieve_size == itFile->second->fileSize)
		{
			itFile->second->complete = true;
			ReassembleFileBuffer(itFile++);
		}
		else
		{
			itFile++;
		}
	}
	// end add	
	return 1;
}
//3.极致6.0 版 vip附件
int email::EMail163::PCWeb6UpViploadAttachment(const char *packet)
{
	//从uri 中获取user
	int retval = 0;
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	
	sm.Replace(uri, "?", "&");
	sm.SplitStringToMap(uri, '&', '=');
	string uid = sm.GetValueFromMapByKey("uid");
	string sid = sm.GetValueFromMapByKey("sid");
	if (uid.empty())
	{
		return retval;
	}	
	//request->ProcessAuthentication(&auth_info_);	
	string attachment_name = sm.UTF8ToGBK(sm.UrlDecode(http_session_->m_Requestline.m_emailAttName.GetBuffer(0)));
	int file_size = sm.ToInt(http_session_->m_Requestline.m_emailAttSize.GetBuffer(0));	
	int offset = sm.ToInt(http_session_->m_Requestline.m_emailAttOffset.GetBuffer(0));
	int length = sm.ToInt(http_session_->m_Requestline.m_emailAttLegnth.GetBuffer(0));	
	if (file_size == length)
	{		
		string real_file_name = WriteTempRealFile(packet, attachment_name.c_str(), file_size);	
		stringstream ss;
		ss<<"user="<<uid<<"\r\n"
			<<"onick="<<attachment_name<<"\r\n"
			<<"state=上传附件(JS6)"<<"\r\n";
		string ot = ss.str();
		output_text_ = ot;
		WriteIndexFile(real_file_name);
		retval = 1;
	}
	else
	{	
		string file_key = sid;			
		UploadFileBuffer *ufb = new UploadFileBuffer();
		ufb->offset = offset;
		ufb->length = length;
		ufb->data = new char[length + 1];
		memset(ufb->data,0,length + 1);
		if (length != http_session_->m_datalen)
		{
			//todolog
			WriteLog(HTTPPRO, "163 upload att len err\r\n");
			//	log->Output("");
		}
		memcpy(ufb->data, packet, http_session_->m_datalen);
		map<string, UploadFileBufferList *>::iterator iter = file_buffer_.find(file_key);
		if (iter == file_buffer_.end())
		{
			UploadFileBufferList *ufbl = new UploadFileBufferList();
			//			request->ProcessAuthentication(&ufbl->authInfo);
			time_t timestamp = 0;
			ufb->offset = 0;
			ufbl->lastTime = ::time(&timestamp);
			ufbl->complete = false;
			ufbl->uid = uid;
			ufbl->fileName = attachment_name;
			ufbl->fileSize = file_size;
			ufbl->recieve_size += length; // add by cxl
			ufbl->buffers.push_back(ufb);
			file_buffer_.insert(make_pair(file_key, ufbl));
		}
		else
		{
			time_t timestamp = 0;
			iter->second->lastTime = ::time(&timestamp);
			iter->second->recieve_size += length; // add by cxl
			iter->second->buffers.push_back(ufb);
		}		
	}
	// begin add by cxl 
	map<string, UploadFileBufferList *>::iterator itFile = file_buffer_.begin();
	
	while (itFile != file_buffer_.end())
	{
		if (itFile->second->recieve_size == itFile->second->fileSize)
		{
			itFile->second->complete = true;
			ReassembleFileBuffer(itFile++);
		}
		else
		{
			itFile++;
		}
	}
	// end add	
	return 1;
}  
//4.默认 版 vip附件
int email::EMail163::PCWebUpViploadAttachment(const char *packet)
{
	//默认版本 目前 同 极致6.0 版 
	return 1;
}  


//ym企业163 邮件
int email::EMail163::PCWebYmSendMail(const char *packet)
{
	string src_data(packet);
	//转化
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	ms_->SplitStringToMap(src_data,'&','=');
	//替换转义字符 < => &lt,> =>&gt,
	if ((src_data.find("&lt") != string::npos) && (src_data.find("&gt") != string::npos))
	{
		ms_->Replace(src_data,"&lt;","<");
		ms_->Replace(src_data,"&gt;",">");
		ms_->Replace(src_data,"&quot;","\"");
		ms_->Replace(src_data,"&amp;","&");
		ms_->Replace(src_data,"&nbsp;"," ");
	}
    //未做格式化处理	
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	
	if (from_.empty() || to_.empty() || title_.empty())
	{
		return 0;
	}
	//写入日志
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");	
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}	
	

//ym企业163 附件	
int email::EMail163::PCWebJSYmUploadAttachment(const char *packet)
{
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0); 
	ms_->Replace(uri, "?", "&");
	ms_->SplitStringToMap(uri, "&", "=");
	string user = ms_->GetValueFromMapByKey("email", true);
	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary); 
	BoundaryValue *bv = NULL;
	string key;
	key = bp.GetBoundaryKey("name=\"Filedata\"");          
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


int email::EMail163::PCWebJY6SendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,',',':',"\"");
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	if (from_.empty() || to_.empty() /*|| title_.empty()*/)
	{
		return 0;
	}	
	/*
	string temp_content = ms_->GetValueBetweenBeginEnd(content_,"<div>","</div>");
	if (!temp_content.empty())
	{
	content_ = temp_content;
	}
	*/
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail163::PCWeb6SendMail(const char *packet)
{
	string src_data = ms_->UrlDecode(packet);
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"to\"><string>","</string>",true);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);
	
	if (from_.empty() || to_.empty() /*|| title_.empty() || content_.empty()*/)
	{
		return 0;
	}	
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail163::PCWebJY5SendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data,',',':',"\"");	
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail163::PCWeb5SendMail(const char *packet)
{
	string src_data = ms_->UrlDecode(packet);
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"to\"><string>","</string>",true);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);	
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail163::MWebSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);	
	ms_->SplitStringToMap(src_data,',',':',"\"");
	from_ = ms_->GetValueFromMapByKey("account",true);
	to_ = ms_->GetValueFromMapByKey("to",true);
	title_ = ms_->GetValueFromMapByKey("subject",true);
	content_ = ms_->GetValueFromMapByKey("content",true);
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}	
	WriteCaseInfo(from_);
	WriteVirtualIdentity(from_,"发送邮件");	
	ConstructSendMailOutputString();
	WriteIndexFile();	
	return 1;
}

int email::EMail163::PCWebJSUploadAttachment(const char *packet)
{
	int retval = 0;
	StringManipulation sm;	
	string uri = http_session_->m_Requestline.m_URI.GetBuffer(0);
	sm.Replace(uri, "?", "&");
	sm.SplitStringToMap(uri, '&', '=');
	string uid = sm.GetValueFromMapByKey("uid");
	string sid = sm.GetValueFromMapByKey("sid");
	if (uid.empty())
	{
		return retval;
	}	
	//request->ProcessAuthentication(&auth_info_);	
	string attachment_name = sm.UTF8ToGBK(sm.UrlDecode(http_session_->m_Requestline.m_emailAttName.GetBuffer(0)));
	int file_size = sm.ToInt(http_session_->m_Requestline.m_emailAttSize.GetBuffer(0));	
	int offset = sm.ToInt(http_session_->m_Requestline.m_emailAttOffset.GetBuffer(0));
	int length = sm.ToInt(http_session_->m_Requestline.m_emailAttLegnth.GetBuffer(0));	
	if (file_size == length)
	{		
		string real_file_name = WriteTempRealFile(packet, attachment_name.c_str(), file_size);	
		stringstream ss;
		ss<<"user="<<uid<<"\r\n"
			<<"onick="<<attachment_name<<"\r\n"
			<<"state=上传附件(JS6)"<<"\r\n";
		string ot = ss.str();
		output_text_ = ot;
		WriteIndexFile(real_file_name);
		retval = 1;
	}
	else
	{	
		string file_key = sid;			
		UploadFileBuffer *ufb = new UploadFileBuffer();
		ufb->offset = offset;
		ufb->length = length;
		ufb->data = new char[length + 1];
		memset(ufb->data,0,length + 1);
		if (length != http_session_->m_datalen)
		{
			//todolog
			WriteLog(HTTPPRO, "163 upload att len err\r\n");
			//	log->Output("");
		}
		memcpy(ufb->data, packet, http_session_->m_datalen);
		map<string, UploadFileBufferList *>::iterator iter = file_buffer_.find(file_key);
		if (iter == file_buffer_.end())
		{
			UploadFileBufferList *ufbl = new UploadFileBufferList();
			//			request->ProcessAuthentication(&ufbl->authInfo);
			time_t timestamp = 0;
			ufb->offset = 0;
			ufbl->lastTime = ::time(&timestamp);
			ufbl->complete = false;
			ufbl->uid = uid;
			ufbl->fileName = attachment_name;
			ufbl->fileSize = file_size;
			ufbl->recieve_size += length; // add by cxl
			ufbl->buffers.push_back(ufb);
			file_buffer_.insert(make_pair(file_key, ufbl));
		}
		else
		{
			time_t timestamp = 0;
			iter->second->lastTime = ::time(&timestamp);
			iter->second->recieve_size += length; // add by cxl
			iter->second->buffers.push_back(ufb);
		}
		
	}
	// begin add by cxl 
	map<string, UploadFileBufferList *>::iterator itFile = file_buffer_.begin();
	
	while (itFile != file_buffer_.end())
	{
		if (itFile->second->recieve_size == itFile->second->fileSize)
		{
			itFile->second->complete = true;
			ReassembleFileBuffer(itFile++);
		}
		else
		{
			itFile++;
		}
	}
	// end add
	return 1;
}



int email::EMail163::PCWebJSUploadAttachment2(const char *packet)
{
	StringManipulation sm;	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	sm.SplitStringToMap(cookie, ';', '=');
	from_ = sm.GetValueFromMapByKey("mail_uid");	
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));	
	BoundaryParser bp;
	bp.Parse(packet, http_session_->m_datalen, boundary);
	BoundaryValue *bv = NULL;
	string key;
	int fno = 1;
	while(1)
	{
		stringstream ss;
		ss<<"\"file"<<fno<<"\"";
		
		key = bp.GetBoundaryKey(ss.str());
		bv = bp.GetBoundaryValue(key);
		ss.clear();
		ss.str("");
		
		if (bv == NULL || key.empty())
		{
			break;
		}
		
		string file_name = sm.GetValueBetweenBeginEnd(key,"filename=\"","\"",true);
		attachment_ = file_name;
		if (file_name.empty())
		{
			break;
		}
		
		string real_file_name = WriteTempRealFile(bv->ptr,file_name.c_str(),bv->len);
		
		output_text_ = 
			"from=" + from_ + "\r\n" + 
			"onick=" + attachment_ + "\r\n" + 
			"state=上传附件\r\n";	
		WriteIndexFile(real_file_name);	
		fno++;
	}	
	return 1;
}

int email::EMail163::PCWebJYUploadAttachment(const char *packet)
{
	string boundary(http_session_->m_Requestline.m_strBoundary.GetBuffer(0));
	
	if (boundary.empty())
	{
		return 0;
	}
	boundary = "\r\n--" + boundary;
	ms_->SplitStringToMap(http_session_->m_Requestline.m_URI.GetBuffer(0),'&','=');
	from_ = ms_->GetValueFromMapByKey("uid");	
	if (from_.empty())
	{
		return 0;
	}	
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	
	string tmp_file_size = ms_->GetValueBetweenBeginEnd(src_data,"name=\"size\"\r\n\r\n",boundary);
	attachment_ = ms_->GetValueBetweenBeginEnd(src_data,"name=\"name\"\r\n\r\n",boundary,true);
	if (!attachment_.empty() && !tmp_file_size.empty())
	{ 
		int file_size = ms_->ToInt(tmp_file_size);
		if ( (file_size > 0) && (file_size < real_file_max_size_) )
		{
			char *stream = strstr(packet,"Content-Type: application/octet-stream\r\n\r\n");
			if (stream)
			{
				int offset = strlen("Content-Type: application/octet-stream\r\n\r\n");
				string real_file_name = WriteTempRealFile(stream + offset,attachment_.c_str(),file_size);
				
				ConstructUploadAttachmentOuptputString();
				WriteIndexFile(real_file_name);
			}
			else
			{
				ConstructUploadAttachmentOuptputString();
				WriteIndexFile();
			}
			
		}
		else
		{
			ConstructUploadAttachmentOuptputString();
			WriteIndexFile();
		}
		return 1;
	}
	return 0;
}

void email::EMail163::ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl)
{
	time_t timestamp = 0;
	time_t current_time = ::time(&timestamp);
	
	UploadFileBufferList *ufbl = itufbl->second;
	
	char *fp = new char[ufbl->fileSize + 1];
	memset(fp,0,ufbl->fileSize + 1);
	
	for (list<UploadFileBuffer *>::iterator itufb2 = ufbl->buffers.begin(); itufb2 != ufbl->buffers.end(); ++itufb2)
	{
		UploadFileBuffer *ufb = *itufb2;
		if ( ufb->offset < ufbl->fileSize )
		{
			memcpy(fp + ufb->offset,ufb->data,ufb->length);
		}
		delete ufb;
	}
	
	string uid = itufbl->second->uid;
	string file_name = itufbl->second->fileName;
	//	auth_info_ = itufbl->second->authInfo;
	string real_file_name = WriteTempRealFile(fp, file_name.c_str(), ufbl->fileSize);
	delete [] fp;
	
	delete ufbl;
	file_buffer_.erase(itufbl);
	
	stringstream ss;
	ss<<"user="<<uid<<"\r\n"
		<<"onick="<<file_name<<"\r\n"
		<<"state=上传附件(JS6)"<<"\r\n";
	string ot = ss.str();
	output_text_ = ot;
	WriteIndexFile(real_file_name);
	
}

