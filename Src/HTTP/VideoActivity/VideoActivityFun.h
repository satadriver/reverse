#ifndef HTTP_VIDEOACTIVITY_FUN_H_H_H
#define HTTP_VIDEOACTIVITY_FUN_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class FunVideo: public VideoActivity
	{
	public:
		FunVideo(HttpSession *http_session, const string &pro_name);
		~FunVideo();
		static int IsFunVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//ÍøÒ³ËÑË÷
		int PcWebSearch(const char *packet);
		//ÍøÒ³ÆÀÂÛ 
		int PcWebComment(const char *packet);
		//pcËÑË÷
		int PcClientSearch(const char *packet);
		//androidËÑË÷
		int AndroidSearch(const char *packet);
		//androidÆÀÂÛ 
		int AndroidComment(const char *packet);
		enum
		{
			//ÍøÒ³ËÑË÷
			kPcWebSearch = 1,
			//ÍøÒ³ÆÀÂÛ 
			kPcWebComment,
			//pcËÑËÑ
			kPcClientSerch,
			//androidËÑË÷
			kAndroidSearch,
			//androidÆÀÂÛ 
			kAndroidComment,
		};
	};
}
#endif