
#ifndef HTTP_KUAIDI_SHUNFENG_H_H_H
#define HTTP_KUAIDI_SHUNFENG_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class ShunFeng : public KuaiDi
	{
	public:
		ShunFeng(HttpSession *http_session, const string &pro_name);
		~ShunFeng();
		static int IsShunFeng(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int pcshunfeng(const char *packet);
		int androidshunfeng(const char *packet);
		enum
		{
			kpcshunfeng = 1,
			kandroidshunfeng,
		};
	};
}

#endif