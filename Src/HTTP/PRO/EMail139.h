
#ifndef HTTP_EMAIL_EMAIL139_H_H_H
#define HTTP_EMAIL_EMAIL139_H_H_H

#include "EMail.h"

namespace email
{
	class EMail139 : public EMail
	{
	public:
		EMail139(HttpSession *http_session, const string &pro_name);
		
		~EMail139();
		int static IsEMail139(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		enum
		{
			kPCWebSendMail = 1,
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