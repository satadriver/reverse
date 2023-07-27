

#ifndef HTTP_EMAIL_EMAILWO_H_H_H
#define HTTP_EMAIL_EMAILWO_H_H_H

#include "EMail.h"

namespace email
{
	class EMailWo : public EMail
	{
	public:
		EMailWo(HttpSession *http_session, const string &pro_name);
		
		~EMailWo();
		int static IsEMailWo(HttpSession *http_session);
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