#ifndef HTTP_VIDEOACTIVITY_YOUKU_H_H_H
#define HTTP_VIDEOACTIVITY_YOUKU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class YouKuVideo: public VideoActivity
	{
	public:
		YouKuVideo(HttpSession *http_session, const string &pro_name);
		~YouKuVideo();
		static int IsYouKuVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³ËÑË÷
		//pc¿Í»§¶ËËÑË÷
		int PcWebSearch(const char *packet);
		//ÍøÒ³ÆÀÂÛ »Ø¸´
		int PcWebComment(const char *packet);
		//ÍøÒ³µ¯Ä»
		int PcWebDanMu(const char *packet);
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
			//pc¿Í»§¶ËËÑË÷
			kPcWebSearch = 1,
			//ÍøÒ³ÆÀÂÛ »Ø¸´
			kPcWebComment,
			//ÍøÒ³µ¯Ä»
			kPcWebDanMu,
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