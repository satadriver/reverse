#ifndef HTTP_EMAIL_EMAIL126_H_H_H
#define HTTP_EMAIL_EMAIL126_H_H_H

#include "EMail.h"

namespace email
{
	class EMail126 : public EMail
	{
	public:
		EMail126(HttpSession *http_session, const string &pro_name);
		
		~EMail126();
		int static IsEMail126(HttpSession *http_session);
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
			kMobileSendMail,

			kPCWebJSUploadAttachment,
			kPCWebJYUploadAttachment,
			kPCWebJSUploadAttachment2,
			kReassembleFileBuffer,
			kPCWebvipSendMail,
			kPCWebvipUploadAttachment,
			kPCWebvipSendMail1,
			kPCWebvipUploadAttachmentvip,
			kMboileUploadAttachment,
		};
		
		int PCWebJY6SendMail(const char *packet);
		int PCWeb6SendMail(const char *packet);
		int PCWebJY5SendMail(const char *packet);
		int PCWeb5SendMail(const char *packet);
		int MWebSendMail(const char *packet);	
		int MobileSendMail(const char *packet);
		int PCWebJSUploadAttachment(const char *packet);
		int PCWebJYUploadAttachment(const char *packet);
		int PCWebJSUploadAttachment2(const char *packet);
		int PCWebvipSendMail(const char *packet);
		int PCWebvipUploadAttachment(const char *packet);
		int PCWebvipSendMail1(const char *packet);
		int PCWebvipUploadAttachmentvip(const char *packet);
		int MboileUploadAttchment(const char *packet);

		void ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl);		
		static map<string, UploadFileBufferList *>file_buffer_;
		string version_;
	};
}
#endif