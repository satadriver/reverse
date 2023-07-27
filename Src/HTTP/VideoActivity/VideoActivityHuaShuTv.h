#ifndef HTTP_VIDEOACTIVITY_HUASHUTV_H_H_H
#define HTTP_VIDEOACTIVITY_HUASHUTV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class HuaShuTvVideo: public VideoActivity
	{
	public:
		HuaShuTvVideo(HttpSession *http_session, const string &pro_name);
		~HuaShuTvVideo();
		static int IsHuaShuTvVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户评论
		int PcWebComment(const char *packet);
		//pc客户端搜索
		int PcClientSearch(const char *packet);
		//手机用户搜索
		int AndroidSearch(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcClientSearch,
			kAndroidSearch,
		};
	};
}

#endif