#ifndef HTTP_EMPLOY_SHUZIYINGCAI_H_H_H
#define HTTP_EMPLOY_SHUZIYINGCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class ShuZiYingCai : public Employ
	{
	public:
		ShuZiYingCai(HttpSession *http_session, const string &pro_name);
		~ShuZiYingCai();
		static int IsShuZiYingCai(HttpSession *http_session);
		int Process(const char *packet,int action);
		string GetIdType(string idType);

	private:
		//ע��(�û���ע��)
		int PcWebRegisterUser(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//������Ϣ(��׼�ͼ���)
		int PcWebPaMain(const char *packet);
		//������Ϣ(�����ͼ���)
		int PcWebPaSimple(const char *packet);
		//������Ϣ(΢����)
		int PcWebPaPrivate(const char *packet);
		//�ϴ�����(��������) 
		int PcWebPaResume(const char *packet);

		//�ϴ�ͷ��
		int PcWebPaPhoto(const char *packet);

		//androidע��(�û���ע��)
		int AndroidRegisterUser(const char *packet);
		//android��¼
		int AndroidLogin(const char *packet);
		enum
		{
            //ע��(�û���ע��)
			kPcWebRegisterUser = 1,
			//��¼
			kPcWebLogin,
			//������Ϣ(��׼�ͼ���)
			kPcWebPaMain,
			//������Ϣ(�����ͼ���)
			kPcWebPaSimple,
			//������Ϣ(΢����)
			kPcWebPaPrivate,
			//�ϴ�����(��������) 
			kPcWebPaResume,
			//�ϴ�ͷ��	
			kPcWebPaPhoto,
            //androidע��(�û���ע��)
			kAndroidRegisterUser = 1,
			//android��¼
			kAndroidLogin,
		};
	};
}

#endif