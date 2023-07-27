
#ifndef HTTP_EMAIL_EMAILYEAH_H_H_H
#define HTTP_EMAIL_EMAILYEAH_H_H_H

#include "EMail.h"

namespace email
{
	class EMailYeah : public EMail
	{
	public:
		EMailYeah(HttpSession *http_session, const string &pro_name);
		
		~EMailYeah();
		int static IsEMailYeah(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			kLogin = 1,
			kPCWebJY6SendMail,
			kPCWeb6SendMail,
			kPCWebJY5SendMail,
			kPCWeb5SendMail,
			kMWebSendMail,
			kPCWebJSUploadAttachment,
			kPCWebJYUploadAttachment,
			kPCWebJSUploadAttachment2,
			kReassembleFileBuffer
		};
		
		int PCWebJY6SendMail(const char *packet);
		int PCWeb6SendMail(const char *packet);
		int PCWebJY5SendMail(const char *packet);
		int PCWeb5SendMail(const char *packet);
		
		int MWebSendMail(const char *packet);
		
		int PCWebJSUploadAttachment(const char *packet);
		int PCWebJYUploadAttachment(const char *packet);
		int PCWebJSUploadAttachment2(const char *packet);

		void ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl);
		
		static map<string, UploadFileBufferList *>file_buffer_;
		string version_;
	};
}
#endif