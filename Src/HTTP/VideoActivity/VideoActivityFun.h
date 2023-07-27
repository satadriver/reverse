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
		//��ҳ����
		int PcWebSearch(const char *packet);
		//��ҳ���� 
		int PcWebComment(const char *packet);
		//pc����
		int PcClientSearch(const char *packet);
		//android����
		int AndroidSearch(const char *packet);
		//android���� 
		int AndroidComment(const char *packet);
		enum
		{
			//��ҳ����
			kPcWebSearch = 1,
			//��ҳ���� 
			kPcWebComment,
			//pc����
			kPcClientSerch,
			//android����
			kAndroidSearch,
			//android���� 
			kAndroidComment,
		};
	};
}
#endif