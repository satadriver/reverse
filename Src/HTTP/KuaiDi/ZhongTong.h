
#ifndef HTTP_KUAIDI_ZHONGTONG_H_H_H
#define HTTP_KUAIDI_ZHONGTONG_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class ZhongTong : public KuaiDi
	{
	public:
		ZhongTong(HttpSession *http_session, const string &pro_name);
		~ZhongTong();
		static int IsZhongTong(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int zhongtong(const char *packet);
		int AndroidZT(const char *packet);
		enum
		{
			kzhongtong = 1,
			kAndroidZT,
		};
	};
}

#endif