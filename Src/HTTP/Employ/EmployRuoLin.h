#ifndef HTTP_EMPLOY_RUOLIN_H_H_H
#define HTTP_EMPLOY_RUOLIN_H_H_H

#include "Employ.h"

namespace employ
{
	class RuoLin: public Employ
	{
	public:
		RuoLin(HttpSession *http_session, const string &pro_name);
		~RuoLin();
		static int IsRuoLin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//������Ϣ
		int PcWebPaMain(const char *packet);
		//�޸Ļ�����Ϣ
		int PcWebResume(const char *packet);
		//�ϴ�ͷ��
		int PcWebPaPhoto(const char *packet);
		//android��¼
		int AndroidLogin(const char * packet);
		//android�޸Ļ�����Ϣ
		int AndroidResume(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//��¼
			kPcWebLogin,
			//�½�������Ϣ
			kPcWebPaMain ,
			//�޸Ļ�����Ϣ
			kPcWebResume,
			//�ϴ�ͷ��	
			kPcWebPaPhoto,
			//android��¼
			kAndroidLogin,
			//android�޸Ļ�����Ϣ
			kAndroidResume,
		};
	};
}

#endif