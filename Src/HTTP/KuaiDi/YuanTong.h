
#ifndef HTTP_KUAIDI_YUANTONG_H_H_H
#define HTTP_KUAIDI_YUANTONG_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class Yuantong : public KuaiDi
	{
	public:
		Yuantong(HttpSession *http_session, const string &pro_name);
		~Yuantong();
		static int IsYuanTong(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int YuanTong(const char *packet);
		int AndroidLogin(const char *packet);
		int PCLogin(const char *packet);
		int PCDingdan(const char *packet);
		enum
		{
			kYuanTong = 1,
			kAndroidLogin,
			kPCLogin,
			kPCDingdan,
		};
	};
}

#endif