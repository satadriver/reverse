#ifndef HTTP_VIDEOACTIVITY_PPS_H_H_H
#define HTTP_VIDEOACTIVITY_PPS_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class PPsVideo: public VideoActivity
	{
	public:
		PPsVideo(HttpSession *http_session, const string &pro_name);
		~PPsVideo();
		static int IsPPsVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÓÃ»§ËÑË÷
		int PcWebSearch(const char *packet);
		enum
		{
			kPcWebSearch = 1,
		};
	};
}

#endif