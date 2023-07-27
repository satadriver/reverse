#ifndef HTTP_EMAIL_EMAILSINA_H_H_H
#define HTTP_EMAIL_EMAILSINA_H_H_H

#include "EMail.h"
namespace email
{
	class EMailSina : public EMail
	{
	public:
		EMailSina(HttpSession *http_session, const string &pro_name);
		
		~EMailSina();
		int static IsEMailSina(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			kLogin = 1,
			kPCWebSendMail,
			kMWebSendMail,
			kPCWebUploadAttachment
		};	
		int PCWebSendMail(const char *packet);
		int MWebSendMail(const char *packet);	
		int PCWebUploadAttachment(const char *packet);
		string version_;
	};
}
#endif