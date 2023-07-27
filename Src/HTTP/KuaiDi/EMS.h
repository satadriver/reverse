
#ifndef HTTP_KUAIDI_EMS_H_H_H
#define HTTP_KUAIDI_EMS_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class EMS : public KuaiDi
	{
	public:
		EMS(HttpSession *http_session, const string &pro_name);
		~EMS();
		static int IsEms(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int pcems(const char *packet);
		int pcemsduo(const char *packet);
		int androidems(const char *packet);
		enum
		{
			kpcems = 1,
			kpcemsduo,
			kandroidems,
		};
	};
}

#endif