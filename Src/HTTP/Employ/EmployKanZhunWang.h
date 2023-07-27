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
		//注册(邮箱注册)
		int PcWebRegisterEmail(const char *packet);
		//登录
		int PcWebLogin(const char *packet);
		//修改基本信息
		int PCWebModiy(const char *packet);
		enum
		{
			//注册(邮箱注册)
			kPcWebRegisterEmail = 1,
			//登录
			kPcWebLogin,
			//修改基本信息
			kPCWebModiy,
		};
	};
}

#endif