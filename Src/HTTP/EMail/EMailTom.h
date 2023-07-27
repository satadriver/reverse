

#ifndef HTTP_EMAIL_EMAILTom_H_H_H
#define HTTP_EMAIL_EMAILTom_H_H_H

#include "EMail.h"


namespace email
{
	class EMailTom : public EMail
	{
	public:
		EMailTom(HttpSession *http_session, const string &pro_name);
		
		~EMailTom();
		int static IsEMailTom(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			kLogin = 1,
			kPCWebSendMail,
			kSendMailCW2,
			kMWebSendMail,
			kPCWebUploadAttachment
		};
		
		int PCWebSendMail(const char *packet);		
		int PCWebUploadAttachment(const char *packet);

		string version_;
	};
}
#endif