
#ifndef HTTP_EMAIL_EMAIL163_H_H_H
#define HTTP_EMAIL_EMAIL163_H_H_H

#include "EMail.h"

namespace email
{
	class EMail163 : public EMail
	{
	public:
		EMail163(HttpSession *http_session, const string &pro_name);
		~EMail163();
		int static IsEMail163(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		enum
		{
			kLogin = 1,
			kPCWebJY6SendMail,	// ºÚ‘º6
			kPCWeb6SendMail,		// 6
			kPCWebJY5SendMail,	// ºÚ‘º5
			kPCWeb5SendMail,		// 5
			kMWebSendMail,
			kPCWebJSUploadAttachment,
			kPCWebJYUploadAttachment,	
		};
		
		int PCWebJY6SendMail(const char *packet);
		int PCWeb6SendMail(const char *packet);
		int PCWebJY5SendMail(const char *packet);
		int PCWeb5SendMail(const char *packet);
		
		int MWebSendMail(const char *packet);
		
		int PCWebJSUploadAttachment(const char *packet);
		int PCWebJYUploadAttachment(const char *packet);
		
		string version_;
	};
}
#endif