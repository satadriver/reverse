#ifndef HTTP_VIDEOACTIVITY_MANGGUO_H_H_H
#define HTTP_VIDEOACTIVITY_MANGGUO_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class MangGuoVideo: public VideoActivity
	{
	public:
		MangGuoVideo(HttpSession *http_session, const string &pro_name);
		~MangGuoVideo();
		static int IsMangGuoVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索1
		int PcWebSearch(const char *packet);
		//用户搜索2
		int PcWebSearcht(const char *packet);
		//用户评论 回复
		int PcWebComment(const char *packet);
		//pc端搜索
		int PcClientSearch(const char *packet);
		//pc端评论 回复
		int PcClientComment(const char *packet);
		//pc端播放
		int PcClientPlay(const char *packet);
		//手机搜索
		int AndroidSearch(const char *packet);
		//手机评论回复
		int AndroidComment(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebSearcht,
			kPcWebComment,
			kPcClientSearch,
			kPcClientComment,
			kPcClientPlay,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif