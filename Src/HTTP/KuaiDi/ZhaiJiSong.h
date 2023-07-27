
#ifndef HTTP_KUAIDI_ZHAIJISONG_H_H_H
#define HTTP_KUAIDI_ZHAIJISONG_H_H_H

#include "KuaiDi.h"

namespace kuaidi
{
	class ZhaiJiSong : public KuaiDi
	{
	public:
		ZhaiJiSong(HttpSession *http_session, const string &pro_name);
		~ZhaiJiSong();
		static int IsZhaiJiSong(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		
		int zhaijisong(const char *packet);
		int zhaijisonggw(const char *packet);
		enum
		{
			kzhaijisong = 1,
			kzhaijisonggw,
		};
	};
}

#endif