#ifndef HTTP_EMPLOY_ZHILIANZHAOPIN_H_H_H
#define HTTP_EMPLOY_ZHILIANZHAOPIN_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhiLianZhaoPin : public Employ
	{
	public:
		ZhiLianZhaoPin(HttpSession *http_session, const string &pro_name);
		~ZhiLianZhaoPin();
		static int IsZhiLianZhaoPin(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//����׿Ƹ��¼
        int PcWebLogin(const char *packet);
		//����׿Ƹ�½� �޸� ������Ϣ
		int PcWebInfo(const char *packet);
		//�ϴ�ͷ��
		int PcWebPaPhoto(const char *packet);
		enum
		{
			//����׿Ƹ��¼
			kPcWebLogin = 1,
			//������Ϣ
			kPcWebInfo,
			//�ϴ�ͷ��	
			kPcWebPaPhoto,
		};
	};
}

#endif