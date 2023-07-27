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
			//��ͨ�ʼ�
			kLogin = 1,
			kPCWebJY6SendMail,		// ��Լ6
			kPCWeb6SendMail,		// 6
			kPCWebJY5SendMail,		// ��Լ5
			kPCWeb5SendMail,		// 5
			kMWebSendMail,     
			//�ֻ������
			kMobileSendMail,

			//��ͨ����
			kPCWebJSUploadAttachment,
			kPCWebJYUploadAttachment,
			kPCWebJSUploadAttachment2,
	        //�ڴ����
			kReassembleFileBuffer,
			//vip�ʼ� 
			kPCWebEnVipSendMail,			//1.English ��
			kPCWeb5VipSendMail,				//2.�ܼ�5.0 ��
			kPCWeb6VipSendMail,				//3.����6.0 ��
			kPCWebVipSendMail,				//4.Ĭ�� ��			
			//vip����
			kPCWebEnUpViploadAttachment,	//1.English ��
			kPCWeb5UpViploadAttachment,		//2.�ܼ�5.0 ��
			kPCWeb6UpViploadAttachment,		//3.����6.0 ��
			kPCWebUpViploadAttachment,		//4.Ĭ�� ��	
			//��ҵ��
			kPCWebYmSendMail,				//163 ��ҵ�ʼ�ym
			kPCWebUpYmloadAttachment,		//163 ��ҵ���� ym
			//�ֻ����������
			kMboileUploadAttachment,
		};

		//��ͨ�ʼ�
		int PCWebJY6SendMail(const char *packet);
		int PCWeb6SendMail(const char *packet);
		int PCWebJY5SendMail(const char *packet);
		int PCWeb5SendMail(const char *packet);		
		int MWebSendMail(const char *packet);
		//��ͨ����
		int PCWebJSUploadAttachment(const char *packet);
		int PCWebJYUploadAttachment(const char *packet);
		int PCWebJSUploadAttachment2(const char *packet);
        //vip�ʼ�
		int PCWebEnVipSendMail(const char *packet);				//1.English ��
		int PCWeb5VipSendMail(const char *packet);				//2.�ܼ�5.0 ��
		int PCWeb6VipSendMail(const char *packet);				//3.����6.0 ��
		int PCWebVipSendMail(const char *packet);				//4.Ĭ�� ��		
		//vip����
		int PCWebEnUpViploadAttachment(const char *packet);		//1.English ��
		int PCWeb5UpViploadAttachment(const char *packet);		//2.�ܼ�5.0 ��
		int PCWeb6UpViploadAttachment(const char *packet);		//3.����6.0 ��
		int PCWebUpViploadAttachment(const char *packet);		//4.Ĭ�� ��
		//ym�ʼ�
		//ym����
		int PCWebYmSendMail(const char *packet);				//ym��ҵ163 �ʼ�
		int PCWebJSYmUploadAttachment(const char *packet);		//ym��ҵ163 ����
		//�ֻ������
		//�ֻ����������
		int MobileSendMail(const char *packet);
		int MboileUploadAttachment(const char *packet);

		void ReassembleFileBuffer(map<string,UploadFileBufferList *>::iterator itufbl);	
		static map<string, UploadFileBufferList *>file_buffer_;
		string version_;
	};
}
#endif