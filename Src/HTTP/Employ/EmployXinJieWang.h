#ifndef HTTP_EMPLOY_XINJIEWANG_H_H_H
#define HTTP_EMPLOY_XINJIEWANG_H_H_H

#include "Employ.h"

namespace employ
{
	class XinJieWang : public Employ
	{
	public:
		XinJieWang(HttpSession *http_session, const string &pro_name);
		~XinJieWang();
		static int IsXinJieWang(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//注册(邮箱注册)
		int PcWebRegisterEmail(const char *packet);
		//注册(用户名)
		int PcWebRegisteruser(const char *packet);
		//登录
		int PcWebLogin(const char *packet);
		//新建基本信息
		int PcWebRegisterInfo(const char *packet);
		//修改基本信息
		int PcWebModiy(const char *packet);
		enum
		{
			//注册(邮箱注册)
			kPcWebRegisterEmail = 1,
			//注册(用户名)
			kPcWebRegisteruser,
			//登录
			kPcWebLogin,
			//新建基本信息
			kPcWebRegisterInfo,
			//修改基本信息
			kPcWebModiy,
		};
	};
}

#endif