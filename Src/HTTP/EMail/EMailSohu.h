

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
			//pc登录
			kLogin = 1,
			//pc邮件
			kPCWebSendMail,
			//pc附件
			kPCWebUploadAttachment,
			//pcvip登录
			kVipLogin,
			//pcvip邮件
			kPCVipWebSendMail,
			//pcvip附件
			kPCVipWebUploadAttachment,
		};
	    //登录信息
		int PCWebLogin(const char *packet);				
		int PCWebSendMail(const char *packet);		
		int PCWebUploadAttachment(const char *packet);
		//vip登录信息
		int PCVipWebLogin(const char *packet);				
		int PCVipWebSendMail(const char *packet);		
		int PCVipWebUploadAttachment(const char *packet);

		//记录版本
		string version_;
	};
}
#endif