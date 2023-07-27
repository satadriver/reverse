#ifndef HTTP_VIDEOACTIVITY_SOHU_H_H_H
#define HTTP_VIDEOACTIVITY_SOHU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class SoHuVideo: public VideoActivity
	{
	public:
		SoHuVideo(HttpSession *http_session, const string &pro_name);
		~SoHuVideo();
		static int IsSoHuVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//用户搜索
		int PcWebSearch(const char *packet);
		//用户评论 回复
		int PcWebComment(const char *packet);
		//用户弹幕
		int PcWebDanmu(const char *packet);
		//pc客户端搜索
		int PcClientSearch(const char *packet);
		//手机用户搜索
		int AndroidSearch(const char *packet);
		//android评论 回复
		int AndroidComment(const char *packet);
		//android弹幕
		int AndroidDanmu(const char *packet);
		enum
		{
			//用户搜索
			kPcWebSearch = 1,
			//用户评论 回复
			kPcWebComment,
			//用户弹幕
			kPcWebDanmu,
			//pc客户端搜索
			kPcClientSearch,
			//android搜索
			kAndroidSearch,
			//android评论 回复
			kAndroidComment,
			//android弹幕
			kAndroidDanmu,
		};
	};
}

#endif