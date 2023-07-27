#ifndef HTTP_VIDEOACTIVITY_KULIU_H_H_H
#define HTTP_VIDEOACTIVITY_KULIU_H_H_H

#include "VideoActivity.h"

namespace videoactivity
{
	class KuLiuVideo: public VideoActivity
	{
	public:
		KuLiuVideo(HttpSession *http_session, const string &pro_name);
		~KuLiuVideo();
		static int IsKuLiuVideo(HttpSession *http_session);
		int Process(const char *packet,int action);
	private:
		//��ҳ����
		int PcWebSearch(const char *packet);
		//��ҳ���� 
		int PcWebComment(const char *packet);
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
			//android����
			kAndroidSearch,
			//android����
			kAndroidComment,
		};
	};
}

#endif