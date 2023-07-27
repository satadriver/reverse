

#ifndef HTTP_EMAIL_EMAIL189_H_H_H
#define HTTP_EMAIL_EMAIL189_H_H_H

#include "EMail.h"

namespace email
{
	class EMail_189 : public EMail
	{
	public:
		EMail_189(HttpSession *http_session, const string &pro_name);
		
		~EMail_189();
		int static IsEMail_189(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			kLogin = 1,
			kPCWebSendMail,
			kSendMailCW2,
			kMWebSendMail,
			kPCWebUploadAttachment,
			kMWebSendMailIE,
			kPCWebUploadAttachmentIE
		};
		
		int PCWebSendMail(const char *packet);		
		int PCWebUploadAttachment(const char *packet);
		int PCWebSendMailIE(const char *packet);		
		int PCWebUploadAttachmentIE(const char *packet);
		string version_;
	};
}
#endif