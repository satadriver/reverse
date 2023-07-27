#ifndef HTTP_EMPLOY_ZHONGGUORENCAI_H_H_H
#define HTTP_EMPLOY_ZHONGGUORENCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class ZhongGuoRenCai : public Employ
	{
	public:
		ZhongGuoRenCai(HttpSession *http_session, const string &pro_name);
		~ZhongGuoRenCai();
		static int IsZhongGuoRenCai(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//�޸ĸ�����Ϣ
		int PcWebModiy(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//�޸Ļ�����Ϣ
			kPcWebModiy,
		};
	};
}

#endif