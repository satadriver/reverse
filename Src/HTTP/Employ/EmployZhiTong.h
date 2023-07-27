#ifndef HTTP_EMPLOY_ZHITONG_H_H_H
#define HTTP_EMPLOY_ZHITONG_H_H_H
#include "Employ.h"

namespace employ
{
	class ZhiTong : public Employ
	{
	public:
		ZhiTong(HttpSession *http_session, const string &pro_name);
		~ZhiTong();
		static int IsZhiTong(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע�������Ϣ
		int PcWebRegisterEmail(const char *packet);
		//�޸Ļ�����Ϣ
		int PCWebModiy(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//�޸Ļ�����Ϣ
			kPCWebModiy,
		};
	};
}

#endif