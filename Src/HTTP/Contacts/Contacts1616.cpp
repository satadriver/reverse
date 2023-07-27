
#include "StdAfx.h"

#include "Contacts1616.h"

contacts::Contacts1616::Contacts1616(HttpSession *http_session, const string &pro_name) : Contacts(http_session,pro_name)
{
	
}

contacts::Contacts1616::~Contacts1616()
{
	
}

int contacts::Contacts1616::IsContacts1616(HttpSession *http_session)
{
	if(  ( http_session->m_Requestline.m_Host.Find("apps.1616.net")  != -1 ) && 
		( http_session->m_Requestline.m_URI.Find("/contact/mobile.php") != -1 ))
	{
		return kUploadContacts;
	}
	return 0;
}

int contacts::Contacts1616::Process(const char *packet,int action)
{
	if (action == kUploadContacts)
	{
		return UploadContacts(packet);
	}
	return 0;
}

int contacts::Contacts1616::UploadContacts(const char *packet)
{
	string src_data(packet);
	src_data = ms_->UrlDecode(src_data);

	ms_->Replace(src_data,"{","");
	ms_->Replace(src_data,"}","");
	ms_->Replace(src_data,"\"","");
	ms_->Replace(src_data,"mobile:","");	
	ms_->SplitStringToMap(src_data,'&','=');
	name_ = ms_->GetValueFromMapByKey("name",true);
	phone_ = ms_->GetValueFromMapByKey("phone");
	if (name_.empty() || phone_.empty())
	{
		return 0;
	}	
	ms_->Replace(phone_,"+86","");
	ms_->Replace(phone_," ","");
	output_text_ = 
		"nick=" + name_+ "\r\n" + 
		"user=" + phone_ + "\r\n";
	WriteIndexFile();
	return 1;
}