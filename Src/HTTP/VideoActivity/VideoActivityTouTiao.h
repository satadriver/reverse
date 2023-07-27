#ifndef HTTP_VIDEOACTIVITY_TOUTIAO_H_H_H
#define HTTP_VIDEOACTIVITY_TOUTIAO_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class TouTiaoVideo: public VideoActivity
	{
	public:
		TouTiaoVideo(HttpSession *http_session, const string &pro_name);
		~TouTiaoVideo();
		static int IsTouTiaoVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//androidËÑË÷
		int AndroidSearch(const char *packet);
		enum
		{
			//androidËÑË÷
			kAndroidSearch = 1,
		};
	};
}
#endif