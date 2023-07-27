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
		//注册个人信息
		int PcWebRegisterEmail(const char *packet);
		//修改基本信息
		int PCWebModiy(const char *packet);
		enum
		{
			//注册(邮箱注册)
			kPcWebRegisterEmail = 1,
			//修改基本信息
			kPCWebModiy,
		};
	};
}

#endif