#ifndef HTTP_VIDEOACTIVITY_WOLE_H_H_H
#define HTTP_VIDEOACTIVITY_WOLE_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class WoLeVideo: public VideoActivity
	{
	public:
		WoLeVideo(HttpSession *http_session, const string &pro_name);
		~WoLeVideo();
		static int IsWoLeVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³ËÑË÷
		int PcWebSearch(const char *packet);
		//ÍøÒ³ÆÀÂÛ »Ø¸´
		int PcWebComment(const char *packet);
		//ÍøÒ³µ¯Ä»
		int PcWebDanMu(const char *packet);
		//ÍøÒ³²¥·Å
		int PcWebPlay(const char *packet);
		//androidËÑËÑ
		int AndroidSearch(const char *packet);
		enum
		{
			//ÍøÒ³ËÑË÷
			kPcWebSearch = 1,
			//ÍøÒ³ÆÀÂÛ »Ø¸´
			kPcWebComment,
			//ÍøÒ³µ¯Ä»
			kPcWebDanMu,
			//ÍøÒ³²¥·Å
			kPcWebPlay,
			//androidËÑË÷
			kAndroidSearch,
		};
	};
}
#endif