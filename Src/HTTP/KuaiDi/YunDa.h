
#ifndef HTTP_KUAIDI_YUNDA_H_H_H
#define HTTP_KUAIDI_YUNDA_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class YunDa : public KuaiDi
	{
	public:
		YunDa(HttpSession *http_session, const string &pro_name);
		~YunDa();
		static int IsYunDa(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int pcyunda(const char *packet);
		int androidyunda(const char *packet);
		enum
		{
			kpcyunda = 1,
			kandroidyunda,
		};
	};
}

#endif