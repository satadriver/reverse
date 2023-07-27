
#ifndef HTTP_KUAIDI_TIANTIAN_H_H_H
#define HTTP_KUAIDI_TIANTIAN_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class TianTian : public KuaiDi
	{
	public:
		TianTian(HttpSession *http_session, const string &pro_name);
		~TianTian();
		static int IsTianTian(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int Tiantian(const char *packet);
		int TianCX(const char *packet);
		int tiantiangw(const char *packet);
		enum
		{
			ktiantiangw = 1,
		};
	};
}

#endif