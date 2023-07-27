#ifndef HTTP_EMPLOY_GANJI_H_H_H
#define HTTP_EMPLOY_GANJI_H_H_H

#include "Employ.h"

namespace employ
{
	class GanJi: public Employ
	{
	public:
		GanJi(HttpSession *http_session, const string &pro_name);
		~GanJi();
		static int IsGanJi(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(�ֻ���ע��)
		int PcWebRegisterPhone(const char *packet);
		//������Ϣ
		int PcWebPaMain(const char *packet);
		//�޸Ļ�����Ϣ
		int PcWebResume(const char *packet);
		//�ϴ�ͷ��
		int PcWebPaPhoto(const char *packet);
		enum
		{
			//ע��(�ֻ���ע��)
			kPcWebRegisterPhone = 1,
			//�½�������Ϣ
			kPcWebPaMain ,
			//�޸Ļ�����Ϣ
			kPcWebResume,
			//�ϴ�ͷ��	
			kPcWebPaPhoto,
		};
	};
}

#endif