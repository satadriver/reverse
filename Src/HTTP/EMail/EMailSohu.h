

#ifndef HTTP_EMAIL_EMAILSOHU_H_H_H
#define HTTP_EMAIL_EMAILSOHU_H_H_H

#include "EMail.h"


namespace email
{
	class EMailSohu : public EMail
	{
	public:
		EMailSohu(HttpSession *http_session, const string &pro_name);
		
		~EMailSohu();
		int static IsEMailSohu(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:	
		enum
		{
			//pc��¼
			kLogin = 1,
			//pc�ʼ�
			kPCWebSendMail,
			//pc����
			kPCWebUploadAttachment,
			//pcvip��¼
			kVipLogin,
			//pcvip�ʼ�
			kPCVipWebSendMail,
			//pcvip����
			kPCVipWebUploadAttachment,
		};
	    //��¼��Ϣ
		int PCWebLogin(const char *packet);				
		int PCWebSendMail(const char *packet);		
		int PCWebUploadAttachment(const char *packet);
		//vip��¼��Ϣ
		int PCVipWebLogin(const char *packet);				
		int PCVipWebSendMail(const char *packet);		
		int PCVipWebUploadAttachment(const char *packet);

		//��¼�汾
		string version_;
	};
}
#endif