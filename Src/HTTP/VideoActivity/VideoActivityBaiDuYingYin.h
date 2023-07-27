#ifndef HTTP_VIDEOACTIVITY_BAIDUYINGYIN_H_H_H
#define HTTP_VIDEOACTIVITY_BAIDUYINGYIN_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class BaiDuYingYinVideo: public VideoActivity
	{
	public:
		BaiDuYingYinVideo(HttpSession *http_session, const string &pro_name);
		~BaiDuYingYinVideo();
		static int IsBaiDuYingYinVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÓÃ»§ËÑË÷
		int PcWebSearch(const char *packet);
		//androidËÑË÷
		int AndroidSearch(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kAndroidSearch,
		};
	};
}

#endif