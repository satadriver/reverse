#ifndef HTTP_EMPLOY_YILANYINGCAI_H_H_H
#define HTTP_EMPLOY_YILANYINGCAI_H_H_H

#include "Employ.h"

namespace employ
{
	class YiLanYingCai : public Employ
	{
	public:
		YiLanYingCai(HttpSession *http_session, const string &pro_name);
		~YiLanYingCai();
		static int IsYiLanYingCai(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//注册(邮箱注册)
		int PcWebRegisterEmail(const char *packet);
		//登录
		int PcWebLogin(const char *packet);
		//修改个人信息
		int PcWebModiy(const char *packet);
		enum
		{
			//注册(邮箱注册)
			kPcWebRegisterEmail = 1,
			//登录
			kPcWebLogin,
			//修改基本信息
			kPcWebModiy,
		};
	};
}

#endif