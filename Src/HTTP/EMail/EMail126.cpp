#include "StdAfx.h"
#include "EMail126.h"

map<string, UploadFileBufferList * > email::EMail126::file_buffer_ = map<string, UploadFileBufferList * >();

email::EMail126::EMail126(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail126::~EMail126()
{
	
}

int email::EMail126::IsEMail126(HttpSession *http_session)
{
	//登录	
	if((http_session->m_Requestline.m_Host.Find("entry.mail.163.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/corEMail/fcg/ntesdoor2")!=-1))
	{
		return kLogin;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("compose/compose")!=-1))
	{
		return kPCWebJY6SendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/jy5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("compose/compose")!=-1))
	{
		return kPCWebJY5SendMail;
	}	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js6")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("action=deliver")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("cl_send=1")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose&")!=-1))
	{
		return kPCWeb6SendMail;
	}	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose&")!=-1))
	{
		return kPCWeb5SendMail;
	}	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/js5")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
	{
		return kMWebSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/m/s")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("mbox:compose")!=-1))
	{
		return kMobileSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("ver=js")!=-1))
	{
		return kPCWebJSUploadAttachment;
	}	
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1) &&
		(http_session->m_Requestline.m_URI.Find("ver=jy")!=-1))
	{
		return kPCWebJYUploadAttachment;
	}
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.126.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/js")!=-1))
	{
		return kPCWebvipSendMail;
	}
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.126.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("compose/upload")!=-1)
		&&(http_session->m_Requestline.m_URI.Find("/js")!=-1))
	{
		return kPCWebvipUploadAttachmentvip;
	}
	if((http_session->m_Requestline.m_Host.Find("vip.126.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1))
	{
		return kPCWebvipUploadAttachment;
	}
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/m/compose/upload.jsp")!=-1))
	{
		return kMboileUploadAttachment;
	}
	// 	if( (http_session->m_Requestline.m_Host.Find(".")!=-1))
	// 	{
	// 		return kReassembleFileBuffer;
	// 	}
	
	/*
	if((http_session->m_Requestline.m_Host.Find("mail.126.com")!=-1) && 
	(http_session->m_Requestline.m_URI.Find("/js6/compose/upload.jsp")!=-1) )
	{
	return kPCWebJSUploadAttachment2;
}*/
	return 0;
}

int email::EMail126::Process(const char *packet,int action)
{
	switch (action)
	{
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
		return MWebSendMail(packet);
    case kMobileSendMail:
		version_ = "手机浏览器";
		return MobileSendMail(packet);
	case kPCWebJSUploadAttachment :
		return PCWebJSUploadAttachment(packet);
	case kPCWebJSUploadAttachment2 :
		return PCWebJSUploadAttachment2(packet);
	case kPCWebvipSendMail:
		return PCWebvipSendMail(packet);
	case kPCWebvipUploadAttachment:
		return PCWebvipUploadAttachment(packet);
	case kPCWebvipUploadAttachmentvip:
		return PCWebvipUploadAttachmentvip(packet);
	case kMboileUploadAttachment:
		return MboileUploadAttchment(packet);
	//**********************2016-8-15************************//
		// 	case kPCWebJYUploadAttachment :
		// 		return PCWebJYUploadAttachment(packet);
		// 	case kReassembleFileBuffer :
		// 		ReassembleFileBuffer();
	default:
		break;
	}
	return 0;
}

int email::EMail126::PCWebJY6SendMail(const char *packet)
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
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWeb6SendMail(const char *packet)
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
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWebJY5SendMail(const char *packet)
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
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWeb5SendMail(const char *packet)
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
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::MWebSendMail(const char *packet)
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
	string::size_type pos_begin = content_.find("\n\n");
	if (pos_begin != string::npos)
	{
		content_ = content_.substr(pos_begin);
	}
	string::size_type pos_end = content_.find("\n\n\n--");
	if (pos_end != string::npos)
	{
		content_ = content_.substr(0,pos_end);
	}	
	WriteVirtualIdentity(from_,"发送邮件");	
	ConstructSendMailOutputString();
	WriteIndexFile();	
	return 1;
}

int email::EMail126::MobileSendMail(const char *packet)
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

int email::EMail126::PCWebJSUploadAttachment(const char *packet)
{
/*
ms_->SplitStringToMap(http_session_->m_Requestline.m_URI.GetBuffer(0),'&','=');
from_ = ms_->GetValueFromMapByKey("uid");

  if (from_.empty())
  {
		return 0;
		}
		int file_size = atoi(http_session_->m_Requestline.m_emailAttSize.GetBuffer(0));
		
		  if ( (file_size > 0 )&& (file_size < real_file_max_size_) )
		  {
		  attachment_ = ms_->UTF8ToGBK(ms_->UrlDecode(http_session_->m_Requestline.m_emailAttName.GetBuffer(0)));
		  string real_file_name = WriteTempRealFile(packet,attachment_.c_str(),file_size);
		  
			
			  ConstructUploadAttachmentOuptputString();
			  WriteIndexFile(real_file_name);
			  }
			  else
			  {
			  // 由于又没文件名又没文件实体故略去不写
			  //	ConstructUploadAttachmentOuptputString();
			  //	WriteIndexFile();
			  }
			  
	*/
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
	//	request->ProcessAuthentication(&auth_info_);	
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
			WriteLog(HTTPPRO, "126 upload att len err\r\n");
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

int email::EMail126::PCWebJYUploadAttachment(const char *packet)
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

int email::EMail126::PCWebJSUploadAttachment2(const char *packet)
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

void email::EMail126::ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl)
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
//*******************2016-8-15************************//
int email::EMail126::PCWebvipSendMail(const char *packet)
{
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

	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
		from_ = ms_->GetValueBetweenBeginEnd(from_,"<",">",true);

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
		content_ = ms_->GetValueBetweenBeginEnd(content_,">","</div>",true);

	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWebvipUploadAttachment(const char *packet)
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
			WriteLog(HTTPPRO, "126 upload att len err\r\n");
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


int email::EMail126::PCWebvipSendMail1(const char *packet)
{

	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
	to_ = ms_->GetValueBetweenBeginEnd(src_data,"to\"><string>","</string>",true);
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
	content_ = ms_->GetValueBetweenBeginEnd(src_data,"\"content\">","</string>",true);
	from_=ms_->GetValueBetweenBeginEnd(from_,"&lt;","&gt;");
	content_=ms_->GetValueBetweenBeginEnd(content_,"&gt;","&lt;/");
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}

int email::EMail126::PCWebvipUploadAttachmentvip(const char *packet)
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
		
		if (file_name.empty())
		{
			break;
		}		
		string real_file_name = WriteTempRealFile(bv->ptr,file_name.c_str(),bv->len);		
		output_text_ = 
			"from=" + from_ + "\r\n" + 
			"onick=" + file_name + "\r\n" + 
			"state=上传附件\r\n";		
		WriteIndexFile(real_file_name);		
		fno++;
	}	
	return 1;
}

int email::EMail126::MboileUploadAttchment(const char *packet)
{
	StringManipulation sm;	
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	sm.SplitStringToMap(cookie, ';', '=');
	from_ = sm.GetValueFromMapByKey("mail_uid");
	
	//数据部分
	string src_data(packet);
	src_data=ms_->UTF8ToGBK(UrlDecode(src_data));
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
	//写入临时文件
	string real_file_name = WriteTempRealFile(bv->ptr,attachment_.c_str(),bv->len);
	ConstructUploadAttachmentOuptputString();
	WriteIndexFile(real_file_name);
	return 1;
}