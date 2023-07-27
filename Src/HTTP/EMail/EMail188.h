#ifndef HTTP_EMAIL_EMAIL188_H_H_H
#define HTTP_EMAIL_EMAIL188_H_H_H

#include "EMail.h"

namespace email
{
	class EMail188 : public EMail
	{
	public:
		EMail188(HttpSession *http_session, const string &pro_name);
		
		~EMail188();
		int static IsEMail188(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			kReassembleFileBuffer = 1,
			kPCWebvipSendMail,
			kPCWebvipUploadAttachment,
			kPCWebvipSendMailvip,
			kPCWebvipUploadAttachmentvip,
			kPCWebvipSendMailEnglish,
			kPCWebvipUploadAttachmentEnglish

		};	
		//*********************2016-8-15*****************************//
		//vip¼«ÖÂ
		int PCWebvipSendMail(const char *packet);
		int PCWebvipUploadAttachment(const char *packet);
		int PCWebvipUploadAttachmentvip(const char *packet);
		int PCWebvipSendMailEnglish(const char *packet);
		int PCWebvipUploadAttachmentEnglish(const char *packet);
		//**********************2016-8-15****************************//
		void ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl);
		
		static map<string, UploadFileBufferList *>file_buffer_;
		string version_;
	};
}
#endif