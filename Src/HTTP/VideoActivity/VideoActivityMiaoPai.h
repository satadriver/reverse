#ifndef HTTP_VIDEOACTIVITY_MIAOPAI_H_H_H
#define HTTP_VIDEOACTIVITY_MIAOPAI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class MiaoPaiVideo: public VideoActivity
	{
	public:
		MiaoPaiVideo(HttpSession *http_session, const string &pro_name);
		~MiaoPaiVideo();
		static int IsMiaoPaiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//手机搜索
		int AndroidSearch(const char *packet);
		//手机评论回复
		int AndroidComment(const char *packet);
		enum
		{
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif