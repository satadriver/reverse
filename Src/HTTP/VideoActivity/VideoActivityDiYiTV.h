#ifndef HTTP_VIDEOACTIVITY_DIYITV_H_H_H
#define HTTP_VIDEOACTIVITY_DIYITV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class DiYiVideo: public VideoActivity
	{
	public:
		DiYiVideo(HttpSession *http_session, const string &pro_name);
		~DiYiVideo();
		static int IsDiYiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户评论
		int PcWebComment(const char *packet);
		//手机搜索
		int AndroidSearch(const char *packet);
		//手机评论
		int AndroidComment(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif