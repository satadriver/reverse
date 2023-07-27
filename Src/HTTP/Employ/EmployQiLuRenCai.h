#ifndef HTTP_EMPLOY_QILURENCAI_H_H_H
#define HTTP_EMPLOY_QILURENCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class QiLuRenCai : public Employ
	{
	public:
		QiLuRenCai(HttpSession *http_session, const string &pro_name);
		~QiLuRenCai();
		static int IsQiLuRenCai(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//ע��(�ֻ���ע��)
		int PcWebRegisterPhone(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//������Ϣ
		int PcWebPaMain(const char *packet);
		//�ϴ�ͷ��
		int PcWebPaPhoto(const char *packet);
		//�ϴ�����
		int PcWebPaResume(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
            //ע��(�ֻ���ע��)
			kPcWebRegisterPhone,
			//��¼
			kPcWebLogin,
			//������Ϣ
			kPcWebPaMain,
			//�ϴ�ͷ��	
			kPcWebPaPhoto,
			//�ϴ����� 
			kPcWebPaResume,
		};
	};
}

#endif