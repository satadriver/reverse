
#ifndef HTTP_EMAIL_EMAILQQ_H_H_H
#define HTTP_EMAIL_EMAILQQ_H_H_H

#include "EMail.h"


namespace email
{
	class EMailQQ : public EMail
	{
	public:
		EMailQQ(HttpSession *http_session, const string &pro_name);
		~EMailQQ();
		int static IsEMailQQ(HttpSession *http_session);
		int Process(const char *packet,int action);
		
	private:
		enum 
		{
			kLogin = 1,
			mail_qq_text,
			kAndroidPhoneSendMail,
			kMWebSendMail,
			kPCWebEnglishSendMail,
			kPCWebBaseSendMail,
			kPCWebDefaultSendMail,
			kPCWebUploadAttachment,
			kPCWebUploadAttachmentInfo,
			kPCWebUploadAttachmentBody
		};
		
		int AndroidPhoneSendMail(const char *packet);
		int MWebSendMail(const char *packet);
		int PCWebEnglishSendMail(const char *packet);
		int PCWebBaseSendMail(const char *packet);
		int PCWebDefaultSendMail(const char *packet);
		
		int PCWebUploadAttachment(const char *packet);
		int PCWebUploadAttachmentInfo(const char *packet);
		int PCWebUploadAttachmentBody(const char *packet); 
		string version_;
		//<FILEMD5 ,FILENAME&&FILESIZE>
		static map<string,string>filemd5_fileinfo_;
	};
}
#endif
