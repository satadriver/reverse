#ifndef HTTP_VIDEOACTIVITY_PPTV_H_H_H
#define HTTP_VIDEOACTIVITY_PPTV_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class PpTvVideo: public VideoActivity
	{
	public:
		PpTvVideo(HttpSession *http_session, const string &pro_name);
		~PpTvVideo();
		static int IsPpTvVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户弹幕
		int PcWebDanmu(const char *packet);
		//网页评论 
		int PcWebComment(const char *packet);
		//网页回复
		int PcWebReply(const char *packet);
		//pc客户端搜索
		int PcClientSearch(const char *packet);
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
			//网页搜索
			kPcWebSearch = 1,
			//网页评论
			kPcWebComment,
			//网页回复
			kPcWebReply,
			//网页弹幕
			kPcWebDanmu,
			//pc客户端搜索
			kPcClientSearch,
			//android搜索
			kAndroidSearch,
			//android评论
			kAndroidComment,
			//android回复
			kAndroidReply,
			//android弹幕
			kAndroidDanmu,
		};
	};
}

#endif