#ifndef HTTP_VIDEOACTIVITY_IQIYI_H_H_H
#define HTTP_VIDEOACTIVITY_IQIYI_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class IQiYiVideo: public VideoActivity
	{
	public:
		IQiYiVideo(HttpSession *http_session, const string &pro_name);
		~IQiYiVideo();
		static int IsIQiYiVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户评论(泡泡评论)
		int PcWebComment(const char *packet);
		//用户评论(普通评论)
		int PcWebPublish(const char *packet);
		//用户回复(泡泡回复)
		int PcWebReply(const char *packet);
		//用户回复(普通回复)
		int PcWebReplyGeneral(const char *packet);
		//用户弹幕
		int PcWebDanmu(const char *packet);
		//电脑客户端搜索
		int PcSearch(const char *packet);
		//手机客户端搜索
		int AndroidSearch(const char *packet);
		//手机客户端评论
		int AndroidComment(const char *packet);
		//手机客户端回复
		int AndroidReply(const char *packet);
		//手机客户端弹幕
		int AndroidDanmu(const char *packet);
		enum
		{
			kPcWebSearch = 1,
			kPcWebComment,
			kPcWebPublish,
			kPcWebReply,
			kPcWebReplyGeneral,
			//用户弹幕
			kPcWebDanmu,
			kPcSearch,
			kAndroidSearch,
			kAndroidComment,
			kAndroidReply,
			kAndroidDanmu,
		};
	};
}

#endif