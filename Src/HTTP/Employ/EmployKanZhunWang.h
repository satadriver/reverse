#ifndef HTTP_EMPLOY_KANZHUNWANG_H_H_H
#define HTTP_EMPLOY_KANZHUNWANG_H_H_H

#include "Employ.h"

namespace employ
{
	class KanZhunWang : public Employ
	{
	public:
		KanZhunWang(HttpSession *http_session, const string &pro_name);
		~KanZhunWang();
		static int IsKanZhunWang(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ע��(����ע��)
		int PcWebRegisterEmail(const char *packet);
		//��¼
		int PcWebLogin(const char *packet);
		//�޸Ļ�����Ϣ
		int PCWebModiy(const char *packet);
		enum
		{
			//ע��(����ע��)
			kPcWebRegisterEmail = 1,
			//��¼
			kPcWebLogin,
			//�޸Ļ�����Ϣ
			kPCWebModiy,
		};
	};
}

#endif