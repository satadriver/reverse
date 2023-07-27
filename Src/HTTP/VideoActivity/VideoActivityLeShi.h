#ifndef HTTP_VIDEOACTIVITY_LESHI_H_H_H
#define HTTP_VIDEOACTIVITY_LESHI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class LeShiVideo: public VideoActivity
	{
	public:
		LeShiVideo(HttpSession *http_session, const string &pro_name);
		~LeShiVideo();
		static int IsLeShiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户评论
		int PcWebComment(const char *packet);
		//用户回复
		int PcWebReply(const char *packet);
		//用户弹幕
		int PcWebDanmu(const char *packet);
		//电脑客户端搜索
		int PcClientSearch(const char *packet);
		//电脑客户端弹幕
		int PcClientDanMu(const char *packet);
		//手机用户搜索
		int AndroidSearch(const char *packet);
		//手机用户评论
		int AndroidComment(const char *packet);
		//手机用户回复
		int AndroidReply(const char *packet);
		//手机用户弹幕
		int AndroidDanmu(const char *packet);

		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcWebReply,
			kPcWebDanmu,
			//电脑客户端搜索
			kPcClientSearch,
			//电脑客户端弹幕
			kPcClientDanMu,
			kAndroidSearch,
			kAndroidComment,
			kAndroidReply,
			kAndroidDanmu,
		};
	};
}

#endif