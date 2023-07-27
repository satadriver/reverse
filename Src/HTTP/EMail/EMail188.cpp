#include "StdAfx.h"
#include "EMail188.h"

map<string, UploadFileBufferList * > email::EMail188::file_buffer_ = map<string, UploadFileBufferList * >();
email::EMail188::EMail188(HttpSession *http_session, const string &pro_name) : EMail(http_session, pro_name)
{
	
}

email::EMail188::~EMail188()
{
	
}

int email::EMail188::IsEMail188(HttpSession *http_session)
{	
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.188.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("compose/upload")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/js")!=-1))
	{
		return kPCWebvipUploadAttachmentvip;
	}
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.188.com")!=-1)&&
	   (http_session->m_Requestline.m_URI.Find("/js")!=-1))
	{
		return kPCWebvipSendMail;
	}
	//Google浏览器
	if((http_session->m_Requestline.m_Host.Find("hz.188.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/upxmail/upload")!=-1))
	{
		return kPCWebvipUploadAttachment;
	}
	//英文版
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.188.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/jyen/compose/compose")!=-1))
	{
		return kPCWebvipSendMailEnglish;
	}
	//英文版附件
	if((http_session->m_Requestline.m_Host.Find("webmail.vip.188.com")!=-1)&&
		(http_session->m_Requestline.m_URI.Find("/jyen/compose/autoUploadForH")!=-1))
	{
		return kPCWebvipUploadAttachmentEnglish;
	}
	return 0;
}

int email::EMail188::Process(const char *packet,int action)
{
	switch (action)
	{
		case kPCWebvipSendMail:
			return PCWebvipSendMail(packet);
		case kPCWebvipUploadAttachment:
			return PCWebvipUploadAttachment(packet);
		case kPCWebvipUploadAttachmentvip:
			return PCWebvipUploadAttachmentvip(packet);
		case kPCWebvipSendMailEnglish:
			return PCWebvipSendMailEnglish(packet);
		case kPCWebvipUploadAttachmentEnglish:
			return PCWebvipUploadAttachmentEnglish(packet);
		default:
			break;
	}
	return 0;
}

void email::EMail188::ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl)
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

//*******************2016-8-17************************//
int email::EMail188::PCWebvipSendMail(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	//去除前面的 var
	src_data = src_data.substr(strlen("var="));
	//替换转义字符 < => &lt,> =>&gt,
	if ((src_data.find("&lt") != string::npos) && (src_data.find("&gt") != string::npos))
	{
	 	ms_->Replace(src_data,"&lt;","<");
	 	ms_->Replace(src_data,"&gt;",">");
	 	ms_->Replace(src_data,"&quot;","\"");
	 	ms_->Replace(src_data,"&amp;","&");
	 	ms_->Replace(src_data,"&nbsp;"," ");
	}
    //提取 from
	from_ = ms_->GetValueBetweenBeginEnd(src_data,"\"account\">","</string>",true);
		from_ = ms_->GetValueBetweenBeginEnd(from_,"<",">",true);
    //提取 to
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
	//提取 主题	
	title_ = ms_->GetValueBetweenBeginEnd(src_data,"\"subject\">","</string>",true);
    //提取 内容 
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

//附件
int email::EMail188::PCWebvipUploadAttachment(const char *packet)
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
	//得到文件名
	string attachment_name = sm.UTF8ToGBK(sm.UrlDecode(http_session_->m_Requestline.m_emailAttName.GetBuffer(0)));
	//文件的大小
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
		//判断数据量
		if (length != http_session_->m_datalen)
		{
			WriteLog(HTTPPRO, "126 upload att len err\r\n");
		}
        //拷贝packet包中的数据到缓存区
		memcpy(ufb->data, packet, http_session_->m_datalen);
		map<string, UploadFileBufferList *>::iterator iter = file_buffer_.find(file_key);
		if (iter == file_buffer_.end())
		{
			UploadFileBufferList *ufbl = new UploadFileBufferList();
			//	request->ProcessAuthentication(&ufbl->authInfo);
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
int email::EMail188::PCWebvipUploadAttachmentvip(const char *packet)
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

int email::EMail188::PCWebvipSendMailEnglish(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);
	ms_->SplitStringToMap(src_data, '&', '=');
	//替换转义字符 < => &lt,> =>&gt,
	if ((src_data.find("&lt") != string::npos) && (src_data.find("&gt") != string::npos))
	{
		ms_->Replace(src_data,"&lt;","<");
		ms_->Replace(src_data,"&gt;",">");
		ms_->Replace(src_data,"&quot;","\"");
		ms_->Replace(src_data,"&amp;","&");
		ms_->Replace(src_data,"&nbsp;"," ");
	}
	from_ = ms_->GetValueFromMapByKey("account", true);
	to_=ms_->GetValueFromMapByKey("to",true);
	title_=ms_->GetValueFromMapByKey("subject",true);
	content_=ms_->GetValueFromMapByKey("content",true);
	if (from_.empty() || to_.empty() )
	{
		return 0;
	}
	WriteVirtualIdentity(from_,"发送邮件");
	ConstructSendMailOutputString();
	WriteIndexFile();
	return 1;
}
int email::EMail188::PCWebvipUploadAttachmentEnglish(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UTF8ToGBK(UrlDecode(src_data));
	StringManipulation sm;
	string cookie(http_session_->m_Requestline.m_Cookie.GetBuffer(0));
	sm.SplitStringToMap(cookie, ';', '=');
	from_ = sm.GetValueFromMapByKey("mail_uid");
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
//*******************2016-8-17************************//