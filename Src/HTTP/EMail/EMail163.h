#ifndef HTTP_EMAIL_EMAIL163_H_H_H
#define HTTP_EMAIL_EMAIL163_H_H_H
#include "EMail.h"
namespace email
{
	class EMail163 : public EMail
	{
	public:
		EMail163(HttpSession *http_session, const string &pro_name);
		~EMail163();
		int static IsEMail163(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		enum
		{
			//普通邮件
			kLogin = 1,
			kPCWebJY6SendMail,		// 简约6
			kPCWeb6SendMail,		// 6
			kPCWebJY5SendMail,		// 简约5
			kPCWeb5SendMail,		// 5
			kMWebSendMail,     
			//手机浏览器
			kMobileSendMail,

			//普通附件
			kPCWebJSUploadAttachment,
			kPCWebJYUploadAttachment,
			kPCWebJSUploadAttachment2,
	        //内存分离
			kReassembleFileBuffer,
			//vip邮件 
			kPCWebEnVipSendMail,			//1.English 版
			kPCWeb5VipSendMail,				//2.管家5.0 版
			kPCWeb6VipSendMail,				//3.极致6.0 版
			kPCWebVipSendMail,				//4.默认 版			
			//vip附件
			kPCWebEnUpViploadAttachment,	//1.English 版
			kPCWeb5UpViploadAttachment,		//2.管家5.0 版
			kPCWeb6UpViploadAttachment,		//3.极致6.0 版
			kPCWebUpViploadAttachment,		//4.默认 版	
			//企业版
			kPCWebYmSendMail,				//163 企业邮件ym
			kPCWebUpYmloadAttachment,		//163 企业附件 ym
			//手机浏览器附件
			kMboileUploadAttachment,
		};

		//普通邮件
		int PCWebJY6SendMail(const char *packet);
		int PCWeb6SendMail(const char *packet);
		int PCWebJY5SendMail(const char *packet);
		int PCWeb5SendMail(const char *packet);		
		int MWebSendMail(const char *packet);
		//普通附件
		int PCWebJSUploadAttachment(const char *packet);
		int PCWebJYUploadAttachment(const char *packet);
		int PCWebJSUploadAttachment2(const char *packet);
        //vip邮件
		int PCWebEnVipSendMail(const char *packet);				//1.English 版
		int PCWeb5VipSendMail(const char *packet);				//2.管家5.0 版
		int PCWeb6VipSendMail(const char *packet);				//3.极致6.0 版
		int PCWebVipSendMail(const char *packet);				//4.默认 版		
		//vip附件
		int PCWebEnUpViploadAttachment(const char *packet);		//1.English 版
		int PCWeb5UpViploadAttachment(const char *packet);		//2.管家5.0 版
		int PCWeb6UpViploadAttachment(const char *packet);		//3.极致6.0 版
		int PCWebUpViploadAttachment(const char *packet);		//4.默认 版
		//ym邮件
		//ym附件
		int PCWebYmSendMail(const char *packet);				//ym企业163 邮件
		int PCWebJSYmUploadAttachment(const char *packet);		//ym企业163 附件
		//手机浏览器
		//手机浏览器附件
		int MobileSendMail(const char *packet);
		int MboileUploadAttachment(const char *packet);

		void ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl);	
		static map<string, UploadFileBufferList *>file_buffer_;
		string version_;
	};
}
#endif