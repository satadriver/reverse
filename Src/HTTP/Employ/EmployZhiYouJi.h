#ifndef HTTP_EMPLOY_ZHIYOUJI_H_H_H
#define HTTP_EMPLOY_ZHIYOUJI_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhiYouJi: public Employ
	{
	public:
		ZhiYouJi(HttpSession *http_session, const string &pro_name);
		~ZhiYouJi();
		static int IsZhiYouJi(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//������Ϣ
		int PcWebPaMain(const char *packet);
		//�޸Ļ�����Ϣ
		int PcWebResume(const char *packet);
		enum
		{
			//�½�������Ϣ
			kPcWebPaMain = 1,
			//�޸Ļ�����Ϣ
			kPcWebResume,
		};
	};
}

#endif