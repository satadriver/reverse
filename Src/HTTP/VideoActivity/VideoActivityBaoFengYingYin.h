#ifndef HTTP_VIDEOACTIVITY_BAOFENGYINGYIN_H_H_H
#define HTTP_VIDEOACTIVITY_BAOFENGYINGYIN_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class BaoFengVideo: public VideoActivity
	{
	public:
		BaoFengVideo(HttpSession *http_session, const string &pro_name);
		~BaoFengVideo();
		static int IsBaoFengVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户评论 回复
		int PcWebComment(const char *packet);
		//pc客户端搜素
		int PcClientSearch(const char *packet);
		//pc客户端评论
		int PcClientComment(const char *packet);
		//pc客户端弹幕
		int PcClientDanmu(const char *packet);
		//pc客户端播放
		int PcClientPlay(const char *packet);
		//手机搜索
		int AndroidSearch(const char *packet);
		//手机评论
		int AndroidComment(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcClientSearch,
			kPcClientComment,
			kPcClientDanmu,
			kPcClientPlay,
			kAndroidSearch,
			kAndroidComment,
		};
	};
}

#endif