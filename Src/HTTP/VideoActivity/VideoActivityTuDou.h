#ifndef HTTP_VIDEOACTIVITY_TUDOU_H_H_H
#define HTTP_VIDEOACTIVITY_TUDOU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class TuDouVideo: public VideoActivity
	{
	public:
		TuDouVideo(HttpSession *http_session, const string &pro_name);
		~TuDouVideo();
		static int IsTuDouVideo(HttpSession *http_session);
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
		//pc¿Í»§¶ËËÑË÷
		int PcClientSearch(const char *packet);
		//pc¿Í»§¶Ëµ¯Ä»
		int PcClientDanMu(const char *packet);
		//androidËÑËÑ
		int AndroidSearch(const char *packet);
		//androidÆÀÂÛ »Ø¸´
		int AndroidComment(const char *packet);
		//androidµ¯Ä»
		int AndroidDanMu(const char *packet);

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
			//pc¿Í»§¶ËËÑË÷
			kPcClientSearch,
			//pc¿Í»§¶Ëµ¯Ä»
			kPcClientDanMu,
			//androidËÑË÷
			kAndroidSearch,
			//androidÆÀÂÛ »Ø¸´
			kAndroidComment,
			//androidµ¯Ä»
			kAndroidDanmu,
		};
	};
}

#endif