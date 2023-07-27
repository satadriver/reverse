
#ifndef HTTP_EMAIL_EMAIL21CN_H_H_H
#define HTTP_EMAIL_EMAIL21CN_H_H_H

#include "EMail.h"


namespace email
{
	class EMail21CN : public EMail
	{
	public:
		EMail21CN(HttpSession *http_session, const string &pro_name);
		
		~EMail21CN();
		int static IsEMail21CN(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			kPCWebLogin = 1,//µçÄÔ¶ËµÇÂ½
			kMWebLogin,		//ÒÆ¶¯¶ËµÇÂ½
			kPCWebSendMail,		//µçÄÔä¯ÀÀÆ÷
			kMWebSendMail,		//ÒÆ¶¯ä¯ÀÀÆ÷
			kPCWebUploadAttachment
		};
		
		int PCWebLogin(const char *packet);
		int MWebLogin(const char *packet);
		int PCWebSendMail(const char *packet);
		int MWebSendMail(const char *packet);
		
		int PCWebUploadAttachment(const char *packet);
		
		string pass_;
		string version_;
	};
}

#endif