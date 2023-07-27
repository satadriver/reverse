#ifndef HTTP_VIDEOACTIVITY_CCTV_H_H_H
#define HTTP_VIDEOACTIVITY_CCTV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class CCTVVideo: public VideoActivity
	{
	public:
		CCTVVideo(HttpSession *http_session, const string &pro_name);
		~CCTVVideo();
		static int IsCCTVVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索1
		int PcWebSearch(const char *packet);
		//用户评论 回复
		int PcWebComment(const char *packet);
		//pc电脑端搜索
		int PcClientSearch(const char *packet);
		//手机搜索
		int AndroidSearch(const char *packet);
		//手机评论回复(电脑端)
		int AndroidComment(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcClientSearch,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif