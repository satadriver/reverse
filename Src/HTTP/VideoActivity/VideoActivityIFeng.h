#ifndef HTTP_VIDEOACTIVITY_IFENG_H_H_H
#define HTTP_VIDEOACTIVITY_IFENG_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class IFengVideo: public VideoActivity
	{
	public:
		IFengVideo(HttpSession *http_session, const string &pro_name);
		~IFengVideo();
		static int IsIFengVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³ËÑË÷
		int PcWebSearch(const char *packet);
		//ÍøÒ³ÆÀÂÛ
		int PcWebComment(const char *packet);
		//androidËÑËÑ
		int AndroidSearch(const char *packet);
		//androidÆÀÂÛ »Ø¸´
		int AndroidComment(const char *packet);
		enum
		{
			//ÍøÒ³ËÑË÷
			kPcWebSearch = 1,
			//ÍøÒ³ÆÀÂÛ
			kPcWebComment,
			//androidËÑË÷
			kAndroidSearch,
			//androidÆÀÂÛ »Ø¸´
			kAndroidComment,
		};
	};
}
#endif